/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "alure_effect.hpp"
#include "alure_sound_system.hpp"
#include <AL/alure2.h>

static alure::Context *apply_effect_type(alure::Effect *effect,int32_t newType,const std::string &typeName,ALuint &effectId)
{
	auto *alEffect = static_cast<alure::Effect*>(effect);
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

void al::AlureEffect::SetProperties(al::EfxChorusProperties props)
{
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
}

void al::AlureEffect::SetProperties(al::EfxEaxReverbProperties props)
{
	// TODO
}

void al::AlureEffect::SetProperties(al::EfxDistortionProperties props)
{
	ALuint effectId;
	auto *context = apply_effect_type(m_effect,AL_EFFECT_DISTORTION,"distortion",effectId);
	if(context == nullptr)
		return;
	context->alEffectf(effectId,AL_DISTORTION_EDGE,props.flEdge);
	context->alEffectf(effectId,AL_DISTORTION_GAIN,props.flGain);
	context->alEffectf(effectId,AL_DISTORTION_LOWPASS_CUTOFF,props.flLowpassCutoff);
	context->alEffectf(effectId,AL_DISTORTION_EQCENTER,props.flEQCenter);
	context->alEffectf(effectId,AL_DISTORTION_EQBANDWIDTH,props.flEQBandwidth);
}
void al::AlureEffect::SetProperties(al::EfxEchoProperties props)
{
	ALuint effectId;
	auto *context = apply_effect_type(m_effect,AL_EFFECT_ECHO,"echo",effectId);
	if(context == nullptr)
		return;
	context->alEffectf(effectId,AL_ECHO_DELAY,props.flDelay);
	context->alEffectf(effectId,AL_ECHO_LRDELAY,props.flLRDelay);
	context->alEffectf(effectId,AL_ECHO_DAMPING,props.flDamping);
	context->alEffectf(effectId,AL_ECHO_FEEDBACK,props.flFeedback);
	context->alEffectf(effectId,AL_ECHO_SPREAD,props.flSpread);
}
void al::AlureEffect::SetProperties(al::EfxFlangerProperties props)
{
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
}
void al::AlureEffect::SetProperties(al::EfxFrequencyShifterProperties props)
{
	ALuint effectId;
	auto *context = apply_effect_type(m_effect,AL_EFFECT_FREQUENCY_SHIFTER,"frequency shifter",effectId);
	if(context == nullptr)
		return;
	context->alEffectf(effectId,AL_FREQUENCY_SHIFTER_FREQUENCY,props.flFrequency);
	context->alEffecti(effectId,AL_FREQUENCY_SHIFTER_LEFT_DIRECTION,props.iLeftDirection);
	context->alEffecti(effectId,AL_FREQUENCY_SHIFTER_RIGHT_DIRECTION,props.iRightDirection);
}
void al::AlureEffect::SetProperties(al::EfxVocalMorpherProperties props)
{
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
}
void al::AlureEffect::SetProperties(al::EfxPitchShifterProperties props)
{
	ALuint effectId;
	auto *context = apply_effect_type(m_effect,AL_EFFECT_PITCH_SHIFTER,"pitch shifter",effectId);
	if(context == nullptr)
		return;
	context->alEffecti(effectId,AL_PITCH_SHIFTER_COARSE_TUNE,props.iCoarseTune);
	context->alEffecti(effectId,AL_PITCH_SHIFTER_FINE_TUNE,props.iFineTune);
}
void al::AlureEffect::SetProperties(al::EfxRingModulatorProperties props)
{
	ALuint effectId;
	auto *context = apply_effect_type(m_effect,AL_EFFECT_RING_MODULATOR,"ring modulator",effectId);
	if(context == nullptr)
		return;
	context->alEffectf(effectId,AL_RING_MODULATOR_FREQUENCY,props.flFrequency);
	context->alEffectf(effectId,AL_RING_MODULATOR_HIGHPASS_CUTOFF,props.flHighpassCutoff);
	context->alEffecti(effectId,AL_RING_MODULATOR_WAVEFORM,props.iWaveform);
}
void al::AlureEffect::SetProperties(al::EfxAutoWahProperties props)
{
	ALuint effectId;
	auto *context = apply_effect_type(m_effect,AL_EFFECT_AUTOWAH,"auto wah",effectId);
	if(context == nullptr)
		return;
	context->alEffectf(effectId,AL_AUTOWAH_ATTACK_TIME,props.flAttackTime);
	context->alEffectf(effectId,AL_AUTOWAH_RELEASE_TIME,props.flReleaseTime);
	context->alEffectf(effectId,AL_AUTOWAH_RESONANCE,props.flResonance);
	context->alEffectf(effectId,AL_AUTOWAH_PEAK_GAIN,props.flPeakGain);
}
void al::AlureEffect::SetProperties(al::EfxCompressor props)
{
	ALuint effectId;
	auto *context = apply_effect_type(m_effect,AL_EFFECT_COMPRESSOR,"compressor",effectId);
	if(context == nullptr)
		return;
	context->alEffecti(effectId,AL_COMPRESSOR_ONOFF,props.iOnOff);
}
void al::AlureEffect::SetProperties(al::EfxEqualizer props)
{
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
}
