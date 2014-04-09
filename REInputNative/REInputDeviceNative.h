#pragma once

#include "REInputNative.h"
#include <atlbase.h>
#include <dinput.h>

namespace RE
{ 			
	namespace InputNative
	{	
		class InputDevice : public IInputDevice
		{
		public:
			InputDevice(DeviceInfo& _deviceInfo, CComPtr<IDirectInputDevice8> _device, HWND _hwnd);
			const DeviceInfo& GetDeviceInfo() const override { return m_DeviceInfo; }
			void Update() override;
			const DeviceState& GetDeviceState() const override { return m_DeviceState; }
		private:
			void Acquire();
			std::pair<int, int> GetJoystickRange(DWORD _prop);
			float CalcAxisValue(int _value, const std::pair<int,int>& _range);
			float CalcSliderValue(int _value, const std::pair<int,int>& _range);
			POV_DIRECTION CalcPOVValue(int _value);

			HWND m_hwnd;
			DeviceInfo m_DeviceInfo;
			CComPtr<IDirectInputDevice8> m_Device;
			DeviceState m_DeviceState;
			std::pair<int,int> XAxisRange;
			std::pair<int,int> YAxisRange;
			std::pair<int,int> ZAxisRange;
			std::pair<int,int> XRotRange;
			std::pair<int,int> YRotRange;
			std::pair<int,int> ZRotRange;
			std::pair<int,int> SliderRange[2];
		};
	}
}