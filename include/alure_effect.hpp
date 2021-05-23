/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __ALURE_EFFECT_HPP__
#define __ALURE_EFFECT_HPP__

#include <alsound_effect.hpp>

namespace al
{
	class AlureEffect
		: public IEffect
	{
	public:
		virtual void SetProperties(al::EfxEaxReverbProperties props) override;
		virtual void SetProperties(al::EfxChorusProperties props) override;
		virtual void SetProperties(al::EfxDistortionProperties props) override;
		virtual void SetProperties(al::EfxEchoProperties props) override;
		virtual void SetProperties(al::EfxFlangerProperties props) override;
		virtual void SetProperties(al::EfxFrequencyShifterProperties props) override;
		virtual void SetProperties(al::EfxVocalMorpherProperties props) override;
		virtual void SetProperties(al::EfxPitchShifterProperties props) override;
		virtual void SetProperties(al::EfxRingModulatorProperties props) override;
		virtual void SetProperties(al::EfxAutoWahProperties props) override;
		virtual void SetProperties(al::EfxCompressor props) override;
		virtual void SetProperties(al::EfxEqualizer props) override;
	private:
		FMEffect(ISoundSystem &soundSys);
	};
};

#endif
