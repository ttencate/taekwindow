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
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.startAtLogon = new System.Windows.Forms.CheckBox();
			this.keysAndButtons = new System.Windows.Forms.TabPage();
			this.groupBox4 = new System.Windows.Forms.GroupBox();
			this.resizeMiddle = new System.Windows.Forms.RadioButton();
			this.resizeRight = new System.Windows.Forms.RadioButton();
			this.resizeLeft = new System.Windows.Forms.RadioButton();
			this.groupBox3 = new System.Windows.Forms.GroupBox();
			this.moveMiddle = new System.Windows.Forms.RadioButton();
			this.moveRight = new System.Windows.Forms.RadioButton();
			this.moveLeft = new System.Windows.Forms.RadioButton();
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.eitherAlt = new System.Windows.Forms.RadioButton();
			this.rightAlt = new System.Windows.Forms.RadioButton();
			this.leftAlt = new System.Windows.Forms.RadioButton();
			this.resizing = new System.Windows.Forms.TabPage();
			this.groupBox5 = new System.Windows.Forms.GroupBox();
			this.bottomRight = new System.Windows.Forms.RadioButton();
			this.nineRectangles = new System.Windows.Forms.RadioButton();
			this.groupBox6 = new System.Windows.Forms.GroupBox();
			this.about = new System.Windows.Forms.TabPage();
			this.groupBox7 = new System.Windows.Forms.GroupBox();
			this.license = new System.Windows.Forms.Label();
			this.aboutBox = new System.Windows.Forms.GroupBox();
			this.logo = new System.Windows.Forms.PictureBox();
			this.title = new System.Windows.Forms.Label();
			this.website = new System.Windows.Forms.LinkLabel();
			this.version = new System.Windows.Forms.Label();
			this.email = new System.Windows.Forms.LinkLabel();
			this.author = new System.Windows.Forms.Label();
			this.cancel = new System.Windows.Forms.Button();
			this.tabControl.SuspendLayout();
			this.general.SuspendLayout();
			this.groupBox1.SuspendLayout();
			this.keysAndButtons.SuspendLayout();
			this.groupBox4.SuspendLayout();
			this.groupBox3.SuspendLayout();
			this.groupBox2.SuspendLayout();
			this.resizing.SuspendLayout();
			this.groupBox5.SuspendLayout();
			this.about.SuspendLayout();
			this.groupBox7.SuspendLayout();
			this.aboutBox.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.logo)).BeginInit();
			this.SuspendLayout();
			// 
			// apply
			// 
			this.apply.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.apply.Enabled = false;
			this.apply.Location = new System.Drawing.Point(277, 357);
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
			this.ok.Location = new System.Drawing.Point(115, 357);
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
			this.tabControl.Size = new System.Drawing.Size(340, 339);
			this.tabControl.TabIndex = 4;
			// 
			// general
			// 
			this.general.Controls.Add(this.groupBox1);
			this.general.Location = new System.Drawing.Point(4, 22);
			this.general.Name = "general";
			this.general.Padding = new System.Windows.Forms.Padding(8);
			this.general.Size = new System.Drawing.Size(332, 313);
			this.general.TabIndex = 0;
			this.general.Text = "General";
			this.general.UseVisualStyleBackColor = true;
			// 
			// groupBox1
			// 
			this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox1.Controls.Add(this.startAtLogon);
			this.groupBox1.Location = new System.Drawing.Point(11, 11);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Padding = new System.Windows.Forms.Padding(8, 3, 8, 8);
			this.groupBox1.Size = new System.Drawing.Size(310, 160);
			this.groupBox1.TabIndex = 11;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Startup";
			// 
			// startAtLogon
			// 
			this.startAtLogon.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.startAtLogon.CheckAlign = System.Drawing.ContentAlignment.TopLeft;
			this.startAtLogon.Image = ((System.Drawing.Image)(resources.GetObject("startAtLogon.Image")));
			this.startAtLogon.ImageAlign = System.Drawing.ContentAlignment.BottomLeft;
			this.startAtLogon.Location = new System.Drawing.Point(11, 19);
			this.startAtLogon.Name = "startAtLogon";
			this.startAtLogon.Size = new System.Drawing.Size(288, 130);
			this.startAtLogon.TabIndex = 7;
			this.startAtLogon.Text = "Start when &logging on\r\n\r\nThis option creates or removes a shortcut to Taekwindow" +
				" in the Startup folder in the Start Menu for your user account.";
			this.startAtLogon.TextAlign = System.Drawing.ContentAlignment.TopLeft;
			this.startAtLogon.TextImageRelation = System.Windows.Forms.TextImageRelation.TextAboveImage;
			this.startAtLogon.UseVisualStyleBackColor = true;
			this.startAtLogon.CheckedChanged += new System.EventHandler(this.SettingChanged);
			// 
			// keysAndButtons
			// 
			this.keysAndButtons.Controls.Add(this.groupBox4);
			this.keysAndButtons.Controls.Add(this.groupBox3);
			this.keysAndButtons.Controls.Add(this.groupBox2);
			this.keysAndButtons.Location = new System.Drawing.Point(4, 22);
			this.keysAndButtons.Name = "keysAndButtons";
			this.keysAndButtons.Padding = new System.Windows.Forms.Padding(8);
			this.keysAndButtons.Size = new System.Drawing.Size(332, 313);
			this.keysAndButtons.TabIndex = 2;
			this.keysAndButtons.Text = "Keys and buttons";
			this.keysAndButtons.UseVisualStyleBackColor = true;
			// 
			// groupBox4
			// 
			this.groupBox4.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox4.Controls.Add(this.resizeMiddle);
			this.groupBox4.Controls.Add(this.resizeRight);
			this.groupBox4.Controls.Add(this.resizeLeft);
			this.groupBox4.Location = new System.Drawing.Point(11, 209);
			this.groupBox4.Name = "groupBox4";
			this.groupBox4.Padding = new System.Windows.Forms.Padding(8, 3, 8, 8);
			this.groupBox4.Size = new System.Drawing.Size(310, 93);
			this.groupBox4.TabIndex = 16;
			this.groupBox4.TabStop = false;
			this.groupBox4.Text = "Resizing";
			// 
			// resizeMiddle
			// 
			this.resizeMiddle.AutoSize = true;
			this.resizeMiddle.Location = new System.Drawing.Point(11, 42);
			this.resizeMiddle.Name = "resizeMiddle";
			this.resizeMiddle.Size = new System.Drawing.Size(179, 17);
			this.resizeMiddle.TabIndex = 10;
			this.resizeMiddle.TabStop = true;
			this.resizeMiddle.Text = "Resize with mi&ddle mouse button";
			this.resizeMiddle.UseVisualStyleBackColor = true;
			this.resizeMiddle.CheckedChanged += new System.EventHandler(this.SettingChanged);
			// 
			// resizeRight
			// 
			this.resizeRight.AutoSize = true;
			this.resizeRight.Location = new System.Drawing.Point(11, 65);
			this.resizeRight.Name = "resizeRight";
			this.resizeRight.Size = new System.Drawing.Size(169, 17);
			this.resizeRight.TabIndex = 9;
			this.resizeRight.TabStop = true;
			this.resizeRight.Text = "Resize with ri&ght mouse button";
			this.resizeRight.UseVisualStyleBackColor = true;
			this.resizeRight.CheckedChanged += new System.EventHandler(this.SettingChanged);
			// 
			// resizeLeft
			// 
			this.resizeLeft.AutoSize = true;
			this.resizeLeft.Location = new System.Drawing.Point(11, 19);
			this.resizeLeft.Name = "resizeLeft";
			this.resizeLeft.Size = new System.Drawing.Size(163, 17);
			this.resizeLeft.TabIndex = 8;
			this.resizeLeft.TabStop = true;
			this.resizeLeft.Text = "Resize with le&ft mouse button";
			this.resizeLeft.UseVisualStyleBackColor = true;
			this.resizeLeft.CheckedChanged += new System.EventHandler(this.SettingChanged);
			// 
			// groupBox3
			// 
			this.groupBox3.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox3.Controls.Add(this.moveMiddle);
			this.groupBox3.Controls.Add(this.moveRight);
			this.groupBox3.Controls.Add(this.moveLeft);
			this.groupBox3.Location = new System.Drawing.Point(11, 110);
			this.groupBox3.Name = "groupBox3";
			this.groupBox3.Padding = new System.Windows.Forms.Padding(8, 3, 8, 8);
			this.groupBox3.Size = new System.Drawing.Size(310, 93);
			this.groupBox3.TabIndex = 15;
			this.groupBox3.TabStop = false;
			this.groupBox3.Text = "Moving";
			// 
			// moveMiddle
			// 
			this.moveMiddle.AutoSize = true;
			this.moveMiddle.Location = new System.Drawing.Point(11, 42);
			this.moveMiddle.Name = "moveMiddle";
			this.moveMiddle.Size = new System.Drawing.Size(174, 17);
			this.moveMiddle.TabIndex = 10;
			this.moveMiddle.TabStop = true;
			this.moveMiddle.Text = "Move with &middle mouse button";
			this.moveMiddle.UseVisualStyleBackColor = true;
			this.moveMiddle.CheckedChanged += new System.EventHandler(this.SettingChanged);
			// 
			// moveRight
			// 
			this.moveRight.AutoSize = true;
			this.moveRight.Location = new System.Drawing.Point(11, 65);
			this.moveRight.Name = "moveRight";
			this.moveRight.Size = new System.Drawing.Size(164, 17);
			this.moveRight.TabIndex = 9;
			this.moveRight.TabStop = true;
			this.moveRight.Text = "Move with r&ight mouse button";
			this.moveRight.UseVisualStyleBackColor = true;
			this.moveRight.CheckedChanged += new System.EventHandler(this.SettingChanged);
			// 
			// moveLeft
			// 
			this.moveLeft.AutoSize = true;
			this.moveLeft.Location = new System.Drawing.Point(11, 19);
			this.moveLeft.Name = "moveLeft";
			this.moveLeft.Size = new System.Drawing.Size(158, 17);
			this.moveLeft.TabIndex = 8;
			this.moveLeft.TabStop = true;
			this.moveLeft.Text = "Move with l&eft mouse button";
			this.moveLeft.UseVisualStyleBackColor = true;
			this.moveLeft.CheckedChanged += new System.EventHandler(this.SettingChanged);
			// 
			// groupBox2
			// 
			this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox2.Controls.Add(this.eitherAlt);
			this.groupBox2.Controls.Add(this.rightAlt);
			this.groupBox2.Controls.Add(this.leftAlt);
			this.groupBox2.Location = new System.Drawing.Point(11, 11);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Padding = new System.Windows.Forms.Padding(8, 3, 8, 8);
			this.groupBox2.Size = new System.Drawing.Size(310, 93);
			this.groupBox2.TabIndex = 14;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "Keyboard";
			// 
			// eitherAlt
			// 
			this.eitherAlt.AutoSize = true;
			this.eitherAlt.Location = new System.Drawing.Point(11, 42);
			this.eitherAlt.Name = "eitherAlt";
			this.eitherAlt.Size = new System.Drawing.Size(151, 17);
			this.eitherAlt.TabIndex = 10;
			this.eitherAlt.TabStop = true;
			this.eitherAlt.Text = "Move or resize with a&ny Alt";
			this.eitherAlt.UseVisualStyleBackColor = true;
			this.eitherAlt.CheckedChanged += new System.EventHandler(this.SettingChanged);
			// 
			// rightAlt
			// 
			this.rightAlt.AutoSize = true;
			this.rightAlt.Location = new System.Drawing.Point(11, 65);
			this.rightAlt.Name = "rightAlt";
			this.rightAlt.Size = new System.Drawing.Size(154, 17);
			this.rightAlt.TabIndex = 9;
			this.rightAlt.TabStop = true;
			this.rightAlt.Text = "Move or resize with &right Alt";
			this.rightAlt.UseVisualStyleBackColor = true;
			this.rightAlt.CheckedChanged += new System.EventHandler(this.SettingChanged);
			// 
			// leftAlt
			// 
			this.leftAlt.AutoSize = true;
			this.leftAlt.Location = new System.Drawing.Point(11, 19);
			this.leftAlt.Name = "leftAlt";
			this.leftAlt.Size = new System.Drawing.Size(148, 17);
			this.leftAlt.TabIndex = 8;
			this.leftAlt.TabStop = true;
			this.leftAlt.Text = "Move or resize with &left Alt";
			this.leftAlt.UseVisualStyleBackColor = true;
			this.leftAlt.CheckedChanged += new System.EventHandler(this.SettingChanged);
			// 
			// resizing
			// 
			this.resizing.Controls.Add(this.groupBox5);
			this.resizing.Location = new System.Drawing.Point(4, 22);
			this.resizing.Margin = new System.Windows.Forms.Padding(8);
			this.resizing.Name = "resizing";
			this.resizing.Padding = new System.Windows.Forms.Padding(8);
			this.resizing.Size = new System.Drawing.Size(332, 313);
			this.resizing.TabIndex = 3;
			this.resizing.Text = "Resizing";
			this.resizing.UseVisualStyleBackColor = true;
			// 
			// groupBox5
			// 
			this.groupBox5.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox5.Controls.Add(this.bottomRight);
			this.groupBox5.Controls.Add(this.nineRectangles);
			this.groupBox5.Controls.Add(this.groupBox6);
			this.groupBox5.Location = new System.Drawing.Point(11, 11);
			this.groupBox5.Name = "groupBox5";
			this.groupBox5.Padding = new System.Windows.Forms.Padding(8, 3, 8, 10);
			this.groupBox5.Size = new System.Drawing.Size(310, 251);
			this.groupBox5.TabIndex = 0;
			this.groupBox5.TabStop = false;
			this.groupBox5.Text = "Resize mode";
			// 
			// bottomRight
			// 
			this.bottomRight.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.bottomRight.CheckAlign = System.Drawing.ContentAlignment.TopLeft;
			this.bottomRight.Image = ((System.Drawing.Image)(resources.GetObject("bottomRight.Image")));
			this.bottomRight.ImageAlign = System.Drawing.ContentAlignment.TopRight;
			this.bottomRight.Location = new System.Drawing.Point(11, 19);
			this.bottomRight.Name = "bottomRight";
			this.bottomRight.Size = new System.Drawing.Size(288, 99);
			this.bottomRight.TabIndex = 6;
			this.bottomRight.Text = "&Bottom right corner\r\n\r\nRegardless of where you grab the window, you always resiz" +
				"e from its lower right corner.";
			this.bottomRight.TextAlign = System.Drawing.ContentAlignment.TopLeft;
			this.bottomRight.TextImageRelation = System.Windows.Forms.TextImageRelation.TextBeforeImage;
			this.bottomRight.UseVisualStyleBackColor = true;
			this.bottomRight.CheckedChanged += new System.EventHandler(this.SettingChanged);
			// 
			// nineRectangles
			// 
			this.nineRectangles.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.nineRectangles.CheckAlign = System.Drawing.ContentAlignment.TopLeft;
			this.nineRectangles.Image = ((System.Drawing.Image)(resources.GetObject("nineRectangles.Image")));
			this.nineRectangles.ImageAlign = System.Drawing.ContentAlignment.TopRight;
			this.nineRectangles.Location = new System.Drawing.Point(11, 139);
			this.nineRectangles.Margin = new System.Windows.Forms.Padding(3, 5, 3, 3);
			this.nineRectangles.Name = "nineRectangles";
			this.nineRectangles.Size = new System.Drawing.Size(288, 99);
			this.nineRectangles.TabIndex = 7;
			this.nineRectangles.Text = "&Nine rectangles\r\n\r\nThe window is resized from the corner or edge that is closest" +
				" to the point where you grab it.";
			this.nineRectangles.TextAlign = System.Drawing.ContentAlignment.TopLeft;
			this.nineRectangles.TextImageRelation = System.Windows.Forms.TextImageRelation.TextBeforeImage;
			this.nineRectangles.UseVisualStyleBackColor = true;
			this.nineRectangles.CheckedChanged += new System.EventHandler(this.SettingChanged);
			// 
			// groupBox6
			// 
			this.groupBox6.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox6.Location = new System.Drawing.Point(11, 129);
			this.groupBox6.Margin = new System.Windows.Forms.Padding(3, 8, 3, 3);
			this.groupBox6.Name = "groupBox6";
			this.groupBox6.Size = new System.Drawing.Size(288, 2);
			this.groupBox6.TabIndex = 12;
			this.groupBox6.TabStop = false;
			// 
			// about
			// 
			this.about.Controls.Add(this.groupBox7);
			this.about.Controls.Add(this.aboutBox);
			this.about.Location = new System.Drawing.Point(4, 22);
			this.about.Name = "about";
			this.about.Padding = new System.Windows.Forms.Padding(8);
			this.about.Size = new System.Drawing.Size(332, 313);
			this.about.TabIndex = 1;
			this.about.Text = "About";
			this.about.UseVisualStyleBackColor = true;
			// 
			// groupBox7
			// 
			this.groupBox7.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox7.Controls.Add(this.license);
			this.groupBox7.Location = new System.Drawing.Point(11, 229);
			this.groupBox7.Name = "groupBox7";
			this.groupBox7.Padding = new System.Windows.Forms.Padding(8, 8, 8, 13);
			this.groupBox7.Size = new System.Drawing.Size(310, 73);
			this.groupBox7.TabIndex = 9;
			this.groupBox7.TabStop = false;
			this.groupBox7.Text = "Legal";
			// 
			// license
			// 
			this.license.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.license.Location = new System.Drawing.Point(11, 21);
			this.license.Name = "license";
			this.license.Size = new System.Drawing.Size(288, 39);
			this.license.TabIndex = 7;
			this.license.Text = "License information";
			// 
			// aboutBox
			// 
			this.aboutBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.aboutBox.Controls.Add(this.logo);
			this.aboutBox.Controls.Add(this.title);
			this.aboutBox.Controls.Add(this.website);
			this.aboutBox.Controls.Add(this.version);
			this.aboutBox.Controls.Add(this.email);
			this.aboutBox.Controls.Add(this.author);
			this.aboutBox.Location = new System.Drawing.Point(11, 11);
			this.aboutBox.Name = "aboutBox";
			this.aboutBox.Padding = new System.Windows.Forms.Padding(8, 3, 8, 13);
			this.aboutBox.Size = new System.Drawing.Size(310, 133);
			this.aboutBox.TabIndex = 8;
			this.aboutBox.TabStop = false;
			this.aboutBox.Text = "About";
			// 
			// logo
			// 
			this.logo.Image = ((System.Drawing.Image)(resources.GetObject("logo.Image")));
			this.logo.Location = new System.Drawing.Point(11, 19);
			this.logo.Name = "logo";
			this.logo.Size = new System.Drawing.Size(64, 64);
			this.logo.TabIndex = 1;
			this.logo.TabStop = false;
			// 
			// title
			// 
			this.title.AutoSize = true;
			this.title.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.title.Location = new System.Drawing.Point(81, 16);
			this.title.Name = "title";
			this.title.Size = new System.Drawing.Size(43, 20);
			this.title.TabIndex = 2;
			this.title.Text = "Title";
			// 
			// website
			// 
			this.website.AutoSize = true;
			this.website.Location = new System.Drawing.Point(82, 107);
			this.website.Margin = new System.Windows.Forms.Padding(3, 8, 3, 0);
			this.website.Name = "website";
			this.website.Size = new System.Drawing.Size(46, 13);
			this.website.TabIndex = 6;
			this.website.TabStop = true;
			this.website.Text = "Website";
			this.website.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.websiteLinkLabel_LinkClicked);
			// 
			// version
			// 
			this.version.AutoSize = true;
			this.version.Location = new System.Drawing.Point(82, 44);
			this.version.Margin = new System.Windows.Forms.Padding(3, 8, 3, 0);
			this.version.Name = "version";
			this.version.Size = new System.Drawing.Size(45, 13);
			this.version.TabIndex = 3;
			this.version.Text = "Version ";
			// 
			// email
			// 
			this.email.AutoSize = true;
			this.email.Location = new System.Drawing.Point(82, 86);
			this.email.Margin = new System.Windows.Forms.Padding(3, 8, 3, 0);
			this.email.Name = "email";
			this.email.Size = new System.Drawing.Size(35, 13);
			this.email.TabIndex = 5;
			this.email.TabStop = true;
			this.email.Text = "E-mail";
			this.email.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.emailLinkLabel_LinkClicked);
			// 
			// author
			// 
			this.author.AutoSize = true;
			this.author.Location = new System.Drawing.Point(81, 65);
			this.author.Margin = new System.Windows.Forms.Padding(3, 8, 3, 0);
			this.author.Name = "author";
			this.author.Size = new System.Drawing.Size(38, 13);
			this.author.TabIndex = 4;
			this.author.Text = "Author";
			// 
			// cancel
			// 
			this.cancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.cancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.cancel.Location = new System.Drawing.Point(196, 357);
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
			this.ClientSize = new System.Drawing.Size(364, 392);
			this.Controls.Add(this.cancel);
			this.Controls.Add(this.tabControl);
			this.Controls.Add(this.ok);
			this.Controls.Add(this.apply);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.Name = "ConfigForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Configuration";
			this.tabControl.ResumeLayout(false);
			this.general.ResumeLayout(false);
			this.groupBox1.ResumeLayout(false);
			this.keysAndButtons.ResumeLayout(false);
			this.groupBox4.ResumeLayout(false);
			this.groupBox4.PerformLayout();
			this.groupBox3.ResumeLayout(false);
			this.groupBox3.PerformLayout();
			this.groupBox2.ResumeLayout(false);
			this.groupBox2.PerformLayout();
			this.resizing.ResumeLayout(false);
			this.groupBox5.ResumeLayout(false);
			this.about.ResumeLayout(false);
			this.groupBox7.ResumeLayout(false);
			this.aboutBox.ResumeLayout(false);
			this.aboutBox.PerformLayout();
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
		private System.Windows.Forms.Label title;
		private System.Windows.Forms.LinkLabel website;
		private System.Windows.Forms.Label license;
		private System.Windows.Forms.TabPage keysAndButtons;
		private System.Windows.Forms.TabPage resizing;
		private System.Windows.Forms.CheckBox startAtLogon;
		private System.Windows.Forms.RadioButton eitherAlt;
		private System.Windows.Forms.RadioButton rightAlt;
		private System.Windows.Forms.RadioButton leftAlt;
		private System.Windows.Forms.Button cancel;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.GroupBox groupBox2;
		private System.Windows.Forms.GroupBox groupBox3;
		private System.Windows.Forms.RadioButton moveMiddle;
		private System.Windows.Forms.RadioButton moveRight;
		private System.Windows.Forms.RadioButton moveLeft;
		private System.Windows.Forms.GroupBox groupBox4;
		private System.Windows.Forms.RadioButton resizeMiddle;
		private System.Windows.Forms.RadioButton resizeRight;
		private System.Windows.Forms.RadioButton resizeLeft;
		private System.Windows.Forms.GroupBox groupBox5;
		private System.Windows.Forms.RadioButton nineRectangles;
		private System.Windows.Forms.RadioButton bottomRight;
		private System.Windows.Forms.GroupBox groupBox6;
		private System.Windows.Forms.GroupBox aboutBox;
		private System.Windows.Forms.GroupBox groupBox7;
	}
}

