#pragma once

using namespace System;

namespace RE
{
	namespace Input
	{
		public value struct DeviceInfo
		{
			System::Guid ID;
			System::String^ Name;
			bool XAxis;
			bool YAxis;
			bool ZAxis;
			bool XRotation;
			bool YRotation;
			bool ZRotation;
			int NumSliders;
			int NumPOVs;
			int NumButtons;

			virtual System::String^ ToString() override { return Name; }
		};
		
		public enum class POV_DIRECTION
		{
			Null,
			Up,
			Right,
			Down,
			Left
		};

		public value struct DeviceState
		{
			float XAxis;
			float YAxis;
			float ZAxis;
			float XRotation;
			float YRotation;
			float ZRotation;
			array<float>^ Sliders;// size will be DeviceInfo::NumSliders
			array<POV_DIRECTION>^ POVs; // size will be DeviceInfo::NumPOVs
			array<bool>^ Buttons; // size will be DeviceInfo::NumButtons
		};
	}
}