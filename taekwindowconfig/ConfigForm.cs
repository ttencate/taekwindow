using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Reflection;

namespace TaekwindowConfig
{
	public partial class ConfigForm : Form
	{
		public ConfigForm()
		{
			InitializeComponent();
			versionLabel.Text += Assembly.GetExecutingAssembly().GetName().Version.ToString();
		}

		/// <summary>
		/// Whether or not settings have changed since last save.
		/// </summary>
		private bool settingsSaved {
			get { return !applyButton.Enabled; }
			set { applyButton.Enabled = !value; }
		}

		/// <summary>
		/// Saves the settings that are currently in the GUI to the registry, and makes the DLL reload them.
		/// </summary>
		private void saveSettings()
		{
			settingsSaved = true;
		}

		private void settingChanged()
		{
			settingsSaved = false;
		}

		private void applyButton_Click(object sender, EventArgs e)
		{
			saveSettings();
		}

		private void exitButton_Click(object sender, EventArgs e)
		{
			if (!settingsSaved) {
				switch (MessageBox.Show(this, "Some of the settings were changed but not yet saved. Save them before exiting?", "Settings not saved", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question, MessageBoxDefaultButton.Button1)) {
					case DialogResult.Yes:
						saveSettings();
						break;
					case DialogResult.No:
						break;
					case DialogResult.Cancel:
						return;
				}
			}
			Close();
		}

		private void emailLinkLabel_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
		{
			System.Diagnostics.Process.Start(emailLinkLabel.Text);
			e.Link.Visited = true;
		}

		private void websiteLinkLabel_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
		{
			System.Diagnostics.Process.Start(websiteLinkLabel.Text);
			e.Link.Visited = true;
		}

		private void SettingChanged(object sender, EventArgs e)
		{
			settingsSaved = false;
		}
	}
}