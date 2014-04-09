#include "stdafx.h"

#include "REInputDevice.h"
#include "REInputNative.h"
#include "Util.h"

namespace RE
{
	namespace Input
	{
		private class InputDeviceImpl
		{
		public:
			InputDeviceImpl(std::shared_ptr<RE::InputNative::IInputDevice> _device);
			DeviceInfo GetDeviceInfo() const;
			void Update();
			const RE::InputNative::DeviceState& GetDeviceState() const { return m_device->GetDeviceState(); }
			const RE::InputNative::DeviceInfo& GetDeviceNativeInfo() const { return m_device->GetDeviceInfo(); }
		private:
			std::shared_ptr<RE::InputNative::IInputDevice> m_device;
		};
	}
}

using namespace RE::Input;

InputDeviceImpl::InputDeviceImpl(std::shared_ptr<RE::InputNative::IInputDevice> _device) : m_device(_device)
{
}

DeviceInfo InputDeviceImpl::GetDeviceInfo() const
{
	auto devInfo = m_device->GetDeviceInfo();
	return ConvertDevInfo(devInfo);
}

void InputDeviceImpl::Update()
{
	m_device->Update();
}

#pragma region Wrapper

InputDevice::InputDevice(std::shared_ptr<RE::InputNative::IInputDevice> _device)
{
	m_Pimpl = new InputDeviceImpl(_device);
	m_DeviceInfo = m_Pimpl->GetDeviceInfo();
}

InputDevice::~InputDevice()
{
	this->!InputDevice();
}

InputDevice::!InputDevice()
{
	delete m_Pimpl;
	m_Pimpl = nullptr;
}

void InputDevice::Update()
{
	m_Pimpl->Update();
	m_DeviceState = ConvertDevState(m_Pimpl->GetDeviceNativeInfo(), m_Pimpl->GetDeviceState());
}

#pragma endregion