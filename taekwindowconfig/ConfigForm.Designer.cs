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
			this.applyButton = new System.Windows.Forms.Button();
			this.exitButton = new System.Windows.Forms.Button();
			this.tabControl = new System.Windows.Forms.TabControl();
			this.generalTabPage = new System.Windows.Forms.TabPage();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.label1 = new System.Windows.Forms.Label();
			this.startAtLogonCheckBox = new System.Windows.Forms.CheckBox();
			this.trayIconCheckBox = new System.Windows.Forms.CheckBox();
			this.keysAndButtonsTabPage = new System.Windows.Forms.TabPage();
			this.movingGroupBox = new System.Windows.Forms.GroupBox();
			this.resizingTabPage = new System.Windows.Forms.TabPage();
			this.aboutTabPage = new System.Windows.Forms.TabPage();
			this.label2 = new System.Windows.Forms.Label();
			this.websiteLinkLabel = new System.Windows.Forms.LinkLabel();
			this.emailLinkLabel = new System.Windows.Forms.LinkLabel();
			this.authorLabel = new System.Windows.Forms.Label();
			this.versionLabel = new System.Windows.Forms.Label();
			this.nameLabel = new System.Windows.Forms.Label();
			this.logoPictureBox = new System.Windows.Forms.PictureBox();
			this.label3 = new System.Windows.Forms.Label();
			this.keyComboPicker1 = new TaekwindowConfig.KeyComboPicker();
			this.tabControl.SuspendLayout();
			this.generalTabPage.SuspendLayout();
			this.groupBox1.SuspendLayout();
			this.keysAndButtonsTabPage.SuspendLayout();
			this.movingGroupBox.SuspendLayout();
			this.aboutTabPage.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.logoPictureBox)).BeginInit();
			this.SuspendLayout();
			// 
			// applyButton
			// 
			this.applyButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.applyButton.Enabled = false;
			this.applyButton.Location = new System.Drawing.Point(196, 379);
			this.applyButton.Name = "applyButton";
			this.applyButton.Size = new System.Drawing.Size(75, 23);
			this.applyButton.TabIndex = 2;
			this.applyButton.Text = "&Apply";
			this.applyButton.UseVisualStyleBackColor = true;
			this.applyButton.Click += new System.EventHandler(this.applyButton_Click);
			// 
			// exitButton
			// 
			this.exitButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.exitButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.exitButton.Location = new System.Drawing.Point(277, 379);
			this.exitButton.Name = "exitButton";
			this.exitButton.Size = new System.Drawing.Size(75, 23);
			this.exitButton.TabIndex = 3;
			this.exitButton.Text = "&Exit";
			this.exitButton.UseVisualStyleBackColor = true;
			this.exitButton.Click += new System.EventHandler(this.exitButton_Click);
			// 
			// tabControl
			// 
			this.tabControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.tabControl.Controls.Add(this.generalTabPage);
			this.tabControl.Controls.Add(this.keysAndButtonsTabPage);
			this.tabControl.Controls.Add(this.resizingTabPage);
			this.tabControl.Controls.Add(this.aboutTabPage);
			this.tabControl.Location = new System.Drawing.Point(12, 12);
			this.tabControl.Name = "tabControl";
			this.tabControl.SelectedIndex = 0;
			this.tabControl.Size = new System.Drawing.Size(340, 361);
			this.tabControl.TabIndex = 4;
			// 
			// generalTabPage
			// 
			this.generalTabPage.Controls.Add(this.groupBox1);
			this.generalTabPage.Location = new System.Drawing.Point(4, 22);
			this.generalTabPage.Name = "generalTabPage";
			this.generalTabPage.Padding = new System.Windows.Forms.Padding(3);
			this.generalTabPage.Size = new System.Drawing.Size(332, 335);
			this.generalTabPage.TabIndex = 0;
			this.generalTabPage.Text = "General";
			this.generalTabPage.UseVisualStyleBackColor = true;
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.label1);
			this.groupBox1.Controls.Add(this.startAtLogonCheckBox);
			this.groupBox1.Controls.Add(this.trayIconCheckBox);
			this.groupBox1.Location = new System.Drawing.Point(6, 6);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(320, 323);
			this.groupBox1.TabIndex = 0;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "General settings";
			// 
			// label1
			// 
			this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.label1.Location = new System.Drawing.Point(24, 39);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(290, 29);
			this.label1.TabIndex = 6;
			this.label1.Text = "This option creates or removes a shortcut to Taekwindow in the Startup folder in " +
				"the Start Menu for your user account.";
			// 
			// startAtLogonCheckBox
			// 
			this.startAtLogonCheckBox.AutoSize = true;
			this.startAtLogonCheckBox.Location = new System.Drawing.Point(6, 19);
			this.startAtLogonCheckBox.Name = "startAtLogonCheckBox";
			this.startAtLogonCheckBox.Size = new System.Drawing.Size(129, 17);
			this.startAtLogonCheckBox.TabIndex = 5;
			this.startAtLogonCheckBox.Text = "Start when &logging on";
			this.startAtLogonCheckBox.UseVisualStyleBackColor = true;
			this.startAtLogonCheckBox.CheckedChanged += new System.EventHandler(this.SettingChanged);
			// 
			// trayIconCheckBox
			// 
			this.trayIconCheckBox.AutoSize = true;
			this.trayIconCheckBox.Location = new System.Drawing.Point(6, 85);
			this.trayIconCheckBox.Name = "trayIconCheckBox";
			this.trayIconCheckBox.Size = new System.Drawing.Size(131, 17);
			this.trayIconCheckBox.TabIndex = 4;
			this.trayIconCheckBox.Text = "&Show system tray icon";
			this.trayIconCheckBox.UseVisualStyleBackColor = true;
			this.trayIconCheckBox.CheckedChanged += new System.EventHandler(this.SettingChanged);
			// 
			// keysAndButtonsTabPage
			// 
			this.keysAndButtonsTabPage.Controls.Add(this.movingGroupBox);
			this.keysAndButtonsTabPage.Location = new System.Drawing.Point(4, 22);
			this.keysAndButtonsTabPage.Name = "keysAndButtonsTabPage";
			this.keysAndButtonsTabPage.Size = new System.Drawing.Size(332, 335);
			this.keysAndButtonsTabPage.TabIndex = 2;
			this.keysAndButtonsTabPage.Text = "Keys and buttons";
			this.keysAndButtonsTabPage.UseVisualStyleBackColor = true;
			// 
			// movingGroupBox
			// 
			this.movingGroupBox.Controls.Add(this.label3);
			this.movingGroupBox.Controls.Add(this.keyComboPicker1);
			this.movingGroupBox.Location = new System.Drawing.Point(3, 3);
			this.movingGroupBox.Name = "movingGroupBox";
			this.movingGroupBox.Size = new System.Drawing.Size(326, 160);
			this.movingGroupBox.TabIndex = 0;
			this.movingGroupBox.TabStop = false;
			this.movingGroupBox.Text = "Moving";
			// 
			// resizingTabPage
			// 
			this.resizingTabPage.Location = new System.Drawing.Point(4, 22);
			this.resizingTabPage.Name = "resizingTabPage";
			this.resizingTabPage.Size = new System.Drawing.Size(332, 335);
			this.resizingTabPage.TabIndex = 3;
			this.resizingTabPage.Text = "Resizing";
			this.resizingTabPage.UseVisualStyleBackColor = true;
			// 
			// aboutTabPage
			// 
			this.aboutTabPage.Controls.Add(this.label2);
			this.aboutTabPage.Controls.Add(this.websiteLinkLabel);
			this.aboutTabPage.Controls.Add(this.emailLinkLabel);
			this.aboutTabPage.Controls.Add(this.authorLabel);
			this.aboutTabPage.Controls.Add(this.versionLabel);
			this.aboutTabPage.Controls.Add(this.nameLabel);
			this.aboutTabPage.Controls.Add(this.logoPictureBox);
			this.aboutTabPage.Location = new System.Drawing.Point(4, 22);
			this.aboutTabPage.Name = "aboutTabPage";
			this.aboutTabPage.Padding = new System.Windows.Forms.Padding(3);
			this.aboutTabPage.Size = new System.Drawing.Size(332, 335);
			this.aboutTabPage.TabIndex = 1;
			this.aboutTabPage.Text = "About";
			this.aboutTabPage.UseVisualStyleBackColor = true;
			// 
			// label2
			// 
			this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.label2.Location = new System.Drawing.Point(6, 300);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(320, 32);
			this.label2.TabIndex = 7;
			this.label2.Text = "You are free and welcome to distribute this program. See the Readme.txt file for " +
				"license details.";
			// 
			// websiteLinkLabel
			// 
			this.websiteLinkLabel.AutoSize = true;
			this.websiteLinkLabel.Location = new System.Drawing.Point(76, 94);
			this.websiteLinkLabel.Margin = new System.Windows.Forms.Padding(3, 8, 3, 0);
			this.websiteLinkLabel.Name = "websiteLinkLabel";
			this.websiteLinkLabel.Size = new System.Drawing.Size(129, 13);
			this.websiteLinkLabel.TabIndex = 6;
			this.websiteLinkLabel.TabStop = true;
			this.websiteLinkLabel.Text = "http://taekwindow.sf.net/";
			this.websiteLinkLabel.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.websiteLinkLabel_LinkClicked);
			// 
			// emailLinkLabel
			// 
			this.emailLinkLabel.AutoSize = true;
			this.emailLinkLabel.Location = new System.Drawing.Point(76, 73);
			this.emailLinkLabel.Margin = new System.Windows.Forms.Padding(3, 8, 3, 0);
			this.emailLinkLabel.Name = "emailLinkLabel";
			this.emailLinkLabel.Size = new System.Drawing.Size(167, 13);
			this.emailLinkLabel.TabIndex = 5;
			this.emailLinkLabel.TabStop = true;
			this.emailLinkLabel.Text = "ttencate+taekwindow@gmail.com";
			this.emailLinkLabel.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.emailLinkLabel_LinkClicked);
			// 
			// authorLabel
			// 
			this.authorLabel.AutoSize = true;
			this.authorLabel.Location = new System.Drawing.Point(76, 52);
			this.authorLabel.Margin = new System.Windows.Forms.Padding(3, 8, 3, 0);
			this.authorLabel.Name = "authorLabel";
			this.authorLabel.Size = new System.Drawing.Size(139, 13);
			this.authorLabel.TabIndex = 4;
			this.authorLabel.Text = "Written by Thomas ten Cate";
			// 
			// versionLabel
			// 
			this.versionLabel.AutoSize = true;
			this.versionLabel.Location = new System.Drawing.Point(76, 31);
			this.versionLabel.Margin = new System.Windows.Forms.Padding(3, 8, 3, 0);
			this.versionLabel.Name = "versionLabel";
			this.versionLabel.Size = new System.Drawing.Size(45, 13);
			this.versionLabel.TabIndex = 3;
			this.versionLabel.Text = "Version ";
			// 
			// nameLabel
			// 
			this.nameLabel.AutoSize = true;
			this.nameLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.nameLabel.Location = new System.Drawing.Point(75, 3);
			this.nameLabel.Name = "nameLabel";
			this.nameLabel.Size = new System.Drawing.Size(106, 20);
			this.nameLabel.TabIndex = 2;
			this.nameLabel.Text = "Taekwindow";
			// 
			// logoPictureBox
			// 
			this.logoPictureBox.Image = ((System.Drawing.Image)(resources.GetObject("logoPictureBox.Image")));
			this.logoPictureBox.Location = new System.Drawing.Point(6, 6);
			this.logoPictureBox.Name = "logoPictureBox";
			this.logoPictureBox.Size = new System.Drawing.Size(64, 64);
			this.logoPictureBox.TabIndex = 1;
			this.logoPictureBox.TabStop = false;
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(6, 22);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(61, 13);
			this.label3.TabIndex = 1;
			this.label3.Text = "&Hold down:";
			// 
			// keyComboPicker1
			// 
			this.keyComboPicker1.Location = new System.Drawing.Point(73, 19);
			this.keyComboPicker1.Name = "keyComboPicker1";
			this.keyComboPicker1.Size = new System.Drawing.Size(247, 43);
			this.keyComboPicker1.TabIndex = 0;
			// 
			// ConfigForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(364, 414);
			this.Controls.Add(this.tabControl);
			this.Controls.Add(this.exitButton);
			this.Controls.Add(this.applyButton);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.Name = "ConfigForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Taekwindow configuration";
			this.tabControl.ResumeLayout(false);
			this.generalTabPage.ResumeLayout(false);
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			this.keysAndButtonsTabPage.ResumeLayout(false);
			this.movingGroupBox.ResumeLayout(false);
			this.movingGroupBox.PerformLayout();
			this.aboutTabPage.ResumeLayout(false);
			this.aboutTabPage.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.logoPictureBox)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Button applyButton;
		private System.Windows.Forms.Button exitButton;
		private System.Windows.Forms.TabControl tabControl;
		private System.Windows.Forms.TabPage generalTabPage;
		private System.Windows.Forms.TabPage aboutTabPage;
		private System.Windows.Forms.PictureBox logoPictureBox;
		private System.Windows.Forms.LinkLabel emailLinkLabel;
		private System.Windows.Forms.Label authorLabel;
		private System.Windows.Forms.Label versionLabel;
		private System.Windows.Forms.Label nameLabel;
		private System.Windows.Forms.LinkLabel websiteLinkLabel;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.CheckBox startAtLogonCheckBox;
		private System.Windows.Forms.CheckBox trayIconCheckBox;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.TabPage keysAndButtonsTabPage;
		private System.Windows.Forms.TabPage resizingTabPage;
		private System.Windows.Forms.GroupBox movingGroupBox;
		private System.Windows.Forms.Label label3;
		private KeyComboPicker keyComboPicker1;
	}
}

