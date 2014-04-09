#pragma once

#include <vector>
#include <memory>
#include "REInputNative.h"

namespace RE
{
	namespace InputNative
	{
		class InputManager
		{
		public:
			InputManager(void* _instance, int* _hwnd);
			~InputManager();
			std::vector<DeviceInfo> EnumDevices();
			std::shared_ptr<IInputDevice> CreateDevice(GUID _id);
		private:
			class InputManagerImpl* m_Pimpl;
		};
	}
}