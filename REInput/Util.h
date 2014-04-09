#pragma once
#include "REInput.h"
#include "REInputNative.h"

inline RE::Input::DeviceInfo ConvertDevInfo(const RE::InputNative::DeviceInfo& _devInfo)
{
		RE::Input::DeviceInfo newDevice;
		newDevice.ID = System::Guid::Guid(_devInfo.ID.Data1, _devInfo.ID.Data2, _devInfo.ID.Data3, _devInfo.ID.Data4[0], 
			_devInfo.ID.Data4[1], _devInfo.ID.Data4[2], _devInfo.ID.Data4[3], _devInfo.ID.Data4[4], _devInfo.ID.Data4[5],
			_devInfo.ID.Data4[6], _devInfo.ID.Data4[7]);
		newDevice.Name = gcnew System::String(_devInfo.Name.c_str());
		newDevice.XAxis = _devInfo.XAxis;
		newDevice.YAxis = _devInfo.YAxis;
		newDevice.ZAxis = _devInfo.ZAxis;
		newDevice.XRotation = _devInfo.XRotation;
		newDevice.YRotation = _devInfo.YRotation;
		newDevice.ZRotation = _devInfo.ZRotation;
		newDevice.NumSliders = _devInfo.NumSliders;
		newDevice.NumPOVs = _devInfo.NumPOVs;
		newDevice.NumButtons = _devInfo.NumButtons;

		return newDevice;
}

inline RE::Input::DeviceState ConvertDevState(const RE::InputNative::DeviceInfo& _devInfo, const RE::InputNative::DeviceState& _devState)
{
	RE::Input::DeviceState result;
	if(_devInfo.XAxis)
		result.XAxis = _devState.XAxis;
	else
		result.XAxis = 0.0f;

	if(_devInfo.YAxis)
		result.YAxis = _devState.YAxis;
	else
		result.YAxis = 0.0f;

	if(_devInfo.ZAxis)
		result.ZAxis = _devState.ZAxis;
	else
		result.ZAxis = 0.0f;
	
	if(_devInfo.XRotation)
		result.XRotation = _devState.XRotation;
	else
		result.XRotation = 0.0f;

	if(_devInfo.YRotation)
		result.YRotation = _devState.YRotation;
	else
		result.YRotation = 0.0f;
	
	if(_devInfo.ZRotation)
		result.ZRotation = _devState.ZRotation;
	else
		result.ZRotation = 0.0f;

	result.Sliders = gcnew array<float>(_devInfo.NumSliders);
	for(int i = 0;i < _devInfo.NumSliders; ++i)
	{
		result.Sliders[i] = _devState.Sliders[i];
	}

	result.POVs = gcnew array<RE::Input::POV_DIRECTION>(_devInfo.NumPOVs);
	for(int i = 0;i < _devInfo.NumPOVs; ++i)
	{
		switch (_devState.POVs[i])
		{
		case RE::InputNative::POV_DIRECTION::Null:
			result.POVs[i] = RE::Input::POV_DIRECTION::Null;
			break;
		case RE::InputNative::POV_DIRECTION::Down:
			result.POVs[i] = RE::Input::POV_DIRECTION::Down;
			break;
		case RE::InputNative::POV_DIRECTION::Left:
			result.POVs[i] = RE::Input::POV_DIRECTION::Left;
			break;
		case RE::InputNative::POV_DIRECTION::Right:
			result.POVs[i] = RE::Input::POV_DIRECTION::Right;
			break;
		case RE::InputNative::POV_DIRECTION::Up:
			result.POVs[i] = RE::Input::POV_DIRECTION::Up;
			break;
		default:
			result.POVs[i] = RE::Input::POV_DIRECTION::Null;
			break;
		}
	}

	result.Buttons = gcnew array<bool>(_devInfo.NumButtons);
	for(int i = 0;i < _devInfo.NumButtons; ++i)
	{
		result.Buttons[i] = _devState.Buttons[i];
	}

	return result;
}
