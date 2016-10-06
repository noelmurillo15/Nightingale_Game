namespace AnimationEditor
{
    partial class Form1
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
            if (disposing && (components != null))
            {
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
            this.FramesPanel = new System.Windows.Forms.Panel();
            this.PlayPanel = new System.Windows.Forms.Panel();
            this.AnimationsLB = new System.Windows.Forms.ListBox();
            this.label1 = new System.Windows.Forms.Label();
            this.ObjectNameTB = new System.Windows.Forms.TextBox();
            this.SaveB = new System.Windows.Forms.Button();
            this.FramePanel = new System.Windows.Forms.Panel();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label7 = new System.Windows.Forms.Label();
            this.Animname = new System.Windows.Forms.TextBox();
            this.button2 = new System.Windows.Forms.Button();
            this.PreviewPrevB = new System.Windows.Forms.Button();
            this.PreviewNextB = new System.Windows.Forms.Button();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.button1 = new System.Windows.Forms.Button();
            this.AddAB = new System.Windows.Forms.Button();
            this.LoopingCB = new System.Windows.Forms.CheckBox();
            this.EventL = new System.Windows.Forms.Label();
            this.DurationL = new System.Windows.Forms.Label();
            this.FDurationUD = new System.Windows.Forms.NumericUpDown();
            this.PauseB = new System.Windows.Forms.Button();
            this.PlayB = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.button4 = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.button3 = new System.Windows.Forms.Button();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.YUD = new System.Windows.Forms.NumericUpDown();
            this.XUD = new System.Windows.Forms.NumericUpDown();
            this.HeightUD = new System.Windows.Forms.NumericUpDown();
            this.WidthUD = new System.Windows.Forms.NumericUpDown();
            this.AddFB = new System.Windows.Forms.Button();
            this.EventRectB = new System.Windows.Forms.Button();
            this.CollisionRectB = new System.Windows.Forms.Button();
            this.AnchorB = new System.Windows.Forms.Button();
            this.DrawRectB = new System.Windows.Forms.Button();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openBitmapToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.FDurationUD)).BeginInit();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.YUD)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.XUD)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.HeightUD)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.WidthUD)).BeginInit();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // FramesPanel
            // 
            this.FramesPanel.Location = new System.Drawing.Point(12, 27);
            this.FramesPanel.Name = "FramesPanel";
            this.FramesPanel.Size = new System.Drawing.Size(963, 100);
            this.FramesPanel.TabIndex = 0;
            this.FramesPanel.MouseClick += new System.Windows.Forms.MouseEventHandler(this.FramesPanel_MouseClick);
            // 
            // PlayPanel
            // 
            this.PlayPanel.Location = new System.Drawing.Point(7, 153);
            this.PlayPanel.Name = "PlayPanel";
            this.PlayPanel.Size = new System.Drawing.Size(277, 333);
            this.PlayPanel.TabIndex = 1;
            // 
            // AnimationsLB
            // 
            this.AnimationsLB.FormattingEnabled = true;
            this.AnimationsLB.Location = new System.Drawing.Point(0, 19);
            this.AnimationsLB.Name = "AnimationsLB";
            this.AnimationsLB.Size = new System.Drawing.Size(174, 264);
            this.AnimationsLB.TabIndex = 2;
            this.AnimationsLB.SelectedIndexChanged += new System.EventHandler(this.AnimationsLB_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(0, 313);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(69, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Object Name";
            // 
            // ObjectNameTB
            // 
            this.ObjectNameTB.Location = new System.Drawing.Point(0, 329);
            this.ObjectNameTB.Name = "ObjectNameTB";
            this.ObjectNameTB.Size = new System.Drawing.Size(174, 20);
            this.ObjectNameTB.TabIndex = 4;
            this.ObjectNameTB.TextChanged += new System.EventHandler(this.ObjectNameTB_TextChanged);
            // 
            // SaveB
            // 
            this.SaveB.Location = new System.Drawing.Point(115, 355);
            this.SaveB.Name = "SaveB";
            this.SaveB.Size = new System.Drawing.Size(59, 23);
            this.SaveB.TabIndex = 5;
            this.SaveB.Text = "Save";
            this.SaveB.UseVisualStyleBackColor = true;
            this.SaveB.Click += new System.EventHandler(this.SaveB_Click);
            // 
            // FramePanel
            // 
            this.FramePanel.Location = new System.Drawing.Point(6, 80);
            this.FramePanel.Name = "FramePanel";
            this.FramePanel.Size = new System.Drawing.Size(459, 406);
            this.FramePanel.TabIndex = 6;
            this.FramePanel.Scroll += new System.Windows.Forms.ScrollEventHandler(this.FramePanel_Scroll);
            this.FramePanel.MouseDown += new System.Windows.Forms.MouseEventHandler(this.FramePanel_MouseDown);
            this.FramePanel.MouseMove += new System.Windows.Forms.MouseEventHandler(this.FramePanel_MouseMove);
            this.FramePanel.MouseUp += new System.Windows.Forms.MouseEventHandler(this.FramePanel_MouseUp);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label7);
            this.groupBox1.Controls.Add(this.Animname);
            this.groupBox1.Controls.Add(this.button2);
            this.groupBox1.Controls.Add(this.PreviewPrevB);
            this.groupBox1.Controls.Add(this.PreviewNextB);
            this.groupBox1.Controls.Add(this.comboBox1);
            this.groupBox1.Controls.Add(this.button1);
            this.groupBox1.Controls.Add(this.AddAB);
            this.groupBox1.Controls.Add(this.LoopingCB);
            this.groupBox1.Controls.Add(this.EventL);
            this.groupBox1.Controls.Add(this.DurationL);
            this.groupBox1.Controls.Add(this.FDurationUD);
            this.groupBox1.Controls.Add(this.PauseB);
            this.groupBox1.Controls.Add(this.PlayB);
            this.groupBox1.Controls.Add(this.PlayPanel);
            this.groupBox1.Location = new System.Drawing.Point(491, 133);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(290, 496);
            this.groupBox1.TabIndex = 7;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Previewing";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(172, 80);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(84, 13);
            this.label7.TabIndex = 18;
            this.label7.Text = "Animation Name";
            // 
            // Animname
            // 
            this.Animname.Location = new System.Drawing.Point(172, 96);
            this.Animname.Name = "Animname";
            this.Animname.Size = new System.Drawing.Size(100, 20);
            this.Animname.TabIndex = 17;
            this.Animname.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(6, 90);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 27);
            this.button2.TabIndex = 16;
            this.button2.Text = "Display";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // PreviewPrevB
            // 
            this.PreviewPrevB.Location = new System.Drawing.Point(7, 127);
            this.PreviewPrevB.Name = "PreviewPrevB";
            this.PreviewPrevB.Size = new System.Drawing.Size(43, 23);
            this.PreviewPrevB.TabIndex = 12;
            this.PreviewPrevB.Text = "Prev";
            this.PreviewPrevB.UseVisualStyleBackColor = true;
            this.PreviewPrevB.Click += new System.EventHandler(this.PreviewPrevB_Click);
            // 
            // PreviewNextB
            // 
            this.PreviewNextB.Location = new System.Drawing.Point(69, 127);
            this.PreviewNextB.Name = "PreviewNextB";
            this.PreviewNextB.Size = new System.Drawing.Size(43, 23);
            this.PreviewNextB.TabIndex = 13;
            this.PreviewNextB.Text = "Next";
            this.PreviewNextB.UseVisualStyleBackColor = true;
            this.PreviewNextB.Click += new System.EventHandler(this.PreviewNextB_Click);
            // 
            // comboBox1
            // 
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Location = new System.Drawing.Point(168, 21);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(105, 21);
            this.comboBox1.TabIndex = 15;
            this.comboBox1.SelectedIndexChanged += new System.EventHandler(this.comboBox1_SelectedIndexChanged);
            this.comboBox1.TextChanged += new System.EventHandler(this.comboBox1_TextChanged);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(87, 90);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 27);
            this.button1.TabIndex = 14;
            this.button1.Text = "Hide";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // AddAB
            // 
            this.AddAB.Location = new System.Drawing.Point(175, 127);
            this.AddAB.Name = "AddAB";
            this.AddAB.Size = new System.Drawing.Size(97, 23);
            this.AddAB.TabIndex = 8;
            this.AddAB.Text = "Add Animation";
            this.AddAB.UseVisualStyleBackColor = true;
            this.AddAB.Click += new System.EventHandler(this.AddAB_Click);
            // 
            // LoopingCB
            // 
            this.LoopingCB.AutoSize = true;
            this.LoopingCB.Checked = true;
            this.LoopingCB.CheckState = System.Windows.Forms.CheckState.Checked;
            this.LoopingCB.Location = new System.Drawing.Point(87, 64);
            this.LoopingCB.Name = "LoopingCB";
            this.LoopingCB.Size = new System.Drawing.Size(64, 17);
            this.LoopingCB.TabIndex = 11;
            this.LoopingCB.Text = "Looping";
            this.LoopingCB.UseVisualStyleBackColor = true;
            this.LoopingCB.CheckedChanged += new System.EventHandler(this.LoopingCB_CheckedChanged);
            // 
            // EventL
            // 
            this.EventL.AutoSize = true;
            this.EventL.Location = new System.Drawing.Point(165, 8);
            this.EventL.Name = "EventL";
            this.EventL.Size = new System.Drawing.Size(35, 13);
            this.EventL.TabIndex = 10;
            this.EventL.Text = "Event";
            // 
            // DurationL
            // 
            this.DurationL.AutoSize = true;
            this.DurationL.Location = new System.Drawing.Point(169, 41);
            this.DurationL.Name = "DurationL";
            this.DurationL.Size = new System.Drawing.Size(79, 13);
            this.DurationL.TabIndex = 7;
            this.DurationL.Text = "Frame Duration";
            // 
            // FDurationUD
            // 
            this.FDurationUD.DecimalPlaces = 2;
            this.FDurationUD.Increment = new decimal(new int[] {
            10,
            0,
            0,
            131072});
            this.FDurationUD.Location = new System.Drawing.Point(172, 57);
            this.FDurationUD.Name = "FDurationUD";
            this.FDurationUD.Size = new System.Drawing.Size(76, 20);
            this.FDurationUD.TabIndex = 6;
            this.FDurationUD.ValueChanged += new System.EventHandler(this.FDurationUD_ValueChanged);
            // 
            // PauseB
            // 
            this.PauseB.Location = new System.Drawing.Point(87, 19);
            this.PauseB.Name = "PauseB";
            this.PauseB.Size = new System.Drawing.Size(75, 30);
            this.PauseB.TabIndex = 3;
            this.PauseB.Text = "Pause";
            this.PauseB.UseVisualStyleBackColor = true;
            this.PauseB.Click += new System.EventHandler(this.PauseB_Click);
            // 
            // PlayB
            // 
            this.PlayB.Location = new System.Drawing.Point(6, 19);
            this.PlayB.Name = "PlayB";
            this.PlayB.Size = new System.Drawing.Size(75, 30);
            this.PlayB.TabIndex = 2;
            this.PlayB.Text = "Play";
            this.PlayB.UseVisualStyleBackColor = true;
            this.PlayB.Click += new System.EventHandler(this.PlayB_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.button4);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Controls.Add(this.ObjectNameTB);
            this.groupBox2.Controls.Add(this.SaveB);
            this.groupBox2.Controls.Add(this.AnimationsLB);
            this.groupBox2.Location = new System.Drawing.Point(787, 241);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(188, 388);
            this.groupBox2.TabIndex = 8;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Object";
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(99, 289);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(75, 23);
            this.button4.TabIndex = 6;
            this.button4.Text = "delete Animation";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.button3);
            this.groupBox3.Controls.Add(this.label5);
            this.groupBox3.Controls.Add(this.label4);
            this.groupBox3.Controls.Add(this.label3);
            this.groupBox3.Controls.Add(this.label2);
            this.groupBox3.Controls.Add(this.YUD);
            this.groupBox3.Controls.Add(this.XUD);
            this.groupBox3.Controls.Add(this.HeightUD);
            this.groupBox3.Controls.Add(this.WidthUD);
            this.groupBox3.Controls.Add(this.AddFB);
            this.groupBox3.Controls.Add(this.EventRectB);
            this.groupBox3.Controls.Add(this.CollisionRectB);
            this.groupBox3.Controls.Add(this.AnchorB);
            this.groupBox3.Controls.Add(this.DrawRectB);
            this.groupBox3.Controls.Add(this.FramePanel);
            this.groupBox3.Location = new System.Drawing.Point(12, 133);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(473, 496);
            this.groupBox3.TabIndex = 9;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Frame Editing";
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(390, 19);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(75, 23);
            this.button3.TabIndex = 22;
            this.button3.Text = "Delete Frame";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(309, 41);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(38, 13);
            this.label5.TabIndex = 19;
            this.label5.Text = "Height";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(308, 6);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(35, 13);
            this.label4.TabIndex = 18;
            this.label4.Text = "Width";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(225, 41);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(14, 13);
            this.label3.TabIndex = 17;
            this.label3.Text = "Y";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(224, 6);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(14, 13);
            this.label2.TabIndex = 16;
            this.label2.Text = "X";
            // 
            // YUD
            // 
            this.YUD.DecimalPlaces = 4;
            this.YUD.Location = new System.Drawing.Point(224, 57);
            this.YUD.Maximum = new decimal(new int[] {
            900000000,
            0,
            0,
            0});
            this.YUD.Name = "YUD";
            this.YUD.Size = new System.Drawing.Size(77, 20);
            this.YUD.TabIndex = 15;
            this.YUD.ValueChanged += new System.EventHandler(this.YUD_ValueChanged);
            // 
            // XUD
            // 
            this.XUD.DecimalPlaces = 4;
            this.XUD.Location = new System.Drawing.Point(224, 22);
            this.XUD.Maximum = new decimal(new int[] {
            90000000,
            0,
            0,
            0});
            this.XUD.Name = "XUD";
            this.XUD.Size = new System.Drawing.Size(77, 20);
            this.XUD.TabIndex = 14;
            this.XUD.ValueChanged += new System.EventHandler(this.XUD_ValueChanged);
            // 
            // HeightUD
            // 
            this.HeightUD.DecimalPlaces = 4;
            this.HeightUD.Location = new System.Drawing.Point(308, 57);
            this.HeightUD.Maximum = new decimal(new int[] {
            900000000,
            0,
            0,
            0});
            this.HeightUD.Name = "HeightUD";
            this.HeightUD.Size = new System.Drawing.Size(77, 20);
            this.HeightUD.TabIndex = 13;
            this.HeightUD.ValueChanged += new System.EventHandler(this.HeightUD_ValueChanged);
            // 
            // WidthUD
            // 
            this.WidthUD.DecimalPlaces = 4;
            this.WidthUD.Location = new System.Drawing.Point(308, 22);
            this.WidthUD.Maximum = new decimal(new int[] {
            -194313216,
            20,
            0,
            0});
            this.WidthUD.Name = "WidthUD";
            this.WidthUD.Size = new System.Drawing.Size(76, 20);
            this.WidthUD.TabIndex = 12;
            this.WidthUD.ValueChanged += new System.EventHandler(this.WidthUD_ValueChanged);
            // 
            // AddFB
            // 
            this.AddFB.Location = new System.Drawing.Point(389, 54);
            this.AddFB.Name = "AddFB";
            this.AddFB.Size = new System.Drawing.Size(78, 23);
            this.AddFB.TabIndex = 11;
            this.AddFB.Text = "Add Frame";
            this.AddFB.UseVisualStyleBackColor = true;
            this.AddFB.Click += new System.EventHandler(this.AddFB_Click);
            // 
            // EventRectB
            // 
            this.EventRectB.Location = new System.Drawing.Point(111, 49);
            this.EventRectB.Name = "EventRectB";
            this.EventRectB.Size = new System.Drawing.Size(107, 23);
            this.EventRectB.TabIndex = 10;
            this.EventRectB.Text = "Event Rectangle";
            this.EventRectB.UseVisualStyleBackColor = true;
            this.EventRectB.Click += new System.EventHandler(this.EventRectB_Click);
            // 
            // CollisionRectB
            // 
            this.CollisionRectB.Location = new System.Drawing.Point(111, 19);
            this.CollisionRectB.Name = "CollisionRectB";
            this.CollisionRectB.Size = new System.Drawing.Size(107, 23);
            this.CollisionRectB.TabIndex = 9;
            this.CollisionRectB.Text = "Collision Rectangle";
            this.CollisionRectB.UseVisualStyleBackColor = true;
            this.CollisionRectB.Click += new System.EventHandler(this.CollisionRectB_Click);
            // 
            // AnchorB
            // 
            this.AnchorB.Location = new System.Drawing.Point(6, 49);
            this.AnchorB.Name = "AnchorB";
            this.AnchorB.Size = new System.Drawing.Size(98, 23);
            this.AnchorB.TabIndex = 8;
            this.AnchorB.Text = "Anchor Point";
            this.AnchorB.UseVisualStyleBackColor = true;
            this.AnchorB.Click += new System.EventHandler(this.AnchorB_Click);
            // 
            // DrawRectB
            // 
            this.DrawRectB.Location = new System.Drawing.Point(6, 19);
            this.DrawRectB.Name = "DrawRectB";
            this.DrawRectB.Size = new System.Drawing.Size(98, 23);
            this.DrawRectB.TabIndex = 7;
            this.DrawRectB.Text = "Draw Rectengle";
            this.DrawRectB.UseVisualStyleBackColor = true;
            this.DrawRectB.Click += new System.EventHandler(this.DrawRectB_Click);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(987, 24);
            this.menuStrip1.TabIndex = 10;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.saveToolStripMenuItem,
            this.openToolStripMenuItem,
            this.openBitmapToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(144, 22);
            this.saveToolStripMenuItem.Text = "Save";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.Size = new System.Drawing.Size(144, 22);
            this.openToolStripMenuItem.Text = "Open";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
            // 
            // openBitmapToolStripMenuItem
            // 
            this.openBitmapToolStripMenuItem.Name = "openBitmapToolStripMenuItem";
            this.openBitmapToolStripMenuItem.Size = new System.Drawing.Size(144, 22);
            this.openBitmapToolStripMenuItem.Text = "Open Bitmap";
            this.openBitmapToolStripMenuItem.Click += new System.EventHandler(this.openBitmapToolStripMenuItem_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(987, 672);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.FramesPanel);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.FDurationUD)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.YUD)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.XUD)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.HeightUD)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.WidthUD)).EndInit();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel FramesPanel;
        private System.Windows.Forms.Panel PlayPanel;
        private System.Windows.Forms.ListBox AnimationsLB;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox ObjectNameTB;
        private System.Windows.Forms.Button SaveB;
        private System.Windows.Forms.Panel FramePanel;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label DurationL;
        private System.Windows.Forms.NumericUpDown FDurationUD;
        private System.Windows.Forms.Button PauseB;
        private System.Windows.Forms.Button PlayB;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button DrawRectB;
        private System.Windows.Forms.CheckBox LoopingCB;
        private System.Windows.Forms.Label EventL;
        private System.Windows.Forms.Button AddAB;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.NumericUpDown YUD;
        private System.Windows.Forms.NumericUpDown XUD;
        private System.Windows.Forms.NumericUpDown HeightUD;
        private System.Windows.Forms.NumericUpDown WidthUD;
        private System.Windows.Forms.Button AddFB;
        private System.Windows.Forms.Button EventRectB;
        private System.Windows.Forms.Button CollisionRectB;
        private System.Windows.Forms.Button AnchorB;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openBitmapToolStripMenuItem;
        private System.Windows.Forms.Button PreviewNextB;
        private System.Windows.Forms.Button PreviewPrevB;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox Animname;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button button4;
    }
}

