/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "alure_sound_buffer.hpp"
#include "alure_sound_system.hpp"
#include <AL/alure2.h>

al::AlureSoundBuffer::AlureSoundBuffer(alure::Context &context,alure::Buffer *buffer,const std::string &path)
	: ISoundBuffer{},impl::BufferBase(path),m_context(context),m_buffer(buffer)
{}
alure::Buffer *al::AlureSoundBuffer::GetALBuffer() {return m_buffer;}

al::AlureSoundBuffer::~AlureSoundBuffer()
{
	m_context.removeBuffer(*m_buffer);
}

bool al::AlureSoundBuffer::IsReady() const
{
	auto status = m_buffer->getLoadStatus();
	return (status == alure::BufferLoadStatus::Ready) ? true : false;
}

uint64_t al::AlureSoundBuffer::GetLength() const
{
	if(IsReady() == false)
		return 0;
	return m_buffer->getLength();
}
uint32_t al::AlureSoundBuffer::GetFrequency() const
{
	return m_buffer->getFrequency();
}
al::ChannelConfig al::AlureSoundBuffer::GetChannelConfig() const
{
	return static_cast<al::ChannelConfig>(m_buffer->getChannelConfig());
}
al::SampleType al::AlureSoundBuffer::GetSampleType() const
{
	return static_cast<al::SampleType>(m_buffer->getSampleType());
}
uint32_t al::AlureSoundBuffer::GetSize() const
{
	if(IsReady() == false)
		return 0;
	return m_buffer->getSize();
}
void al::AlureSoundBuffer::SetLoopFramePoints(uint32_t start,uint32_t end)
{
	if(IsReady() == false)
		return; // TODO: Set when ready
	m_buffer->setLoopPoints(start,end);
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
	if(IsReady() == false)
		return {0,0};
	return m_buffer->getLoopPoints();
}

std::string al::AlureSoundBuffer::GetName() const
{
	return m_buffer->getName().data();
}
bool al::AlureSoundBuffer::IsInUse() const
{
	return m_buffer->getSourceCount() > 0;
}
