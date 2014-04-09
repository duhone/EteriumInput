#include "stdafx.h"
#include "REInputManagerNative.h"
#include <atlbase.h>
#include <dinput.h>
#include "Util.h"
#include "REInputDeviceNative.h"

namespace RE
{
	namespace InputNative
	{
		class InputManagerImpl
		{
		public:
			InputManagerImpl(HINSTANCE _instance, HWND _hwnd);
			std::vector<DeviceInfo> EnumDevices();
			std::shared_ptr<IInputDevice> CreateDevice(GUID _id);
		private:
			static BOOL PASCAL EnumDeviceCallback(LPCDIDEVICEINSTANCEW _deviceInstance, LPVOID _result);
			static BOOL PASCAL EnumDeviceObjectsCallback(LPCDIDEVICEOBJECTINSTANCE _object, LPVOID _result);

			CComPtr<IDirectInput8> m_Dinput;
			HWND m_Hwnd;
		};
	}
}

using namespace std;
using namespace RE::InputNative;

InputManagerImpl::InputManagerImpl(HINSTANCE _instance, HWND _hwnd) : m_Hwnd(_hwnd)
{
	DirectInput8Create(_instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&m_Dinput, nullptr);
}

BOOL PASCAL InputManagerImpl::EnumDeviceCallback(LPCDIDEVICEINSTANCEW _deviceInstance, LPVOID _result)
{
	if(!IsXInputDevice(&_deviceInstance->guidProduct))
	{
		std::vector<DeviceInfo>* result = reinterpret_cast<std::vector<DeviceInfo>*>(_result);
		result->emplace_back();
		auto& currentDevice = result->back();
		currentDevice.ID = _deviceInstance->guidInstance;
		currentDevice.Name = _deviceInstance->tszProductName;
	}
	return DIENUM_CONTINUE;
}

BOOL PASCAL InputManagerImpl::EnumDeviceObjectsCallback(LPCDIDEVICEOBJECTINSTANCE _object, LPVOID _result)
{
	auto devInfo = reinterpret_cast<DeviceInfo*>(_result);
	if(_object->guidType == GUID_XAxis)
		devInfo->XAxis = true;
	if(_object->guidType == GUID_YAxis)
		devInfo->YAxis = true;
	if(_object->guidType == GUID_ZAxis)
		devInfo->ZAxis = true;
	if(_object->guidType == GUID_RxAxis)
		devInfo->XRotation = true;
	if(_object->guidType == GUID_RyAxis)
		devInfo->YRotation = true;
	if(_object->guidType == GUID_RzAxis)
		devInfo->ZRotation = true;
	if(_object->guidType == GUID_Slider)
		devInfo->NumSliders++;
	return DIENUM_CONTINUE;
}

std::vector<DeviceInfo> InputManagerImpl::EnumDevices()
{
	std::vector<DeviceInfo> result;

	auto hr = m_Dinput->EnumDevices(DI8DEVCLASS_GAMECTRL, &InputManagerImpl::EnumDeviceCallback, &result, DIEDFL_ALLDEVICES);
	if(FAILED(hr))
		return result;

	std::vector<GUID> failedDevices;

	for(auto& devInfo : result)
	{
		CComPtr<IDirectInputDevice8> comDevice;
		hr = m_Dinput->CreateDevice(devInfo.ID, &comDevice, nullptr);
		if(FAILED(hr))
		{
			failedDevices.push_back(devInfo.ID);
			continue;
		}

		DIDEVCAPS caps;
		caps.dwSize = sizeof(caps);
		hr = comDevice->GetCapabilities(&caps);
		if(FAILED(hr))
		{
			failedDevices.push_back(devInfo.ID);
			continue;
		}

		devInfo.NumButtons = caps.dwButtons;
		devInfo.NumPOVs = caps.dwPOVs;

		hr = comDevice->EnumObjects(&InputManagerImpl::EnumDeviceObjectsCallback, &devInfo, DIDFT_ABSAXIS);
		if(FAILED(hr))		
		{
			failedDevices.push_back(devInfo.ID);
			continue;
		}
	}
	//remove any devices we couldn't get info for
	result.erase(RE::remove_if(result, [&](const DeviceInfo& _devInfo) {
		return RE::find(failedDevices, _devInfo.ID) != end(failedDevices);
	}), end(result));
	
	//get rid of any that have no usable axis or buttons. For example razer hydra reports 2 sticks. one has no buttons, axis
	//	or anything else. get rid of that one. If you cant use it for anything, no reason to report it.
	auto remIter = RE::remove_if(result, [](const DeviceInfo& _devInfo) -> bool {
		return _devInfo.NumButtons == 0 && _devInfo.NumPOVs == 0 && _devInfo.NumSliders == 0 && !_devInfo.XAxis &&
			!_devInfo.XRotation && !_devInfo.YAxis && !_devInfo.YRotation && !_devInfo.ZAxis && !_devInfo.ZRotation;
	});
	result.erase(remIter, end(result));

	return result;
}

std::shared_ptr<IInputDevice> InputManagerImpl::CreateDevice(GUID _id)
{
	auto devices = EnumDevices(); //not the fastest way, but this is not a fast path function. reuse existing code
	auto device = RE::find_if(devices, [&](const DeviceInfo& _info) { return _info.ID == _id;} );
	if(device == end(devices))
		return nullptr;
	
	CComPtr<IDirectInputDevice8> comDevice;
	auto hr = m_Dinput->CreateDevice(device->ID, &comDevice, nullptr);
	if(FAILED(hr))
		return nullptr;

	return make_shared<InputDevice>(*device, comDevice, m_Hwnd);
}

#pragma region pimpl wrapper

InputManager::InputManager(void* _instance, int* _hwnd) : m_Pimpl(new InputManagerImpl((HINSTANCE)_instance, (HWND)_hwnd))
{

}

InputManager::~InputManager()
{
	delete m_Pimpl;
}

std::vector<DeviceInfo> InputManager::EnumDevices()
{
	return m_Pimpl->EnumDevices();
}

std::shared_ptr<IInputDevice> InputManager::CreateDevice(GUID _id)
{
	return m_Pimpl->CreateDevice(_id);
}

#pragma endregion