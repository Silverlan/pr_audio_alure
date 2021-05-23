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
class AlureSoundSystem
{
	public:
		const alure::Device *GetALDevice() const;
		alure::Device *GetALDevice();
		const alure::Context *GetALContext() const;
		alure::Context *GetALContext();
	private:
		SoundSystem(alure::Device *device,alure::Context *context,float metersPerUnit);
		alure::Device *m_device = nullptr;
		alure::Context *m_context = nullptr;
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
		PSoundSource CreateSource(Decoder &decoder);
#endif
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
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

class AlureListener
{
		Listener(alure::Listener &listener);
		alure::Listener &m_listener;
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

void al::ISoundSource::SetEffectParameters(uint32_t slotId,const IEffect::Params &params)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_source->setSendFilter(slotId,reinterpret_cast<const alure::FilterParams&>(params));
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
#endif
}

void al::ISoundSource::SetDirectFilter(const IEffect::Params &params)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_source->setDirectFilter(reinterpret_cast<const alure::FilterParams&>(m_directFilter = params));
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
#endif
}

void al::ISoundSource::DoRemoveInternalEffect(uint32_t slotId)
{
	m_source->setAuxiliarySend(nullptr,slotId);
}

void al::ISoundSource::DoRemoveEffect(uint32_t slotId)
{
	m_source->setAuxiliarySend(nullptr,slotId);
}

virtual void ISoundSource::DoAddEffect(AuxiliaryEffectSlot &slot,uint32_t slotId,const IEffect::Params &params)
{
	m_source->setAuxiliarySendFilter(slot->GetALSlot(),slotId,reinterpret_cast<const alure::FilterParams&>(params));
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
al::Listener::Listener(alure::Listener &listener)
	: m_listener(listener)
{}

void al::IListener::DoSetMetersPerUnit(float mu)
{
	m_listener.setMetersPerUnit(m_metersPerUnit);
}

al::PDecoder al::ISoundSystem::CreateDecoder(const std::string &path,bool bConvertToMono)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	auto normPath = FileManager::GetNormalizedPath(path);

	auto userData = std::make_shared<impl::BufferLoadData>(*this);
	if(bConvertToMono == true)
		userData->flags |= impl::BufferLoadData::Flags::ConvertToMono;
#if ALSYS_STEAM_AUDIO_SUPPORT_ENABLED == 1
	if(IsSteamAudioEnabled() == true)
		userData->flags |= impl::BufferLoadData::Flags::SingleSourceDecoder; // TODO
#endif
	auto originalChannel = ChannelConfig::Mono;
	auto alDecoder = m_context->createDecoder(normPath,userData,nullptr,reinterpret_cast<alure::ChannelConfig*>(&originalChannel));
	if(alDecoder == nullptr)
		return nullptr;
	auto r = PDecoder(new Decoder(alDecoder,normPath));
	if(bConvertToMono == true)
		r->SetTargetChannelConfig(al::ChannelConfig::Mono);
	return r;
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	return nullptr;
#endif
}

al::PSoundSource al::ISoundSystem::CreateSource(Decoder &decoder)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	alure::Source *src = nullptr;
	try {src = m_context->createSource();}
	catch(const std::runtime_error &err) {}
	if(src == nullptr)
		return nullptr;
	auto *snd = m_soundSourceFactory->CreateSoundSource(*this,decoder,src);
	auto r = InitializeSource(snd);
	if(r == nullptr)
		return r;
	auto userData = decoder.GetALDecoder()->userData;
	if(userData != nullptr)
	{
		auto &loadData = *static_cast<impl::BufferLoadData*>(userData.get());
		if((loadData.flags &impl::BufferLoadData::Flags::SingleSourceDecoder) != impl::BufferLoadData::Flags::None)
			loadData.userData = std::make_shared<al::SoundSourceHandle>(r->GetHandle());
	}
	return r;
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	return nullptr; // FMOD TODO
#endif
}

std::vector<std::string> al::ISoundSystem::GetHRTFNames() const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	return m_device->enumerateHRTFNames();
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	return {}; // FMOD TODO
#endif
}

std::string al::ISoundSystem::GetCurrentHRTF() const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	return m_device->getCurrentHRTF();
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	return ""; // FMOD TODO
#endif
}
bool al::ISoundSystem::IsHRTFEnabled() const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	return m_device->isHRTFEnabled();
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	return false; // FMOD TODO
#endif
}

void al::ISoundSystem::SetHRTF(uint32_t id)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	if(m_device->queryExtension("ALC_SOFT_HRTF") == false)
		return;
	m_device->reset({
		{ALC_HRTF_SOFT,ALC_TRUE},
		{ALC_HRTF_ID_SOFT,id},
		{0,0}
	});
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
#endif
}
void al::ISoundSystem::DisableHRTF()
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	if(m_device->queryExtension("ALC_SOFT_HRTF") == false)
		return;
	m_device->reset({
		{ALC_HRTF_SOFT,ALC_FALSE},
		{0,0}
	});
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
#endif
}

uint32_t al::ISoundSystem::GetMaxAuxiliaryEffectsPerSource() const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	return m_device->getMaxAuxiliarySends();
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	return 0u; // FMOD TODO
#endif
}

bool al::ISoundSystem::IsSupported(ChannelConfig channels,SampleType type) const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	return m_context->isSupported(static_cast<alure::ChannelConfig>(channels),static_cast<alure::SampleType>(type));
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	return true; // FMOD TODO
#endif
}

float al::ISoundSystem::GetDopplerFactor() const {return m_dopplerFactor;}
void al::ISoundSystem::SetDopplerFactor(float factor)
{
	m_dopplerFactor = factor;
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_context->setDopplerFactor(m_dopplerFactor);
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
#endif
}

float al::ISoundSystem::GetSpeedOfSound() const {return m_speedOfSound;}
void al::ISoundSystem::SetSpeedOfSound(float speed)
{
	m_speedOfSound = speed;
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_context->setSpeedOfSound(m_speedOfSound);
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
#endif
}

std::string al::ISoundSystem::GetDeviceName() const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	return m_device->getName();
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	return "";// FMOD TODO
#endif
}

void al::ISoundSystem::PauseDeviceDSP()
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_device->pauseDSP();
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
#endif
}
void al::ISoundSystem::ResumeDeviceDSP()
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_device->resumeDSP();
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
#endif
}

al::AuxiliaryEffectSlot *al::ISoundSystem::CreateAuxiliaryEffectSlot()
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	alure::AuxiliaryEffectSlot *effectSlot = nullptr;
	try
	{
		effectSlot = m_context->createAuxiliaryEffectSlot();
	}
	catch(const std::runtime_error &err)
	{}
	if(effectSlot == nullptr)
		return nullptr;
	auto r = std::shared_ptr<AuxiliaryEffectSlot>(new AuxiliaryEffectSlot(effectSlot));
	m_effectSlots.push_back(r);
	return r.get();
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	return nullptr; // FMOD TODO
#endif
}

