namespace TaekwindowConfig
{
	partial class ModifierPicker
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
			this.leftShiftCheckBox = new System.Windows.Forms.CheckBox();
			this.leftCtrlCheckBox = new System.Windows.Forms.CheckBox();
			this.leftAltCheckBox = new System.Windows.Forms.CheckBox();
			this.rightAltCheckBox = new System.Windows.Forms.CheckBox();
			this.rightCtrlCheckBox = new System.Windows.Forms.CheckBox();
			this.rightShiftCheckBox = new System.Windows.Forms.CheckBox();
			this.explanationLabel = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// leftShiftCheckBox
			// 
			this.leftShiftCheckBox.Appearance = System.Windows.Forms.Appearance.Button;
			this.leftShiftCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.leftShiftCheckBox.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.leftShiftCheckBox.Location = new System.Drawing.Point(3, 3);
			this.leftShiftCheckBox.Margin = new System.Windows.Forms.Padding(3, 3, 0, 0);
			this.leftShiftCheckBox.Name = "leftShiftCheckBox";
			this.leftShiftCheckBox.Size = new System.Drawing.Size(87, 24);
			this.leftShiftCheckBox.TabIndex = 0;
			this.leftShiftCheckBox.Text = "Shift";
			this.leftShiftCheckBox.TextAlign = System.Drawing.ContentAlignment.TopLeft;
			this.leftShiftCheckBox.UseVisualStyleBackColor = true;
			this.leftShiftCheckBox.CheckedChanged += new System.EventHandler(this.CheckedChanged);
			// 
			// leftCtrlCheckBox
			// 
			this.leftCtrlCheckBox.Appearance = System.Windows.Forms.Appearance.Button;
			this.leftCtrlCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.leftCtrlCheckBox.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.leftCtrlCheckBox.Location = new System.Drawing.Point(3, 27);
			this.leftCtrlCheckBox.Margin = new System.Windows.Forms.Padding(3, 0, 0, 0);
			this.leftCtrlCheckBox.Name = "leftCtrlCheckBox";
			this.leftCtrlCheckBox.Size = new System.Drawing.Size(47, 24);
			this.leftCtrlCheckBox.TabIndex = 1;
			this.leftCtrlCheckBox.Text = "Ctrl";
			this.leftCtrlCheckBox.TextAlign = System.Drawing.ContentAlignment.TopLeft;
			this.leftCtrlCheckBox.UseVisualStyleBackColor = true;
			this.leftCtrlCheckBox.CheckedChanged += new System.EventHandler(this.CheckedChanged);
			// 
			// leftAltCheckBox
			// 
			this.leftAltCheckBox.Appearance = System.Windows.Forms.Appearance.Button;
			this.leftAltCheckBox.Checked = true;
			this.leftAltCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
			this.leftAltCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.leftAltCheckBox.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.leftAltCheckBox.Location = new System.Drawing.Point(77, 27);
			this.leftAltCheckBox.Margin = new System.Windows.Forms.Padding(0);
			this.leftAltCheckBox.Name = "leftAltCheckBox";
			this.leftAltCheckBox.Size = new System.Drawing.Size(47, 24);
			this.leftAltCheckBox.TabIndex = 2;
			this.leftAltCheckBox.Text = "Alt";
			this.leftAltCheckBox.TextAlign = System.Drawing.ContentAlignment.TopLeft;
			this.leftAltCheckBox.UseVisualStyleBackColor = true;
			this.leftAltCheckBox.CheckedChanged += new System.EventHandler(this.CheckedChanged);
			// 
			// rightAltCheckBox
			// 
			this.rightAltCheckBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.rightAltCheckBox.Appearance = System.Windows.Forms.Appearance.Button;
			this.rightAltCheckBox.Checked = true;
			this.rightAltCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
			this.rightAltCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.rightAltCheckBox.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.rightAltCheckBox.Location = new System.Drawing.Point(178, 27);
			this.rightAltCheckBox.Margin = new System.Windows.Forms.Padding(0);
			this.rightAltCheckBox.Name = "rightAltCheckBox";
			this.rightAltCheckBox.Size = new System.Drawing.Size(47, 24);
			this.rightAltCheckBox.TabIndex = 5;
			this.rightAltCheckBox.Text = "Alt";
			this.rightAltCheckBox.TextAlign = System.Drawing.ContentAlignment.TopLeft;
			this.rightAltCheckBox.UseVisualStyleBackColor = true;
			this.rightAltCheckBox.CheckedChanged += new System.EventHandler(this.CheckedChanged);
			// 
			// rightCtrlCheckBox
			// 
			this.rightCtrlCheckBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.rightCtrlCheckBox.Appearance = System.Windows.Forms.Appearance.Button;
			this.rightCtrlCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.rightCtrlCheckBox.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.rightCtrlCheckBox.Location = new System.Drawing.Point(251, 27);
			this.rightCtrlCheckBox.Margin = new System.Windows.Forms.Padding(0, 0, 3, 0);
			this.rightCtrlCheckBox.Name = "rightCtrlCheckBox";
			this.rightCtrlCheckBox.Size = new System.Drawing.Size(47, 24);
			this.rightCtrlCheckBox.TabIndex = 4;
			this.rightCtrlCheckBox.Text = "Ctrl";
			this.rightCtrlCheckBox.TextAlign = System.Drawing.ContentAlignment.TopLeft;
			this.rightCtrlCheckBox.UseVisualStyleBackColor = true;
			this.rightCtrlCheckBox.CheckedChanged += new System.EventHandler(this.CheckedChanged);
			// 
			// rightShiftCheckBox
			// 
			this.rightShiftCheckBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.rightShiftCheckBox.Appearance = System.Windows.Forms.Appearance.Button;
			this.rightShiftCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.rightShiftCheckBox.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.rightShiftCheckBox.Location = new System.Drawing.Point(211, 3);
			this.rightShiftCheckBox.Margin = new System.Windows.Forms.Padding(0, 3, 3, 0);
			this.rightShiftCheckBox.Name = "rightShiftCheckBox";
			this.rightShiftCheckBox.Size = new System.Drawing.Size(87, 24);
			this.rightShiftCheckBox.TabIndex = 3;
			this.rightShiftCheckBox.Text = "Shift";
			this.rightShiftCheckBox.TextAlign = System.Drawing.ContentAlignment.TopLeft;
			this.rightShiftCheckBox.UseVisualStyleBackColor = true;
			this.rightShiftCheckBox.CheckedChanged += new System.EventHandler(this.CheckedChanged);
			// 
			// explanationLabel
			// 
			this.explanationLabel.AutoSize = true;
			this.explanationLabel.Location = new System.Drawing.Point(0, 56);
			this.explanationLabel.Margin = new System.Windows.Forms.Padding(3, 5, 3, 0);
			this.explanationLabel.Name = "explanationLabel";
			this.explanationLabel.Size = new System.Drawing.Size(60, 13);
			this.explanationLabel.TabIndex = 6;
			this.explanationLabel.Text = "Any Alt key";
			// 
			// ModifierPicker
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.explanationLabel);
			this.Controls.Add(this.rightAltCheckBox);
			this.Controls.Add(this.rightCtrlCheckBox);
			this.Controls.Add(this.rightShiftCheckBox);
			this.Controls.Add(this.leftAltCheckBox);
			this.Controls.Add(this.leftCtrlCheckBox);
			this.Controls.Add(this.leftShiftCheckBox);
			this.Name = "ModifierPicker";
			this.Size = new System.Drawing.Size(301, 166);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.CheckBox leftShiftCheckBox;
		private System.Windows.Forms.CheckBox leftCtrlCheckBox;
		private System.Windows.Forms.CheckBox leftAltCheckBox;
		private System.Windows.Forms.CheckBox rightAltCheckBox;
		private System.Windows.Forms.CheckBox rightCtrlCheckBox;
		private System.Windows.Forms.CheckBox rightShiftCheckBox;
		private System.Windows.Forms.Label explanationLabel;

	}
}
