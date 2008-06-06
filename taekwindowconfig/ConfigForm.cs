using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Reflection;
using System.IO;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace TaekwindowConfig
{
	public partial class ConfigForm : Form
	{
		public ConfigForm()
		{
			InitializeComponent();
			Text = VersionInfo.Title + " configuration";
			aboutBox.Text = "About " + VersionInfo.Title;
			title.Text = VersionInfo.Title;
			version.Text = "Version " + VersionInfo.ShortVersionString;
			author.Text = "Written by " + VersionInfo.Author;
			website.Text = VersionInfo.Website;
			email.Text = VersionInfo.Email;
			license.Text = "This program is licensed under the BSD license. This means you are free and welcome to distribute this program. See the " + VersionInfo.ReadmeFile + " file for details.";

			loadSettings();
		}

		private void loadSettings()
		{
			Configuration config = new Configuration();
			try {
				config.ReadFromEnvironment();
			} catch (Exception ex) {
				MessageBox.Show(ex.Message, "Error loading configuration", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
			configToGui(config);
			settingsSaved = true;
		}

		private void configToGui(Configuration config)
		{
			startAtLogon.Checked = config.StartAtLogon;
			systemTrayIcon.Checked = config.SystemTrayIcon;

			leftAlt.Checked = (config.Modifier == Modifier.LeftAlt);
			eitherAlt.Checked = (config.Modifier == Modifier.Alt);
			rightAlt.Checked = (config.Modifier == Modifier.RightAlt);

			moveLeft.Checked = (config.MoveButton == MouseButton.Left);
			moveMiddle.Checked = (config.MoveButton == MouseButton.Middle);
			moveRight.Checked = (config.MoveButton == MouseButton.Right);

			resizeLeft.Checked = (config.ResizeButton == MouseButton.Left);
			resizeMiddle.Checked = (config.ResizeButton == MouseButton.Middle);
			resizeRight.Checked = (config.ResizeButton == MouseButton.Right);

			bottomRight.Checked = (config.ResizeMode == ResizeMode.BottomRight);
			nineRectangles.Checked = (config.ResizeMode == ResizeMode.NineRectangles);

			scrollFocused.Checked = !config.ScrollWindowUnderCursor;
			scrollUnderCursor.Checked = config.ScrollWindowUnderCursor;
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
			Configuration config = guiToConfig();
			try {
				config.WriteToEnvironment();
			} catch (Exception ex) {
				MessageBox.Show(ex.Message, "Error saving configuration", MessageBoxButtons.OK, MessageBoxIcon.Error);
				return;
			}
			settingsSaved = true;

			triggerReload();
		}

		/// <summary>
		/// Takes the settings from the GUI controls and puts them into a Configuration object.
		/// </summary>
		private Configuration guiToConfig()
		{
			Configuration config = new Configuration();

			config.StartAtLogon = startAtLogon.Checked;
			config.SystemTrayIcon = systemTrayIcon.Checked;
			config.Modifier =
				rightAlt.Checked ? Modifier.RightAlt :
				eitherAlt.Checked ? Modifier.Alt :
				Modifier.LeftAlt;
			config.MoveButton =
				moveMiddle.Checked ? MouseButton.Middle :
				moveRight.Checked ? MouseButton.Right :
				MouseButton.Left;
			config.ResizeButton =
				resizeLeft.Checked ? MouseButton.Left :
				resizeMiddle.Checked ? MouseButton.Middle :
				MouseButton.Right;
			config.ResizeMode =
				bottomRight.Checked ? ResizeMode.BottomRight :
				ResizeMode.NineRectangles;
			config.ScrollWindowUnderCursor = scrollUnderCursor.Checked;

			return config;
		}

		/// <summary>
		/// Kicks the Taekwindow DLL to reload its settings.
		/// </summary>
		private void triggerReload()
		{
			Process[] procs = Process.GetProcessesByName(VersionInfo.MainExeFile); // .exe filename without extension or path
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