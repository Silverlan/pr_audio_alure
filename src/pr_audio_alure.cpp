/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "pr_audio_dummy.hpp"
#include <alsoundsystem.hpp>
#include <AL/alure2.h>

namespace alure
{
	class Device;
	class Context;
	class Buffer;
	class Source;
	class Listener;
	class Effect;
};
class AlureEffect
{
	public:
		const alure::Effect &GetALEffect() const;
		alure::Effect &GetALEffect();
	private:
		Effect(SoundSystem &soundSys,alure::Effect *effect);
		alure::Effect *m_effect = nullptr;
		AuxiliaryEffectSlot *m_slot = nullptr;
#endif
};

class AlureSoundSource
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	public:
		alure::Buffer *GetALBuffer();
	private:
		SoundBuffer(alure::Context &context,alure::Buffer *buffer,const std::string &path="");
		alure::Buffer *m_buffer = nullptr;
		alure::Context &m_context;
#endif
};

al::AuxiliaryEffectSlot::~AuxiliaryEffectSlot()
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_slot->destroy();
#endif
}

const alure::AuxiliaryEffectSlot &al::AuxiliaryEffectSlot::GetALSlot() const {return const_cast<AuxiliaryEffectSlot*>(this)->GetALSlot();}
alure::AuxiliaryEffectSlot &al::AuxiliaryEffectSlot::GetALSlot() {return *m_slot;}

void al::AuxiliaryEffectSlot::SetGain(float gain)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_slot->setGain(m_gain = umath::clamp(gain,0.f,1.f));
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
#endif
}
float al::AuxiliaryEffectSlot::GetGain() const {return m_gain;}

void al::AuxiliaryEffectSlot::SetSendAuto(bool bAuto)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_slot->setSendAuto(m_bSendAuto = bAuto);
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
#endif
}
bool al::AuxiliaryEffectSlot::GetSendAuto() const {return m_bSendAuto;}

void al::AuxiliaryEffectSlot::ApplyEffect(const Effect &effect)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_slot->applyEffect(effect.GetALEffect());
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
#endif
}

bool al::AuxiliaryEffectSlot::IsInUse() const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	return m_slot->getUseCount() > 0;
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
	return true;
#endif
}

	class DLLALSYS AlureAuxiliaryEffectSlot
	{
	public:
		virtual ~AlureAuxiliaryEffectSlot();

		virtual void SetGain(float gain)=0;
		virtual float GetGain() const=0;

		virtual void SetSendAuto(bool bAuto)=0;
		virtual bool GetSendAuto() const=0;

		virtual void ApplyEffect(const Effect &effect)=0;

		virtual bool IsInUse() const=0;

		const alure::AuxiliaryEffectSlot &GetALSlot() const;
		alure::AuxiliaryEffectSlot &GetALSlot();
	private:
		IAuxiliaryEffectSlot(alure::AuxiliaryEffectSlot *slot);

		alure::AuxiliaryEffectSlot *m_slot = nullptr;
		float m_gain = 1.f;
		bool m_bSendAuto = true;

		friend SoundSystem;
	};

al::AuxiliaryEffectSlot::AuxiliaryEffectSlot(alure::AuxiliaryEffectSlot *slot)
	: m_slot(slot)
{}

al::AuxiliaryEffectSlot::~AuxiliaryEffectSlot()
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_slot->destroy();
#endif
}

const alure::AuxiliaryEffectSlot &al::AuxiliaryEffectSlot::GetALSlot() const {return const_cast<AuxiliaryEffectSlot*>(this)->GetALSlot();}
alure::AuxiliaryEffectSlot &al::AuxiliaryEffectSlot::GetALSlot() {return *m_slot;}

void al::AuxiliaryEffectSlot::SetGain(float gain)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_slot->setGain(m_gain = umath::clamp(gain,0.f,1.f));
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
#endif
}
float al::AuxiliaryEffectSlot::GetGain() const {return m_gain;}

void al::AuxiliaryEffectSlot::SetSendAuto(bool bAuto)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_slot->setSendAuto(m_bSendAuto = bAuto);
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
#endif
}
bool al::AuxiliaryEffectSlot::GetSendAuto() const {return m_bSendAuto;}

void al::AuxiliaryEffectSlot::ApplyEffect(const Effect &effect)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_slot->applyEffect(effect.GetALEffect());
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
#endif
}

