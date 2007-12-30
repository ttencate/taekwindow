namespace TaekwindowConfig
{
	partial class KeyComboPicker
	{
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary> 
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null)) {
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Component Designer generated code

		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.leftRadioButton = new System.Windows.Forms.RadioButton();
			this.anyRadioButton = new System.Windows.Forms.RadioButton();
			this.rightRadioButton = new System.Windows.Forms.RadioButton();
			this.modifierPicker = new TaekwindowConfig.ModifierPicker();
			this.SuspendLayout();
			// 
			// leftRadioButton
			// 
			this.leftRadioButton.AutoSize = true;
			this.leftRadioButton.Checked = true;
			this.leftRadioButton.Location = new System.Drawing.Point(3, 26);
			this.leftRadioButton.Name = "leftRadioButton";
			this.leftRadioButton.Size = new System.Drawing.Size(43, 17);
			this.leftRadioButton.TabIndex = 1;
			this.leftRadioButton.TabStop = true;
			this.leftRadioButton.Text = "Left";
			this.leftRadioButton.UseVisualStyleBackColor = true;
			// 
			// anyRadioButton
			// 
			this.anyRadioButton.AutoSize = true;
			this.anyRadioButton.Location = new System.Drawing.Point(52, 26);
			this.anyRadioButton.Name = "anyRadioButton";
			this.anyRadioButton.Size = new System.Drawing.Size(43, 17);
			this.anyRadioButton.TabIndex = 2;
			this.anyRadioButton.Text = "Any";
			this.anyRadioButton.UseVisualStyleBackColor = true;
			// 
			// rightRadioButton
			// 
			this.rightRadioButton.AutoSize = true;
			this.rightRadioButton.Location = new System.Drawing.Point(101, 26);
			this.rightRadioButton.Name = "rightRadioButton";
			this.rightRadioButton.Size = new System.Drawing.Size(50, 17);
			this.rightRadioButton.TabIndex = 3;
			this.rightRadioButton.Text = "Right";
			this.rightRadioButton.UseVisualStyleBackColor = true;
			// 
			// modifierPicker
			// 
			this.modifierPicker.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.modifierPicker.Location = new System.Drawing.Point(0, 0);
			this.modifierPicker.Name = "modifierPicker";
			this.modifierPicker.Size = new System.Drawing.Size(156, 20);
			this.modifierPicker.TabIndex = 0;
			// 
			// KeyComboPicker
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.rightRadioButton);
			this.Controls.Add(this.anyRadioButton);
			this.Controls.Add(this.leftRadioButton);
			this.Controls.Add(this.modifierPicker);
			this.Name = "KeyComboPicker";
			this.Size = new System.Drawing.Size(156, 43);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private ModifierPicker modifierPicker;
		private System.Windows.Forms.RadioButton leftRadioButton;
		private System.Windows.Forms.RadioButton anyRadioButton;
		private System.Windows.Forms.RadioButton rightRadioButton;
	}
}
