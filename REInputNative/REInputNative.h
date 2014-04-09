#pragma once

#include <guiddef.h>
#include <string>
#include <algorithm>

namespace RE
{
	namespace InputNative
	{
		struct DeviceInfo
		{
			DeviceInfo() : XAxis(false), YAxis(false), ZAxis(false),
				XRotation(false), YRotation(false), ZRotation(false), NumSliders(0), NumPOVs(0), NumButtons(0) {}

			GUID ID;
			std::wstring Name;
			bool XAxis;
			bool YAxis;
			bool ZAxis;
			bool XRotation;
			bool YRotation;
			bool ZRotation;
			int NumSliders;
			int NumPOVs;
			int NumButtons;
		};

		enum class POV_DIRECTION
		{
			Null,
			Up,
			Right,
			Down,
			Left
		};

		struct DeviceState
		{
			DeviceState() : XAxis(0.0f), YAxis(0.0f), ZAxis(0.0f), XRotation(0.0f), YRotation(0.0f), ZRotation(0.0f)
			{
				std::fill(std::begin(Sliders), std::end(Sliders), 0.0f);
				std::fill(std::begin(POVs), std::end(POVs), POV_DIRECTION::Null);
				std::fill(std::begin(Buttons), std::end(Buttons), false);
			}

			float XAxis;
			float YAxis;
			float ZAxis;
			float XRotation;
			float YRotation;
			float ZRotation;
			float Sliders[2];
			POV_DIRECTION POVs[4];
			bool Buttons[128];
		};

		struct IInputDevice
		{
			virtual ~IInputDevice() {}
			virtual const DeviceInfo& GetDeviceInfo() const = 0;
			virtual void Update() = 0;
			virtual const DeviceState& GetDeviceState() const = 0;
		};
	}
}