bool al::AuxiliaryEffectSlot::IsInUse() const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	return m_slot->getUseCount() > 0;
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
	return true;
#endif
}

al::Decoder::Decoder(const std::shared_ptr<alure::Decoder> &decoder,const std::string &path)
	: impl::BufferBase(path),m_decoder(decoder)
{}

al::Decoder::~Decoder()
{
	m_decoder.reset();
}

uint32_t al::Decoder::GetFrequency() const {return m_decoder->getFrequency();}
al::ChannelConfig al::Decoder::GetChannelConfig() const {return static_cast<ChannelConfig>(m_decoder->getChannelConfig());}
al::SampleType al::Decoder::GetSampleType() const {return static_cast<SampleType>(m_decoder->getSampleType());}
uint64_t al::Decoder::GetLength() const {return m_decoder->getLength();}
std::pair<uint64_t,uint64_t> al::Decoder::GetLoopFramePoints() const {return m_decoder->getLoopPoints();}
bool al::Decoder::Seek(uint64_t frame) {return m_decoder->seek(frame);}

const std::shared_ptr<alure::Decoder> &al::Decoder::GetALDecoder() const {return const_cast<Decoder*>(this)->GetALDecoder();}
std::shared_ptr<alure::Decoder> &al::Decoder::GetALDecoder() {return m_decoder;}

al::IEffect::IEffect(SoundSystem &soundSys,alure::Effect *effect)
	: m_handle(this),m_soundSystem(soundSys),m_effect(effect)
{}
const alure::Effect &al::IEffect::GetALEffect() const {return const_cast<Effect*>(this)->GetALEffect();}
alure::Effect &al::IEffect::GetALEffect() {return *m_effect;}

al::IEffect::~IEffect()
{
	m_effect->destroy();
}

void al::IEffect::SetProperties(EfxEaxReverbProperties props)
{
	m_effect->setReverbProperties(reinterpret_cast<EFXEAXREVERBPROPERTIES&>(props));
}

al::AuxiliaryEffectSlot *al::IEffect::AttachSource(ISoundSource &source)
{
	auto it = std::find(m_attachedSources.begin(),m_attachedSources.end(),&source);
	if(it != m_attachedSources.end())
		return nullptr;
	if(m_slot == nullptr)
	{
		auto *slot = m_soundSystem.CreateAuxiliaryEffectSlot();
		if(slot == nullptr)
			return nullptr;
		m_slot = slot;
		m_slot->ApplyEffect(*this);
	}
	m_attachedSources.push_back(&source);
	return m_slot;
}
void al::IEffect::DetachSource(ISoundSource &source)
{
	auto it = std::find(m_attachedSources.begin(),m_attachedSources.end(),&source);
	if(it == m_attachedSources.end())
		return;
	m_attachedSources.erase(it);
	if(m_attachedSources.empty() == true && m_slot != nullptr)
	{
		m_soundSystem.FreeAuxiliaryEffectSlot(m_slot);
		m_slot = nullptr;
	}
}

#include <AL/alure2.h>
#include <effect.h>
#include <context.h>
#include <efx.h>

static alure::Context *apply_effect_type(alure::Effect *effect,int32_t newType,const std::string &typeName,ALuint &effectId)
{
	auto *alEffect = static_cast<alure::Effect*>(effect);
	auto *context = alEffect->getContext();
	if(context == nullptr)
		return nullptr;
	auto type = alEffect->getType();
	effectId = alEffect->getId();

	if(type != newType)
	{
		alGetError();
		context->alEffecti(effectId,AL_EFFECT_TYPE,newType);
		if(alGetError() == AL_NO_ERROR)
			alEffect->setType(newType);
		else
			throw std::runtime_error("Failed to set " +typeName +" type");
	}
	return context;
}

namespace al
{
	class FileFactory : public alure::FileIOFactory
	{
	public:
		FileFactory()=default;
		virtual ~FileFactory() override final=default;
		virtual alure::UniquePtr<std::istream> openFile(const alure::String &name) override final;
	};
};

~SoundSystem()
{
	alure::Context::MakeCurrent(nullptr);
	try
	{
		m_context->destroy();
	}
	catch(const std::exception &e)
	{
		std::cout<<"WARNING: Unable to destroy sound context: "<<e.what()<<std::endl;
	}
	try
	{
		m_device->close();
	}
	catch(const std::exception &e)
	{
		std::cout<<"WARNING: Unable to close sound device: "<<e.what()<<std::endl;
	}
}

