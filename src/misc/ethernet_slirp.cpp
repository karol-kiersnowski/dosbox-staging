/* *  Copyright (C) 2020  The DOSBox Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "config.h"

#include "ethernet_slirp.h"
#include <time.h>
#include "dosbox.h"

#ifdef WIN32
#include <ws2tcpip.h>
#else
#include <poll.h>
#include <arpa/inet.h>
#endif

ssize_t slirp_send_packet(const void *buf, size_t len, void *opaque)
{
	SlirpEthernetConnection *conn = (SlirpEthernetConnection *)opaque;
	conn->Receive_Packet((Bit8u *)buf, len);
	return len; /* TODO: process this better */
}
void slirp_guest_error(const char *msg, void *opaque)
{
	(void)opaque;
	LOG_MSG("SLIRP: ERROR: %s", msg);
}
int64_t slirp_clock_get_ns(void *opaque)
{
	(void)opaque;
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	/* if clock_gettime fails we have more serious problems */
	return ts.tv_nsec + (ts.tv_sec * 1e9);
}
void *slirp_timer_new(SlirpTimerCb cb, void *cb_opaque, void *opaque)
{
	SlirpEthernetConnection *conn = (SlirpEthernetConnection *)opaque;
	return conn->Timer_New(cb, cb_opaque);
}
void slirp_timer_free(void *timer, void *opaque)
{
	SlirpEthernetConnection *conn = (SlirpEthernetConnection *)opaque;
	struct slirp_timer *real_timer = (struct slirp_timer *)timer;
	conn->Timer_Free(real_timer);
}
void slirp_timer_mod(void *timer, int64_t expire_time, void *opaque)
{
	SlirpEthernetConnection *conn = (SlirpEthernetConnection *)opaque;
	struct slirp_timer *real_timer = (struct slirp_timer *)timer;
	conn->Timer_Mod(real_timer, expire_time);
}
int slirp_add_poll(int fd, int events, void *opaque)
{
	SlirpEthernetConnection *conn = (SlirpEthernetConnection *)opaque;
	return conn->Poll_Add(fd, events);
}
int slirp_get_revents(int idx, void *opaque)
{
	SlirpEthernetConnection *conn = (SlirpEthernetConnection *)opaque;
	return conn->Poll_Get_Slirp_Revents(idx);
}
void slirp_register_poll_fd(int fd, void *opaque)
{
	(void)fd;
	(void)opaque;
	/* TODO: find way to preserve these across polls_clear */
	/*polls_add(fd, POLLIN | POLLOUT);*/
}
void slirp_unregister_poll_fd(int fd, void *opaque)
{
	(void)fd;
	(void)opaque;
	/*polls_remove(fd);*/
}
void slirp_notify(void *opaque)
{
	(void)opaque;
	return;
}

SlirpEthernetConnection::SlirpEthernetConnection() : EthernetConnection()
{
	slirp_callbacks.send_packet = slirp_send_packet;
	slirp_callbacks.guest_error = slirp_guest_error;
	slirp_callbacks.clock_get_ns = slirp_clock_get_ns;
	slirp_callbacks.timer_new = slirp_timer_new;
	slirp_callbacks.timer_free = slirp_timer_free;
	slirp_callbacks.timer_mod = slirp_timer_mod;
	slirp_callbacks.register_poll_fd = slirp_register_poll_fd;
	slirp_callbacks.unregister_poll_fd = slirp_unregister_poll_fd;
	slirp_callbacks.notify = slirp_notify;
}

SlirpEthernetConnection::~SlirpEthernetConnection()
{
	if (slirp)
		slirp_cleanup(slirp);
}

