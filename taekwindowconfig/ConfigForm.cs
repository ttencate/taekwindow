using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Reflection;
using Microsoft.Win32;
using System.IO;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace TaekwindowConfig
{
	public partial class ConfigForm : Form
	{
		private static string settingsKey = "Software\\Taekwindow\\0.2";

		public ConfigForm()
		{
			InitializeComponent();
			Text = VersionInfo.Title + " configuration";
			title.Text = VersionInfo.Title;
			version.Text = VersionInfo.VersionString;
			author.Text = VersionInfo.Author;
			website.Text = VersionInfo.Website;
			email.Text = VersionInfo.Email;
			loadSettings();
		}

		private void loadSettings()
		{
			readRegistry();
			startAtLogon.Checked = File.Exists(linkFilename());
			settingsSaved = true;
		}

		private void readRegistry()
		{
			RegistryKey key = Registry.CurrentUser.OpenSubKey(settingsKey);
			if (key != null) {
				Modifier modifier = (Modifier)key.GetValue("modifier", Modifier.LeftAlt);
				leftAlt.Checked = (modifier == Modifier.LeftAlt);
				eitherAlt.Checked = (modifier == Modifier.Alt);
				rightAlt.Checked = (modifier == Modifier.RightAlt);

				MouseButton moveButton = (MouseButton)key.GetValue("moveButton", MouseButton.Left);
				moveLeft.Checked = (moveButton == MouseButton.Left);
				moveMiddle.Checked = (moveButton == MouseButton.Middle);
				moveRight.Checked = (moveButton == MouseButton.Right);

				MouseButton resizeButton = (MouseButton)key.GetValue("resizeButton", MouseButton.Right);
				resizeLeft.Checked = (resizeButton == MouseButton.Left);
				resizeMiddle.Checked = (resizeButton == MouseButton.Middle);
				resizeRight.Checked = (resizeButton == MouseButton.Right);

				ResizeMode resizeMode = (ResizeMode)key.GetValue("resizeMode", ResizeMode.NineRectangles);
				bottomRight.Checked = (resizeMode == ResizeMode.BottomRight);
				nineRectangles.Checked = (resizeMode == ResizeMode.NineRectangles);
			}
		}

		/// <summary>
		/// Whether or not settings have changed since last save.
		/// </summary>
		private bool settingsSaved {
			get { return !apply.Enabled; }
			set { apply.Enabled = !value; }
		}

		/// <summary>
		/// Saves the settings that are currently in the GUI to the registry, and makes the DLL reload them.
		/// </summary>
		private void saveSettings()
		{
			if (writeRegistry()) {
				triggerReload();
			}
			if (startAtLogon.Checked) {
				createShortcut();
			} else {
				deleteShortcut();
			}
			settingsSaved = true;
		}

		/// <summary>
		/// Writes the settings to the registry.
		/// </summary>
		/// <returns>true on success</returns>
		private bool writeRegistry()
		{
			RegistryKey key;
			try {
				key = Registry.CurrentUser.CreateSubKey(settingsKey);
			} catch (Exception ex) {
				MessageBox.Show(
					String.Format("Could not open registry key {0} for writing:\n{1}\n\nYour settings were not saved.", ex.Message, settingsKey),
					"Error saving settings",
					MessageBoxButtons.OK,
					MessageBoxIcon.Exclamation);
				return false;
			}

			Modifier modifier =
				rightAlt.Checked ? Modifier.RightAlt :
				eitherAlt.Checked ? Modifier.Alt :
				Modifier.LeftAlt;
			key.SetValue("modifier", modifier, RegistryValueKind.DWord);

			MouseButton moveButton =
				moveMiddle.Checked ? MouseButton.Middle :
				moveRight.Checked ? MouseButton.Right :
				MouseButton.Left;
			key.SetValue("moveButton", moveButton, RegistryValueKind.DWord);

			MouseButton resizeButton =
				resizeLeft.Checked ? MouseButton.Left :
				resizeMiddle.Checked ? MouseButton.Middle :
				MouseButton.Right;
			key.SetValue("resizeButton", resizeButton, RegistryValueKind.DWord);

			ResizeMode resizeMode = bottomRight.Checked ? ResizeMode.BottomRight : ResizeMode.NineRectangles;
			key.SetValue("resizeMode", resizeMode, RegistryValueKind.DWord);

			key.Close();
			return true;
		}

		/// <summary>
		/// Kicks the Taekwindow DLL to reload its settings.
		/// </summary>
		private void triggerReload()
		{
			Process[] procs = Process.GetProcessesByName("taekwindow"); // .exe filename without extension or path
			bool attempted = false;
			bool succeeded = false;
			int lastError = 0;
			foreach (Process proc in procs) {
				foreach (ProcessThread thread in proc.Threads) {
					attempted = true;
					if (Win32.PostThreadMessage(thread.Id, Win32.WM_APP, 0, 0)) {
						succeeded = true;
					} else {
						lastError = Marshal.GetLastWin32Error();
					}
				}
			}
			// Show an error message if at least one message posting was attempted,
			// but none succeeded.
			if (attempted && !succeeded) {
				MessageBox.Show(this, String.Format("{1} seems to be running, but the reloading of its settings failed:\n{0}\n\nYour settings were saved successfully. Restart {1} manually to make them effective.", new Win32Exception(lastError).Message, VersionInfo.Title), "Error triggering settings reload", MessageBoxButtons.OK, MessageBoxIcon.Warning);
			}
		}

		/// <summary>
		/// Locates taekwindow.exe.
		/// </summary>
		/// <returns>The full path to taekwindow.exe, or null if not found.</returns>
		private string exeFilename()
		{
			string f = Directory.GetParent(Application.ExecutablePath).FullName + "\\taekwindow.exe";
			if (File.Exists(f))
				return f;
			else
				return null;
		}

		/// <summary>
		/// Determines where to place the startup shortcut.
		/// </summary>
		/// <returns>The full path to the startup shortcut, or null if it could not be determined.</returns>
		private string linkFilename()
		{
			return System.Environment.GetFolderPath(Environment.SpecialFolder.Startup) + "\\Taekwindow.lnk";
		}

		/// <summary>
		/// Creates a startup shortcut, or leaves it alone if it exists.
		/// </summary>
		/// <returns>true on success</returns>
		private bool createShortcut()
		{
			string lnk = linkFilename();
			if (File.Exists(lnk))
				return true;

			string exe = exeFilename();
			if (exe == null) {
				MessageBox.Show(this, "The file taekwindow.exe cannot be found in the directory of the current application. The shortcut can not be created.", "File not found", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
				return false;
			}

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
				MessageBox.Show(this, String.Format("The shortcut can not be created for the following reason:\n{0}", ex.Message), "Failed to create shortcut", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
				return false;
			}
			
			return true;
		}

		/// <summary>
		/// Deletes the startup shortcut if it is there.
		/// </summary>
		/// <returns>true on success</returns>
		private bool deleteShortcut()
		{
			string lnk = linkFilename();
			if (!File.Exists(lnk)) {
				return true;
			}
			
			try {
				File.Delete(lnk);
			} catch (Exception ex) {
				MessageBox.Show(this, String.Format("The shortcut can not be deleted for the following reason:\n{0}", ex.Message), "Failed to create shortcut", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
				return false;
			}

			return true;
		}

		private void okButton_Click(object sender, EventArgs e)
		{
			if (!settingsSaved) {
				saveSettings();
			}
			Close();
		}

		private void cancel_Click(object sender, EventArgs e)
		{
			Close();
		}

		private void applyButton_Click(object sender, EventArgs e)
		{
			if (!settingsSaved) {
				saveSettings();
			}
		}

		private void emailLinkLabel_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
		{
			System.Diagnostics.Process.Start(email.Text);
			e.Link.Visited = true;
		}

		private void websiteLinkLabel_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
		{
			System.Diagnostics.Process.Start(website.Text);
			e.Link.Visited = true;
		}

		private void SettingChanged(object sender, EventArgs e)
		{
			settingsSaved = false;
		}

		private void moveLeft_Click(object sender, EventArgs e)
		{
			if (resizeLeft.Checked)
				resizeRight.Checked = true;
		}

		private void moveMiddle_Click(object sender, EventArgs e)
		{
			if (resizeMiddle.Checked)
				resizeRight.Checked = true;
		}

		private void moveRight_Click(object sender, EventArgs e)
		{
			if (resizeRight.Checked)
				resizeMiddle.Checked = true;
		}

		private void resizeLeft_Click(object sender, EventArgs e)
		{
			if (moveLeft.Checked)
				moveMiddle.Checked = true;
		}

		private void resizeMiddle_Click(object sender, EventArgs e)
		{
			if (moveMiddle.Checked)
				moveLeft.Checked = true;
		}

		private void resizeRight_Click(object sender, EventArgs e)
		{
			if (moveRight.Checked)
				moveLeft.Checked = true;
		}
	}
}