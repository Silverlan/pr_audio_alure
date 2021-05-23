/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <alsoundsystem.hpp>

namespace alure
{
	class Device;
	class Context;
	class Buffer;
	class Source;
	class Listener;
	class Effect;
};
namespace al
{
	class AlureSoundSystem
		: public ISoundSystem
	{
	public:
		static std::shared_ptr<AlureSoundSystem> Create(const std::string &deviceName,float metersPerUnit=1.f);
		static std::shared_ptr<AlureSoundSystem> Create(float metersPerUnit=1.f);

		virtual void Update() override;

		virtual IAuxiliaryEffectSlot *CreateAuxiliaryEffectSlot() override;
		
		virtual PSoundSource CreateSource(ISoundBuffer &buffer) override;
		virtual PSoundSource CreateSource(Decoder &decoder) override;
		virtual PDecoder CreateDecoder(const std::string &path,bool bConvertToMono=false) override;
		virtual PEffect CreateEffect() override;
		virtual bool IsSupported(ChannelConfig channels,SampleType type) const override;

		virtual float GetDopplerFactor() const override;
		virtual void SetDopplerFactor(float factor) override;

		virtual float GetSpeedOfSound() const override;
		virtual void SetSpeedOfSound(float speed) override;

		virtual DistanceModel GetDistanceModel() const override;
		virtual void SetDistanceModel(DistanceModel mdl) override;
		
		virtual std::string GetDeviceName() const override;
		virtual void PauseDeviceDSP() override;
		virtual void ResumeDeviceDSP() override;

		virtual std::vector<std::string> GetDevices() override {return {"fmod"};}
		virtual std::string GetDefaultDeviceName() override {return "fmod";}

		virtual uint32_t GetMaxAuxiliaryEffectsPerSource() const override;

		// HRTF
		virtual std::vector<std::string> GetHRTFNames() const override;
		virtual std::string GetCurrentHRTF() const override;
		virtual bool IsHRTFEnabled() const override;
		virtual void SetHRTF(uint32_t id) override;
		virtual void DisableHRTF() override;

		const alure::Device *GetALDevice() const;
		alure::Device *GetALDevice();
		const alure::Context *GetALContext() const;
		alure::Context *GetALContext();
	private:
		AlureSoundSystem(alure::Device *device,alure::Context *context,float metersPerUnit);
		virtual ISoundBuffer *DoLoadSound(const std::string &path,bool bConvertToMono=false,bool bAsync=true) override;
		virtual std::unique_ptr<IListener> CreateListener() override;
		virtual std::unique_ptr<SoundSourceFactory> InitializeSoundSourceFactory() override;

		alure::Device *m_device = nullptr;
		alure::Context *m_context = nullptr;
	};

	class FMSoundSourceFactory
		: public SoundSourceFactory
	{
	public:
		virtual ISoundSource *CreateSoundSource(ISoundSystem &system,ISoundBuffer &buffer) override;
		virtual ISoundSource *CreateSoundSource(ISoundSystem &system,Decoder &decoder) override;
	};
};