al::DistanceModel al::ISoundSystem::GetDistanceModel() const {return m_distanceModel;}
void al::ISoundSystem::SetDistanceModel(DistanceModel mdl)
{
	m_distanceModel = mdl;
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_context->setDistanceModel(static_cast<alure::DistanceModel>(m_distanceModel));
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
#endif
}

al::PEffect al::ISoundSystem::CreateEffect()
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	alure::Effect *effect = nullptr;
	try
	{
		effect = m_context->createEffect();
	}
	catch(const std::runtime_error &err)
	{}
	if(effect == nullptr)
		return nullptr;
	return std::shared_ptr<Effect>(new Effect(*this,effect),[](Effect *effect) {
		effect->Release();
	});
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	return nullptr; // FMOD TODO
#endif
}

al::ISoundSystem::ISoundSystem(alure::Device *device,alure::Context *context,float metersPerUnit)
	: m_device(device),m_context(context),m_listener(*context->getListener())
{
	m_listener.SetMetersPerUnit(metersPerUnit);
	SetSpeedOfSound(340.29f /metersPerUnit);
	SetSoundSourceFactory(std::make_unique<al::DefaultSoundSourceFactory>());
}
const alure::Device *al::ISoundSystem::GetALDevice() const {return const_cast<SoundSystem*>(this)->GetALDevice();}
alure::Device *al::ISoundSystem::GetALDevice() {return m_device;}
const alure::Context *al::ISoundSystem::GetALContext() const {return const_cast<SoundSystem*>(this)->GetALContext();}
alure::Context *al::ISoundSystem::GetALContext() {return m_context;}

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

void al::FMEffect::SetProperties(al::EfxChorusProperties props)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	ALuint effectId;
	auto *context = apply_effect_type(m_effect,AL_EFFECT_CHORUS,"chorus",effectId);
	if(context == nullptr)
		return;
	context->alEffecti(effectId,AL_CHORUS_WAVEFORM,props.iWaveform);
	context->alEffecti(effectId,AL_CHORUS_PHASE,props.iPhase);
	context->alEffectf(effectId,AL_CHORUS_RATE,props.flRate);
	context->alEffectf(effectId,AL_CHORUS_DEPTH,props.flDepth);
	context->alEffectf(effectId,AL_CHORUS_FEEDBACK,props.flFeedback);
	context->alEffectf(effectId,AL_CHORUS_DELAY,props.flDelay);
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	FMOD::DSP *dsp;
	al::fmod::check_result(static_cast<FMSoundSystem&>(m_soundSystem).GetFMODLowLevelSystem().createDSPByType(FMOD_DSP_TYPE_CHORUS,&dsp));
	m_fmDsp = std::shared_ptr<FMOD::DSP>(dsp,[](FMOD::DSP *dsp) {
		al::fmod::check_result(dsp->release());
	});
	dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY,props.flDelay);
	dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK,props.flFeedback);
	//dsp->setParameterFloat(FMOD_DSP_ECHO_DRYLEVEL,props.);
	//dsp->setParameterFloat(FMOD_DSP_ECHO_WETLEVEL,props.);
#endif
}

void al::FMEffect::SetProperties(al::EfxEaxReverbProperties props)
{
	// TODO
}

void al::FMEffect::SetProperties(al::EfxDistortionProperties props)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	ALuint effectId;
	auto *context = apply_effect_type(m_effect,AL_EFFECT_DISTORTION,"distortion",effectId);
	if(context == nullptr)
		return;
	context->alEffectf(effectId,AL_DISTORTION_EDGE,props.flEdge);
	context->alEffectf(effectId,AL_DISTORTION_GAIN,props.flGain);
	context->alEffectf(effectId,AL_DISTORTION_LOWPASS_CUTOFF,props.flLowpassCutoff);
	context->alEffectf(effectId,AL_DISTORTION_EQCENTER,props.flEQCenter);
	context->alEffectf(effectId,AL_DISTORTION_EQBANDWIDTH,props.flEQBandwidth);
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	FMOD::DSP *dsp;
	al::fmod::check_result(static_cast<FMSoundSystem&>(m_soundSystem).GetFMODLowLevelSystem().createDSPByType(FMOD_DSP_TYPE_DISTORTION,&dsp));
	m_fmDsp = std::shared_ptr<FMOD::DSP>(dsp,[](FMOD::DSP *dsp) {
		al::fmod::check_result(dsp->release());
	});
	dsp->setParameterFloat(FMOD_DSP_DISTORTION_LEVEL,props.flGain);
#endif
}
void al::FMEffect::SetProperties(al::EfxEchoProperties props)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	ALuint effectId;
	auto *context = apply_effect_type(m_effect,AL_EFFECT_ECHO,"echo",effectId);
	if(context == nullptr)
		return;
	context->alEffectf(effectId,AL_ECHO_DELAY,props.flDelay);
	context->alEffectf(effectId,AL_ECHO_LRDELAY,props.flLRDelay);
	context->alEffectf(effectId,AL_ECHO_DAMPING,props.flDamping);
	context->alEffectf(effectId,AL_ECHO_FEEDBACK,props.flFeedback);
	context->alEffectf(effectId,AL_ECHO_SPREAD,props.flSpread);
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	FMOD::DSP *dsp;
	al::fmod::check_result(static_cast<FMSoundSystem&>(m_soundSystem).GetFMODLowLevelSystem().createDSPByType(FMOD_DSP_TYPE_ECHO,&dsp));
	m_fmDsp = std::shared_ptr<FMOD::DSP>(dsp,[](FMOD::DSP *dsp) {
		al::fmod::check_result(dsp->release());
	});
	dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY,props.flDelay);
	dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK,props.flFeedback);
	//dsp->setParameterFloat(FMOD_DSP_ECHO_DRYLEVEL,props.);
	//dsp->setParameterFloat(FMOD_DSP_ECHO_WETLEVEL,props.);
