/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __ALURE_SOUND_BUFFER_HPP__
#define __ALURE_SOUND_BUFFER_HPP__

#include <alsound_buffer.hpp>

namespace al
{
	class AlureSoundBuffer
		: public ISoundBuffer
	{
	public:
		AlureSoundBuffer(alure::Context &context,alure::Buffer *buffer,const std::string &path);
		virtual ~AlureSoundBuffer() override;

		alure::Buffer *GetALBuffer() {return m_buffer;}
		virtual bool IsReady() const override;

		virtual uint32_t GetSize() const override;
		virtual void SetLoopFramePoints(uint32_t start,uint32_t end) override;
		virtual void SetLoopTimePoints(float tStart,float tEnd) override;

		virtual uint32_t GetFrequency() const override;
		virtual ChannelConfig GetChannelConfig() const override;
		virtual SampleType GetSampleType() const override;
		virtual uint64_t GetLength() const override;
		virtual std::pair<uint64_t,uint64_t> GetLoopFramePoints() const override;

		virtual std::string GetName() const override;
		virtual bool IsInUse() const override;
	private:
		alure::Buffer *m_buffer = nullptr;
		alure::Context &m_context;
	};
};

#endif
