
#pragma once

using namespace System;
#include "REInputNativeFwd.h"
#include "REInput.h"
#include <memory>

namespace RE
{
	namespace Input
	{
		public ref class InputDevice
		{
		public:
			InputDevice(std::shared_ptr<RE::InputNative::IInputDevice> _device);
			~InputDevice();
			!InputDevice();
			DeviceInfo GetDeviceInfo() { return m_DeviceInfo; }
			void Update();
			DeviceState GetDeviceState() { return m_DeviceState; }
		private:
			class InputDeviceImpl* m_Pimpl;
			DeviceState m_DeviceState;
			DeviceInfo m_DeviceInfo;
		};
	}
}