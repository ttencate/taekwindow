using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace TaekwindowConfig
{
	public partial class ModifierPicker : TextBox
	{
		public ModifierPicker()
		{
			InitializeComponent();
		}

		protected override void OnKeyDown(KeyEventArgs e)
		{
			if (ModifierKeys != Keys.None) {
				Text = ModifierKeys.ToString();
			}
			e.Handled = true;
			e.SuppressKeyPress = true;
			base.OnKeyDown(e);
		}

		protected override void OnKeyPress(KeyPressEventArgs e)
		{
			e.Handled = true;
			base.OnKeyPress(e);
		}
	}
}
