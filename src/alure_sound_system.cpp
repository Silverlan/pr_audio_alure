/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "alure_sound_system.hpp"
#include "alure_sound_buffer.hpp"
#include "alure_sound_source.hpp"
#include "alure_listener.hpp"
#include <fsys/filesystem.h>

al::PDecoder al::AlureSoundSystem::CreateDecoder(const std::string &path,bool bConvertToMono)
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

al::PSoundSource al::AlureSoundSystem::CreateSource(Decoder &decoder)
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

std::vector<std::string> al::AlureSoundSystem::GetHRTFNames() const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	return m_device->enumerateHRTFNames();
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	return {}; // FMOD TODO
#endif
}

std::string al::AlureSoundSystem::GetCurrentHRTF() const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	return m_device->getCurrentHRTF();
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	return ""; // FMOD TODO
#endif
}
bool al::AlureSoundSystem::IsHRTFEnabled() const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	return m_device->isHRTFEnabled();
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	return false; // FMOD TODO
#endif
}

void al::AlureSoundSystem::SetHRTF(uint32_t id)
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
void al::AlureSoundSystem::DisableHRTF()
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

uint32_t al::AlureSoundSystem::GetMaxAuxiliaryEffectsPerSource() const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	return m_device->getMaxAuxiliarySends();
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	return 0u; // FMOD TODO
#endif
}

bool al::AlureSoundSystem::IsSupported(ChannelConfig channels,SampleType type) const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	return m_context->isSupported(static_cast<alure::ChannelConfig>(channels),static_cast<alure::SampleType>(type));
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	return true; // FMOD TODO
#endif
}

float al::AlureSoundSystem::GetDopplerFactor() const {return m_dopplerFactor;}
void al::AlureSoundSystem::SetDopplerFactor(float factor)
{
	m_dopplerFactor = factor;
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_context->setDopplerFactor(m_dopplerFactor);
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
#endif
}

float al::AlureSoundSystem::GetSpeedOfSound() const {return m_speedOfSound;}
void al::AlureSoundSystem::SetSpeedOfSound(float speed)
{
	m_speedOfSound = speed;
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_context->setSpeedOfSound(m_speedOfSound);
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
#endif
}

std::string al::AlureSoundSystem::GetDeviceName() const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	return m_device->getName();
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	return "";// FMOD TODO
#endif
}

void al::AlureSoundSystem::PauseDeviceDSP()
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_device->pauseDSP();
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
#endif
}
void al::AlureSoundSystem::ResumeDeviceDSP()
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_device->resumeDSP();
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
#endif
}

al::AuxiliaryEffectSlot *al::AlureSoundSystem::CreateAuxiliaryEffectSlot()
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

al::DistanceModel al::AlureSoundSystem::GetDistanceModel() const {return m_distanceModel;}
void al::AlureSoundSystem::SetDistanceModel(DistanceModel mdl)
{
	m_distanceModel = mdl;
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_context->setDistanceModel(static_cast<alure::DistanceModel>(m_distanceModel));
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
#endif
}

al::PEffect al::AlureSoundSystem::CreateEffect()
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
	AlureSoundSystem::Update();
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
	: AlureSoundSystem{},m_fmSystem(fmSystem),m_fmLowLevelSystem(lowLevelSystem)
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

al::FMSoundSystem::AlureSoundSystem(alure::Device *device,alure::Context *context,float metersPerUnit)
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
