/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "alure_sound_buffer.hpp"
#include "alure_sound_system.hpp"

al::AlureSoundBuffer::AlureSoundBuffer(alure::Context &context,alure::Buffer *buffer,const std::string &path)
	: impl::BufferBase(path),m_context(context),m_buffer(buffer)
{}
alure::Buffer *al::AlureSoundBuffer::GetALBuffer() {return m_buffer;}

al::AlureSoundBuffer::~AlureSoundBuffer()
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_context.removeBuffer(*m_buffer);
#endif
}

bool al::AlureSoundBuffer::IsReady() const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	auto status = m_buffer->getLoadStatus();
	return (status == alure::BufferLoadStatus::Ready) ? true : false;
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	FMOD_OPENSTATE openState;
	uint32_t percentBuffered;
	bool starving,diskBusy;
	al::fmod::check_result(m_fmSound->getOpenState(&openState,&percentBuffered,&starving,&diskBusy));
	return openState != FMOD_OPENSTATE_LOADING && 
		openState != FMOD_OPENSTATE_ERROR && 
		openState != FMOD_OPENSTATE_CONNECTING; // TODO: What about FMOD_OPENSTATE_BUFFERING?
#endif
}

uint64_t al::AlureSoundBuffer::GetLength() const
{
	if(IsReady() == false)
		return 0;
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	return m_buffer->getLength();
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	auto length = 0u;
	al::fmod::check_result(m_fmSound->getLength(&length,FMOD_TIMEUNIT_PCM));
	return length;
#endif
}
uint32_t al::AlureSoundBuffer::GetFrequency() const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	return m_buffer->getFrequency();
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	float frequency;
	int32_t priority;
	al::fmod::check_result(m_fmSound->getDefaults(&frequency,&priority));
	return static_cast<uint32_t>(frequency);
#endif
}
al::ChannelConfig al::AlureSoundBuffer::GetChannelConfig() const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	return static_cast<al::ChannelConfig>(m_buffer->getChannelConfig());
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	int32_t channels;
	al::fmod::check_result(m_fmSound->getFormat(nullptr,nullptr,&channels,nullptr));
	return (channels >= 2) ? al::ChannelConfig::Stereo : al::ChannelConfig::Mono;
#endif
}
al::SampleType al::AlureSoundBuffer::GetSampleType() const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	return static_cast<al::SampleType>(m_buffer->getSampleType());
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	FMOD_SOUND_FORMAT format;
	al::fmod::check_result(m_fmSound->getFormat(nullptr,&format,nullptr,nullptr));
	switch(format)
	{
		case FMOD_SOUND_FORMAT_PCMFLOAT:
			return al::SampleType::Float32;
		case FMOD_SOUND_FORMAT_PCM8:
			return al::SampleType::UInt8;
		case FMOD_SOUND_FORMAT_PCM16:
			return al::SampleType::Int16;
		default:
			// FMOD TODO
			throw std::runtime_error("Unsupported sample type " +std::to_string(format));
			break;
	}
#endif
}
uint32_t al::AlureSoundBuffer::GetSize() const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	if(IsReady() == false)
		return 0;
	return m_buffer->getSize();
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
	return 0u;
#endif
}
void al::AlureSoundBuffer::SetLoopFramePoints(uint32_t start,uint32_t end)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	if(IsReady() == false)
		return; // TODO: Set when ready
	m_buffer->setLoopPoints(start,end);
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	al::fmod::check_result(m_fmSound->setLoopPoints(start,FMOD_TIMEUNIT_PCM,end,FMOD_TIMEUNIT_PCM));
#endif
}
void al::AlureSoundBuffer::SetLoopTimePoints(float tStart,float tEnd)
{
	auto dur = GetDuration();
	auto start = 0u;
	auto end = 0u;
	if(dur > 0.f)
	{
		tStart /= dur;
		tEnd /= dur;

		auto l = GetLength();
		start = umath::round(tStart *l);
		end = umath::round(tEnd *l);
	}
	SetLoopFramePoints(start,end);
}
std::pair<uint64_t,uint64_t> al::AlureSoundBuffer::GetLoopFramePoints() const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	if(IsReady() == false)
		return {0,0};
	return m_buffer->getLoopPoints();
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	uint32_t start,end;
	al::fmod::check_result(m_fmSound->getLoopPoints(&start,FMOD_TIMEUNIT_PCM,&end,FMOD_TIMEUNIT_PCM));
	return {start,end};
#endif
}

std::string al::AlureSoundBuffer::GetName() const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	return m_buffer->getName().data();
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
	static std::string r = "";
	return r;
#endif
}
bool al::AlureSoundBuffer::IsInUse() const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	return m_buffer->getSourceCount() > 0;
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
	return true;
#endif
}
