/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "alure_listener.hpp"
#include "alure_sound_system.hpp"
#include <alsound_coordinate_system.hpp>
#include <AL/alure2.h>

al::AlureListener::AlureListener(alure::Listener &listener,al::ISoundSystem &system)
	: IListener{system},m_listener(listener)
{}

void al::AlureListener::DoSetMetersPerUnit(float mu)
{
	m_listener.setMetersPerUnit(m_metersPerUnit);
}

void al::AlureListener::SetGain(float gain)
{
#if ALSYS_LIBRARY_TYPE == ALSYS_LIBRARY_ALURE
	m_listener.setGain(gain);
#endif
}
void al::AlureListener::SetPosition(const Vector3 &pos)
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
void al::AlureListener::SetVelocity(const Vector3 &vel)
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
void al::AlureListener::SetOrientation(const Vector3 &at,const Vector3 &up)
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
