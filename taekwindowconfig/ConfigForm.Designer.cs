namespace TaekwindowConfig
{
	partial class ConfigForm
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

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ConfigForm));
			this.apply = new System.Windows.Forms.Button();
			this.ok = new System.Windows.Forms.Button();
			this.tabControl = new System.Windows.Forms.TabControl();
			this.general = new System.Windows.Forms.TabPage();
			this.label1 = new System.Windows.Forms.Label();
			this.startAtLogon = new System.Windows.Forms.CheckBox();
			this.keysAndButtons = new System.Windows.Forms.TabPage();
			this.label5 = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.panel3 = new System.Windows.Forms.Panel();
			this.resizeMiddle = new System.Windows.Forms.RadioButton();
			this.resizeRight = new System.Windows.Forms.RadioButton();
			this.resizeLeft = new System.Windows.Forms.RadioButton();
			this.panel1 = new System.Windows.Forms.Panel();
			this.moveMiddle = new System.Windows.Forms.RadioButton();
			this.moveRight = new System.Windows.Forms.RadioButton();
			this.moveLeft = new System.Windows.Forms.RadioButton();
			this.panel2 = new System.Windows.Forms.Panel();
			this.eitherAlt = new System.Windows.Forms.RadioButton();
			this.rightAlt = new System.Windows.Forms.RadioButton();
			this.leftAlt = new System.Windows.Forms.RadioButton();
			this.label3 = new System.Windows.Forms.Label();
			this.resizing = new System.Windows.Forms.TabPage();
			this.nineRectangles = new System.Windows.Forms.RadioButton();
			this.bottomRight = new System.Windows.Forms.RadioButton();
			this.about = new System.Windows.Forms.TabPage();
			this.label2 = new System.Windows.Forms.Label();
			this.website = new System.Windows.Forms.LinkLabel();
			this.email = new System.Windows.Forms.LinkLabel();
			this.author = new System.Windows.Forms.Label();
			this.version = new System.Windows.Forms.Label();
			this.name = new System.Windows.Forms.Label();
			this.logo = new System.Windows.Forms.PictureBox();
			this.cancel = new System.Windows.Forms.Button();
			this.tabControl.SuspendLayout();
			this.general.SuspendLayout();
			this.keysAndButtons.SuspendLayout();
			this.panel3.SuspendLayout();
			this.panel1.SuspendLayout();
			this.panel2.SuspendLayout();
			this.resizing.SuspendLayout();
			this.about.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.logo)).BeginInit();
			this.SuspendLayout();
			// 
			// apply
			// 
			this.apply.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.apply.Enabled = false;
			this.apply.Location = new System.Drawing.Point(277, 379);
			this.apply.Name = "apply";
			this.apply.Size = new System.Drawing.Size(75, 23);
			this.apply.TabIndex = 2;
			this.apply.Text = "&Apply";
			this.apply.UseVisualStyleBackColor = true;
			this.apply.Click += new System.EventHandler(this.applyButton_Click);
			// 
			// ok
			// 
			this.ok.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.ok.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.ok.Location = new System.Drawing.Point(115, 379);
			this.ok.Name = "ok";
			this.ok.Size = new System.Drawing.Size(75, 23);
			this.ok.TabIndex = 3;
			this.ok.Text = "&OK";
			this.ok.UseVisualStyleBackColor = true;
			this.ok.Click += new System.EventHandler(this.okButton_Click);
			// 
			// tabControl
			// 
			this.tabControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.tabControl.Controls.Add(this.general);
			this.tabControl.Controls.Add(this.keysAndButtons);
			this.tabControl.Controls.Add(this.resizing);
			this.tabControl.Controls.Add(this.about);
			this.tabControl.Location = new System.Drawing.Point(12, 12);
			this.tabControl.Name = "tabControl";
			this.tabControl.SelectedIndex = 0;
			this.tabControl.Size = new System.Drawing.Size(340, 361);
			this.tabControl.TabIndex = 4;
			// 
			// general
			// 
			this.general.Controls.Add(this.label1);
			this.general.Controls.Add(this.startAtLogon);
			this.general.Location = new System.Drawing.Point(4, 22);
			this.general.Name = "general";
			this.general.Padding = new System.Windows.Forms.Padding(8);
			this.general.Size = new System.Drawing.Size(332, 335);
			this.general.TabIndex = 0;
			this.general.Text = "General";
			this.general.UseVisualStyleBackColor = true;
			// 
			// label1
			// 
			this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.label1.Location = new System.Drawing.Point(29, 31);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(280, 41);
			this.label1.TabIndex = 8;
			this.label1.Text = "This option creates or removes a shortcut to Taekwindow in the Startup folder in " +
				"the Start Menu for your user account.";
			// 
			// startAtLogon
			// 
			this.startAtLogon.AutoSize = true;
			this.startAtLogon.Location = new System.Drawing.Point(11, 11);
			this.startAtLogon.Name = "startAtLogon";
			this.startAtLogon.Size = new System.Drawing.Size(129, 17);
			this.startAtLogon.TabIndex = 7;
			this.startAtLogon.Text = "Start when &logging on";
			this.startAtLogon.UseVisualStyleBackColor = true;
			this.startAtLogon.CheckedChanged += new System.EventHandler(this.SettingChanged);
			// 
			// keysAndButtons
			// 
			this.keysAndButtons.Controls.Add(this.label5);
			this.keysAndButtons.Controls.Add(this.label4);
			this.keysAndButtons.Controls.Add(this.panel3);
			this.keysAndButtons.Controls.Add(this.panel1);
			this.keysAndButtons.Controls.Add(this.panel2);
			this.keysAndButtons.Controls.Add(this.label3);
			this.keysAndButtons.Location = new System.Drawing.Point(4, 22);
			this.keysAndButtons.Name = "keysAndButtons";
			this.keysAndButtons.Padding = new System.Windows.Forms.Padding(8);
			this.keysAndButtons.Size = new System.Drawing.Size(332, 335);
			this.keysAndButtons.TabIndex = 2;
			this.keysAndButtons.Text = "Keys and buttons";
			this.keysAndButtons.UseVisualStyleBackColor = true;
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(8, 50);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(194, 13);
			this.label5.TabIndex = 13;
			this.label5.Text = "Resize windows with this mouse button:";
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(8, 8);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(189, 13);
			this.label4.TabIndex = 12;
			this.label4.Text = "Move windows with this mouse button:";
			// 
			// panel3
			// 
			this.panel3.Controls.Add(this.resizeMiddle);
			this.panel3.Controls.Add(this.resizeRight);
			this.panel3.Controls.Add(this.resizeLeft);
			this.panel3.Location = new System.Drawing.Point(11, 66);
			this.panel3.Name = "panel3";
			this.panel3.Size = new System.Drawing.Size(222, 23);
			this.panel3.TabIndex = 11;
			// 
			// resizeMiddle
			// 
			this.resizeMiddle.Location = new System.Drawing.Point(77, 3);
			this.resizeMiddle.Name = "resizeMiddle";
			this.resizeMiddle.Size = new System.Drawing.Size(68, 17);
			this.resizeMiddle.TabIndex = 10;
			this.resizeMiddle.TabStop = true;
			this.resizeMiddle.Text = "&Middle";
			this.resizeMiddle.UseVisualStyleBackColor = true;
			this.resizeMiddle.Click += new System.EventHandler(this.resizeMiddle_Click);
			this.resizeMiddle.CheckedChanged += new System.EventHandler(this.SettingChanged);
			// 
			// resizeRight
			// 
			this.resizeRight.Location = new System.Drawing.Point(150, 3);
			this.resizeRight.Name = "resizeRight";
			this.resizeRight.Size = new System.Drawing.Size(68, 17);
			this.resizeRight.TabIndex = 9;
			this.resizeRight.TabStop = true;
			this.resizeRight.Text = "&Right";
			this.resizeRight.UseVisualStyleBackColor = true;
			this.resizeRight.Click += new System.EventHandler(this.resizeRight_Click);
			this.resizeRight.CheckedChanged += new System.EventHandler(this.SettingChanged);
			// 
			// resizeLeft
			// 
			this.resizeLeft.Location = new System.Drawing.Point(3, 3);
			this.resizeLeft.Name = "resizeLeft";
			this.resizeLeft.Size = new System.Drawing.Size(68, 17);
			this.resizeLeft.TabIndex = 8;
			this.resizeLeft.TabStop = true;
			this.resizeLeft.Text = "&Left";
			this.resizeLeft.UseVisualStyleBackColor = true;
			this.resizeLeft.Click += new System.EventHandler(this.resizeLeft_Click);
			this.resizeLeft.CheckedChanged += new System.EventHandler(this.SettingChanged);
			// 
			// panel1
			// 
			this.panel1.Controls.Add(this.moveMiddle);
			this.panel1.Controls.Add(this.moveRight);
			this.panel1.Controls.Add(this.moveLeft);
			this.panel1.Location = new System.Drawing.Point(11, 24);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(222, 23);
			this.panel1.TabIndex = 11;
			// 
			// moveMiddle
			// 
			this.moveMiddle.Location = new System.Drawing.Point(77, 3);
			this.moveMiddle.Name = "moveMiddle";
			this.moveMiddle.Size = new System.Drawing.Size(68, 17);
			this.moveMiddle.TabIndex = 10;
			this.moveMiddle.TabStop = true;
			this.moveMiddle.Text = "&Middle";
			this.moveMiddle.UseVisualStyleBackColor = true;
			this.moveMiddle.Click += new System.EventHandler(this.moveMiddle_Click);
			this.moveMiddle.CheckedChanged += new System.EventHandler(this.SettingChanged);
			// 
			// moveRight
			// 
			this.moveRight.Location = new System.Drawing.Point(150, 3);
			this.moveRight.Name = "moveRight";
			this.moveRight.Size = new System.Drawing.Size(68, 17);
			this.moveRight.TabIndex = 9;
			this.moveRight.TabStop = true;
			this.moveRight.Text = "&Right";
			this.moveRight.UseVisualStyleBackColor = true;
			this.moveRight.Click += new System.EventHandler(this.moveRight_Click);
			this.moveRight.CheckedChanged += new System.EventHandler(this.SettingChanged);
			// 
			// moveLeft
			// 
			this.moveLeft.Location = new System.Drawing.Point(3, 3);
			this.moveLeft.Name = "moveLeft";
			this.moveLeft.Size = new System.Drawing.Size(68, 17);
			this.moveLeft.TabIndex = 8;
			this.moveLeft.TabStop = true;
			this.moveLeft.Text = "&Left";
			this.moveLeft.UseVisualStyleBackColor = true;
			this.moveLeft.Click += new System.EventHandler(this.moveLeft_Click);
			this.moveLeft.CheckedChanged += new System.EventHandler(this.SettingChanged);
			// 
			// panel2
			// 
			this.panel2.Controls.Add(this.eitherAlt);
			this.panel2.Controls.Add(this.rightAlt);
			this.panel2.Controls.Add(this.leftAlt);
			this.panel2.Location = new System.Drawing.Point(11, 301);
			this.panel2.Name = "panel2";
			this.panel2.Size = new System.Drawing.Size(222, 23);
			this.panel2.TabIndex = 9;
			// 
			// eitherAlt
			// 
			this.eitherAlt.Location = new System.Drawing.Point(77, 3);
			this.eitherAlt.Name = "eitherAlt";
			this.eitherAlt.Size = new System.Drawing.Size(68, 17);
			this.eitherAlt.TabIndex = 10;
			this.eitherAlt.TabStop = true;
			this.eitherAlt.Text = "&Either";
			this.eitherAlt.UseVisualStyleBackColor = true;
			this.eitherAlt.CheckedChanged += new System.EventHandler(this.SettingChanged);
			// 
			// rightAlt
			// 
			this.rightAlt.Location = new System.Drawing.Point(150, 3);
			this.rightAlt.Name = "rightAlt";
			this.rightAlt.Size = new System.Drawing.Size(68, 17);
			this.rightAlt.TabIndex = 9;
			this.rightAlt.TabStop = true;
			this.rightAlt.Text = "&Right";
			this.rightAlt.UseVisualStyleBackColor = true;
			this.rightAlt.CheckedChanged += new System.EventHandler(this.SettingChanged);
			// 
			// leftAlt
			// 
			this.leftAlt.Location = new System.Drawing.Point(3, 3);
			this.leftAlt.Name = "leftAlt";
			this.leftAlt.Size = new System.Drawing.Size(68, 17);
			this.leftAlt.TabIndex = 8;
			this.leftAlt.TabStop = true;
			this.leftAlt.Text = "&Left";
			this.leftAlt.UseVisualStyleBackColor = true;
			this.leftAlt.CheckedChanged += new System.EventHandler(this.SettingChanged);
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(8, 285);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(221, 13);
			this.label3.TabIndex = 0;
			this.label3.Text = "Move and resize windows with this Alt button:";
			// 
			// resizing
			// 
			this.resizing.Controls.Add(this.nineRectangles);
			this.resizing.Controls.Add(this.bottomRight);
			this.resizing.Location = new System.Drawing.Point(4, 22);
			this.resizing.Margin = new System.Windows.Forms.Padding(8);
			this.resizing.Name = "resizing";
			this.resizing.Padding = new System.Windows.Forms.Padding(8);
			this.resizing.Size = new System.Drawing.Size(332, 335);
			this.resizing.TabIndex = 3;
			this.resizing.Text = "Resizing";
			this.resizing.UseVisualStyleBackColor = true;
			// 
			// nineRectangles
			// 
			this.nineRectangles.AutoSize = true;
			this.nineRectangles.Location = new System.Drawing.Point(11, 34);
			this.nineRectangles.Name = "nineRectangles";
			this.nineRectangles.Size = new System.Drawing.Size(99, 17);
			this.nineRectangles.TabIndex = 1;
			this.nineRectangles.Text = "&Nine rectangles";
			this.nineRectangles.UseVisualStyleBackColor = true;
			this.nineRectangles.CheckedChanged += new System.EventHandler(this.SettingChanged);
			// 
			// bottomRight
			// 
			this.bottomRight.AutoSize = true;
			this.bottomRight.Checked = true;
			this.bottomRight.Location = new System.Drawing.Point(11, 11);
			this.bottomRight.Name = "bottomRight";
			this.bottomRight.Size = new System.Drawing.Size(114, 17);
			this.bottomRight.TabIndex = 0;
			this.bottomRight.TabStop = true;
			this.bottomRight.Text = "&Bottom right corner";
			this.bottomRight.UseVisualStyleBackColor = true;
			this.bottomRight.CheckedChanged += new System.EventHandler(this.SettingChanged);
			// 
			// about
			// 
			this.about.Controls.Add(this.label2);
			this.about.Controls.Add(this.website);
			this.about.Controls.Add(this.email);
			this.about.Controls.Add(this.author);
			this.about.Controls.Add(this.version);
			this.about.Controls.Add(this.name);
			this.about.Controls.Add(this.logo);
			this.about.Location = new System.Drawing.Point(4, 22);
			this.about.Name = "about";
			this.about.Padding = new System.Windows.Forms.Padding(8);
			this.about.Size = new System.Drawing.Size(332, 335);
			this.about.TabIndex = 1;
			this.about.Text = "About";
			this.about.UseVisualStyleBackColor = true;
			// 
			// label2
			// 
			this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.label2.Location = new System.Drawing.Point(11, 295);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(310, 32);
			this.label2.TabIndex = 7;
			this.label2.Text = "You are free and welcome to distribute this program. See the Readme.txt file for " +
				"license details.";
			// 
			// website
			// 
			this.website.AutoSize = true;
			this.website.Location = new System.Drawing.Point(81, 99);
			this.website.Margin = new System.Windows.Forms.Padding(3, 8, 3, 0);
			this.website.Name = "website";
			this.website.Size = new System.Drawing.Size(129, 13);
			this.website.TabIndex = 6;
			this.website.TabStop = true;
			this.website.Text = "http://taekwindow.sf.net/";
			this.website.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.websiteLinkLabel_LinkClicked);
			// 
			// email
			// 
			this.email.AutoSize = true;
			this.email.Location = new System.Drawing.Point(81, 78);
			this.email.Margin = new System.Windows.Forms.Padding(3, 8, 3, 0);
			this.email.Name = "email";
			this.email.Size = new System.Drawing.Size(167, 13);
			this.email.TabIndex = 5;
			this.email.TabStop = true;
			this.email.Text = "ttencate+taekwindow@gmail.com";
			this.email.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.emailLinkLabel_LinkClicked);
			// 
			// author
			// 
			this.author.AutoSize = true;
			this.author.Location = new System.Drawing.Point(81, 57);
			this.author.Margin = new System.Windows.Forms.Padding(3, 8, 3, 0);
			this.author.Name = "author";
			this.author.Size = new System.Drawing.Size(139, 13);
			this.author.TabIndex = 4;
			this.author.Text = "Written by Thomas ten Cate";
			// 
			// version
			// 
			this.version.AutoSize = true;
			this.version.Location = new System.Drawing.Point(81, 36);
			this.version.Margin = new System.Windows.Forms.Padding(3, 8, 3, 0);
			this.version.Name = "version";
			this.version.Size = new System.Drawing.Size(45, 13);
			this.version.TabIndex = 3;
			this.version.Text = "Version ";
			// 
			// name
			// 
			this.name.AutoSize = true;
			this.name.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.name.Location = new System.Drawing.Point(80, 8);
			this.name.Name = "name";
			this.name.Size = new System.Drawing.Size(106, 20);
			this.name.TabIndex = 2;
			this.name.Text = "Taekwindow";
			// 
			// logo
			// 
			this.logo.Image = ((System.Drawing.Image)(resources.GetObject("logo.Image")));
			this.logo.Location = new System.Drawing.Point(6, 6);
			this.logo.Name = "logo";
			this.logo.Size = new System.Drawing.Size(64, 64);
			this.logo.TabIndex = 1;
			this.logo.TabStop = false;
			// 
			// cancel
			// 
			this.cancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.cancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.cancel.Location = new System.Drawing.Point(196, 379);
			this.cancel.Name = "cancel";
			this.cancel.Size = new System.Drawing.Size(75, 23);
			this.cancel.TabIndex = 5;
			this.cancel.Text = "&Cancel";
			this.cancel.UseVisualStyleBackColor = true;
			this.cancel.Click += new System.EventHandler(this.cancel_Click);
			// 
			// ConfigForm
			// 
			this.AcceptButton = this.apply;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.cancel;
			this.ClientSize = new System.Drawing.Size(364, 414);
			this.Controls.Add(this.cancel);
			this.Controls.Add(this.tabControl);
			this.Controls.Add(this.ok);
			this.Controls.Add(this.apply);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.Name = "ConfigForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Taekwindow configuration";
			this.tabControl.ResumeLayout(false);
			this.general.ResumeLayout(false);
			this.general.PerformLayout();
			this.keysAndButtons.ResumeLayout(false);
			this.keysAndButtons.PerformLayout();
			this.panel3.ResumeLayout(false);
			this.panel1.ResumeLayout(false);
			this.panel2.ResumeLayout(false);
			this.resizing.ResumeLayout(false);
			this.resizing.PerformLayout();
			this.about.ResumeLayout(false);
			this.about.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.logo)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Button apply;
		private System.Windows.Forms.Button ok;
		private System.Windows.Forms.TabControl tabControl;
		private System.Windows.Forms.TabPage general;
		private System.Windows.Forms.TabPage about;
		private System.Windows.Forms.PictureBox logo;
		private System.Windows.Forms.LinkLabel email;
		private System.Windows.Forms.Label author;
		private System.Windows.Forms.Label version;
		private System.Windows.Forms.Label name;
		private System.Windows.Forms.LinkLabel website;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.TabPage keysAndButtons;
		private System.Windows.Forms.TabPage resizing;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.CheckBox startAtLogon;
		private System.Windows.Forms.RadioButton nineRectangles;
		private System.Windows.Forms.RadioButton bottomRight;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Panel panel2;
		private System.Windows.Forms.RadioButton eitherAlt;
		private System.Windows.Forms.RadioButton rightAlt;
		private System.Windows.Forms.RadioButton leftAlt;
		private System.Windows.Forms.Panel panel1;
		private System.Windows.Forms.RadioButton moveMiddle;
		private System.Windows.Forms.RadioButton moveRight;
		private System.Windows.Forms.RadioButton moveLeft;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Panel panel3;
		private System.Windows.Forms.RadioButton resizeMiddle;
		private System.Windows.Forms.RadioButton resizeRight;
		private System.Windows.Forms.RadioButton resizeLeft;
		private System.Windows.Forms.Button cancel;
	}
}

