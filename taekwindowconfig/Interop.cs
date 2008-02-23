using System.Runtime.InteropServices;
using System;

namespace TaekwindowConfig
{

	class Win32
	{

		[DllImport("user32.dll", SetLastError = true)]
		public static extern bool PostThreadMessage(int threadId, int message, int wParam, int lParam);

		[DllImport("user32.dll")]
		public static extern bool SetForegroundWindow(IntPtr hWnd);

		public const int WM_APP = 0x8000;

	}

}
