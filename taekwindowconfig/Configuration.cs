using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Win32;
using System.IO;
using System.Windows.Forms;

namespace TaekwindowConfig
{
	class Configuration
	{
		public bool StartAtLogon;
		public bool SystemTrayIcon;
		public Modifier Modifier;
		public MouseButton MoveButton;
		public MouseButton ResizeButton;
		public ResizeMode ResizeMode;

		private static string settingsKey = "Software\\Taekwindow\\0.2";

		public Configuration()
		{
			SetDefaults();
		}

		/// <summary>
		/// Sets all settings in this object to their default values.
		/// </summary>
		public void SetDefaults()
		{
			StartAtLogon = false;
			SystemTrayIcon = true;
			Modifier = Modifier.LeftAlt;
			MoveButton = MouseButton.Left;
			ResizeButton = MouseButton.Right;
			ResizeMode = ResizeMode.NineRectangles;
		}

		/// <summary>
		/// Reads the settings from the environment (mainly the registry).
		/// Settings that could not be read are set to their default values.
		/// </summary>
		public void ReadFromEnvironment()
		{
			SetDefaults();

			// Do our best to do everything; throw the last caught exception if any.
			Exception outEx = null;

			try {
				readFromFilesystem();
			} catch (Exception ex) {
				outEx = ex;
			}

			try {
				readFromRegistry();
			} catch (Exception ex) {
				outEx = ex;
			}

			if (outEx != null)
				throw outEx;
		}

		public void WriteToEnvironment()
		{
			// Do our best to do everything; throw the last caught exception if any.
			Exception outEx = null;

			try {
				writeToFilesystem();
			} catch (Exception ex) {
				outEx = ex;
			}

			try {
				writeToRegistry();
			} catch (Exception ex) {
				outEx = ex;
			}

			if (outEx != null)
				throw outEx;
		}

		private void writeToRegistry()
		{
			RegistryKey key;
			try {
				key = Registry.CurrentUser.CreateSubKey(settingsKey);
			} catch (Exception ex) {
				throw new Exception(
					String.Format("Could not open registry key {0} for writing:\n{1}\n\nYour settings were not saved.", ex.Message, settingsKey), ex);
			}

			key.SetValue("systemTrayIcon", SystemTrayIcon ? 1 : 0, RegistryValueKind.DWord);
			key.SetValue("modifier", Modifier, RegistryValueKind.DWord);
			key.SetValue("moveButton", MoveButton, RegistryValueKind.DWord);
			key.SetValue("resizeButton", ResizeButton, RegistryValueKind.DWord);
			key.SetValue("resizeMode", ResizeMode, RegistryValueKind.DWord);

			key.Close();
		}

		private void writeToFilesystem()
		{
			StartupShortcut.SetExistence(StartAtLogon);
		}

		private void readFromRegistry()
		{
			RegistryKey key = Registry.CurrentUser.OpenSubKey(settingsKey);
			if (key != null) {
				SystemTrayIcon = ((int)key.GetValue("systemTrayIcon", SystemTrayIcon ? 1 : 0) != 0);
				Modifier = (Modifier)key.GetValue("modifier", Modifier);
				MoveButton = (MouseButton)key.GetValue("moveButton", MoveButton);
				ResizeButton = (MouseButton)key.GetValue("resizeButton", ResizeButton);
				ResizeMode = (ResizeMode)key.GetValue("resizeMode", ResizeMode);
			}
		}

		private void readFromFilesystem()
		{
			StartAtLogon = StartupShortcut.Exists();
		}

	}

	static class StartupShortcut
	{
		public static bool Exists()
		{
			return File.Exists(linkFilename());
		}

		public static void SetExistence(bool existence)
		{
			if (existence == Exists())
				return;

			if (existence) {
				createShortcut();
			} else {
				deleteShortcut();
			}
		}

		/// <summary>
		/// Determines where to place the startup shortcut.
		/// </summary>
		/// <returns>The full path to the startup shortcut, or null if it could not be determined.</returns>
		private static string linkFilename()
		{
			return System.Environment.GetFolderPath(Environment.SpecialFolder.Startup) + "\\Taekwindow.lnk";
		}

		/// <summary>
		/// Locates the main exe.
		/// </summary>
		/// <returns>The full path to the main exe, or null if not found.</returns>
		private static string exeFilename()
		{
			string f = Directory.GetParent(Application.ExecutablePath).FullName + "\\" + VersionInfo.MainExeFile + ".exe";
			if (File.Exists(f))
				return f;
			else
				throw new FileNotFoundException("The file " + VersionInfo.MainExeFile + ".exe cannot be found in the directory of the current application. The shortcut can not be created.");
		}

		/// <summary>
		/// Creates a startup shortcut, or leaves it alone if it exists.
		/// </summary>
		private static void createShortcut()
		{
			string lnk = linkFilename();
			string exe = exeFilename();

			try {
				// Use the Windows Scripting Host to create a shortcut.
				// It would probably be better to use the IShellLink COM interface from shell32.dll,
				// but I somehow can't get that to work.
				// Also, it would be nice if I managed to manually declare the required interfaces,
				// so we could do away with the generated interop DLL.
				IWshRuntimeLibrary.IWshShell shell = new IWshRuntimeLibrary.WshShell();
				IWshRuntimeLibrary.IWshShortcut link = (IWshRuntimeLibrary.IWshShortcut)shell.CreateShortcut(lnk);
				link.TargetPath = exe;
				link.Arguments = "";
				link.WorkingDirectory = Directory.GetParent(exe).FullName;
				link.Description = VersionInfo.Title;
				link.IconLocation = exe;
				link.Save();
			} catch (Exception ex) {
				throw new Exception(String.Format("The shortcut cannot be created for the following reason:\n{0}", ex.Message), ex);
			}
		}

		/// <summary>
		/// Deletes the startup shortcut.
		/// </summary>
		private static void deleteShortcut()
		{
			string lnk = linkFilename();
			
			try {
				File.Delete(lnk);
			} catch (Exception ex) {
				throw new Exception(String.Format("The shortcut cannot be deleted for the following reason:\n{0}", ex.Message), ex);
			}
		}
	}
}