#endif
}
void al::FMEffect::SetProperties(al::EfxFlangerProperties props)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	ALuint effectId;
	auto *context = apply_effect_type(m_effect,AL_EFFECT_FLANGER,"flanger",effectId);
	if(context == nullptr)
		return;
	context->alEffecti(effectId,AL_FLANGER_WAVEFORM,props.iWaveform);
	context->alEffecti(effectId,AL_FLANGER_PHASE,props.iPhase);
	context->alEffectf(effectId,AL_FLANGER_RATE,props.flRate);
	context->alEffectf(effectId,AL_FLANGER_DEPTH,props.flDepth);
	context->alEffectf(effectId,AL_FLANGER_FEEDBACK,props.flFeedback);
	context->alEffectf(effectId,AL_FLANGER_DELAY,props.flDelay);
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	FMOD::DSP *dsp;
	al::fmod::check_result(static_cast<FMSoundSystem&>(m_soundSystem).GetFMODLowLevelSystem().createDSPByType(FMOD_DSP_TYPE_FLANGE,&dsp));
	m_fmDsp = std::shared_ptr<FMOD::DSP>(dsp,[](FMOD::DSP *dsp) {
		al::fmod::check_result(dsp->release());
	});
	//dsp->setParameterFloat(FMOD_DSP_FLANGE_MIX,props.); // FMOD TODO
	dsp->setParameterFloat(FMOD_DSP_FLANGE_DEPTH,props.flDepth);
	dsp->setParameterFloat(FMOD_DSP_FLANGE_RATE,props.flRate);
#endif
}
void al::FMEffect::SetProperties(al::EfxFrequencyShifterProperties props)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	ALuint effectId;
	auto *context = apply_effect_type(m_effect,AL_EFFECT_FREQUENCY_SHIFTER,"frequency shifter",effectId);
	if(context == nullptr)
		return;
	context->alEffectf(effectId,AL_FREQUENCY_SHIFTER_FREQUENCY,props.flFrequency);
	context->alEffecti(effectId,AL_FREQUENCY_SHIFTER_LEFT_DIRECTION,props.iLeftDirection);
	context->alEffecti(effectId,AL_FREQUENCY_SHIFTER_RIGHT_DIRECTION,props.iRightDirection);
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
#endif
}
void al::FMEffect::SetProperties(al::EfxVocalMorpherProperties props)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	ALuint effectId;
	auto *context = apply_effect_type(m_effect,AL_EFFECT_VOCAL_MORPHER,"vocal morpher",effectId);
	if(context == nullptr)
		return;
	context->alEffecti(effectId,AL_VOCAL_MORPHER_PHONEMEA,props.iPhonemeA);
	context->alEffecti(effectId,AL_VOCAL_MORPHER_PHONEMEB,props.iPhonemeB);
	context->alEffecti(effectId,AL_VOCAL_MORPHER_PHONEMEA_COARSE_TUNING,props.iPhonemeACoarseTuning);
	context->alEffecti(effectId,AL_VOCAL_MORPHER_PHONEMEB_COARSE_TUNING,props.iPhonemeBCoarseTuning);
	context->alEffecti(effectId,AL_VOCAL_MORPHER_WAVEFORM,props.iWaveform);
	context->alEffectf(effectId,AL_VOCAL_MORPHER_RATE,props.flRate);
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
#endif
}
void al::FMEffect::SetProperties(al::EfxPitchShifterProperties props)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	ALuint effectId;
	auto *context = apply_effect_type(m_effect,AL_EFFECT_PITCH_SHIFTER,"pitch shifter",effectId);
	if(context == nullptr)
		return;
	context->alEffecti(effectId,AL_PITCH_SHIFTER_COARSE_TUNE,props.iCoarseTune);
	context->alEffecti(effectId,AL_PITCH_SHIFTER_FINE_TUNE,props.iFineTune);
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
#endif
}
void al::FMEffect::SetProperties(al::EfxRingModulatorProperties props)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	ALuint effectId;
	auto *context = apply_effect_type(m_effect,AL_EFFECT_RING_MODULATOR,"ring modulator",effectId);
	if(context == nullptr)
		return;
	context->alEffectf(effectId,AL_RING_MODULATOR_FREQUENCY,props.flFrequency);
	context->alEffectf(effectId,AL_RING_MODULATOR_HIGHPASS_CUTOFF,props.flHighpassCutoff);
	context->alEffecti(effectId,AL_RING_MODULATOR_WAVEFORM,props.iWaveform);
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
#endif
}
void al::FMEffect::SetProperties(al::EfxAutoWahProperties props)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	ALuint effectId;
	auto *context = apply_effect_type(m_effect,AL_EFFECT_AUTOWAH,"auto wah",effectId);
	if(context == nullptr)
		return;
	context->alEffectf(effectId,AL_AUTOWAH_ATTACK_TIME,props.flAttackTime);
	context->alEffectf(effectId,AL_AUTOWAH_RELEASE_TIME,props.flReleaseTime);
	context->alEffectf(effectId,AL_AUTOWAH_RESONANCE,props.flResonance);
	context->alEffectf(effectId,AL_AUTOWAH_PEAK_GAIN,props.flPeakGain);
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
#endif
}
void al::FMEffect::SetProperties(al::EfxCompressor props)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	ALuint effectId;
	auto *context = apply_effect_type(m_effect,AL_EFFECT_COMPRESSOR,"compressor",effectId);
	if(context == nullptr)
		return;
	context->alEffecti(effectId,AL_COMPRESSOR_ONOFF,props.iOnOff);
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
#endif
}
void al::FMEffect::SetProperties(al::EfxEqualizer props)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	ALuint effectId;
	auto *context = apply_effect_type(m_effect,AL_EFFECT_EQUALIZER,"equalizer",effectId);
	if(context == nullptr)
		return;
	context->alEffectf(effectId,AL_EQUALIZER_LOW_GAIN,props.flLowGain);
	context->alEffectf(effectId,AL_EQUALIZER_LOW_CUTOFF,props.flLowCutoff);
	context->alEffectf(effectId,AL_EQUALIZER_MID1_GAIN,props.flMid1Gain);
	context->alEffectf(effectId,AL_EQUALIZER_MID1_CENTER,props.flMid1Center);
	context->alEffectf(effectId,AL_EQUALIZER_MID1_WIDTH,props.flMid1Width);
	context->alEffectf(effectId,AL_EQUALIZER_MID2_GAIN,props.flMid2Gain);
	context->alEffectf(effectId,AL_EQUALIZER_MID2_CENTER,props.flMid2Center);
	context->alEffectf(effectId,AL_EQUALIZER_MID2_WIDTH,props.flMid2Width);
	context->alEffectf(effectId,AL_EQUALIZER_HIGH_GAIN,props.flHighGain);
	context->alEffectf(effectId,AL_EQUALIZER_HIGH_CUTOFF,props.flHighCutoff);
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
#endif
}

al::FMSoundBuffer::FMSoundBuffer(alure::Context &context,alure::Buffer *buffer,const std::string &path)
	: impl::BufferBase(path),m_context(context),m_buffer(buffer)
{}
alure::Buffer *al::FMSoundBuffer::GetALBuffer() {return m_buffer;}

al::FMSoundBuffer::~FMSoundBuffer()
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_context.removeBuffer(*m_buffer);
#endif
}

bool al::FMSoundBuffer::IsReady() const
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

