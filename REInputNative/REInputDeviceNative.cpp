#include "stdafx.h"
#include "REInputDeviceNative.h"
#include "Util.h"

using namespace std;
using namespace RE::InputNative;

InputDevice::InputDevice(DeviceInfo& _deviceInfo, CComPtr<IDirectInputDevice8> _device, HWND _hwnd) : m_DeviceInfo(_deviceInfo), 
	m_Device(_device), m_hwnd(_hwnd)
{
	Acquire();
	Update();
}

void InputDevice::Acquire()
{
	auto hr = m_Device->SetDataFormat(&c_dfDIJoystick2);
	if(FAILED(hr))
		return;

	hr = m_Device->SetCooperativeLevel(m_hwnd, DISCL_EXCLUSIVE | DISCL_BACKGROUND);
	if(FAILED(hr))
		return;
	
	if(m_DeviceInfo.XAxis)
		XAxisRange = GetJoystickRange(DIJOFS_X);
	if(m_DeviceInfo.YAxis)
		YAxisRange = GetJoystickRange(DIJOFS_Y);
	if(m_DeviceInfo.ZAxis)
		ZAxisRange = GetJoystickRange(DIJOFS_Z);
	if(m_DeviceInfo.XRotation)
		XRotRange = GetJoystickRange(DIJOFS_RX);
	if(m_DeviceInfo.YRotation)
		YRotRange = GetJoystickRange(DIJOFS_RY);
	if(m_DeviceInfo.ZRotation)
		ZRotRange = GetJoystickRange(DIJOFS_RZ);
	if(m_DeviceInfo.NumSliders >= 1)
		SliderRange[0] = GetJoystickRange(DIJOFS_SLIDER(0));
	if(m_DeviceInfo.NumSliders >= 2)
		SliderRange[1] = GetJoystickRange(DIJOFS_SLIDER(1));
	
	m_Device->Acquire();
}

void InputDevice::Update()
{
	m_Device->Poll();
	DIJOYSTATE2 data;
	auto hr = m_Device->GetDeviceState(sizeof(data), &data);
	if(FAILED(hr)) //hand waving. if we fail for any reason then just try to recreate/reaquire everything.
	{
		m_DeviceState = DeviceState(); //clear everything out so we dont get left turning.
		Acquire();
		return;
	}

	if(m_DeviceInfo.XAxis)
		m_DeviceState.XAxis = CalcAxisValue(data.lX, XAxisRange);
	if(m_DeviceInfo.YAxis)
		m_DeviceState.YAxis = CalcAxisValue(data.lY, YAxisRange) * -1.0f;
	if(m_DeviceInfo.ZAxis)
		m_DeviceState.ZAxis = CalcAxisValue(data.lZ, ZAxisRange);
	if(m_DeviceInfo.XRotation)
		m_DeviceState.XRotation = CalcAxisValue(data.lRx, XRotRange);
	if(m_DeviceInfo.YRotation)
		m_DeviceState.YRotation = CalcAxisValue(data.lRy, YRotRange);
	if(m_DeviceInfo.ZRotation)
		m_DeviceState.ZRotation = CalcAxisValue(data.lRz, ZRotRange);
	if(m_DeviceInfo.NumSliders >= 1)
		m_DeviceState.Sliders[0] = CalcSliderValue(data.rglSlider[0], SliderRange[0]);
	if(m_DeviceInfo.NumSliders >= 2)
		m_DeviceState.Sliders[1] = CalcSliderValue(data.rglSlider[1], SliderRange[1]);

	if(m_DeviceInfo.NumPOVs >= 1)
		m_DeviceState.POVs[0] = CalcPOVValue(data.rgdwPOV[0]);
	if(m_DeviceInfo.NumPOVs >= 2)
		m_DeviceState.POVs[1] = CalcPOVValue(data.rgdwPOV[1]);
	if(m_DeviceInfo.NumPOVs >= 3)
		m_DeviceState.POVs[2] = CalcPOVValue(data.rgdwPOV[2]);
	if(m_DeviceInfo.NumPOVs >= 4)
		m_DeviceState.POVs[3] = CalcPOVValue(data.rgdwPOV[3]);

	for(int i = 0;i < m_DeviceInfo.NumButtons; ++i)
	{
		m_DeviceState.Buttons[i] = !(data.rgbButtons[i] == 0);
	}
}

pair<int, int> InputDevice::GetJoystickRange(DWORD _prop)
{
	DIPROPRANGE range;
	range.diph.dwSize = sizeof(range);
	range.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	range.diph.dwHow = DIPH_BYOFFSET;
	range.diph.dwObj = _prop;
	auto hr = m_Device->GetProperty(DIPROP_RANGE, &range.diph);
	if(FAILED(hr))
	{
		return make_pair(0, 0); //todo not a good way to deal with this failure, but probably extremely rare for only this to fail.
	}
	return make_pair(range.lMin, range.lMax);
}

//axis is always -1 to 1
float InputDevice::CalcAxisValue(int _value, const std::pair<int,int>& _range)
{
	return 2.0f*(static_cast<float>(_value-_range.first)/(_range.second-_range.first))-1.0f;
}

//slider is always 0 to 1
float InputDevice::CalcSliderValue(int _value, const std::pair<int,int>& _range)
{
	return static_cast<float>(_value-_range.first)/(_range.second-_range.first);
}

POV_DIRECTION InputDevice::CalcPOVValue(int _value)
{
	if((LOWORD(_value) == 0xFFFF))
		return POV_DIRECTION::Null;
	if(_value > 290*DI_DEGREES || _value < 30*DI_DEGREES)
		return POV_DIRECTION::Up;
	if(_value > 60*DI_DEGREES && _value < 120*DI_DEGREES)
		return POV_DIRECTION::Right;
	if(_value > 150*DI_DEGREES && _value < 210*DI_DEGREES)
		return POV_DIRECTION::Down;
	if(_value > 240*DI_DEGREES && _value < 300*DI_DEGREES)
		return POV_DIRECTION::Left;

	return POV_DIRECTION::Null;
}