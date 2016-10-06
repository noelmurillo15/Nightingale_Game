namespace MapEditor
{
    partial class MapEditor
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MapEditor));
            this.MenuStrip = new System.Windows.Forms.MenuStrip();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.importTilesetToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolStrip = new System.Windows.Forms.ToolStrip();
            this.New = new System.Windows.Forms.ToolStripButton();
            this.ShowGrid = new System.Windows.Forms.ToolStripButton();
            this.toolStripButton3 = new System.Windows.Forms.ToolStripButton();
            this.SplitContainer1 = new System.Windows.Forms.SplitContainer();
            this.Editor_Tab = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.Grid_Column_Num = new System.Windows.Forms.NumericUpDown();
            this.Grid_Row_Num = new System.Windows.Forms.NumericUpDown();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.Grid_Height_Num = new System.Windows.Forms.NumericUpDown();
            this.Grid_Width_Num = new System.Windows.Forms.NumericUpDown();
            this.tabPage4 = new System.Windows.Forms.TabPage();
            this.Creature_List = new System.Windows.Forms.ListBox();
            this.label2 = new System.Windows.Forms.Label();
            this.tabPage5 = new System.Windows.Forms.TabPage();
            this.Event_Save_Button = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.Event_Name_TextBox = new System.Windows.Forms.TextBox();
            this.fileSystemWatcher1 = new System.IO.FileSystemWatcher();
            this.Event_List = new System.Windows.Forms.TabPage();
            this.Event_List_Box = new System.Windows.Forms.ListBox();
            this.tabPage6 = new System.Windows.Forms.TabPage();
            this.button1 = new System.Windows.Forms.Button();
            this.Bush = new System.Windows.Forms.CheckBox();
            this.WinCount = new System.Windows.Forms.NumericUpDown();
            this.label7 = new System.Windows.Forms.Label();
            this.button2 = new System.Windows.Forms.Button();
            this.Collider = new System.Windows.Forms.CheckBox();
            this.Zoom = new System.Windows.Forms.NumericUpDown();
            this.label8 = new System.Windows.Forms.Label();
            this.button3 = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.SecondTabControl = new System.Windows.Forms.TabControl();
            this.MenuStrip.SuspendLayout();
            this.ToolStrip.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.SplitContainer1)).BeginInit();
            this.SplitContainer1.Panel2.SuspendLayout();
            this.SplitContainer1.SuspendLayout();
            this.Editor_Tab.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Grid_Column_Num)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.Grid_Row_Num)).BeginInit();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Grid_Height_Num)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.Grid_Width_Num)).BeginInit();
            this.tabPage4.SuspendLayout();
            this.tabPage5.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.fileSystemWatcher1)).BeginInit();
            this.Event_List.SuspendLayout();
            this.tabPage6.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.WinCount)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.Zoom)).BeginInit();
            this.SecondTabControl.SuspendLayout();
            this.SuspendLayout();
            // 
            // MenuStrip
            // 
            this.MenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem1,
            this.editToolStripMenuItem,
            this.viewToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.MenuStrip.Location = new System.Drawing.Point(0, 0);
            this.MenuStrip.Name = "MenuStrip";
            this.MenuStrip.Size = new System.Drawing.Size(1134, 24);
            this.MenuStrip.TabIndex = 0;
            this.MenuStrip.Text = "MenuStrip";
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.openToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.saveToToolStripMenuItem,
            this.importTilesetToolStripMenuItem});
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(37, 20);
            this.toolStripMenuItem1.Text = "File";
            // 
            // newToolStripMenuItem
            // 
            this.newToolStripMenuItem.Name = "newToolStripMenuItem";
            this.newToolStripMenuItem.Size = new System.Drawing.Size(149, 22);
            this.newToolStripMenuItem.Text = "New";
            this.newToolStripMenuItem.Click += new System.EventHandler(this.New_Grid_Click);
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.Size = new System.Drawing.Size(149, 22);
            this.openToolStripMenuItem.Text = "Open";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.Load_Map_File_Click);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(149, 22);
            this.saveToolStripMenuItem.Text = "Save";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.Save_Click);
            // 
            // saveToToolStripMenuItem
            // 
            this.saveToToolStripMenuItem.Name = "saveToToolStripMenuItem";
            this.saveToToolStripMenuItem.Size = new System.Drawing.Size(149, 22);
            this.saveToToolStripMenuItem.Text = "Save To Game";
            this.saveToToolStripMenuItem.Click += new System.EventHandler(this.saveToToolStripMenuItem_Click);
            // 
            // importTilesetToolStripMenuItem
            // 
            this.importTilesetToolStripMenuItem.Name = "importTilesetToolStripMenuItem";
            this.importTilesetToolStripMenuItem.Size = new System.Drawing.Size(149, 22);
            this.importTilesetToolStripMenuItem.Text = "Import Tileset";
            this.importTilesetToolStripMenuItem.Click += new System.EventHandler(this.Import_ToolBar_Click);
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(39, 20);
            this.editToolStripMenuItem.Text = "Edit";
            // 
            // viewToolStripMenuItem
            // 
            this.viewToolStripMenuItem.Name = "viewToolStripMenuItem";
            this.viewToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.viewToolStripMenuItem.Text = "View";
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // ToolStrip
            // 
            this.ToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.New,
            this.ShowGrid,
            this.toolStripButton3});
            this.ToolStrip.Location = new System.Drawing.Point(0, 24);
            this.ToolStrip.Name = "ToolStrip";
            this.ToolStrip.Size = new System.Drawing.Size(1134, 25);
            this.ToolStrip.TabIndex = 1;
            this.ToolStrip.Text = "toolStrip1";
            // 
            // New
            // 
            this.New.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.New.Image = ((System.Drawing.Image)(resources.GetObject("New.Image")));
            this.New.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.New.Name = "New";
            this.New.Size = new System.Drawing.Size(23, 22);
            this.New.Text = "New";
            this.New.Click += new System.EventHandler(this.New_Grid_Click);
            // 
            // ShowGrid
            // 
            this.ShowGrid.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.ShowGrid.Image = ((System.Drawing.Image)(resources.GetObject("ShowGrid.Image")));
            this.ShowGrid.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.ShowGrid.Name = "ShowGrid";
            this.ShowGrid.Size = new System.Drawing.Size(23, 22);
            this.ShowGrid.Text = "ShowGrid";
            this.ShowGrid.Click += new System.EventHandler(this.Show_Grid_Click);
            // 
            // toolStripButton3
            // 
            this.toolStripButton3.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButton3.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton3.Image")));
            this.toolStripButton3.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton3.Name = "toolStripButton3";
            this.toolStripButton3.Size = new System.Drawing.Size(23, 22);
            this.toolStripButton3.Text = "Fill";
            this.toolStripButton3.Click += new System.EventHandler(this.Fill_With_SelectedTile_Click);
            // 
            // SplitContainer1
            // 
            this.SplitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.SplitContainer1.Location = new System.Drawing.Point(0, 49);
            this.SplitContainer1.Name = "SplitContainer1";
            // 
            // SplitContainer1.Panel1
            // 
            this.SplitContainer1.Panel1.Paint += new System.Windows.Forms.PaintEventHandler(this.Map_Panel_Paint);
            this.SplitContainer1.Panel1.MouseClick += new System.Windows.Forms.MouseEventHandler(this.Place_Selected_Tile_Click);
            this.SplitContainer1.Panel1.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Place_Multiple_Tiles_MouseMove);
            // 
            // SplitContainer1.Panel2
            // 
            this.SplitContainer1.Panel2.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.SplitContainer1.Panel2.Controls.Add(this.SecondTabControl);
            this.SplitContainer1.Panel2.Controls.Add(this.Editor_Tab);
            this.SplitContainer1.Size = new System.Drawing.Size(1134, 726);
            this.SplitContainer1.SplitterDistance = 765;
            this.SplitContainer1.TabIndex = 2;
            // 
            // Editor_Tab
            // 
            this.Editor_Tab.Controls.Add(this.tabPage6);
            this.Editor_Tab.Controls.Add(this.tabPage1);
            this.Editor_Tab.Controls.Add(this.tabPage2);
            this.Editor_Tab.Controls.Add(this.tabPage4);
            this.Editor_Tab.Controls.Add(this.tabPage5);
            this.Editor_Tab.Location = new System.Drawing.Point(3, 4);
            this.Editor_Tab.Name = "Editor_Tab";
            this.Editor_Tab.SelectedIndex = 0;
            this.Editor_Tab.Size = new System.Drawing.Size(358, 395);
            this.Editor_Tab.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(350, 369);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "TileSet";
            this.tabPage1.UseVisualStyleBackColor = true;
            this.tabPage1.Paint += new System.Windows.Forms.PaintEventHandler(this.TileSet_Paint);
            this.tabPage1.MouseClick += new System.Windows.Forms.MouseEventHandler(this.Select_Tile_Click);
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.groupBox2);
            this.tabPage2.Controls.Add(this.groupBox1);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(350, 369);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Edit Sizes";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.Grid_Column_Num);
            this.groupBox2.Controls.Add(this.Grid_Row_Num);
            this.groupBox2.Location = new System.Drawing.Point(6, 145);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(132, 100);
            this.groupBox2.TabIndex = 9;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Map Size";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(7, 61);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(44, 13);
            this.label5.TabIndex = 9;
            this.label5.Text = "Height :";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(7, 35);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(41, 13);
            this.label6.TabIndex = 8;
            this.label6.Text = "Width :";
            // 
            // Grid_Column_Num
            // 
            this.Grid_Column_Num.Location = new System.Drawing.Point(63, 33);
            this.Grid_Column_Num.Maximum = new decimal(new int[] {
            200,
            0,
            0,
            0});
            this.Grid_Column_Num.Minimum = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.Grid_Column_Num.Name = "Grid_Column_Num";
            this.Grid_Column_Num.Size = new System.Drawing.Size(63, 20);
            this.Grid_Column_Num.TabIndex = 7;
            this.Grid_Column_Num.Value = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.Grid_Column_Num.ValueChanged += new System.EventHandler(this.Grid_Column_ValueChanged);
            // 
            // Grid_Row_Num
            // 
            this.Grid_Row_Num.Location = new System.Drawing.Point(63, 59);
            this.Grid_Row_Num.Maximum = new decimal(new int[] {
            200,
            0,
            0,
            0});
            this.Grid_Row_Num.Minimum = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.Grid_Row_Num.Name = "Grid_Row_Num";
            this.Grid_Row_Num.Size = new System.Drawing.Size(63, 20);
            this.Grid_Row_Num.TabIndex = 6;
            this.Grid_Row_Num.Value = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.Grid_Row_Num.ValueChanged += new System.EventHandler(this.Grid_Row_ValueChanged);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.Grid_Height_Num);
            this.groupBox1.Controls.Add(this.Grid_Width_Num);
            this.groupBox1.Location = new System.Drawing.Point(6, 24);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(132, 100);
            this.groupBox1.TabIndex = 8;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Tile Size";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(7, 59);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(44, 13);
            this.label4.TabIndex = 7;
            this.label4.Text = "Height :";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(7, 33);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(41, 13);
            this.label3.TabIndex = 6;
            this.label3.Text = "Width :";
            // 
            // Grid_Height_Num
            // 
            this.Grid_Height_Num.Increment = new decimal(new int[] {
            16,
            0,
            0,
            0});
            this.Grid_Height_Num.Location = new System.Drawing.Point(63, 57);
            this.Grid_Height_Num.Maximum = new decimal(new int[] {
            64,
            0,
            0,
            0});
            this.Grid_Height_Num.Minimum = new decimal(new int[] {
            16,
            0,
            0,
            0});
            this.Grid_Height_Num.Name = "Grid_Height_Num";
            this.Grid_Height_Num.Size = new System.Drawing.Size(63, 20);
            this.Grid_Height_Num.TabIndex = 5;
            this.Grid_Height_Num.Value = new decimal(new int[] {
            16,
            0,
            0,
            0});
            this.Grid_Height_Num.ValueChanged += new System.EventHandler(this.Grid_Height_ValueChanged);
            // 
            // Grid_Width_Num
            // 
            this.Grid_Width_Num.Increment = new decimal(new int[] {
            16,
            0,
            0,
            0});
            this.Grid_Width_Num.Location = new System.Drawing.Point(63, 31);
            this.Grid_Width_Num.Maximum = new decimal(new int[] {
            64,
            0,
            0,
            0});
            this.Grid_Width_Num.Minimum = new decimal(new int[] {
            16,
            0,
            0,
            0});
            this.Grid_Width_Num.Name = "Grid_Width_Num";
            this.Grid_Width_Num.Size = new System.Drawing.Size(63, 20);
            this.Grid_Width_Num.TabIndex = 4;
            this.Grid_Width_Num.Value = new decimal(new int[] {
            16,
            0,
            0,
            0});
            this.Grid_Width_Num.ValueChanged += new System.EventHandler(this.Grid_Width_ValueChanged);
            // 
            // tabPage4
            // 
            this.tabPage4.Controls.Add(this.Creature_List);
            this.tabPage4.Controls.Add(this.label2);
            this.tabPage4.Location = new System.Drawing.Point(4, 22);
            this.tabPage4.Name = "tabPage4";
            this.tabPage4.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage4.Size = new System.Drawing.Size(350, 369);
            this.tabPage4.TabIndex = 3;
            this.tabPage4.Text = "Spawn Points";
            this.tabPage4.UseVisualStyleBackColor = true;
            this.tabPage4.Enter += new System.EventHandler(this.Spawn_Points_Enter);
            this.tabPage4.Leave += new System.EventHandler(this.Spawn_Points_Leave);
            // 
            // Creature_List
            // 
            this.Creature_List.FormattingEnabled = true;
            this.Creature_List.Items.AddRange(new object[] {
            "Bishop",
            "Bumbles",
            "Castle",
            "King",
            "Knight",
            "Passant",
            "Pawn",
            "Player",
            "Prancer",
            "Queen",
            "Rook"});
            this.Creature_List.Location = new System.Drawing.Point(10, 57);
            this.Creature_List.Name = "Creature_List";
            this.Creature_List.Size = new System.Drawing.Size(120, 43);
            this.Creature_List.Sorted = true;
            this.Creature_List.TabIndex = 2;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(7, 41);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(53, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "Creature :";
            // 
            // tabPage5
            // 
            this.tabPage5.Controls.Add(this.Event_Save_Button);
            this.tabPage5.Controls.Add(this.label1);
            this.tabPage5.Controls.Add(this.Event_Name_TextBox);
            this.tabPage5.Location = new System.Drawing.Point(4, 22);
            this.tabPage5.Name = "tabPage5";
            this.tabPage5.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage5.Size = new System.Drawing.Size(350, 369);
            this.tabPage5.TabIndex = 4;
            this.tabPage5.Text = "Event Layer";
            this.tabPage5.UseVisualStyleBackColor = true;
            this.tabPage5.Enter += new System.EventHandler(this.Event_Layer_Enter);
            this.tabPage5.Leave += new System.EventHandler(this.Event_Layer_Leave);
            // 
            // Event_Save_Button
            // 
            this.Event_Save_Button.Location = new System.Drawing.Point(258, 95);
            this.Event_Save_Button.Name = "Event_Save_Button";
            this.Event_Save_Button.Size = new System.Drawing.Size(75, 23);
            this.Event_Save_Button.TabIndex = 3;
            this.Event_Save_Button.Text = "Save";
            this.Event_Save_Button.UseVisualStyleBackColor = true;
            this.Event_Save_Button.Click += new System.EventHandler(this.Event_Save_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 28);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(72, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Event Name :";
            // 
            // Event_Name_TextBox
            // 
            this.Event_Name_TextBox.Location = new System.Drawing.Point(84, 25);
            this.Event_Name_TextBox.Name = "Event_Name_TextBox";
            this.Event_Name_TextBox.Size = new System.Drawing.Size(249, 20);
            this.Event_Name_TextBox.TabIndex = 1;
            // 
            // fileSystemWatcher1
            // 
            this.fileSystemWatcher1.EnableRaisingEvents = true;
            this.fileSystemWatcher1.SynchronizingObject = this;
            // 
            // Event_List
            // 
            this.Event_List.Controls.Add(this.Event_List_Box);
            this.Event_List.Location = new System.Drawing.Point(4, 22);
            this.Event_List.Name = "Event_List";
            this.Event_List.Padding = new System.Windows.Forms.Padding(3);
            this.Event_List.Size = new System.Drawing.Size(349, 292);
            this.Event_List.TabIndex = 1;
            this.Event_List.Text = "Event List";
            this.Event_List.UseVisualStyleBackColor = true;
            this.Event_List.Enter += new System.EventHandler(this.Event_Layer_Enter);
            // 
            // Event_List_Box
            // 
            this.Event_List_Box.FormattingEnabled = true;
            this.Event_List_Box.Location = new System.Drawing.Point(8, 17);
            this.Event_List_Box.Name = "Event_List_Box";
            this.Event_List_Box.Size = new System.Drawing.Size(336, 264);
            this.Event_List_Box.TabIndex = 0;
            this.Event_List_Box.SelectedIndexChanged += new System.EventHandler(this.Event_List_Box_SelectedIndexChanged);
            // 
            // tabPage6
            // 
            this.tabPage6.Controls.Add(this.button4);
            this.tabPage6.Controls.Add(this.button3);
            this.tabPage6.Controls.Add(this.label8);
            this.tabPage6.Controls.Add(this.Zoom);
            this.tabPage6.Controls.Add(this.Collider);
            this.tabPage6.Controls.Add(this.button2);
            this.tabPage6.Controls.Add(this.label7);
            this.tabPage6.Controls.Add(this.WinCount);
            this.tabPage6.Controls.Add(this.Bush);
            this.tabPage6.Controls.Add(this.button1);
            this.tabPage6.Location = new System.Drawing.Point(4, 22);
            this.tabPage6.Name = "tabPage6";
            this.tabPage6.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage6.Size = new System.Drawing.Size(350, 369);
            this.tabPage6.TabIndex = 0;
            this.tabPage6.Text = "Map Tools";
            this.tabPage6.UseVisualStyleBackColor = true;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(6, 77);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(109, 38);
            this.button1.TabIndex = 0;
            this.button1.Text = "Show Collision Rects";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.Show_Collision_Button_Click);
            // 
            // Bush
            // 
            this.Bush.AutoSize = true;
            this.Bush.Location = new System.Drawing.Point(6, 8);
            this.Bush.Name = "Bush";
            this.Bush.Size = new System.Drawing.Size(77, 17);
            this.Bush.TabIndex = 1;
            this.Bush.Text = "Is A Bush?";
            this.Bush.UseVisualStyleBackColor = true;
            this.Bush.CheckedChanged += new System.EventHandler(this.Bush_CheckedChanged);
            // 
            // WinCount
            // 
            this.WinCount.Location = new System.Drawing.Point(217, 5);
            this.WinCount.Margin = new System.Windows.Forms.Padding(2);
            this.WinCount.Name = "WinCount";
            this.WinCount.Size = new System.Drawing.Size(26, 20);
            this.WinCount.TabIndex = 4;
            this.WinCount.ValueChanged += new System.EventHandler(this.numericUpDown2_ValueChanged);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(248, 11);
            this.label7.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(93, 13);
            this.label7.TabIndex = 5;
            this.label7.Text = "Scanned Per Map";
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(6, 121);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(109, 38);
            this.button2.TabIndex = 6;
            this.button2.Text = "Show Bush Rects";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.Show_Bush_Rect_Click);
            // 
            // Collider
            // 
            this.Collider.AutoSize = true;
            this.Collider.Location = new System.Drawing.Point(6, 31);
            this.Collider.Name = "Collider";
            this.Collider.Size = new System.Drawing.Size(86, 17);
            this.Collider.TabIndex = 7;
            this.Collider.Text = "Is a Collider?";
            this.Collider.UseVisualStyleBackColor = true;
            this.Collider.CheckedChanged += new System.EventHandler(this.Collision_CheckedChanged);
            // 
            // Zoom
            // 
            this.Zoom.Increment = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.Zoom.Location = new System.Drawing.Point(217, 31);
            this.Zoom.Margin = new System.Windows.Forms.Padding(2);
            this.Zoom.Maximum = new decimal(new int[] {
            200,
            0,
            0,
            0});
            this.Zoom.Minimum = new decimal(new int[] {
            20,
            0,
            0,
            0});
            this.Zoom.Name = "Zoom";
            this.Zoom.Size = new System.Drawing.Size(64, 20);
            this.Zoom.TabIndex = 8;
            this.Zoom.Value = new decimal(new int[] {
            100,
            0,
            0,
            0});
            this.Zoom.ValueChanged += new System.EventHandler(this.Zoom_ValueChanged);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(285, 35);
            this.label8.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(45, 13);
            this.label8.TabIndex = 9;
            this.label8.Text = "Zoom %";
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(232, 77);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(109, 38);
            this.button3.TabIndex = 10;
            this.button3.Text = "Show Event Rects";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.Show_Event_Rects_Click);
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(232, 121);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(109, 38);
            this.button4.TabIndex = 11;
            this.button4.Text = "Show Spawn Rects";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.Show_Spawn_Rects_Click);
            // 
            // SecondTabControl
            // 
            this.SecondTabControl.Controls.Add(this.Event_List);
            this.SecondTabControl.Location = new System.Drawing.Point(4, 405);
            this.SecondTabControl.Name = "SecondTabControl";
            this.SecondTabControl.SelectedIndex = 0;
            this.SecondTabControl.Size = new System.Drawing.Size(357, 318);
            this.SecondTabControl.TabIndex = 1;
            // 
            // MapEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1134, 775);
            this.Controls.Add(this.SplitContainer1);
            this.Controls.Add(this.ToolStrip);
            this.Controls.Add(this.MenuStrip);
            this.MainMenuStrip = this.MenuStrip;
            this.Name = "MapEditor";
            this.Text = "Nightingale Map Editor";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MapEditor_FormClosing);
            this.Load += new System.EventHandler(this.MapEditor_Load);
            this.ResizeEnd += new System.EventHandler(this.MapEditor_ResizeEnd);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.Map_Panel_Paint);
            this.MenuStrip.ResumeLayout(false);
            this.MenuStrip.PerformLayout();
            this.ToolStrip.ResumeLayout(false);
            this.ToolStrip.PerformLayout();
            this.SplitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.SplitContainer1)).EndInit();
            this.SplitContainer1.ResumeLayout(false);
            this.Editor_Tab.ResumeLayout(false);
            this.tabPage2.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Grid_Column_Num)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.Grid_Row_Num)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Grid_Height_Num)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.Grid_Width_Num)).EndInit();
            this.tabPage4.ResumeLayout(false);
            this.tabPage4.PerformLayout();
            this.tabPage5.ResumeLayout(false);
            this.tabPage5.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.fileSystemWatcher1)).EndInit();
            this.Event_List.ResumeLayout(false);
            this.tabPage6.ResumeLayout(false);
            this.tabPage6.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.WinCount)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.Zoom)).EndInit();
            this.SecondTabControl.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip MenuStrip;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStrip ToolStrip;
        private System.Windows.Forms.ToolStripButton New;
        private System.Windows.Forms.SplitContainer SplitContainer1;
        private System.Windows.Forms.TabControl Editor_Tab;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.ToolStripButton ShowGrid;
        private System.Windows.Forms.ToolStripMenuItem importTilesetToolStripMenuItem;
        private System.Windows.Forms.NumericUpDown Grid_Column_Num;
        private System.Windows.Forms.NumericUpDown Grid_Row_Num;
        private System.Windows.Forms.NumericUpDown Grid_Height_Num;
        private System.Windows.Forms.NumericUpDown Grid_Width_Num;
        private System.Windows.Forms.ToolStripMenuItem saveToToolStripMenuItem;
        private System.Windows.Forms.ToolStripButton toolStripButton3;
        private System.Windows.Forms.TabPage tabPage4;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TabPage tabPage5;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox Event_Name_TextBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ListBox Creature_List;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button Event_Save_Button;
        private System.IO.FileSystemWatcher fileSystemWatcher1;
        private System.Windows.Forms.TabControl SecondTabControl;
        private System.Windows.Forms.TabPage Event_List;
        private System.Windows.Forms.ListBox Event_List_Box;
        private System.Windows.Forms.TabPage tabPage6;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.NumericUpDown Zoom;
        private System.Windows.Forms.CheckBox Collider;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.NumericUpDown WinCount;
        private System.Windows.Forms.CheckBox Bush;
        private System.Windows.Forms.Button button1;
    }
}