uint64_t al::FMSoundBuffer::GetLength() const
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
uint32_t al::FMSoundBuffer::GetFrequency() const
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
al::ChannelConfig al::FMSoundBuffer::GetChannelConfig() const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	return static_cast<al::ChannelConfig>(m_buffer->getChannelConfig());
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	int32_t channels;
	al::fmod::check_result(m_fmSound->getFormat(nullptr,nullptr,&channels,nullptr));
	return (channels >= 2) ? al::ChannelConfig::Stereo : al::ChannelConfig::Mono;
#endif
}
al::SampleType al::FMSoundBuffer::GetSampleType() const
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
uint32_t al::FMSoundBuffer::GetSize() const
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
void al::FMSoundBuffer::SetLoopFramePoints(uint32_t start,uint32_t end)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	if(IsReady() == false)
		return; // TODO: Set when ready
	m_buffer->setLoopPoints(start,end);
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	al::fmod::check_result(m_fmSound->setLoopPoints(start,FMOD_TIMEUNIT_PCM,end,FMOD_TIMEUNIT_PCM));
#endif
}
void al::FMSoundBuffer::SetLoopTimePoints(float tStart,float tEnd)
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
std::pair<uint64_t,uint64_t> al::FMSoundBuffer::GetLoopFramePoints() const
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

std::string al::FMSoundBuffer::GetName() const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	return m_buffer->getName().data();
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
	static std::string r = "";
	return r;
#endif
}
bool al::FMSoundBuffer::IsInUse() const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	return m_buffer->getSourceCount() > 0;
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
	return true;
#endif
}

al::FMListener::FMListener(al::ISoundSystem &system)
	: IListener{system}
{}
void al::FMListener::SetGain(float gain)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_listener.setGain(gain);
#endif
}
void al::FMListener::SetPosition(const Vector3 &pos)
{
	auto posAudio = al::to_audio_position(pos);
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_listener.setPosition(&posAudio[0]);
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	auto &fmodSys = static_cast<FMSoundSystem&>(m_soundSystem).GetFMODSystem();
	FMOD_3D_ATTRIBUTES attributes;
	al::fmod::check_result(fmodSys.getListenerAttributes(0,&attributes));
	attributes.position = {posAudio.x,posAudio.y,posAudio.z};
	al::fmod::check_result(fmodSys.setListenerAttributes(0,&attributes));
#endif
}
void al::FMListener::SetVelocity(const Vector3 &vel)
{
	auto velAudio = al::to_audio_position(vel);
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_listener.setVelocity(&velAudio[0]);
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	auto &fmodSys = static_cast<FMSoundSystem&>(m_soundSystem).GetFMODSystem();
	FMOD_3D_ATTRIBUTES attributes;
	al::fmod::check_result(fmodSys.getListenerAttributes(0,&attributes));
	attributes.velocity = {velAudio.x,velAudio.y,velAudio.z};
	al::fmod::check_result(fmodSys.setListenerAttributes(0,&attributes));
#endif
}
void al::FMListener::SetOrientation(const Vector3 &at,const Vector3 &up)
{
	auto atAudio = al::to_audio_direction(at);
	auto atUp = al::to_audio_direction(up);
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_listener.setOrientation(&atAudio[0],&atUp[0]);
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	auto &fmodSys = static_cast<FMSoundSystem&>(m_soundSystem).GetFMODSystem();
	FMOD_3D_ATTRIBUTES attributes;
	al::fmod::check_result(fmodSys.getListenerAttributes(0,&attributes));
	attributes.forward = {atAudio.x,atAudio.y,atAudio.z};
	attributes.up = {atUp.x,atUp.y,atUp.z};
	al::fmod::check_result(fmodSys.setListenerAttributes(0,&attributes));
#endif
}

std::shared_ptr<al::FMSoundSystem> al::FMSoundSystem::Create(const std::string &deviceName,float metersPerUnit)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	auto &devMgr = alure::DeviceManager::get();
	auto *dev = devMgr.openPlayback();
	if(dev == nullptr)
		return nullptr;
	auto *context = dev->createContext();
	if(context == nullptr)
	{
		dev->close();
		return nullptr;
	}
	alure::FileIOFactory::set(std::make_unique<al::FileFactory>());

	alure::Context::MakeCurrent(context);
#ifdef _DEBUG
	context->setMessageHandler(std::make_shared<MessageHandler>());