bool SlirpEthernetConnection::Initialize()
{
	/* Config */
	config.version = 1;
	config.restricted = 0;            /* Allow access to host */
	config.disable_host_loopback = 0; /* Allow access to 127.0.0.1 */
	config.if_mtu = 0;                /* IF_MTU_DEFAULT */
	config.if_mru = 0;                /* IF_MRU_DEFAULT */
	config.enable_emu = 0,            /* Buggy, don't use */
	        /* IPv4 */
	        config.in_enabled = 1;
	inet_pton(AF_INET, "10.0.2.0", &config.vnetwork);
	inet_pton(AF_INET, "255.255.255.0", &config.vnetmask);
	inet_pton(AF_INET, "10.0.2.2", &config.vhost);
	inet_pton(AF_INET, "10.0.2.3", &config.vnameserver);
	inet_pton(AF_INET, "10.0.2.15", &config.vdhcp_start);
	/* IPv6 */
	config.in6_enabled = 0;
	inet_pton(AF_INET6, "fec0::", &config.vprefix_addr6);
	config.vprefix_len = 64;
	inet_pton(AF_INET6, "fec0::2", &config.vhost6);
	inet_pton(AF_INET6, "fec0::3", &config.vnameserver6);
	/* DHCPv4 */
	config.vhostname = "DOSBox-X";
	config.tftp_server_name = NULL;
	config.tftp_path = NULL;
	config.bootfile = NULL;
	config.vdnssearch = NULL;
	config.vdomainname = NULL;

	slirp = slirp_new(&config, &slirp_callbacks, this);
	if (slirp) {
		LOG_MSG("initialized slirp");
		return true;
	} else {
		LOG_MSG("failed to initialize slirp");
		return false;
	}
}

void SlirpEthernetConnection::SendPacket(Bit8u *packet, int len)
{
	slirp_input(slirp, packet, len);
}

void SlirpEthernetConnection::Receive_Packet(Bit8u *packet, int len)
{
	get_packet_callback(packet, len);
}

void SlirpEthernetConnection::GetPackets(std::function<void(Bit8u *, int)> callback)
{
	get_packet_callback = callback;
	uint32_t timeout = 0;
	Polls_Clear();
	slirp_pollfds_fill(slirp, &timeout, slirp_add_poll, this);
	int ret = -1; /*poll(polls, 256, timeout);*/
	slirp_pollfds_poll(slirp, (ret <= -1), slirp_get_revents, this);
	Timers_Run();
}

struct slirp_timer *SlirpEthernetConnection::Timer_New(SlirpTimerCb cb, void *cb_opaque)
{
	for (int i = 0; i < 256; ++i) {
		struct slirp_timer *timer = &timers[i];
		if (!timer->used) {
			timer->used = 1;
			timer->expires = 0;
			timer->cb = cb;
			timer->cb_opaque = cb_opaque;
			return timer;
		}
	}
	return nullptr;
}

void SlirpEthernetConnection::Timer_Free(struct slirp_timer *timer)
{
	timer->used = 0;
}

void SlirpEthernetConnection::Timer_Mod(struct slirp_timer *timer, int64_t expire_time)
{
	timer->expires = expire_time;
}

void SlirpEthernetConnection::Timers_Run(void)
{
	int64_t now = slirp_clock_get_ns(NULL);
	for (int i = 0; i < 256; ++i) {
		struct slirp_timer *timer = &timers[i];
		if (timer->used && timer->expires && timer->expires < now) {
			timer->expires = 0;
			timer->cb(timer->cb_opaque);
		}
	}
}

int SlirpEthernetConnection::Poll_Add(int fd, int slirp_events)
{
	int real_events = 0;
	if (slirp_events & SLIRP_POLL_IN)
		real_events |= POLLIN;
	if (slirp_events & SLIRP_POLL_OUT)
		real_events |= POLLOUT;
	if (slirp_events & SLIRP_POLL_PRI)
		real_events |= POLLPRI;
	/* TODO: check fd <= 256 */
	// polls[fd].fd = fd;
	// polls[fd].events = real_events;
	return fd;
}

int SlirpEthernetConnection::Poll_Get_Slirp_Revents(int idx)
{
	int real_revents = 0; // polls[idx].revents;
	int slirp_revents = 0;
	if (real_revents & POLLIN)
		slirp_revents |= SLIRP_POLL_IN;
	if (real_revents & POLLOUT)
		slirp_revents |= SLIRP_POLL_OUT;
	if (real_revents & POLLPRI)
		slirp_revents |= SLIRP_POLL_PRI;
	if (real_revents & POLLERR)
		slirp_revents |= SLIRP_POLL_ERR;
	if (real_revents & POLLHUP)
		slirp_revents |= SLIRP_POLL_HUP;
	return slirp_revents;
}

void SlirpEthernetConnection::Polls_Clear(void)
{
	for (int i = 0; i < 256; ++i) {
		// polls[i].fd = -1;
		// polls[i].events = 0;
	}
}
