using System.Runtime.InteropServices;

namespace TaekwindowConfig
{

	class Win32
	{

		[DllImport("user32.dll", SetLastError = true)]
		public static extern bool PostThreadMessage(int threadId, int message, int wParam, int lParam);

		public const int WM_APP = 0x8000;

	}

}