#endif

	context->setBufferDataCallback([](uint8_t *inputData,std::vector<uint8_t> &outputData,int32_t &format,alure::ChannelConfig &channel,uint32_t &frequency,uint32_t dataLen,uint32_t fragmentSize,const std::shared_ptr<void> &userData) -> bool {
		if(userData == nullptr)
			return false;
		auto &loadData = *static_cast<impl::BufferLoadData*>(userData.get());
#if ALSYS_STEAM_AUDIO_SUPPORT_ENABLED == 1
		if(channel == alure::ChannelConfig::Mono && loadData.soundSystem.m_iplScene != nullptr)
			return apply_steam_audio_processing(*loadData.soundSystem.m_iplScene,loadData,inputData,outputData,format,channel,frequency,dataLen);
#endif
		if(channel != alure::ChannelConfig::Stereo || (loadData.flags &impl::BufferLoadData::Flags::ConvertToMono) == impl::BufferLoadData::Flags::None)
			return false;
		switch(format)
		{
			case AL_STEREO8_SOFT:
				stereo_to_mono<int8_t,int16_t>(reinterpret_cast<int8_t*>(inputData),outputData,dataLen);
				format = AL_MONO8_SOFT;
				break;
			case AL_STEREO16_SOFT:
				stereo_to_mono<int16_t,int32_t>(reinterpret_cast<int16_t*>(inputData),outputData,dataLen);
				format = AL_MONO16_SOFT;
				break;
			case AL_STEREO32F_SOFT:
				stereo_to_mono<float,double>(reinterpret_cast<float*>(inputData),outputData,dataLen);
				format = AL_MONO32F_SOFT;
				break;
			default:
				return false;
		}
		channel = static_cast<alure::ChannelConfig>(ChannelConfig::Mono);
#if ALSYS_STEAM_AUDIO_SUPPORT_ENABLED == 1
		apply_steam_audio_processing(*loadData.soundSystem.m_iplScene,loadData,outputData.data(),outputData,format,channel,frequency,outputData.size());
#endif
		return true;
	});

	auto r = std::shared_ptr<SoundSystem>(new SoundSystem(dev,context,metersPerUnit));
	return r;
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	FMOD::Studio::System *system = nullptr;
	al::fmod::check_result(FMOD::Studio::System::create(&system));
	auto ptrSystem = std::shared_ptr<FMOD::Studio::System>(system,[](FMOD::Studio::System *system) {
		al::fmod::check_result(system->release());
	});

	FMOD::System *lowLevelSystem = nullptr;
	al::fmod::check_result(system->getLowLevelSystem(&lowLevelSystem));
	al::fmod::check_result(lowLevelSystem->setSoftwareFormat(0,FMOD_SPEAKERMODE_5POINT1,0));

	void *extraDriverData = nullptr;
	al::fmod::check_result(system->initialize(1'024,FMOD_STUDIO_INIT_NORMAL,FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED | FMOD_INIT_VOL0_BECOMES_VIRTUAL,extraDriverData));
	al::fmod::check_result(lowLevelSystem->setFileSystem(
		[](const char *name,uint32_t *fileSize,void **handle,void *userData) -> FMOD_RESULT {
			auto f = FileManager::OpenFile(name,"rb");
			if(f == nullptr)
				return FMOD_RESULT::FMOD_ERR_FILE_NOTFOUND;
			*fileSize = f->GetSize();
			*handle = new VFilePtr(f);
			return FMOD_RESULT::FMOD_OK;
		},[](void *handle,void *userData) -> FMOD_RESULT {
			delete static_cast<VFilePtr*>(handle);
			return FMOD_RESULT::FMOD_OK;
		},[](void *handle,void *buffer,uint32_t sizeBytes,uint32_t *bytesRead,void *userData) -> FMOD_RESULT {
			*bytesRead = (*static_cast<VFilePtr*>(handle))->Read(buffer,sizeBytes);
			if((*static_cast<VFilePtr*>(handle))->Eof())
				return FMOD_RESULT::FMOD_ERR_FILE_EOF;
			return FMOD_RESULT::FMOD_OK;
		},[](void *handle,uint32_t pos,void *userData) -> FMOD_RESULT {
			(*static_cast<VFilePtr*>(handle))->Seek(pos);
			return FMOD_RESULT::FMOD_OK;
		},nullptr,nullptr,-1
	));
	auto soundSys = std::shared_ptr<FMSoundSystem>(new FMSoundSystem(ptrSystem,*lowLevelSystem,metersPerUnit));
	soundSys->Initialize();
	return soundSys;
#endif
}

void al::FMSoundSystem::Update()
{
	ISoundSystem::Update();
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_context->update();
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	al::fmod::check_result(m_fmSystem->update());
#endif
}

std::shared_ptr<al::FMSoundSystem> al::FMSoundSystem::Create(float metersPerUnit) {return Create("",metersPerUnit);}
const FMOD::Studio::System &al::FMSoundSystem::GetFMODSystem() const {return const_cast<FMSoundSystem*>(this)->GetFMODSystem();}
FMOD::Studio::System &al::FMSoundSystem::GetFMODSystem() {return *m_fmSystem;}
const FMOD::System &al::FMSoundSystem::GetFMODLowLevelSystem() const {return const_cast<FMSoundSystem*>(this)->GetFMODLowLevelSystem();}
FMOD::System &al::FMSoundSystem::GetFMODLowLevelSystem() {return m_fmLowLevelSystem;}
al::FMSoundSystem::FMSoundSystem(const std::shared_ptr<FMOD::Studio::System> &fmSystem,FMOD::System &lowLevelSystem,float metersPerUnit)
	: ISoundSystem{},m_fmSystem(fmSystem),m_fmLowLevelSystem(lowLevelSystem)
{
	lowLevelSystem.set3DSettings(1.f,1.f,1.f);
	// FMOD TODO
	//SetSpeedOfSound(340.29f /metersPerUnit);
	SetSoundSourceFactory(std::make_unique<al::FMSoundSourceFactory>());
}

std::unique_ptr<al::SoundSourceFactory> al::FMSoundSystem::InitializeSoundSourceFactory()
{
	return std::make_unique<FMSoundSourceFactory>();
}

std::unique_ptr<al::IListener> al::FMSoundSystem::CreateListener() {return std::unique_ptr<FMListener>{new FMListener{*this}};}

al::ISoundBuffer *al::FMSoundSystem::DoLoadSound(const std::string &normPath,bool bConvertToMono,bool bAsync)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	alure::Buffer alBuf = nullptr;
	auto userData = std::make_shared<impl::BufferLoadData>(*this);
	if(bConvertToMono == true)
		userData->flags |= impl::BufferLoadData::Flags::ConvertToMono;
	auto originalChannel = ChannelConfig::Mono;
	if(bAsync == false)
		alBuf = m_context->getBuffer(normPath,userData,false,nullptr,reinterpret_cast<alure::ChannelConfig*>(&originalChannel));
	else
		alBuf = m_context->getBufferAsync(normPath,userData,false,nullptr,reinterpret_cast<alure::ChannelConfig*>(&originalChannel));
	if(alBuf == nullptr)
		return nullptr;
	auto buf = PSoundBuffer(new SoundBuffer(*m_context,alBuf,normPath));

	if(it == m_buffers.end())
		it = m_buffers.insert(decltype(m_buffers)::value_type({normPath,{}})).first;
	if(buf->GetChannelConfig() == al::ChannelConfig::Mono || bConvertToMono == true)
		it->second.mono = buf;
	else
		it->second.stereo = buf;
	if(originalChannel == ChannelConfig::Mono)
		it->second.fileSourceMonoOnly = true;
	if(bConvertToMono == true)
		buf->SetTargetChannelConfig(al::ChannelConfig::Mono);
	return buf.get();
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	FMOD::Sound *sound;
	FMOD_CREATESOUNDEXINFO exInfo {};
	memset(&exInfo,0,sizeof(exInfo));
	exInfo.cbsize = sizeof(exInfo);
	al::fmod::check_result(m_fmLowLevelSystem.createSound(normPath.c_str(),FMOD_DEFAULT,&exInfo,&sound));
	auto ptrSound = std::shared_ptr<FMOD::Sound>(sound,[](FMOD::Sound *sound) {
		al::fmod::check_result(sound->release());
	});
	auto buf = PSoundBuffer(new FMSoundBuffer(m_fmLowLevelSystem,ptrSound));
	if(buf->GetChannelConfig() == al::ChannelConfig::Mono || bConvertToMono == true)
		m_buffers[normPath].mono = buf;
	else
		m_buffers[normPath].stereo = buf;
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	if(originalChannel == ChannelConfig::Mono)
		it->second.fileSourceMonoOnly = true; // FMOD TODO
#endif
	if(bConvertToMono == true)
		buf->SetTargetChannelConfig(al::ChannelConfig::Mono);
	return buf.get();
#endif
}

al::PSoundSource al::FMSoundSystem::CreateSource(ISoundBuffer &buffer)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
#if ALSYS_STEAM_AUDIO_SUPPORT_ENABLED == 1
	if(IsSteamAudioEnabled() && buffer.GetTargetChannelConfig() == al::ChannelConfig::Mono)
	{
		// Redirect to steam audio decoder (Can't use buffers with steam audio)
		auto r = CreateSource(buffer.GetFilePath(),false,Type::Buffer);
		if(r != nullptr)
			return r;
	}
