/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __ALURE_SOUND_SOURCE_HPP__
#define __ALURE_SOUND_SOURCE_HPP__

#include <alsound_source.hpp>

namespace al
{
	class AlureSoundSource
		: public ISoundSource
	{
	public:
		AlureSoundSource(ISoundSystem &system,ISoundBuffer &buffer);
		AlureSoundSource(ISoundSystem &system,Decoder &decoder);

		virtual void Update() override;

		virtual void Play() override;
		virtual void Stop() override;
		virtual void Pause() override;
		virtual void Resume() override;
		virtual bool IsPlaying() const override;
		virtual bool IsPaused() const override;
		virtual void SetPriority(uint32_t priority) override;
		virtual uint32_t GetPriority() const override;
		virtual void SetFrameOffset(uint64_t offset) override;
		virtual uint64_t GetFrameOffset(uint64_t *latency=nullptr) const override;
		virtual void SetLooping(bool bLoop) override;
		virtual bool IsLooping() const override;

		virtual void SetPitch(float pitch) override;
		virtual float GetPitch() const override;

		virtual void SetGain(float gain) override;
		virtual float GetGain() const override;

		virtual void SetGainRange(float minGain,float maxGain) override;
		virtual std::pair<float,float> GetGainRange() const override;
		virtual float GetMinGain() const override;
		virtual float GetMaxGain() const override;
		virtual void SetDistanceRange(float refDist,float maxDist) override;
		virtual std::pair<float,float> GetDistanceRange() const override;
		virtual void SetPosition(const Vector3 &pos) override;
		virtual Vector3 GetPosition() const override;
		virtual void SetVelocity(const Vector3 &vel) override;
		virtual Vector3 GetVelocity() const override;

		virtual void SetDirection(const Vector3 &dir) override;
		virtual Vector3 GetDirection() const override;

		virtual void SetOrientation(const Vector3 &at,const Vector3 &up) override;
		virtual std::pair<Vector3,Vector3> GetOrientation() const override;

		virtual void SetConeAngles(float inner,float outer) override;
		virtual std::pair<float,float> GetConeAngles() const override;
		virtual void SetOuterConeGains(float gain,float gainHF=1.f) override;
		virtual std::pair<float,float> GetOuterConeGains() const override;
		virtual float GetOuterConeGain() const override;
		virtual float GetOuterConeGainHF() const override;

		virtual void SetRolloffFactors(float factor,float roomFactor=0.f) override;
		virtual std::pair<float,float> GetRolloffFactors() const override;
		virtual float GetRolloffFactor() const override;
		virtual float GetRoomRolloffFactor() const override;

		virtual void SetDopplerFactor(float factor) override;
		virtual float GetDopplerFactor() const override;

		virtual void SetRelative(bool bRelative) override;
		virtual bool IsRelative() const override;

		virtual void SetRadius(float radius) override;
		virtual float GetRadius() const override;

		virtual void SetStereoAngles(float leftAngle,float rightAngle) override;
		virtual std::pair<float,float> GetStereoAngles() const override;
		virtual void SetAirAbsorptionFactor(float factor) override;
		virtual float GetAirAbsorptionFactor() const override;

		virtual void SetGainAuto(bool directHF,bool send,bool sendHF) override;
		virtual std::tuple<bool,bool,bool> GetGainAuto() const override;
		virtual bool GetDirectGainHFAuto() const override;
		virtual bool GetSendGainAuto() const override;
		virtual bool GetSendGainHFAuto() const override;

		virtual void SetDirectFilter(const IEffect::Params &params) override {}
		virtual void SetEffectParameters(uint32_t slotId,const IEffect::Params &params) override {}
	protected:
		virtual void DoAddEffect(AuxiliaryEffectSlot &slot,uint32_t slotId,const IEffect::Params &params) override {}
		virtual void DoRemoveInternalEffect(uint32_t slotId) override {}
		virtual void DoRemoveEffect(uint32_t slotId) override {}
	};
};

#endif
