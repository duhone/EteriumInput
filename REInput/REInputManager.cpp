// This is the main DLL file.
#include "stdafx.h"

#include "REInputManager.h"
#include "REInputManagerNative.h"
#include "Util.h"

using namespace RE::Input;

InputManager::InputManager(System::IntPtr _windowHandle)
{
	IntPtr hInst = System::Runtime::InteropServices::Marshal::GetHINSTANCE(this->GetType()->Module);
	m_Pimpl = new RE::InputNative::InputManager(hInst.ToPointer(), (int*)_windowHandle.ToPointer());
}

InputManager::~InputManager()
{
	this->!InputManager();
}

InputManager::!InputManager()
{
	delete m_Pimpl;
	m_Pimpl = nullptr;
}

System::Collections::Generic::IEnumerable<DeviceInfo>^ InputManager::EnumDevices()
{
	auto result = gcnew System::Collections::Generic::List<DeviceInfo>();
	auto devices = m_Pimpl->EnumDevices();
	for(const auto& device : devices)
	{
		DeviceInfo newDevice = ConvertDevInfo(device);
		result->Add(newDevice);
	}
	return result;
}

InputDevice^ InputManager::CreateDevice(System::Guid _id)
{
	GUID id;
	char* idc = reinterpret_cast<char*>(&id);
	auto idArray = _id.ToByteArray();
	for(int i = 0;i < 16; ++i)
	{
		idc[i] = idArray[i];
	}
	auto device = m_Pimpl->CreateDevice(id);
	return gcnew InputDevice(device);
}