#endif
	alure::Source *src = nullptr;
	try {src = m_context->createSource();}
	catch(const std::runtime_error &err) {}
	if(src == nullptr)
		return nullptr;
	auto *snd = m_soundSourceFactory->CreateSoundSource(*this,buffer,src);
	return InitializeSource(snd);
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	FMOD::Channel *channel;
	al::fmod::check_result(m_fmLowLevelSystem.playSound(static_cast<FMSoundBuffer&>(buffer).GetFMODSound(),nullptr,true,&channel));
	auto *snd = m_soundSourceFactory->CreateSoundSource(*this,buffer,channel);
	if(snd == nullptr)
		return nullptr;
	FMOD_MODE mode;
	al::fmod::check_result(channel->getMode(&mode));
	mode &= ~(FMOD_3D_HEADRELATIVE | FMOD_3D_WORLDRELATIVE | FMOD_3D | FMOD_2D);
	mode |= FMOD_3D_WORLDRELATIVE | FMOD_3D;

	channel->setMode(mode);
	return InitializeSource(snd);
#endif
}

al::FMSoundSystem::ISoundSystem(alure::Device *device,alure::Context *context,float metersPerUnit)
	: m_device(device),m_context(context),m_listener(*context->getListener())
{
	m_listener.SetMetersPerUnit(metersPerUnit);
	SetSpeedOfSound(340.29f /metersPerUnit);
	SetSoundSourceFactory(std::make_unique<al::DefaultSoundSourceFactory>());
}
const alure::Device *al::FMSoundSystem::GetALDevice() const {return const_cast<SoundSystem*>(this)->GetALDevice();}
alure::Device *al::FMSoundSystem::GetALDevice() {return m_device;}
const alure::Context *al::FMSoundSystem::GetALContext() const {return const_cast<SoundSystem*>(this)->GetALContext();}
alure::Context *al::FMSoundSystem::GetALContext() {return m_context;}































al::FMSoundSource::FMSoundSource(ISoundSystem &system,ISoundBuffer &buffer,FMOD::Channel *source)
	: ISoundSource(system,buffer),m_source{source}
{}
al::FMSoundSource::FMSoundSource(ISoundSystem &system,Decoder &decoder,FMOD::Channel *source)
	: ISoundSource(system,decoder),m_source{source}
{}
bool al::FMSoundSource::InitializeChannel()
{
	if(m_source != nullptr || m_buffer.expired())
		return false;
	auto *sound = static_cast<FMSoundBuffer*>(m_buffer.lock().get())->GetFMODSound();
	if(sound == nullptr || CheckResultAndUpdateValidity(static_cast<FMSoundSystem&>(m_system).GetFMODLowLevelSystem().playSound(sound,nullptr,true,&m_source)) == false)
		return false;
	return ISoundSource::InitializeChannel();
}
void al::FMSoundSource::Update()
{
	ISoundSource::Update();
	if(m_source != nullptr)
		m_soundSourceData.offset = GetOffset();
}

void al::FMSoundSource::SetFrameOffset(uint64_t offset)
{
	m_soundSourceData.offset = offset;
	if(m_source != nullptr)
		CheckResultAndUpdateValidity(m_source->setPosition(offset,FMOD_TIMEUNIT_PCM));
}
uint64_t al::FMSoundSource::GetFrameOffset(uint64_t *latency) const
{
	if(m_source != nullptr)
	{
		uint32_t pos;
		if(CheckResultAndUpdateValidity(m_source->getPosition(&pos,FMOD_TIMEUNIT_PCM)))
			return pos;
	}
	return m_soundSourceData.offset;
}

void al::FMSoundSource::Stop()
{
	if(m_source != nullptr)
		CheckResultAndUpdateValidity(m_source->stop());
	m_bSchedulePlay = false;
}

void al::FMSoundSource::Pause()
{
	if(m_source != nullptr)
		CheckResultAndUpdateValidity(m_source->setPaused(true));
}

void al::FMSoundSource::Play()
{
	InitializeChannel();
	m_soundSourceData.offset = 0ull;
	if(m_source != nullptr)
		CheckResultAndUpdateValidity(m_source->setPosition(0u,FMOD_TIMEUNIT_MS));
	if(m_source != nullptr)
		CheckResultAndUpdateValidity(m_source->setPaused(false));
}

void al::FMSoundSource::Resume()
{
	if(m_source == nullptr)
	{
		auto offset = GetFrameOffset();
		Play();
		SetFrameOffset(offset);
		return;
	}
	if(m_source != nullptr)
		CheckResultAndUpdateValidity(m_source->setPaused(false));
}

bool al::FMSoundSource::IsPlaying() const
{
	if(m_bSchedulePlay == true)
		return true;
	if(m_source != nullptr)
	{
		auto r = false;
		if(CheckResultAndUpdateValidity(m_source->isPlaying(&r)))
			return r;
	}
	return false;
}
bool al::FMSoundSource::IsPaused() const
{
	return !IsPlaying();
}

void al::FMSoundSource::SetPriority(uint32_t priority)
{
	m_soundSourceData.priority = priority;
	if(m_source != nullptr)
		CheckResultAndUpdateValidity(m_source->setPriority(priority));
}
uint32_t al::FMSoundSource::GetPriority() const
{
	if(m_source != nullptr)
	{
		int32_t priority;
		if(CheckResultAndUpdateValidity(m_source->getPriority(&priority)))
			return priority;
	}
	return m_soundSourceData.priority;
}

void al::FMSoundSource::SetLooping(bool bLoop)
{
	m_soundSourceData.looping = bLoop;
	if(m_source != nullptr)
	{
		FMOD_MODE mode;
		if(CheckResultAndUpdateValidity(m_source->getMode(&mode)))
		{
			mode &= ~(FMOD_LOOP_OFF | FMOD_LOOP_NORMAL | FMOD_LOOP_BIDI);
			if(bLoop == false)
				mode |= FMOD_LOOP_OFF;
			else
				mode |= FMOD_LOOP_NORMAL;
			CheckResultAndUpdateValidity(m_source->setMode(mode));
		}
	}
}
bool al::FMSoundSource::IsLooping() const
{
	if(m_source != nullptr)
	{
		FMOD_MODE mode;
		if(CheckResultAndUpdateValidity(m_source->getMode(&mode)))
			return !(mode &FMOD_LOOP_OFF) && (mode &(FMOD_LOOP_NORMAL | FMOD_LOOP_BIDI)) != 0;
	}
	return m_soundSourceData.looping;
}

void al::FMSoundSource::SetPitch(float pitch)
{
	m_soundSourceData.pitch = pitch;
	if(m_source != nullptr)
		CheckResultAndUpdateValidity(m_source->setPitch(pitch));
}
float al::FMSoundSource::GetPitch() const
{
	if(m_source != nullptr)
	{
		auto pitch = 0.f;
		if(CheckResultAndUpdateValidity(m_source->getPitch(&pitch)))
			return pitch;
	}
	return m_soundSourceData.pitch;
}

