/*
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *
 *  Copyright (C) 2012-2021  sergm <sergm@bigmir.net>
 *  Copyright (C) 2020-2021  The DOSBox Staging Team
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

#ifndef DOSBOX_MIDI_MT32_H
#define DOSBOX_MIDI_MT32_H

#include "midi_handler.h"

#if C_MT32EMU

#include <array>
#include <memory>
#include <thread>

#define MT32EMU_API_TYPE 3
#include <mt32emu/mt32emu.h>
#if MT32EMU_VERSION_MAJOR != 2 || MT32EMU_VERSION_MINOR < 1
#error Incompatible mt32emu library version
#endif

#include "mixer.h"
#include "../libs/readerwriterqueue/readerwritercircularbuffer.h"

class MidiHandler_mt32 final : public MidiHandler {
private:
	static constexpr int FRAMES_PER_BUFFER = 2048; // synth granularity
	static constexpr int RENDER_QUEUE_DEPTH = 3; // how many to keep rendered

	using buffer_t = std::array<int16_t, FRAMES_PER_BUFFER * 2>; // L & R
	using channel_t = std::unique_ptr<MixerChannel, decltype(&MIXER_DelChannel)>;
	using conditional_t = moodycamel::weak_atomic<bool>;
	using ring_t = moodycamel::BlockingReaderWriterCircularBuffer<buffer_t>;

public:	
	using service_t = std::unique_ptr<MT32Emu::Service>;

	~MidiHandler_mt32();
	void Close() override;
	const char *GetName() const override { return "mt32"; }
	bool Open(const char *conf) override;
	void PlayMsg(const uint8_t *msg) override;
	void PlaySysex(uint8_t *sysex, size_t len) override;

private:
	uint32_t GetMidiEventTimestamp() const;
	void MixerCallBack(uint16_t len);
	uint16_t GetAvailableFrames();
	void Render();

	// Managed objects
	buffer_t buffer{};
	channel_t channel{nullptr, MIXER_DelChannel};
	std::thread renderer{};
	ring_t ring{RENDER_QUEUE_DEPTH};
	service_t service{};

	// Ongoing state-tracking
	uint32_t played_buffers = 0;
	uint16_t current_frame = 0;
	conditional_t keep_rendering = true;
};

#endif // C_MT32EMU

#endif
