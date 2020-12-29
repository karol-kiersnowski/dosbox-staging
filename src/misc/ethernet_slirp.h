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

#ifndef DOSBOX_ETHERNET_SLIRP_H
#define DOSBOX_ETHERNET_SLIRP_H

#include "config.h"

#include "ethernet.h"
#include <slirp/libslirp.h>

struct slirp_timer {
	int used;
	int64_t expires;
	SlirpTimerCb cb;
	void *cb_opaque;
};

class SlirpEthernetConnection : public EthernetConnection {
	public:
		SlirpEthernetConnection();
		~SlirpEthernetConnection();
		bool Initialize();
		void SendPacket(uint8_t* packet, int len);
		void Receive_Packet(uint8_t* packet, int len);
		void GetPackets(std::function<void(uint8_t*, int)> callback);

		struct slirp_timer* Timer_New(SlirpTimerCb cb, void *cb_opaque);
		void Timer_Free(struct slirp_timer* timer);
		void Timer_Mod(struct slirp_timer* timer, int64_t expire_time);

		int Poll_Add(int fd, int slirp_events);
		int Poll_Get_Slirp_Revents(int idx);

	private:
		void Timers_Run();
		void Polls_Clear();

		struct slirp_timer timers[256] = { 0 };
		Slirp* slirp = nullptr;
		SlirpConfig config = { 0 };
		SlirpCb slirp_callbacks = { 0 };
		std::function<void(uint8_t*, int)> get_packet_callback;
};

#endif
