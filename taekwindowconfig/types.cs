namespace TaekwindowConfig
{
	// These correspond to the #defines in winuser.h
	enum Modifier
	{
		Shift = 0x10,
		Control = 0x11,
		Alt = 0x12,
		LeftShift = 0xA0,
		RightShift = 0xA1,
		LeftControl = 0xA2,
		RightControl = 0xA3,
		LeftAlt = 0xA4,
		RightAlt = 0xA5
	};

	enum MouseButton
	{
		Left = 0,
		Middle = 1,
		Right = 2
	};

	enum ResizeMode
	{
		BottomRight = 0, NineRectangles = 1
	};
}