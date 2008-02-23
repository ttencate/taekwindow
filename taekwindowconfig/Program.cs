using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Diagnostics;

namespace TaekwindowConfig
{
	static class Program
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main()
		{
			// Check whether there is already a running instance.
			Process[] procs = Process.GetProcessesByName(VersionInfo.ConfigExeFile); // .exe filename without extension or path
			if (procs.Length > 1) { // we count ourselves too!
				foreach (Process p in procs) {
					if (p.Id == Process.GetCurrentProcess().Id)
						continue;
					Win32.SetForegroundWindow(p.MainWindowHandle);
				}
				return;
			}

			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			Application.Run(new ConfigForm());
		}
	}
}