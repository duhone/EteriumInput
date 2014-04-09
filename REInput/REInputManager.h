// REInput.h

#pragma once

using namespace System;
#include "REInputNativeFwd.h"
#include "REInput.h"
#include "REInputDevice.h"

namespace RE
{
	namespace Input
	{
		public ref class InputManager
		{
		public:
			InputManager(System::IntPtr _windowHandle);
			~InputManager();
			!InputManager();
			System::Collections::Generic::IEnumerable<DeviceInfo>^ EnumDevices();
			InputDevice^ CreateDevice(System::Guid _id);
		private:
			RE::InputNative::InputManager* m_Pimpl;
		};
	}
}
