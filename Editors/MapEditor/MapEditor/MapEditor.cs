using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Printing;
using System.Linq;
using System.Text;
using System.Xml;
using System.IO;
using System.Xml.Serialization;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace MapEditor
{
    public partial class MapEditor : Form
    {
        //  Form data
        SGP.CSGP_Direct3D D3D = SGP.CSGP_Direct3D.GetInstance();
        SGP.CSGP_TextureManager TM = SGP.CSGP_TextureManager.GetInstance();

        bool looping = true;
        bool ShowGrids = true;

        bool IsACollider = false;
        bool IsABush = false;

        bool SpawnMode = false;
        bool EventMode = false;

        bool ShowCollisionRects = false;
        bool ShowBushRects = false;
        bool ShowEventRects = false;
        bool ShowSpawnRects = false;

        float Scale = 1.0f;

        //  Map data
        Tile[] MyMap = new Tile[0];
        Events[] MyEvents = new Events[0];
        Size GridSize;
        Size MapSize;

        //  Bitmap
        Bitmap bitmap;

        //  Tile data   
        Size TileSize;
        Size TileSetSize;
        Tile SelectedTile;

        //  Utilities
        Font font = new Font("Times New Roman", 12);
        Pen pen = new Pen(Color.FromArgb(255, 255, 255)); // white
        Brush brush = new SolidBrush(Color.FromArgb(0, 0, 0)); // black 


        public MapEditor()
        {
            InitializeComponent();
        }

        private void MapEditor_Load(object sender, EventArgs e)
        {
            //  Initialize Sizes
            GridSize = new Size(10, 10);
            TileSize = new Size(32, 32);

            //  Run Import window
            ImportTile();

            //  Set Selected Tile & Resize Map
            SelectedTile = new Tile(0, 0);
            ReSize_Map();

            //  Set up data
            Grid_Width_Num.Value = TileSize.Width;
            Grid_Height_Num.Value = TileSize.Height;

            Grid_Column_Num.Value = GridSize.Width;
            Grid_Row_Num.Value = GridSize.Height;
        }

        private void ImportTile()
        {
            if (bitmap != null)
                bitmap.Dispose();

            OpenFileDialog dlg = new OpenFileDialog();
            if (DialogResult.OK == dlg.ShowDialog())
            {
                bitmap = new Bitmap(dlg.FileName);
            }

            tabPage1.AutoScrollMinSize = bitmap.Size;

            Editor_Tab.Size = new Size(Editor_Tab.Size.Width, bitmap.Size.Height + 50);
            SecondTabControl.Top = Editor_Tab.Bottom + 2;

            TileSetSize.Width = bitmap.Size.Width / TileSize.Width;
            TileSetSize.Height = bitmap.Size.Height / TileSize.Height;

            tabPage1.Invalidate();
            Editor_Tab.Invalidate();
        }

        private void ReSize_Map()
        {
            Array.Resize(ref MyMap, GridSize.Width * GridSize.Height);
            for (int i = 0; i < GridSize.Width; i++)
                for (int j = 0; j < GridSize.Height; j++)
                    if (MyMap[Convert2d_1d(i, j, GridSize.Width)] == null)
                    {
                        MyMap[Convert2d_1d(i, j, GridSize.Width)] = new Tile(i, j);
                        MyMap[Convert2d_1d(i, j, GridSize.Width)].Type = 1;
                    }

            MapSize = new Size(GridSize.Width * TileSize.Width, GridSize.Height * TileSize.Height);
            SplitContainer1.Panel1.AutoScrollMinSize = MapSize;
        }

        private int Convert2d_1d(int x, int y, int width)
        {
            return (x + (y * (width - 1)) + y);     //  magic!
        }
        private Size Convert1d_2d(int i)
        {
            int y = 0;
            int w = TileSetSize.Width;

            y = i / w;
            i -= (y * w);

            Size tmp = new Size(i, y);
            return tmp;
        }

        public bool Looping
        {
            get { return looping; }
            set { looping = value; }
        }



        // *** PAINT *** //

        private void Map_Panel_Paint(object sender, PaintEventArgs e)
        {
            Graphics g = e.Graphics;

            //  Figure out offset and apply it to rect to fix render prob
            Point offset = new Point(0, 0);
            bitmap.SetResolution(g.DpiX, g.DpiY);
            offset.X += (int)(SplitContainer1.Panel1.AutoScrollPosition.X * Scale);
            offset.Y += (int)(SplitContainer1.Panel1.AutoScrollPosition.Y * Scale);

            int right = SplitContainer1.Panel1.ClientSize.Width - offset.X;
            int bottom = SplitContainer1.Panel1.ClientSize.Height - offset.Y;
            int left = right - SplitContainer1.Panel1.Width;
            int top = bottom - SplitContainer1.Panel1.Height;

            

            for (int x = 0; x < GridSize.Width; x++)
                for (int y = 0; y < GridSize.Height; y++)
                {
                    float xpos = x * TileSize.Width * Scale;
                    float ypos = y * TileSize.Height * Scale;

                    if (xpos < right &&  ypos < bottom && xpos > left - 1 && ypos > top - 1)
                        RefreshTile(x, y);
                }

           
        }

        private void TileSet_Paint(object sender, PaintEventArgs e)
        {
            Graphics g = tabPage1.CreateGraphics();
            Point offset = new Point(0, 0);

            bitmap.SetResolution(g.DpiX, g.DpiY);
            offset.X += tabPage1.AutoScrollPosition.X;
            offset.Y += tabPage1.AutoScrollPosition.Y;
            e.Graphics.DrawImage(bitmap, offset);

            for (int x = 0; x < TileSetSize.Width; x++)
            {
                for (int y = 0; y < TileSetSize.Height; y++)
                {
                    Rectangle rect = Rectangle.Empty;
                    rect.X = x * TileSize.Width + offset.X;
                    rect.Y = y * TileSize.Height + offset.Y;
                    rect.Size = TileSize;
                    e.Graphics.DrawRectangle(Pens.Black, rect);

                    if (SelectedTile.Row == y && SelectedTile.Column == x)
                    {
                        brush = new SolidBrush(Color.FromArgb(100, 0, 250, 250));
                        e.Graphics.FillRectangle(brush, rect);
                    }
                }
            }
        }

        private void RefreshTile(int x, int y)
        {
            Graphics g = SplitContainer1.Panel1.CreateGraphics();

            //  Figure out offset and apply it to rect to fix render prob
            Point offset = new Point(0, 0);
            bitmap.SetResolution(g.DpiX, g.DpiY);
            offset.X += SplitContainer1.Panel1.AutoScrollPosition.X;
            offset.Y += SplitContainer1.Panel1.AutoScrollPosition.Y;

            // Calculate where the tile is going to be drawn in the map.
            Rectangle destRect = Rectangle.Empty;
            destRect.X = (int)((x * TileSize.Width + offset.X) * Scale);
            destRect.Y = (int)((y * TileSize.Height + offset.Y)* Scale);

            Size sz  = Size.Empty;
            sz.Width = (int)(TileSize.Width * Scale);
            sz.Height = (int)(TileSize.Height * Scale);
            destRect.Size = sz;
            
            // Calculate where the tile is in the bitmap.
            Rectangle srcRect = Rectangle.Empty;
            Size tmp = Convert1d_2d(MyMap[Convert2d_1d(x, y, GridSize.Width)].Type);

            srcRect.X = tmp.Width * TileSize.Width;
            srcRect.Y = tmp.Height * TileSize.Height;
            srcRect.Size = TileSize;

            // Draw the tile.
            g.DrawImage(bitmap, destRect, srcRect, GraphicsUnit.Pixel);

            if (ShowGrids)
                g.DrawRectangle(Pens.Black, destRect);

            if (ShowCollisionRects)
            {
                brush = new SolidBrush(Color.FromArgb(200, 255, 0, 0));
                if (MyMap[Convert2d_1d(x, y, GridSize.Width)].Collision == true)
                    g.FillRectangle(brush, destRect);

            }

            if (ShowBushRects)
            {
                brush = new SolidBrush(Color.FromArgb(100, 255, 0, 255));
                if (MyMap[Convert2d_1d(x, y, GridSize.Width)].Bush == true)
                    g.FillRectangle(brush, destRect);

            }

            if (SpawnMode)
            {
                if (MyMap[Convert2d_1d(x, y, GridSize.Width)].CreatureName == "Bishop")
                {
                    brush = new SolidBrush(Color.FromArgb(150, 0, 0, 255));
                    g.FillRectangle(brush, destRect);
                }
                if (MyMap[Convert2d_1d(x, y, GridSize.Width)].CreatureName == "Bumbles")
                {
                    brush = new SolidBrush(Color.FromArgb(150, 255, 0, 0));
                    g.FillRectangle(brush, destRect);
                }
                if (MyMap[Convert2d_1d(x, y, GridSize.Width)].CreatureName == "Castle")
                {
                    brush = new SolidBrush(Color.FromArgb(150, 0, 255, 0));
                    g.FillRectangle(brush, destRect);
                }
                if (MyMap[Convert2d_1d(x, y, GridSize.Width)].CreatureName == "King")
                {
                    brush = new SolidBrush(Color.FromArgb(150, 255, 0, 255));
                    g.FillRectangle(brush, destRect);
                }
                if (MyMap[Convert2d_1d(x, y, GridSize.Width)].CreatureName == "Passant")
                {
                    brush = new SolidBrush(Color.FromArgb(150, 255, 255, 0));
                    g.FillRectangle(brush, destRect);
                }
                if (MyMap[Convert2d_1d(x, y, GridSize.Width)].CreatureName == "Pawn")
                {
                    brush = new SolidBrush(Color.FromArgb(150, 0, 255, 255));
                    g.FillRectangle(brush, destRect);
                }
                if (MyMap[Convert2d_1d(x, y, GridSize.Width)].CreatureName == "Queen")
                {
                    brush = new SolidBrush(Color.FromArgb(150, 255, 255, 255));
                    g.FillRectangle(brush, destRect);
                }
                if (MyMap[Convert2d_1d(x, y, GridSize.Width)].CreatureName == "Rook")
                {
                    brush = new SolidBrush(Color.FromArgb(150, 255, 155, 50));
                    g.FillRectangle(brush, destRect);
                }
                if (MyMap[Convert2d_1d(x, y, GridSize.Width)].CreatureName == "Prancer")
                {
                    brush = new SolidBrush(Color.FromArgb(150, 255, 155, 50));
                    g.FillRectangle(brush, destRect);
                }
                if (MyMap[Convert2d_1d(x, y, GridSize.Width)].CreatureName == "Knight")
                {
                    brush = new SolidBrush(Color.FromArgb(150, 255, 155, 50));
                    g.FillRectangle(brush, destRect);
                }
            }

            if (EventMode && Event_List_Box.SelectedItem == null)
            {
                if (MyMap[Convert2d_1d(x, y, GridSize.Width)].Event == Event_Name_TextBox.Text && Event_Name_TextBox.Text != "")
                {
                    brush = new SolidBrush(Color.FromArgb(150, 0, 255, 255));
                    g.FillRectangle(brush, destRect);
                }
            }
            if (EventMode && Event_List_Box.SelectedItem != null)
            {
                string s = (string)Event_List_Box.SelectedItem;
                if (s == MyMap[Convert2d_1d(x, y, GridSize.Width)].Event)
                {
                    brush = new SolidBrush(Color.FromArgb(150, 0, 255, 255));
                    g.FillRectangle(brush, destRect);
                }
            }

            if (ShowSpawnRects)
            {
                if (MyMap[Convert2d_1d(x, y, GridSize.Width)].CreatureName != "none")
                {
                    brush = new SolidBrush(Color.FromArgb(150, 0, 120, 0));
                    g.FillRectangle(brush, destRect);
                }
            }

            if (ShowEventRects)
            {
                if (MyMap[Convert2d_1d(x, y, GridSize.Width)].Event != "none")
                {
                    brush = new SolidBrush(Color.FromArgb(150, 0, 150, 150));
                    g.FillRectangle(brush, destRect);
                }
            }
        }
        


        // *** Interactions *** // 

        //  Menu Strip
        private void New_Grid_Click(object sender, EventArgs e)
        {
            //  Reset all data
            TileSize.Width = 16;
            TileSize.Height = 16;
            GridSize.Width = 10;
            GridSize.Height = 10;
            ShowGrids = true;

            Grid_Width_Num.Value = TileSize.Width;
            Grid_Height_Num.Value = TileSize.Height;

            Grid_Row_Num.Value = GridSize.Height;
            Grid_Column_Num.Value = GridSize.Width;

            MyMap = new Tile[0];

            ReSize_Map();
        }

        private void Import_ToolBar_Click(object sender, EventArgs e)
        {
            ImportTile();
        }

        private void Load_Map_File_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dlg = new FolderBrowserDialog();
            if (DialogResult.OK == dlg.ShowDialog())
            {
                XmlDocument doc = new XmlDocument();  
                
                string filename = "/testmap.xml";
                string filepath = dlg.SelectedPath + filename;

                doc.Load(filepath);

                XmlElement ele = doc.GetElementById("Map");
                XmlNode tiles = doc.FirstChild.LastChild;

                int Col = int.Parse(tiles.Attributes["Row"].Value);
                int Row = int.Parse(tiles.Attributes["Col"].Value);
                int TileWidth = int.Parse(tiles.Attributes["TileWidth"].Value);
                int TileHeight = int.Parse(tiles.Attributes["TileHeight"].Value);
                float TileSetWidth = float.Parse(tiles.Attributes["TileSetWidth"].Value);
                float TileSetHeight = float.Parse(tiles.Attributes["TileSetHeight"].Value);
                int WorldWidth = int.Parse(tiles.Attributes["WorldWidth"].Value);
                int WorldHeight = int.Parse(tiles.Attributes["WorldHeight"].Value);
                
                Array.Resize(ref MyMap, Convert2d_1d(Col, Row, Col));

                GridSize.Width = Col;
                GridSize.Height = Row;

                Grid_Row_Num.Value = Col;
                Grid_Column_Num.Value = Row;

                MapSize.Width = WorldWidth;
                MapSize.Height = WorldHeight;

                TileSize.Width = TileWidth;
                TileSize.Height = TileHeight;

                Grid_Width_Num.Value = TileWidth;
                Grid_Height_Num.Value = TileHeight;

                for (int i = 0; i < GridSize.Width; i++)
                {
                    for (int j = 0; j < GridSize.Height; j++)
                    {
                        tiles = tiles.PreviousSibling;
                        MyMap[Convert2d_1d(i, j, GridSize.Width)] = new Tile(i, j);
                        MyMap[Convert2d_1d(i, j, GridSize.Width)].Type = int.Parse(tiles.Attributes["Type"].Value);
                        MyMap[Convert2d_1d(i, j, GridSize.Width)].Collision = bool.Parse(tiles.Attributes["Collision"].Value);
                        MyMap[Convert2d_1d(i, j, GridSize.Width)].Bush = bool.Parse(tiles.Attributes["Bush"].Value);
                    }
                }
                Load_Events(dlg.SelectedPath);                
            }
        }

        private void Load_Events(string filedirectory)
        {
            XmlDocument doc = new XmlDocument();

            string filename = "/testevent.xml";
            string filepath = filedirectory + filename;

            doc.Load(filepath);


            XmlElement ele = doc.GetElementById("Event");
            XmlNode events = doc.FirstChild.FirstChild;


            while (events != null)
            {
                string name = events.Attributes["Name"].Value.ToString();
                int count = int.Parse(events.Attributes["Count"].Value);
                Events tmp = new Events(name);

                for (int x = 0; x < count; ++x)
                {
                    string col = "WorldPosX" + x;
                    string row = "WorldPosY" + x;
                    int Col = int.Parse(events.Attributes[col].Value);
                    int Row = int.Parse(events.Attributes[row].Value);

                    MyMap[Convert2d_1d(Col, Row, GridSize.Width)].Event = name;
                     tmp.AddEventTiles(MyMap[Convert2d_1d(Col, Row, GridSize.Width)]);                                      
                    
                }
                Array.Resize(ref MyEvents, MyEvents.Length + 1);
                MyEvents[MyEvents.Length - 1] = tmp;
                Event_List_Box.Items.Add(MyEvents[MyEvents.Length - 1].Name);
                events = events.NextSibling;
                
            }
            Event_List.Invalidate();


            Load_Spawn_Data(filedirectory);

        }

        private void Load_Spawn_Data(string filedirectory)
        {
            XmlDocument doc = new XmlDocument();

            string filename = "/testspawn.xml";
            string filepath = filedirectory + filename;

            doc.Load(filepath);

           // XmlElement ele = doc.FirstChild;
            XmlNode spawns = doc.FirstChild;

            spawns = spawns.FirstChild;

            while (spawns != null){

                string name = spawns.Attributes["Creature"].Value.ToString();
                int Col = int.Parse(spawns.Attributes["WorldPosX"].Value);
                int Row = int.Parse(spawns.Attributes["WorldPosY"].Value);

                MyMap[Convert2d_1d(Col, Row, GridSize.Width)].CreatureName = name;

                spawns = spawns.NextSibling;
            }            
        }

        private XmlDocument CreateMapDoc()
        {
            //  Xml doc Creation
            XmlDocument xmlDoc = new XmlDocument();

            //	Root of the xml
            XmlElement rootNode = xmlDoc.CreateElement("Map");
            xmlDoc.PrependChild(rootNode);

            //  Chidren of rootele
            XmlElement userNode = xmlDoc.CreateElement("WorldSettings");

            XmlAttribute attribute =
            xmlDoc.CreateAttribute("Row");
            attribute.Value = GridSize.Height.ToString();
            userNode.Attributes.Append(attribute);
            attribute = xmlDoc.CreateAttribute("Col");
            attribute.Value = GridSize.Width.ToString();
            userNode.Attributes.Append(attribute);
            attribute = xmlDoc.CreateAttribute("TileWidth");
            attribute.Value = TileSize.Width.ToString();
            userNode.Attributes.Append(attribute);
            attribute = xmlDoc.CreateAttribute("TileHeight");
            attribute.Value = TileSize.Height.ToString();
            userNode.Attributes.Append(attribute);
            attribute = xmlDoc.CreateAttribute("TileSetWidth");
            attribute.Value = TileSetSize.Width.ToString();
            userNode.Attributes.Append(attribute);
            attribute = xmlDoc.CreateAttribute("TileSetHeight");
            attribute.Value = TileSetSize.Height.ToString();
            userNode.Attributes.Append(attribute);
            attribute = xmlDoc.CreateAttribute("WorldWidth");
            attribute.Value = (MapSize.Width).ToString();
            userNode.Attributes.Append(attribute);
            attribute = xmlDoc.CreateAttribute("WorldHeight");
            attribute.Value = (MapSize.Height).ToString();
            userNode.Attributes.Append(attribute);

            rootNode.PrependChild(userNode);

            for (int c = 0; c < GridSize.Width; ++c)
            {
                for (int r = 0; r < GridSize.Height; ++r)
                {
                    XmlElement tmp = xmlDoc.CreateElement("Tile");
                    attribute = xmlDoc.CreateAttribute("Col");
                    attribute.Value = c.ToString();
                    tmp.Attributes.Append(attribute);
                    attribute = xmlDoc.CreateAttribute("Row");
                    attribute.Value = r.ToString();
                    tmp.Attributes.Append(attribute);
                    attribute = xmlDoc.CreateAttribute("Type");
                    attribute.Value = MyMap[Convert2d_1d(c, r, GridSize.Width)].Type.ToString();
                    tmp.Attributes.Append(attribute);

                    attribute = xmlDoc.CreateAttribute("Collision");
                    attribute.Value = MyMap[Convert2d_1d(c, r, GridSize.Width)].Collision.ToString();
                    tmp.Attributes.Append(attribute);

                    attribute = xmlDoc.CreateAttribute("Bush");
                    attribute.Value = MyMap[Convert2d_1d(c, r, GridSize.Width)].Bush.ToString();
                    tmp.Attributes.Append(attribute);

                    rootNode.PrependChild(tmp);
                }
            }
            return xmlDoc;
        }

        private XmlDocument CreateSpawnDoc()
        {
            XmlDocument xmlSpawn = new XmlDocument();

            //  root's sibling
            XmlElement spawnroot = xmlSpawn.CreateElement("SpawnPoints");
            xmlSpawn.PrependChild(spawnroot);

            for (int c = 0; c < GridSize.Width; ++c)
            {
                for (int r = 0; r < GridSize.Height; ++r)
                {
                    if (MyMap[Convert2d_1d(c, r, GridSize.Width)].CreatureName != "none")
                    {
                        //  Chidren of rootSibling
                        XmlElement userN = xmlSpawn.CreateElement("Spawn");

                        //  Attribute of UserN
                        XmlAttribute att = xmlSpawn.CreateAttribute("WorldPosX");
                        att.Value = (c).ToString();
                        userN.Attributes.Append(att);
                        att = xmlSpawn.CreateAttribute("WorldPosY");
                        att.Value = (r).ToString();
                        userN.Attributes.Append(att);
                        att = xmlSpawn.CreateAttribute("Creature");
                        att.Value = MyMap[Convert2d_1d(c, r, GridSize.Width)].CreatureName;
                        userN.Attributes.Append(att);

                        spawnroot.PrependChild(userN);
                    }
                }
            }
            return xmlSpawn;
        }

        private XmlDocument CreateEventDoc()
        {
            XmlDocument xmlEvent = new XmlDocument();

            //  root's sibling
            XmlElement spawnroot = xmlEvent.CreateElement("Events");
            xmlEvent.PrependChild(spawnroot);

            for (int x = 0; x < MyEvents.Length; ++x)
            {
                //  Chidren of rootSibling
                XmlElement userN = xmlEvent.CreateElement("Event");

                //  Attribute of UserN
                XmlAttribute att = xmlEvent.CreateAttribute("Name");
                att.Value = MyEvents[x].Name;
                userN.Attributes.Append(att);
                att = xmlEvent.CreateAttribute("Count");
                att.Value = MyEvents[x].GetCount().ToString();
                userN.Attributes.Append(att);
                int num = MyEvents[x].GetCount();

                for (int y = 0; y < num; ++y)
                {                    
                    att = xmlEvent.CreateAttribute("WorldPosX" + y);
                    att.Value = MyEvents[x].GetEventTile(y).Column.ToString();
                    userN.Attributes.Append(att);

                    att = xmlEvent.CreateAttribute("WorldPosY" + y);
                    att.Value = MyEvents[x].GetEventTile(y).Row.ToString();
                    userN.Attributes.Append(att);
                }
                spawnroot.PrependChild(userN);
            }
            return xmlEvent;
        }

        private void Save_Click(object sender, EventArgs e)
        {
            string execpath = Application.StartupPath;
            string tileset = execpath + "\\testimg.jpg";
            string xml = execpath + "\\testmap.xml";
            string xmlspawn = execpath + "\\testspawn.xml";
            string xmlevent = execpath + "\\testevent.xml";
            CreateMapDoc().Save(xml);
            CreateSpawnDoc().Save(xmlspawn);
            CreateEventDoc().Save(xmlevent);
            //bitmap.Save(tileset);          
        }

        private void saveToToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dlg = new FolderBrowserDialog();
            if (DialogResult.OK == dlg.ShowDialog())
            {
                string tileset = dlg.SelectedPath + "\\testimg.jpg";
                string xml = dlg.SelectedPath + "\\testmap.xml";
                string eventxml = dlg.SelectedPath + "\\testevent.xml";
                string spawnxml = dlg.SelectedPath + "\\testspawn.xml";
                CreateMapDoc().Save(xml);
                CreateSpawnDoc().Save(spawnxml);
                CreateEventDoc().Save(eventxml);
                bitmap.Save(tileset);
            }
        }


        //  Tool strip
        private void Show_Grid_Click(object sender, EventArgs e)
        {
            ShowGrids = !ShowGrids;
        }

        private void Fill_With_SelectedTile_Click(object sender, EventArgs e)
        {
            int tmp = SelectedTile.Type;
            for (int i = 0; i < GridSize.Width; i++)
                for (int j = 0; j < GridSize.Height; j++)
                    MyMap[Convert2d_1d(i, j, GridSize.Width)].Type = tmp;

            ReSize_Map();
        }



        // Clicks

        private void Select_Tile_Click(object sender, MouseEventArgs e)
        {

            // Calculate the selected tile.
            SelectedTile.Column = e.X / TileSize.Width;
            SelectedTile.Row = e.Y / TileSize.Height;
            SelectedTile.Type = Convert2d_1d(SelectedTile.Column, SelectedTile.Row, TileSetSize.Width);            
            tabPage1.Invalidate();
        }

        private void Place_Multiple_Tiles_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left && EventMode == false && SpawnMode == false)
                Place_Selected_Tile_Click(sender, e);
        }

        private void Place_Selected_Tile_Click(object sender, MouseEventArgs e)
        {
            //  Get coords of mouse
            Point tmp = SplitContainer1.Panel1.AutoScrollPosition;
            int x = (int)((e.X + Math.Abs(tmp.X)) / (TileSize.Width * Scale));
            int y = (int)((e.Y + Math.Abs(tmp.Y)) / (TileSize.Height * Scale));

            //   if mouse click was within map
            if (x < GridSize.Width && y < GridSize.Height)
            {

                MyMap[Convert2d_1d(x, y, GridSize.Width)].Bush = IsABush;

                MyMap[Convert2d_1d(x, y, GridSize.Width)].Collision = IsACollider;                    

                if (SpawnMode)
                {
                    if (Creature_List.SelectedItem != null)
                        MyMap[Convert2d_1d(x, y, GridSize.Width)].CreatureName = Creature_List.SelectedItem.ToString();
                }
                else if (EventMode && Event_Name_TextBox.Text == "")
                {
                   Event_Name_TextBox.Text = "No Name!";
                   Event_Name_TextBox.ForeColor = Color.Red;
                }
                else if (EventMode && Event_Name_TextBox.Text != "")
                {
                    Event_Name_TextBox.ForeColor = Color.Black;
                    MyMap[Convert2d_1d(x, y, GridSize.Width)].Event = Event_Name_TextBox.Text;
                }
                else
                {
                    MyMap[Convert2d_1d(x, y, GridSize.Width)].Type = SelectedTile.Type;
                }
                RefreshTile(x, y);
            }
        }



        //  Size Changes
        private void Grid_Width_ValueChanged(object sender, EventArgs e)
        {
            TileSize.Width = (int)Grid_Width_Num.Value;
            ReSize_Map();
        }

        private void Grid_Height_ValueChanged(object sender, EventArgs e)
        {
            TileSize.Height = (int)Grid_Height_Num.Value;
            ReSize_Map();
            TileSetSize.Width = bitmap.Size.Width / TileSize.Width;
            TileSetSize.Height = bitmap.Size.Height / TileSize.Height;
        }

        private void Grid_Row_ValueChanged(object sender, EventArgs e)
        {
            GridSize.Height = (int)Grid_Row_Num.Value;
            ReSize_Map();
            TileSetSize.Width = bitmap.Size.Width / TileSize.Width;
            TileSetSize.Height = bitmap.Size.Height / TileSize.Height;
        }

        private void Grid_Column_ValueChanged(object sender, EventArgs e)
        {
            GridSize.Width = (int)Grid_Column_Num.Value;
            ReSize_Map();
        }

        private void MapEditor_ResizeEnd(object sender, EventArgs e)
        {
            int left_ = SplitContainer1.Panel1.Right;
            int right_ = SplitContainer1.Panel2.Right;
            int bottom_ = SplitContainer1.Panel2.Bottom;

            int top_ = SecondTabControl.Location.Y;

            int offset2 = bottom_ - top_;
            int offset = right_ - (left_ + 10);
            Editor_Tab.Size = new Size(offset, Editor_Tab.Size.Height);
            SecondTabControl.Size = new Size(offset, offset2 - 2);

            tabPage1.AutoScrollMinSize = bitmap.Size;
            tabPage1.Invalidate();
        }

        private void MapEditor_FormClosing(object sender, FormClosingEventArgs e)
        {
            looping = false;
        }

        private void Spawn_Points_Enter(object sender, EventArgs e)
        {
            SpawnMode = true;
            
        }

        private void Spawn_Points_Leave(object sender, EventArgs e)
        {
            SpawnMode = false;
            
        }

        private void Event_Layer_Enter(object sender, EventArgs e)
        {
            EventMode = true;        
        }

        private void Event_Layer_Leave(object sender, EventArgs e)
        {
            EventMode = false;            
        }

        private void Event_Save_Click(object sender, EventArgs e)
        {
            if (Event_Name_TextBox.Text == "")
            {               
                Event_Name_TextBox.Text = "Please Enter A Name!";
                Event_Name_TextBox.ForeColor = Color.Red;
            }
            else
            {

                Events tmp = new Events(Event_Name_TextBox.Text);

                for (int x = 0; x < GridSize.Width; ++x)
                    for (int y = 0; y < GridSize.Height; ++y)
                        if (MyMap[Convert2d_1d(x, y, GridSize.Width)].Event == Event_Name_TextBox.Text)
                            tmp.AddEventTiles(MyMap[Convert2d_1d(x, y, GridSize.Width)]);


                Event_Name_TextBox.Text = "";
                Array.Resize(ref MyEvents, MyEvents.Length + 1);
                MyEvents[MyEvents.Length - 1] = tmp;
                Event_List_Box.Items.Add(MyEvents[MyEvents.Length - 1].Name);
                Event_List.Invalidate();
                
            }
        }

        private void Event_List_Box_SelectedIndexChanged(object sender, EventArgs e)
        {
            Event_Name_TextBox.Text = (string)Event_List_Box.SelectedItem;
        }

        private void Show_Collision_Button_Click(object sender, EventArgs e)
        {
            ShowCollisionRects = !ShowCollisionRects;
            SplitContainer1.Panel1.Invalidate();
        }

        private void Show_Spawn_Rects_Click(object sender, EventArgs e)
        {
            ShowSpawnRects = !ShowSpawnRects;
            SplitContainer1.Panel1.Invalidate();
        }

        private void Show_Event_Rects_Click(object sender, EventArgs e)
        {
            ShowEventRects = !ShowEventRects;
            SplitContainer1.Panel1.Invalidate();
        }

        private void Show_Bush_Rect_Click(object sender, EventArgs e)
        {
            ShowBushRects = !ShowBushRects;
            SplitContainer1.Panel1.Invalidate();
        }

        private void Bush_CheckedChanged(object sender, EventArgs e)
        {
            IsABush = Bush.Checked;
        }

        private void numericUpDown2_ValueChanged(object sender, EventArgs e)
        {

        }
      
        private void Collision_CheckedChanged(object sender, EventArgs e)
        {
            IsACollider = Collider.Checked;
        }

        private void Zoom_ValueChanged(object sender, EventArgs e)
        {
            Scale = (float)Zoom.Value / 100.0f;
            SplitContainer1.Panel1.Invalidate();
        }        
    }
}