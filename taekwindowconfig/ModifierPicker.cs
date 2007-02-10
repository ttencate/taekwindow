using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace TaekwindowConfig
{
	public partial class ModifierPicker : UserControl
	{
		public ModifierPicker()
		{
			InitializeComponent();
			updateExplanation();
		}

		private string leftRightString(string key, bool left, bool right)
		{
			if (!left && !right)
				return "";
			else if (left && !right)
				return "left " + key;
			else if (!left && right)
				return "right " + key;
			else // left && right
				return key;
		}

		private string capitalize(string text)
		{
			if (text.Length > 0) {
				return text[0].ToString().ToUpper() + text.Substring(1);
			} else {
				return text;
			}
		}

		private void updateExplanation()
		{
			string keyString = "";
			string[] keys = {
				leftRightString("Ctrl", leftCtrlCheckBox.Checked, rightCtrlCheckBox.Checked),
				leftRightString("Shift", leftShiftCheckBox.Checked, rightShiftCheckBox.Checked),
				leftRightString("Alt", leftAltCheckBox.Checked, rightAltCheckBox.Checked)
			};
			List<string> usedKeys = new List<string>(keys);
			usedKeys.RemoveAll(delegate(string s) {
				return s == "";
			});
			if (usedKeys.Count == 0) {
				explanationLabel.Text = "No modifier key is needed to " + ActionText + " (bad idea)";
				return;
			} else if (usedKeys.Count == 1) {
				keyString = usedKeys[0];
			} else {
				keyString = usedKeys[usedKeys.Count - 1];
				usedKeys.RemoveAt(usedKeys.Count - 1);
				keyString = String.Join(", ", usedKeys.ToArray()) + " and " + keyString;
			}
			explanationLabel.Text = "Hold down " + keyString + " to " + ActionText;
		}

		private string actionText = "";
		public string ActionText
		{
			get { return actionText; }
			set { actionText = value; updateExplanation(); }
		}

		private void CheckedChanged(object sender, EventArgs e)
		{
			updateExplanation();
		}
	}
}
