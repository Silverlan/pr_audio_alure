/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __ALURE_LISTENER_HPP__
#define __ALURE_LISTENER_HPP__

#include <alsound_listener.hpp>

namespace alure {class Listener;};

namespace al
{
	class AlureSoundSystem;
	class AlureListener
		: public IListener
	{
	public:
		virtual void SetGain(float gain) override;
		virtual void SetPosition(const Vector3 &pos) override;
		virtual void SetVelocity(const Vector3 &vel) override;
		virtual void SetOrientation(const Vector3 &at,const Vector3 &up) override;
	protected:
		AlureListener(alure::Listener &listener,al::ISoundSystem &system);
		alure::Listener &m_listener;
		virtual void DoSetMetersPerUnit(float mu) override;
		friend AlureSoundSystem;
	};
};

#endif