void al::FMSoundSource::SetGain(float gain)
{
	m_soundSourceData.gain = gain;
	if(m_source != nullptr)
		CheckResultAndUpdateValidity(m_source->setVolume(gain));
}
float al::FMSoundSource::GetGain() const
{
	if(m_source != nullptr)
	{
		auto gain = 0.f;
		if(CheckResultAndUpdateValidity(m_source->getVolume(&gain)))
			return gain;
	}
	return m_soundSourceData.gain;
}

void al::FMSoundSource::SetDistanceRange(float refDist,float maxDist)
{
	refDist = umath::min(refDist,maxDist);
	auto refDistAudio = al::to_audio_distance(refDist);
	auto maxDistAudio = al::to_audio_distance(maxDist);
	if(maxDistAudio == std::numeric_limits<float>::infinity())
		maxDistAudio = std::numeric_limits<float>::max();
	m_soundSourceData.distanceRange = {refDist,maxDist};
	if(Is3D() && m_source != nullptr)
		CheckResultAndUpdateValidity(m_source->set3DMinMaxDistance(refDistAudio,maxDistAudio));
}

std::pair<float,float> al::FMSoundSource::GetDistanceRange() const
{
	if(Is3D() && m_source != nullptr)
	{
		float minDist,maxDist;
		if(CheckResultAndUpdateValidity(m_source->get3DMinMaxDistance(&minDist,&maxDist)))
			return {al::to_game_distance(minDist),al::to_game_distance(maxDist)};
	}
	return m_soundSourceData.distanceRange;
}

void al::FMSoundSource::SetPosition(const Vector3 &pos)
{
	auto posAudio = al::to_audio_position(pos);
	m_soundSourceData.position = pos;
	UpdateMode();
	if(Is3D() && m_source != nullptr)
	{
		auto fmPos = al::to_custom_vector<FMOD_VECTOR>(posAudio);
		CheckResultAndUpdateValidity(m_source->set3DAttributes(&fmPos,nullptr));
		return;
	}
}

Vector3 al::FMSoundSource::GetPosition() const
{
	if(Is3D() && m_source != nullptr)
	{
		FMOD_VECTOR pos;
		if(CheckResultAndUpdateValidity(m_source->get3DAttributes(&pos,nullptr)))
			return al::to_game_position({pos.x,pos.y,pos.z});
	}
	return m_soundSourceData.position;
}

void al::FMSoundSource::SetVelocity(const Vector3 &vel)
{
	auto velAudio = al::to_audio_position(vel);
	m_soundSourceData.velocity = vel;
	UpdateMode();
	if(Is3D() && m_source != nullptr)
	{
		auto fmVel = al::to_custom_vector<FMOD_VECTOR>(velAudio);
		CheckResultAndUpdateValidity(m_source->set3DAttributes(nullptr,&fmVel));
		return;
	}
}
Vector3 al::FMSoundSource::GetVelocity() const
{
	if(Is3D() && m_source != nullptr)
	{
		FMOD_VECTOR vel;
		if(CheckResultAndUpdateValidity(m_source->get3DAttributes(nullptr,&vel)))
			return al::to_game_position({vel.x,vel.y,vel.z});
	}
	return m_soundSourceData.velocity;
}

void al::FMSoundSource::SetDirection(const Vector3 &dir)
{
	auto dirAudio = al::to_audio_direction(dir);
	// FMOD TODO
}
Vector3 al::FMSoundSource::GetDirection() const
{
	// FMOD TODO
	return {};
}

void al::FMSoundSource::SetOrientation(const Vector3 &at,const Vector3 &up)
{
	auto atAudio = al::to_audio_direction(at);
	auto atUp = al::to_audio_direction(up);
	// FMOD TODO
	//FMOD_VECTOR orientation {at.x,at.y,at.z};
	//al::fmod::check_result(m_source->set3DConeOrientation(&orientation));
}
std::pair<Vector3,Vector3> al::FMSoundSource::GetOrientation() const
{
	// FMOD TODO
	return {{},{}};
}

void al::FMSoundSource::SetConeAngles(float inner,float outer)
{
	m_soundSourceData.coneAngles = {inner,outer};
	UpdateMode();
	if(Is3D() && m_source != nullptr)
	{
		float t,volume;
		m_source->get3DConeSettings(&t,&t,&volume);
		CheckResultAndUpdateValidity(m_source->set3DConeSettings(inner,outer,volume));
	}
}
std::pair<float,float> al::FMSoundSource::GetConeAngles() const
{
	if(Is3D() && m_source != nullptr)
	{
		float inner,outer;
		if(CheckResultAndUpdateValidity(m_source->get3DConeSettings(&inner,&outer,nullptr)))
			return {inner,outer};
	}
	return m_soundSourceData.coneAngles;
}

void al::FMSoundSource::SetOuterConeGains(float gain,float gainHF)
{
	// FMOD TODO
}

std::pair<float,float> al::FMSoundSource::GetOuterConeGains() const
{
	// FMOD TODO
	return {0.f,0.f};
}

float al::FMSoundSource::GetOuterConeGain() const
{
	// FMOD TODO
	return 0.f;
}
float al::FMSoundSource::GetOuterConeGainHF() const
{
	// FMOD TODO
	return 0.f;
}

void al::FMSoundSource::SetRolloffFactors(float factor,float roomFactor)
{
	// FMOD TODO
}

std::pair<float,float> al::FMSoundSource::GetRolloffFactors() const
{
	// FMOD TODO
	return {0.f,0.f};
}

float al::FMSoundSource::GetRolloffFactor() const
{
	// FMOD TODO
	return 0.f;
}
float al::FMSoundSource::GetRoomRolloffFactor() const
{
	// FMOD TODO
	return 0.f;
}

void al::FMSoundSource::SetDopplerFactor(float factor)
{
	m_soundSourceData.dopplerFactor = factor;
	if(Is3D() && m_source != nullptr)
		CheckResultAndUpdateValidity(m_source->set3DDopplerLevel(factor));
}
float al::FMSoundSource::GetDopplerFactor() const
{
	if(Is3D() && m_source != nullptr)
	{
		auto factor = 0.f;
		if(CheckResultAndUpdateValidity(m_source->get3DDopplerLevel(&factor)))
			return factor;
	}
	return m_soundSourceData.dopplerFactor;
}

void al::FMSoundSource::SetRelative(bool bRelative)
{
	auto bRelativeOld = IsRelative();
	m_soundSourceData.relativeToListener = bRelative;
	UpdateMode();
	if(bRelative != bRelativeOld)
		CallCallbacks<void,bool>("OnRelativeChanged",bRelative);
}
bool al::FMSoundSource::IsRelative() const
{
	return m_soundSourceData.relativeToListener;
}

