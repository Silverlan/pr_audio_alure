/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "alure_sound_source.hpp"
#include "alure_sound_buffer.hpp"
#include "alure_sound_system.hpp"
#include <alsound_coordinate_system.hpp>

void al::AlureSoundSource::SetEffectParameters(uint32_t slotId,const IEffect::Params &params)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_source->setSendFilter(slotId,reinterpret_cast<const alure::FilterParams&>(params));
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
#endif
}

void al::AlureSoundSource::SetDirectFilter(const IEffect::Params &params)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_source->setDirectFilter(reinterpret_cast<const alure::FilterParams&>(m_directFilter = params));
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	// FMOD TODO
#endif
}

void al::AlureSoundSource::DoRemoveInternalEffect(uint32_t slotId)
{
	m_source->setAuxiliarySend(nullptr,slotId);
}

void al::AlureSoundSource::DoRemoveEffect(uint32_t slotId)
{
	m_source->setAuxiliarySend(nullptr,slotId);
}

virtual void AlureSoundSource::DoAddEffect(AuxiliaryEffectSlot &slot,uint32_t slotId,const IEffect::Params &params)
{
	m_source->setAuxiliarySendFilter(slot->GetALSlot(),slotId,reinterpret_cast<const alure::FilterParams&>(params));
}

al::FMSoundSource::FMSoundSource(ISoundSystem &system,ISoundBuffer &buffer,FMOD::Channel *source)
	: AlureSoundSource(system,buffer),m_source{source}
{}
al::FMSoundSource::FMSoundSource(ISoundSystem &system,Decoder &decoder,FMOD::Channel *source)
	: AlureSoundSource(system,decoder),m_source{source}
{}
bool al::FMSoundSource::InitializeChannel()
{
	if(m_source != nullptr || m_buffer.expired())
		return false;
	auto *sound = static_cast<FMSoundBuffer*>(m_buffer.lock().get())->GetFMODSound();
	if(sound == nullptr || CheckResultAndUpdateValidity(static_cast<FMSoundSystem&>(m_system).GetFMODLowLevelSystem().playSound(sound,nullptr,true,&m_source)) == false)
		return false;
	return AlureSoundSource::InitializeChannel();
}
void al::FMSoundSource::Update()
{
	AlureSoundSource::Update();
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
void al::AlureSoundSource::SetFMOD3DAttributesEffective(bool b)
{
	m_b3DAttributesEffective = b;
	UpdateMode();
}
void al::AlureSoundSource::UpdateMode()
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
void al::AlureSoundSource::InvalidateSource() const {m_source = nullptr;}
bool al::AlureSoundSource::Is3D() const
{
	if(m_source != nullptr)
	{
		FMOD_MODE mode;
		if(CheckResultAndUpdateValidity(m_source->getMode(&mode)))
			return (mode &FMOD_3D) != 0;
	}
	return false;
}
bool al::AlureSoundSource::Is2D() const {return !Is3D();}
bool al::AlureSoundSource::InitializeChannel()
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
bool al::AlureSoundSource::CheckResultAndUpdateValidity(uint32_t result) const
{
	if(result == FMOD_ERR_INVALID_HANDLE || result == FMOD_ERR_CHANNEL_STOLEN)
	{
		InvalidateSource();
		return false;
	}
	al::fmod::check_result(result);
	return result == FMOD_OK;
}

void al::AlureSoundSource::SetGainRange(float minGain,float maxGain)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_source->setGainRange(minGain,maxGain);
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	m_soundSourceData.minGain = minGain;
	m_soundSourceData.maxGain = maxGain;
	SetGain(umath::clamp(GetGain(),minGain,maxGain));
#endif
}
std::pair<float,float> al::AlureSoundSource::GetGainRange() const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	return m_source->getGainRange();
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	return {m_soundSourceData.minGain,m_soundSourceData.maxGain};
#endif
}

float al::AlureSoundSource::GetMinGain() const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	return m_source->getMinGain();
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	return m_soundSourceData.minGain;
#endif
}

float al::AlureSoundSource::GetMaxGain() const
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	return m_source->getMaxGain();
#elif ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_FMOD
	return m_soundSourceData.maxGain;
#endif
}

const al::InternalSource *al::AlureSoundSource::GetInternalSource() const {return const_cast<al::AlureSoundSource*>(this)->GetInternalSource();}
al::InternalSource *al::AlureSoundSource::GetInternalSource() {return m_source;}