al::ISoundSource::~ISoundSource()
{
	m_source->destroy();
}

namespace al
{
	class StreamBuf
		: public fsys::BaseStreamBuf
	{
	public:
		StreamBuf()=default;
		virtual ~StreamBuf() override final=default;
		virtual int_type underflow() override final;
		virtual pos_type seekoff(off_type offset,std::ios_base::seekdir whence,std::ios_base::openmode mode) override final;
		virtual pos_type seekpos(pos_type pos,std::ios_base::openmode mode) override final;
	private:
		using BufferArrayT = std::array<traits_type::char_type,4096>;
		BufferArrayT mBuffer;
	};
};

std::streambuf::pos_type al::StreamBuf::seekoff(off_type offset,std::ios_base::seekdir whence,std::ios_base::openmode mode)
{
	if(!mFile || (mode &std::ios_base::out) || !(mode &std::ios_base::in))
		return traits_type::eof();

	// PhysFS only seeks using absolute offsets, so we have to convert cur-
	// and end-relative offsets.
	uint64_t fpos;
	switch(whence)
	{
		case std::ios_base::beg:
			break;
		case std::ios_base::cur:
			// Need to offset for the read pointers with std::ios_base::cur
			// regardless
			offset -= off_type(egptr() -gptr());
			if((fpos = mFile->Tell()) == -1)
				return traits_type::eof();
			offset += fpos;
			break;
		case std::ios_base::end:
			if((fpos = mFile->GetSize()) == -1)
				return traits_type::eof();
			offset += fpos;
			break;
		default:
			return traits_type::eof();
	}
	// Range check - absolute offsets cannot be less than 0, nor be greater
	// than PhysFS's offset type.
	if(offset < 0 || offset >= std::numeric_limits<decltype(fpos)>::max())
		return traits_type::eof();
	mFile->Seek(offset);
	if(mFile->Eof() == true)
		return traits_type::eof();
	// Clear read pointers so underflow() gets called on the next read
	// attempt.
	setg(0,0,0);
	return offset;
}

std::streambuf::pos_type al::StreamBuf::seekpos(pos_type pos,std::ios_base::openmode mode)
{
	// Simplified version of seekoff
	if(!mFile || (mode&std::ios_base::out) || !(mode&std::ios_base::in))
		return traits_type::eof();

	if(pos < 0)
		return traits_type::eof();
	mFile->Seek(pos);
	if(mFile->Eof() == true)
		return traits_type::eof();
	setg(0,0,0);
	return pos;
}

std::streambuf::int_type al::StreamBuf::underflow()
{
	if(mFile && gptr() == egptr())
	{
		// Read in the next chunk of data, and set the read pointers on
		// success
		auto got = mFile->Read(mBuffer.data(),sizeof(BufferArrayT::value_type),mBuffer.size());
		if(got != std::numeric_limits<decltype(got)>::max())
			setg(mBuffer.data(),mBuffer.data(),mBuffer.data() +got);
	}
	if(gptr() == egptr())
		return traits_type::eof();
	return traits_type::to_int_type(*gptr());
}

std::vector<std::string> al::get_devices()
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	auto &devMgr = alure::DeviceManager::get();
	return devMgr.enumerate(alure::DeviceEnumeration::Basic);
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
	return {};
#endif
}

std::string al::get_default_device_name()
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	auto &devMgr = alure::DeviceManager::get();
	return devMgr.defaultDeviceName(alure::DefaultDeviceType::Basic);
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
	return "";
#endif
}

///////////////////

alure::UniquePtr<std::istream> al::FileFactory::openFile(const alure::String &name)
{
	auto stream = alure::MakeUnique<fsys::Stream>(name.c_str(),new al::StreamBuf());
	if(stream->fail())
		stream = nullptr;
	return std::move(stream);
}

extern "C"
{
	__declspec(dllexport) bool initialize_audio_api(float metersPerUnit,std::shared_ptr<al::ISoundSystem> &outSoundSystem,std::string &errMsg)
	{
		outSoundSystem = al::AlureSoundSystem::Create(metersPerUnit,[]() {
			sys->OnRelease();
			delete sys;
		});
		return outSoundSystem != nullptr;
	}
};