void al::FMSoundSource::SetRadius(float radius)
{
	auto radiusAudio = al::to_audio_distance(radius);
	SetMaxDistance(radiusAudio);
}
float al::FMSoundSource::GetRadius() const
{
	return al::to_game_distance(GetMaxDistance());
}

void al::FMSoundSource::SetStereoAngles(float leftAngle,float rightAngle)
{
	// FMOD TODO
}
std::pair<float,float> al::FMSoundSource::GetStereoAngles() const
{
	// FMOD TODO
	return {0.f,0.f};
}

void al::FMSoundSource::SetAirAbsorptionFactor(float factor)
{
	// FMOD TODO
}
float al::FMSoundSource::GetAirAbsorptionFactor() const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	return m_source->getAirAbsorptionFactor();
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
	return 0.f;
#endif
}

void al::FMSoundSource::SetGainAuto(bool directHF,bool send,bool sendHF)
{
	// FMOD TODO
}
std::tuple<bool,bool,bool> al::FMSoundSource::GetGainAuto() const
{
	// FMOD TODO
	return {false,false,false};
}

bool al::FMSoundSource::GetDirectGainHFAuto() const
{
	// FMOD TODO
	return false;
}
bool al::FMSoundSource::GetSendGainAuto() const
{
	// FMOD TODO
	return false;
}
bool al::FMSoundSource::GetSendGainHFAuto() const
{
	// FMOD TODO
	return false;
}
void al::ISoundSource::SetFMOD3DAttributesEffective(bool b)
{
	m_b3DAttributesEffective = b;
	UpdateMode();
}
void al::ISoundSource::UpdateMode()
{
	FMOD_MODE mode;
	if(CheckResultAndUpdateValidity(m_source->getMode(&mode)) == false)
		return;
	auto oldMode = mode;
	mode &= ~(FMOD_2D | FMOD_3D | FMOD_3D_HEADRELATIVE | FMOD_3D_WORLDRELATIVE);
	if(m_b3DAttributesEffective == false)
		mode |= FMOD_2D;
	else
	{
		if(IsRelative() == false)
			mode |= FMOD_3D | FMOD_3D_WORLDRELATIVE;
		else
		{
			auto coneAngles = GetConeAngles();
			if(uvec::length_sqr(m_soundSourceData.position) == 0.f && uvec::length_sqr(m_soundSourceData.velocity) == 0.f && m_soundSourceData.coneAngles.first >= 360.f && m_soundSourceData.coneAngles.second >= 360.f) // Note: UpdateMode() has to be called whenever one of these was changed
				mode |= FMOD_2D;
			else
				mode |= FMOD_3D | FMOD_3D_HEADRELATIVE;
		}
	}
	if(mode == oldMode)
		return;
	if((mode &FMOD_3D) == 0 || (oldMode &FMOD_3D) != 0) // No update required if new mode isn't 3D, or if old mode was already 3D
	{
		CheckResultAndUpdateValidity(m_source->setMode(mode));
		return;
	}
	// If this was previously a 2D sound, we have to re-set the 3D attributes
	// after the new mode has been applied
	auto distRange = GetDistanceRange();
	auto pos = GetPosition();
	auto vel = GetVelocity();
	auto coneAngles = GetConeAngles();
	auto dopplerFactor = GetDopplerFactor();
	if(CheckResultAndUpdateValidity(m_source->setMode(mode)) == false)
		return;
	SetDistanceRange(distRange.first,distRange.second);
	SetPosition(pos);
	SetVelocity(vel);
	SetConeAngles(coneAngles.first,coneAngles.second);
	SetDopplerFactor(dopplerFactor);
}
void al::ISoundSource::InvalidateSource() const {m_source = nullptr;}
bool al::ISoundSource::Is3D() const
{
	if(m_source != nullptr)
	{
		FMOD_MODE mode;
		if(CheckResultAndUpdateValidity(m_source->getMode(&mode)))
			return (mode &FMOD_3D) != 0;
	}
	return false;
}
bool al::ISoundSource::Is2D() const {return !Is3D();}
bool al::ISoundSource::InitializeChannel()
{
	if(m_source != nullptr || m_buffer.expired())
		return false;
	SetOffset(m_soundSourceData.offset);
	SetPriority(m_soundSourceData.priority);
	SetLooping(m_soundSourceData.looping);
	SetPitch(m_soundSourceData.pitch);
	SetGain(m_soundSourceData.gain);
	SetDistanceRange(m_soundSourceData.distanceRange.first,m_soundSourceData.distanceRange.second);
	SetPosition(m_soundSourceData.position);
	SetVelocity(m_soundSourceData.velocity);
	SetConeAngles(m_soundSourceData.coneAngles.first,m_soundSourceData.coneAngles.second);
	SetDopplerFactor(m_soundSourceData.dopplerFactor);
	SetRelative(m_soundSourceData.relativeToListener);
#if ALSYS_STEAM_AUDIO_SUPPORT_ENABLED == 1
	SetChannelGroup(GetChannelGroup());
#endif
	return true;
}
bool al::ISoundSource::CheckResultAndUpdateValidity(uint32_t result) const
{
	if(result == FMOD_ERR_INVALID_HANDLE || result == FMOD_ERR_CHANNEL_STOLEN)
	{
		InvalidateSource();
		return false;
	}
	al::fmod::check_result(result);
	return result == FMOD_OK;
}

void al::ISoundSource::SetGainRange(float minGain,float maxGain)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_source->setGainRange(minGain,maxGain);
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	m_soundSourceData.minGain = minGain;
	m_soundSourceData.maxGain = maxGain;
	SetGain(umath::clamp(GetGain(),minGain,maxGain));
#endif
}
std::pair<float,float> al::ISoundSource::GetGainRange() const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	return m_source->getGainRange();
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	return {m_soundSourceData.minGain,m_soundSourceData.maxGain};
#endif
}

float al::ISoundSource::GetMinGain() const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	return m_source->getMinGain();
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	return m_soundSourceData.minGain;
#endif
}

float al::ISoundSource::GetMaxGain() const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	return m_source->getMaxGain();
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	return m_soundSourceData.maxGain;
#endif
}

const al::InternalSource *al::ISoundSource::GetInternalSource() const {return const_cast<al::ISoundSource*>(this)->GetInternalSource();}
al::InternalSource *al::ISoundSource::GetInternalSource() {return m_source;}



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
	__declspec(dllexport) bool initialize_audio_api(std::shared_ptr<al::ISoundSystem> &outSoundSystem,std::string &errMsg)
	{
		outSoundSystem = std::make_shared<al::DummySoundSystem>();
		return true;
	}
};
