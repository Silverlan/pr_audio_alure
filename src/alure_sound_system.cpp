/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "alure_sound_system.hpp"
#include "alure_sound_buffer.hpp"
#include "alure_sound_source.hpp"
#include "alure_listener.hpp"
#include <fsys/filesystem.h>
#include <AL/alure2.h>

al::PDecoder al::AlureSoundSystem::CreateDecoder(const std::string &path,bool bConvertToMono)
{
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
}

al::PSoundChannel al::AlureSoundSystem::CreateChannel(Decoder &decoder)
{
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
}

std::vector<std::string> al::AlureSoundSystem::GetHRTFNames() const
{
	return m_device->enumerateHRTFNames();
}

std::string al::AlureSoundSystem::GetCurrentHRTF() const
{
	return m_device->getCurrentHRTF();
}
bool al::AlureSoundSystem::IsHRTFEnabled() const
{
	return m_device->isHRTFEnabled();
}

void al::AlureSoundSystem::SetHRTF(uint32_t id)
{
	if(m_device->queryExtension("ALC_SOFT_HRTF") == false)
		return;
	m_device->reset({
		{ALC_HRTF_SOFT,ALC_TRUE},
		{ALC_HRTF_ID_SOFT,id},
		{0,0}
	});
}
void al::AlureSoundSystem::DisableHRTF()
{
	if(m_device->queryExtension("ALC_SOFT_HRTF") == false)
		return;
	m_device->reset({
		{ALC_HRTF_SOFT,ALC_FALSE},
		{0,0}
	});
}

uint32_t al::AlureSoundSystem::GetMaxAuxiliaryEffectsPerSource() const
{
	return m_device->getMaxAuxiliarySends();
}

bool al::AlureSoundSystem::IsSupported(ChannelConfig channels,SampleType type) const
{
	return m_context->isSupported(static_cast<alure::ChannelConfig>(channels),static_cast<alure::SampleType>(type));
}

float al::AlureSoundSystem::GetDopplerFactor() const {return m_dopplerFactor;}
void al::AlureSoundSystem::SetDopplerFactor(float factor)
{
	m_dopplerFactor = factor;
	m_context->setDopplerFactor(m_dopplerFactor);
}

float al::AlureSoundSystem::GetSpeedOfSound() const {return m_speedOfSound;}
void al::AlureSoundSystem::SetSpeedOfSound(float speed)
{
	m_speedOfSound = speed;
	m_context->setSpeedOfSound(m_speedOfSound);
}

std::string al::AlureSoundSystem::GetDeviceName() const
{
	return m_device->getName();
}

void al::AlureSoundSystem::PauseDeviceDSP()
{
	m_device->pauseDSP();
}
void al::AlureSoundSystem::ResumeDeviceDSP()
{
	m_device->resumeDSP();
}

al::IAuxiliaryEffectSlot *al::AlureSoundSystem::CreateAuxiliaryEffectSlot()
{
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
}

al::DistanceModel al::AlureSoundSystem::GetDistanceModel() const {return m_distanceModel;}
void al::AlureSoundSystem::SetDistanceModel(DistanceModel mdl)
{
	m_distanceModel = mdl;
	m_context->setDistanceModel(static_cast<alure::DistanceModel>(m_distanceModel));
}

al::PEffect al::AlureSoundSystem::CreateEffect()
{
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
}

al::AlureSoundSystem::AlureSoundSystem(alure::Device *device,alure::Context *context,float metersPerUnit)
	: m_device(device),m_context(context),m_listener(*context->getListener())
{
	m_listener.SetMetersPerUnit(metersPerUnit);
	SetSpeedOfSound(340.29f /metersPerUnit);
	SetSoundSourceFactory(std::make_unique<al::DefaultSoundSourceFactory>());
}
const alure::Device *al::AlureSoundSystem::GetALDevice() const {return const_cast<SoundSystem*>(this)->GetALDevice();}
alure::Device *al::AlureSoundSystem::GetALDevice() {return m_device;}
const alure::Context *al::AlureSoundSystem::GetALContext() const {return const_cast<SoundSystem*>(this)->GetALContext();}
alure::Context *al::AlureSoundSystem::GetALContext() {return m_context;}

std::shared_ptr<al::AlureSoundSystem> al::AlureSoundSystem::Create(const std::string &deviceName,float metersPerUnit)
{
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
}

void al::AlureSoundSystem::Update()
{
	AlureSoundSystem::Update();
	m_context->update();
}

std::shared_ptr<al::AlureSoundSystem> al::AlureSoundSystem::Create(float metersPerUnit) {return Create("",metersPerUnit);}

al::AlureSoundSystem::AlureSoundSystem(const std::shared_ptr<FMOD::Studio::System> &fmSystem,FMOD::System &lowLevelSystem,float metersPerUnit)
	: AlureSoundSystem{},m_fmSystem(fmSystem),m_fmLowLevelSystem(lowLevelSystem)
{
	lowLevelSystem.set3DSettings(1.f,1.f,1.f);
	// FMOD TODO
	//SetSpeedOfSound(340.29f /metersPerUnit);
	SetSoundSourceFactory(std::make_unique<al::FMSoundSourceFactory>());
}

std::unique_ptr<al::SoundSourceFactory> al::AlureSoundSystem::InitializeSoundSourceFactory()
{
	return std::make_unique<FMSoundSourceFactory>();
}

std::unique_ptr<al::IListener> al::AlureSoundSystem::CreateListener() {return std::unique_ptr<FMListener>{new FMListener{*this}};}

al::ISoundBuffer *al::AlureSoundSystem::DoLoadSound(const std::string &normPath,bool bConvertToMono,bool bAsync)
{
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
}

al::PSoundSource al::AlureSoundSystem::CreateSource(ISoundBuffer &buffer)
{
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
}

al::AlureSoundSystem::AlureSoundSystem(alure::Device *device,alure::Context *context,float metersPerUnit)
	: m_device(device),m_context(context),m_listener(*context->getListener())
{
	m_listener.SetMetersPerUnit(metersPerUnit);
	SetSpeedOfSound(340.29f /metersPerUnit);
	SetSoundSourceFactory(std::make_unique<al::DefaultSoundSourceFactory>());
}
const alure::Device *al::AlureSoundSystem::GetALDevice() const {return const_cast<SoundSystem*>(this)->GetALDevice();}
alure::Device *al::AlureSoundSystem::GetALDevice() {return m_device;}
const alure::Context *al::AlureSoundSystem::GetALContext() const {return const_cast<SoundSystem*>(this)->GetALContext();}
alure::Context *al::AlureSoundSystem::GetALContext() {return m_context;}
