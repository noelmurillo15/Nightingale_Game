using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml.Serialization;
using System.Xml.Linq;

namespace AnimationEditor
{
    public partial class Form1 : Form
    {
        SGP.CSGP_Direct3D D3D = SGP.CSGP_Direct3D.GetInstance();
        SGP.CSGP_TextureManager TM = SGP.CSGP_TextureManager.GetInstance();
        List<Animation> Animations;
        int currAnimation;
        List<Rectangle> LR;
        Bitmap currentBM;
        int playFrame;
        int currTool;

        float playdur;
        int currentTexture = -1;
        bool mousedown;
        bool display;
        bool play;
        string BM;
        System.Diagnostics.Stopwatch sw;

        public Form1()
        {
            InitializeComponent();
            D3D.Initialize(FramePanel, true);
            D3D.AddRenderTarget(FramesPanel);
            D3D.AddRenderTarget(PlayPanel);
            TM.Initialize(D3D.Device, D3D.Sprite);
            Animations = new List<Animation>();
            Animation temp = new Animation();
            temp.looping = true;
            temp.Frames = new List<Frame>();
            Animations.Add(temp);
            Frame t = new Frame();
            Animations[currAnimation].Frames.Add(t);
            currAnimation = 0;
            t.duration = .25f;
            playFrame = 0;
            AnimationsLB.Items.Add(temp);
            Rectangle Temp = new Rectangle();
            Temp.X = 0;
            Temp.Y = 0;
            Temp.Width = 100;
            Temp.Height = 100;
            LR = new List<Rectangle>();
            LR.Add(Temp);

            currTool = 0;
            playFrame = 0;
            playdur = 0.0f;
            display = false;
            sw = new System.Diagnostics.Stopwatch();
            //FramePanel.AutoScrollMinSize = new Size(TM.GetTextureWidth(currentTexture), TM.GetTextureHeight(currentTexture));


            Animations[currAnimation].looping = true;
        }

        bool looping = true;

        public bool Looping
        {
            get { return looping; }
            set { looping = value; }
        }

        public new void Update()
        {

            if (play == true)
            {

                playdur -= (float)sw.ElapsedMilliseconds / 1000;
                if (playdur <= 0.0f)
                {
                    playFrame++;
                    if (playFrame >= Animations[currAnimation].Frames.Count)
                    {
                        if (Animations[currAnimation].looping == false)
                            play = false;
                        playFrame = 0;
                    }
                    playdur = Animations[currAnimation].Frames[playFrame].duration;
                }

                sw.Reset();
                sw.Start();
            }
            FDurationUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].duration;


            if (currTool == 0)
            {
                DrawRectB.BackColor = Color.LightBlue;
                CollisionRectB.BackColor = Color.White;
                AnchorB.BackColor = Color.White;
                EventRectB.BackColor = Color.White;
            }
            if (currTool == 1)
            {
                DrawRectB.BackColor = Color.White;
                CollisionRectB.BackColor = Color.LightBlue;
                AnchorB.BackColor = Color.White;
                EventRectB.BackColor = Color.White;
            }
            if (currTool == 3)
            {
                DrawRectB.BackColor = Color.White;
                CollisionRectB.BackColor = Color.White;
                AnchorB.BackColor = Color.White;
                EventRectB.BackColor = Color.LightBlue;
            }
            if (currTool == 2)
            {
                DrawRectB.BackColor = Color.White;
                CollisionRectB.BackColor = Color.White;
                AnchorB.BackColor = Color.LightBlue;
                EventRectB.BackColor = Color.White;
            }

        }

        public void Render()
        {
            D3D.Clear(FramePanel, Color.White);
            D3D.DeviceBegin();
            D3D.SpriteBegin();
            Point ANtemp = Animations[currAnimation].Frames[playFrame].AnchorPoint;
            Rectangle EVtemp = Animations[currAnimation].Frames[playFrame].EventRect;
            Rectangle CLtemp = Animations[currAnimation].Frames[playFrame].CollisionRect;
            Rectangle DRtemp = Animations[currAnimation].Frames[playFrame].DrawRect;
            if (currentTexture >= 0)
                TM.Draw(currentTexture, 0 + FramePanel.AutoScrollPosition.X, 0 + FramePanel.AutoScrollPosition.Y);

            if (Animations[currAnimation].Frames[playFrame].DrawX != -1)
            {

                DRtemp.X += FramePanel.AutoScrollPosition.X;
                DRtemp.Y += FramePanel.AutoScrollPosition.Y;
            }
            if (Animations[currAnimation].Frames[playFrame].CollisionX != -1)
            {

                CLtemp.X += FramePanel.AutoScrollPosition.X;
                CLtemp.Y += FramePanel.AutoScrollPosition.Y;

            }
            if (Animations[currAnimation].Frames[playFrame].EventX != -1)
            {

                EVtemp.X += FramePanel.AutoScrollPosition.X;
                EVtemp.Y += FramePanel.AutoScrollPosition.Y;

            }
            if (Animations[currAnimation].Frames[playFrame].AnchorX != -1)
            {
                ANtemp.X += FramePanel.AutoScrollPosition.X;
                ANtemp.Y += FramePanel.AutoScrollPosition.Y;
            }
            D3D.DrawHollowRect(DRtemp, Color.Blue, 1);
            D3D.DrawHollowRect(CLtemp, Color.Red, 1);
            D3D.DrawHollowRect(EVtemp, Color.Green, 1);
            D3D.DrawRect(new Rectangle(ANtemp.X - 2, ANtemp.Y - 1, 4, 4), Color.Yellow);


            D3D.SpriteEnd();
            D3D.DeviceEnd();
            D3D.Present();
            ///////////////////////////////////////////////////
            D3D.Clear(FramesPanel, Color.White);
            D3D.DeviceBegin();
            D3D.SpriteBegin();
            for (int i = 0; i < Animations[currAnimation].Frames.Count; i++)
            {
                Rectangle t = LR[i];
                t.X += FramesPanel.AutoScrollPosition.X;
                t.Y += FramesPanel.AutoScrollPosition.Y;
                
                D3D.DrawHollowRect(t, Color.Black, 1);




                if (Animations[currAnimation].Frames[i].DrawX != -1 && currentTexture == 0)
                    TM.Draw(currentTexture, t.X, 0, 1, 1, Animations[currAnimation].Frames[i].DrawRect);
                //if (Animations[currAnimation].Frames[i].CollisionX != -1)
                //{
                //    D3D.DrawHollowRect(new Rectangle(LR[i].X + Animations[currAnimation].Frames[i].CollisionX,
                //        Animations[currAnimation].Frames[i].CollisionY,
                //        Animations[currAnimation].Frames[i].CollisionSize.Width,
                //        Animations[currAnimation].Frames[i].CollisionSize.Height), Color.Red, 1);
                //}
                //
                ///if (Animations[currAnimation].Frames[playFrame].CollisionX != -1)
                ///{
                ///    D3D.DrawHollowRect(new Rectangle(LR[i].X + Animations[currAnimation].Frames[i].CollisionX,
                ///        Animations[currAnimation].Frames[i].CollisionY,
                ///        Animations[currAnimation].Frames[i].CollisionSize.Width,
                ///        Animations[currAnimation].Frames[i].CollisionSize.Height), Color.Red, 1);
                ///}
                //if (Animations[currAnimation].Frames[i].AnchorX != -1)
                //{
                //    D3D.DrawRect(new Rectangle(LR[i].X +Animations[currAnimation].Frames[i].AnchorPoint.X - 2, Animations[currAnimation].Frames[i].AnchorPoint.Y - 1, 4, 4), Color.Yellow);
                //}

            }
            D3D.SpriteEnd();
            D3D.DeviceEnd();
            D3D.Present();

            ////////////////////////////////////////////////////////

            D3D.Clear(PlayPanel, Color.White);
            D3D.DeviceBegin();
            D3D.SpriteBegin();

            if (Animations[0].Frames[0].DrawX != -1 && currentTexture == 0)
                TM.Draw(currentTexture, 150 - Animations[currAnimation].Frames[playFrame].AnchorX + Animations[currAnimation].Frames[playFrame].DrawX
                    , 200 - Animations[currAnimation].Frames[playFrame].AnchorY + Animations[currAnimation].Frames[playFrame].DrawY, 1, 1, Animations[currAnimation].Frames[playFrame].DrawRect);
            if (display == true)
            {
                if (Animations[currAnimation].Frames[playFrame].CollisionX != -1)
                    D3D.DrawHollowRect(new Rectangle(150 - Animations[currAnimation].Frames[playFrame].AnchorX + Animations[currAnimation].Frames[playFrame].CollisionX
                        , 200 - Animations[currAnimation].Frames[playFrame].AnchorY + Animations[currAnimation].Frames[playFrame].CollisionY,
                        Animations[currAnimation].Frames[playFrame].CollisionSize.Width,
                        Animations[currAnimation].Frames[playFrame].CollisionSize.Height), Color.Red, 1);
                if (Animations[currAnimation].Frames[playFrame].EventX != -1)
                    D3D.DrawHollowRect(new Rectangle(150 - Animations[currAnimation].Frames[playFrame].EventX, 200 - Animations[currAnimation].Frames[playFrame].EventY,
                        Animations[currAnimation].Frames[playFrame].EventSize.Width,
                        Animations[currAnimation].Frames[playFrame].EventSize.Height), Color.Yellow, 1);
                if (Animations[currAnimation].Frames[playFrame].AnchorX != -1)
                    D3D.DrawRect(new Rectangle(150 - 2, 200 - 1, 4, 4), Color.Green);
            }
            D3D.SpriteEnd();
            D3D.DeviceEnd();
            D3D.Present();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            looping = false;
        }



        private void openBitmapToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            // Set the filter strings

            // Display the dialog to the user
            //if (DialogResult.OK == dlg.ShowDialog())



            if (dlg.ShowDialog() == DialogResult.OK)
            {
                dlg.Title = "Open Image";
                dlg.Filter = "bmp files (*.bmp)|*.bmp";
                PictureBox PictureBox1 = new PictureBox();
                currentBM = new Bitmap(dlg.FileName);
                BM = dlg.FileName;
                // Create a new Bitmap object from the picture file on disk,
                // and assign that to the PictureBox.Image property
                currentTexture = TM.LoadTexture(dlg.FileName);

                Render();
            }
            if (!(currentTexture < 0))
            {
                FramePanel.AutoScrollMinSize = new Size(TM.GetTextureWidth(currentTexture), TM.GetTextureHeight(currentTexture));
            }
        }

        private void FramePanel_MouseDown(object sender, MouseEventArgs e)
        {

            Point offset = new Point(0, 0);

            // Adding Autoscroll Position
            offset.X += FramePanel.AutoScrollPosition.X;
            offset.Y += FramePanel.AutoScrollPosition.Y;
            if (e.Button == MouseButtons.Left)
            {
                if (mousedown == false)
                {

                    XUD.Value = e.X - offset.X;
                    YUD.Value = e.Y - offset.Y;
                    if (currTool == 0)
                    {
                        Animations[currAnimation].Frames[playFrame].DrawX = (int)XUD.Value;
                        Animations[currAnimation].Frames[playFrame].DrawY = (int)YUD.Value;
                        mousedown = true;
                    }
                    else if (currTool == 1)
                    {
                        Animations[currAnimation].Frames[playFrame].CollisionX = (int)XUD.Value;
                        Animations[currAnimation].Frames[playFrame].CollisionY = (int)YUD.Value;
                        mousedown = true;
                    }
                    else if (currTool == 2)
                    {
                        Animations[currAnimation].Frames[playFrame].AnchorX = (int)XUD.Value;
                        Animations[currAnimation].Frames[playFrame].AnchorY = (int)YUD.Value;
                        mousedown = true;
                    }
                    else if (currTool == 3)
                    {
                        Animations[currAnimation].Frames[playFrame].EventX = (int)XUD.Value;
                        Animations[currAnimation].Frames[playFrame].EventY = (int)YUD.Value;
                        mousedown = true;
                    }


                }
                Render();

            }

        }

        private void FramePanel_MouseUp(object sender, MouseEventArgs e)
        {
            mousedown = false;

        }

        private void FramePanel_MouseMove(object sender, MouseEventArgs e)
        {
            Point offset = new Point(0, 0);

            // Adding Autoscroll Position
            offset.X += FramePanel.AutoScrollPosition.X;
            offset.Y += FramePanel.AutoScrollPosition.Y;
            if (e.Button == MouseButtons.Left)
            {
                if (e.Location.X >= 0 && e.Location.Y >= 0)
                {
                    WidthUD.Value = e.X - offset.X;
                    HeightUD.Value = e.Y - offset.Y;
                }
                if (currTool == 0)
                {
                    Animations[currAnimation].Frames[playFrame].DrawRight = (int)WidthUD.Value;
                    Animations[currAnimation].Frames[playFrame].DrawBottom = (int)HeightUD.Value;
                    mousedown = true;
                }
                else if (currTool == 1)
                {
                    Animations[currAnimation].Frames[playFrame].CollisionRight = (int)WidthUD.Value;
                    Animations[currAnimation].Frames[playFrame].CollisionBottom = (int)HeightUD.Value;
                    mousedown = true;
                }

                else if (currTool == 3)
                {
                    Animations[currAnimation].Frames[playFrame].EventRight = (int)WidthUD.Value;
                    Animations[currAnimation].Frames[playFrame].EventBottom = (int)HeightUD.Value;
                    mousedown = true;
                }
                else
                {
                    WidthUD.Value = 0;
                    HeightUD.Value = 0;
                }
                Render();
            }
        }

        private void DrawRectB_Click(object sender, EventArgs e)
        {
            currTool = 0;
        }

        private void CollisionRectB_Click(object sender, EventArgs e)
        {
            currTool = 1;
        }

        private void AnchorB_Click(object sender, EventArgs e)
        {
            currTool = 2;
        }

        private void FramePanel_Scroll(object sender, ScrollEventArgs e)
        {

            Render();
        }

        private void XUD_ValueChanged(object sender, EventArgs e)
        {
            if (currTool == 0)
            {
                Animations[currAnimation].Frames[playFrame].DrawX = (int)XUD.Value;
            }
            else if (currTool == 1)
            {
                Animations[currAnimation].Frames[playFrame].CollisionX = (int)XUD.Value;
            }
            else if (currTool == 2)
            {
                Animations[currAnimation].Frames[playFrame].AnchorX = (int)XUD.Value;
            }
            else if (currTool == 3)
            {
                Animations[currAnimation].Frames[playFrame].EventX = (int)XUD.Value;
            }
        }

        private void YUD_ValueChanged(object sender, EventArgs e)
        {
            if (currTool == 0)
            {
                Animations[currAnimation].Frames[playFrame].DrawY = (int)YUD.Value;
            }
            else if (currTool == 1)
            {
                Animations[currAnimation].Frames[playFrame].CollisionY = (int)YUD.Value;
            }
            else if (currTool == 2)
            {
                Animations[currAnimation].Frames[playFrame].AnchorY = (int)YUD.Value;
            }
            else if (currTool == 3)
            {
                Animations[currAnimation].Frames[playFrame].EventY = (int)YUD.Value;
            }
        }

        private void WidthUD_ValueChanged(object sender, EventArgs e)
        {
            if (currTool == 0)
            {
                Animations[currAnimation].Frames[playFrame].DrawRight = (int)WidthUD.Value;
            }
            else if (currTool == 1)
            {
                Animations[currAnimation].Frames[playFrame].CollisionRight = (int)WidthUD.Value;
            }

            else if (currTool == 3)
            {
                Animations[currAnimation].Frames[playFrame].EventRight = (int)WidthUD.Value;
            }
        }

        private void HeightUD_ValueChanged(object sender, EventArgs e)
        {
            if (currTool == 0)
            {
                Animations[currAnimation].Frames[playFrame].DrawBottom = (int)HeightUD.Value;
            }
            else if (currTool == 1)
            {
                Animations[currAnimation].Frames[playFrame].CollisionBottom = (int)HeightUD.Value;
            }

            else if (currTool == 3)
            {
                Animations[currAnimation].Frames[playFrame].EventBottom = (int)HeightUD.Value;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            display = false;
        }

        private void AddFB_Click(object sender, EventArgs e)
        {
            Frame t = new Frame();
            t.duration = .5f;
            Animations[currAnimation].Frames.Add(t);
            playFrame++;

            Rectangle Temp = new Rectangle();
            Temp.X = LR.Count * 100;
            Temp.Y = 0;
            Temp.Width = 100;
            Temp.Height = 100;
            LR.Add(Temp);

            FramesPanel.AutoScrollMinSize = new Size(LR.Count * 100,100);
        }

        private void PlayB_Click(object sender, EventArgs e)
        {
            play = true;
        }

        private void PauseB_Click(object sender, EventArgs e)
        {
            play = false;
        }



        private void PreviewNextB_Click(object sender, EventArgs e)
        {
            playFrame++;
            if (playFrame >= Animations[currAnimation].Frames.Count)
            {
                playFrame = 0;
            }
            FDurationUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].duration;
        }

        private void PreviewPrevB_Click(object sender, EventArgs e)
        {
            playFrame--;
            if (playFrame < 0)
            {
                playFrame = Animations[currAnimation].Frames.Count - 1;
            }
            FDurationUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].duration;
        }

        private void FDurationUD_ValueChanged(object sender, EventArgs e)
        {
            Animations[currAnimation].Frames[playFrame].duration = (float)FDurationUD.Value;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            display = true;
        }

        private void FramesPanel_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                for (int i = 0; i < Animations[currAnimation].Frames.Count; i++)
                {
                Rectangle t = LR[i];
                t.X += FramesPanel.AutoScrollPosition.X;
                    if (e.X > t.X && e.Y > t.Y
                    && e.X < t.X + t.Height && e.Y < t.Y + t.Width)
                    {
                        playFrame = i;
                    }
                }
            }
            if (Animations[currAnimation].Frames[playFrame].DrawX > 0)
            {
                if (currTool == 0 && Animations[currAnimation].Frames[playFrame].DrawX > 0)
                {
                    comboBox1.Text = Animations[currAnimation].Frames[playFrame].Event;
                    XUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].DrawX;
                    YUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].DrawY;
                    WidthUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].DrawRight;
                    HeightUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].DrawBottom;
                }
                if (currTool == 1 && Animations[currAnimation].Frames[playFrame].CollisionX > 0)
                {
                    comboBox1.Text = Animations[currAnimation].Frames[playFrame].Event;
                    XUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].CollisionX;
                    YUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].CollisionY;
                    WidthUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].CollisionRight;
                    HeightUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].CollisionBottom;
                }
                if (currTool == 2 && Animations[currAnimation].Frames[playFrame].AnchorX > 0)
                {
                    comboBox1.Text = Animations[currAnimation].Frames[playFrame].Event;
                    XUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].AnchorX;
                    YUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].AnchorY;
                }
                if (currTool == 3 && Animations[currAnimation].Frames[playFrame].EventX > 0)
                {

                    comboBox1.Text = Animations[currAnimation].Frames[playFrame].Event;
                    XUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].EventX;
                    YUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].EventY;
                    WidthUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].EventRight;
                    HeightUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].EventBottom;
                }
                
            }

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            Animations[currAnimation].Name = Animname.Text;
            Animname.ForeColor = Color.Black;
        }

        private void AddAB_Click(object sender, EventArgs e)
        {


            AnimationsLB.Items.Add(Animations[currAnimation]);
            Animation nAnim = new Animation();
            nAnim.Frames = new List<Frame>();
            Animations.Add(nAnim);
            nAnim.looping = true;
            currAnimation = Animations.Count - 1;
            playFrame = 0;

            Frame t = new Frame();
            t.duration = .25f;
            Animations[currAnimation].Frames.Add(t);
            Animations[currAnimation].looping = false;

        }

        private void AnimationsLB_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (!(AnimationsLB.SelectedIndex == -1))
            {
                currAnimation = AnimationsLB.SelectedIndex;
                playFrame = 0;
                Animname.Text = Animations[currAnimation].Name;
                comboBox1.Text = Animations[currAnimation].Frames[playFrame].Event;
            }
            if (Animations[currAnimation].Frames[playFrame].DrawX > 0)
            {

                if (currTool == 0 && Animations[currAnimation].Frames[playFrame].DrawX > 0)
                {
                    comboBox1.Text = Animations[currAnimation].Frames[playFrame].Event;
                    XUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].DrawX;
                    YUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].DrawY;
                    WidthUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].DrawRight;
                    HeightUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].DrawBottom;
                }
                if (currTool == 1 && Animations[currAnimation].Frames[playFrame].CollisionX > 0)
                {
                    comboBox1.Text = Animations[currAnimation].Frames[playFrame].Event;
                    XUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].CollisionX;
                    YUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].CollisionY;
                    WidthUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].CollisionRight;
                    HeightUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].CollisionBottom;
                }
                if (currTool == 2 && Animations[currAnimation].Frames[playFrame].AnchorX > 0)
                {
                    comboBox1.Text = Animations[currAnimation].Frames[playFrame].Event;
                    XUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].AnchorX;
                    YUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].AnchorY;
                }
                if (currTool == 3 && Animations[currAnimation].Frames[playFrame].EventX > 0)
                {

                    comboBox1.Text = Animations[currAnimation].Frames[playFrame].Event;
                    XUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].EventX;
                    YUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].EventY;
                    WidthUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].EventRight;
                    HeightUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].EventBottom;
                }
            }

        }

        private void EventRectB_Click(object sender, EventArgs e)
        {
            currTool = 3;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            Animations[currAnimation].Frames.Remove(Animations[currAnimation].Frames[playFrame]);
            LR.Remove(LR[playFrame]);
            playFrame--;
            if (currTool == 0 && Animations[currAnimation].Frames[playFrame].DrawX > 0)
            {
                comboBox1.Text = Animations[currAnimation].Frames[playFrame].Event;
                XUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].DrawX;
                YUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].DrawY;
                WidthUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].DrawRight;
                HeightUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].DrawBottom;
            }
            if (currTool == 1 && Animations[currAnimation].Frames[playFrame].CollisionX > 0)
            {
                comboBox1.Text = Animations[currAnimation].Frames[playFrame].Event;
                XUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].CollisionX;
                YUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].CollisionY;
                WidthUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].CollisionRight;
                HeightUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].CollisionBottom;
            }
            if (currTool == 2 && Animations[currAnimation].Frames[playFrame].AnchorX > 0)
            {
                comboBox1.Text = Animations[currAnimation].Frames[playFrame].Event;
                XUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].AnchorX;
                YUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].AnchorY;
            }
            if (currTool == 3 && Animations[currAnimation].Frames[playFrame].EventX > 0)
            {

                comboBox1.Text = Animations[currAnimation].Frames[playFrame].Event;
                XUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].EventX;
                YUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].EventY;
                WidthUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].EventRight;
                HeightUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].EventBottom;
            }
        }

        private void LoopingCB_CheckedChanged(object sender, EventArgs e)
        {
            if (LoopingCB.Checked == true)
            {
                Animations[currAnimation].looping = true;
            }
            else
            {
                Animations[currAnimation].looping = false;
            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            if (AnimationsLB.SelectedIndex > 0)
            {
                if (AnimationsLB.Items[AnimationsLB.SelectedIndex] == Animations[currAnimation])
                {
                    Animation c = (Animation)AnimationsLB.Items[AnimationsLB.SelectedIndex];
                    AnimationsLB.Items.Remove(c);
                    Animations.Remove(c);
                }
            }
            if (Animations.Count > 0 && Animations[currAnimation].Frames[playFrame].DrawX > 0)
            {
                if (currTool == 0 && Animations[currAnimation].Frames[playFrame].DrawX > 0)
                {
                    comboBox1.Text = Animations[currAnimation].Frames[playFrame].Event;
                    XUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].DrawX;
                    YUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].DrawY;
                    WidthUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].DrawRight;
                    HeightUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].DrawBottom;
                }
                if (currTool == 1 && Animations[currAnimation].Frames[playFrame].CollisionX > 0)
                {
                    comboBox1.Text = Animations[currAnimation].Frames[playFrame].Event;
                    XUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].CollisionX;
                    YUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].CollisionY;
                    WidthUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].CollisionRight;
                    HeightUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].CollisionBottom;
                }
                if (currTool == 2 && Animations[currAnimation].Frames[playFrame].AnchorX > 0)
                {
                    comboBox1.Text = Animations[currAnimation].Frames[playFrame].Event;
                    XUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].AnchorX;
                    YUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].AnchorY;
                }
                if (currTool == 3 && Animations[currAnimation].Frames[playFrame].EventX > 0)
                {

                    comboBox1.Text = Animations[currAnimation].Frames[playFrame].Event;
                    XUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].EventX;
                    YUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].EventY;
                    WidthUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].EventRight;
                    HeightUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].EventBottom;
                }
            }
        }

        private void SaveB_Click(object sender, EventArgs e)
        {
            save();
        }

        void save()
        {

            for (int i = 0; i < Animations.Count; i++)
            {
                if (Animations[i].Name == "" || Animations[i].Name == "(insert object name here)" || Animations[i].Name ==null)
                {
                    currAnimation = i;
                    string an = "Animation " + i + "Insert Name here";
                    Animname.Text = an;
                    Animname.ForeColor = Color.Red;
                    return;
                }
            }



            if (ObjectNameTB.Text == "" || ObjectNameTB.Text == "(insert object name here)")
            {
                ObjectNameTB.Text = "(insert object name here)";
                ObjectNameTB.ForeColor = Color.Red;
            }
            else
            {
             


                FolderBrowserDialog dlg = new FolderBrowserDialog();
                //dlg.Filter = "All Files|*.*|XML Files|*.xml";
                //dlg.FilterIndex = 2;
                //dlg.DefaultExt = "xml";

                if (DialogResult.OK == dlg.ShowDialog())
                {
                    string xmlFilePath = dlg.SelectedPath + "\\" + ObjectNameTB.Text + ".png";
                    currentBM.Save(xmlFilePath);

                    XElement xRoot = new XElement("AnimationFile");
                    XElement xAnimC = new XElement("Animations");
                    xRoot.Add(xAnimC);
                    XAttribute xAnimAmount = new XAttribute("AnimationCount", Animations.Count);
                    xAnimC.Add(xAnimAmount);

                    XElement xObject = new XElement("Object");
                    XAttribute xName = new XAttribute("ObjectName", BM);
                     xObject.Add(xName);
                    xRoot.Add(xObject);
                    for (int i = 0; i < Animations.Count; i++)
                    {


                        XElement xAnimation = new XElement("Animation");
                        xObject.Add(xAnimation);
                        XAttribute xAnimN = new XAttribute("AnimName", Animations[i].Name);


                         XAttribute xFramC = new XAttribute("FrameCount", Animations[i].Frames.Count);
                         xAnimation.Add(xFramC);
                         xAnimation.Add(xAnimN);

                        for (int k = 0; k < Animations[i].Frames.Count; k++)
                        {
                            XElement xFrame = new XElement("Frame");
                            XAttribute xEvent;
                            if (Animations[i].Frames[k].Event == null)
                            {
                                xEvent = new XAttribute("Event", "N/A");
                            }
                            else
                                xEvent = new XAttribute("Event", Animations[i].Frames[k].Event);
                            xFrame.Add(xEvent);
                            XAttribute xDrawL = new XAttribute("DrawLeft", Animations[i].Frames[k].DrawX);
                            xFrame.Add(xDrawL);
                            XAttribute xDrawT = new XAttribute("DrawTop", Animations[i].Frames[k].DrawY);
                            xFrame.Add(xDrawT);
                            XAttribute xDrawR = new XAttribute("DrawRight", Animations[i].Frames[k].DrawRight);
                            xFrame.Add(xDrawR);
                            XAttribute xDrawB = new XAttribute("DrawBottom", Animations[i].Frames[k].DrawBottom);
                            xFrame.Add(xDrawB);

                            XAttribute xCollisionL = new XAttribute("CollisionLeft", Animations[i].Frames[k].CollisionX);
                            xFrame.Add(xCollisionL);
                            XAttribute xCollisionT = new XAttribute("CollisionTop", Animations[i].Frames[k].CollisionY);
                            xFrame.Add(xCollisionT);
                            XAttribute xCollisionR = new XAttribute("CollisionRight", Animations[i].Frames[k].CollisionRight);
                            xFrame.Add(xCollisionR);
                            XAttribute xCollisionB = new XAttribute("CollisionBottom", Animations[i].Frames[k].CollisionBottom);
                            xFrame.Add(xCollisionB);

                            XAttribute xEventL = new XAttribute("EventLeft", Animations[i].Frames[k].EventX);
                            xFrame.Add(xEventL);
                            XAttribute xEventT = new XAttribute("EventTop", Animations[i].Frames[k].EventY);
                            xFrame.Add(xEventT);
                            XAttribute xEventR = new XAttribute("EventRight", Animations[i].Frames[k].EventRight);
                            xFrame.Add(xEventR);
                            XAttribute xEventB = new XAttribute("EventBottom", Animations[i].Frames[k].EventBottom);
                            xFrame.Add(xEventB);

                            XAttribute xAnchorL = new XAttribute("AnchorLeft", Animations[i].Frames[k].AnchorX);
                            xFrame.Add(xAnchorL);
                            XAttribute xAnchorT = new XAttribute("AnchorTop", Animations[i].Frames[k].AnchorY);
                            xFrame.Add(xAnchorT);


                            XAttribute xDuration = new XAttribute("Duration", Animations[i].Frames[k].duration);
                            xFrame.Add(xDuration);

                            xAnimation.Add(xFrame);
                        }
                    }

                     xmlFilePath = dlg.SelectedPath + "\\" + ObjectNameTB.Text + ".xml";
                    xRoot.Save(xmlFilePath);

                }
            }

        }

        private void ObjectNameTB_TextChanged(object sender, EventArgs e)
        {
            ObjectNameTB.ForeColor = Color.Black;

        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            save();
        }



        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "All Files|*.*|XML Files|*.xml";
            dlg.FilterIndex = 2;
            string objn = " ";
            if (DialogResult.OK == dlg.ShowDialog())
            {
                for (int i = 0; i < Animations.Count; i++)
                {
                    for (int k = 0; k < Animations[i].Frames.Count; k++)
                    {
                        Animations[i].Frames.Clear();
                    }

                }
                Animations.Clear();
                AnimationsLB.Items.Clear();
                XElement xRoot = XElement.Load(dlg.FileName);

                XElement xAnimationCount = xRoot.Element("Animations");
                //XAttribute xAnimationcount = xAnimationCount.Attribute("AnimationCount");
                //int AnimationC = Convert.ToInt16(xAnimationcount);

                XElement xObject = xRoot.Element("Object");
                XAttribute xObjectName = xObject.Attribute("ObjectName");
                
                objn = Convert.ToString(xObjectName.Value);

                IEnumerable<XElement> xAnimations = xObject.Elements();
                currAnimation = 0;
                foreach (XElement xAnimation in xAnimations)
                {

                    Animation T = new Animation();
                    XAttribute xAnimname = xAnimation.Attribute("AnimName");
                    T.Name = Convert.ToString(xAnimname.Value);

                   

                    XAttribute xFrameC = xAnimation.Attribute("FrameCount");
                    int framecount = int.Parse(xFrameC.Value);

                    T.Frames = new List<Frame>();

                    Animations.Add(T);
                    AnimationsLB.Items.Add(Animations[currAnimation]);
                    IEnumerable<XElement> xFrames = xAnimation.Elements();
                    playFrame = 0;
                    playFrame = 0;
                    foreach (XElement xFrame in xFrames)
                    {

                        Frame temp = new Frame();

                        XAttribute xDrawLeft = xFrame.Attribute("DrawLeft");
                        temp.DrawX = int.Parse(xDrawLeft.Value);
                        XAttribute xDrawTop = xFrame.Attribute("DrawTop");
                        temp.DrawY = int.Parse(xDrawTop.Value);
                        XAttribute xDrawRight = xFrame.Attribute("DrawRight");
                        temp.DrawRight = int.Parse(xDrawRight.Value);
                        XAttribute xDrawBottom = xFrame.Attribute("DrawBottom");
                        temp.DrawBottom = int.Parse(xDrawBottom.Value);

                        XAttribute xCollisionLeft = xFrame.Attribute("CollisionLeft");
                        temp.CollisionX = int.Parse(xCollisionLeft.Value);
                        XAttribute xCollisionTop = xFrame.Attribute("CollisionTop");
                        temp.CollisionY = int.Parse(xCollisionTop.Value);
                        XAttribute xCollisionRight = xFrame.Attribute("CollisionRight");
                        temp.CollisionRight = int.Parse(xCollisionRight.Value);
                        XAttribute xCollisionBottom = xFrame.Attribute("CollisionBottom");
                        temp.CollisionBottom = int.Parse(xCollisionBottom.Value);

                        XAttribute xEventLeft = xFrame.Attribute("EventLeft");
                        temp.EventX = int.Parse(xEventLeft.Value);
                        XAttribute xEventTop = xFrame.Attribute("EventTop");
                        temp.EventY = int.Parse(xEventTop.Value);
                        XAttribute xEventRight = xFrame.Attribute("EventRight");
                        temp.EventRight = int.Parse(xEventRight.Value);
                        XAttribute xEventBottom = xFrame.Attribute("EventBottom");
                        temp.EventBottom = int.Parse(xEventBottom.Value);

                        XAttribute xAnchorLeft = xFrame.Attribute("AnchorLeft");
                        temp.AnchorX = int.Parse(xAnchorLeft.Value);
                        XAttribute xAnchorTop = xFrame.Attribute("AnchorTop");
                        temp.AnchorY = int.Parse(xAnchorTop.Value);

                        XAttribute xEvent = xFrame.Attribute("Event");
                        if (xEvent != null || xEvent.Value != "")
                        {
                            temp.Event = Convert.ToString(xEvent.Value);

                        }

                        XAttribute xDuration = xFrame.Attribute("Duration");
                        temp.duration = float.Parse(xDuration.Value);

                        Animations[currAnimation].Frames.Add(temp);

                        Rectangle Temp = new Rectangle();
                        Temp.X = LR.Count * 100;
                        Temp.Y = 0;
                        Temp.Width = 100;
                        Temp.Height = 100;
                        LR.Add(Temp);
                    }
                    currAnimation++;
                }

                currAnimation = 0;

            }
            if (objn != "")
            {
                currentBM = new Bitmap(objn);
                currentTexture = TM.LoadTexture(objn);
                BM = objn;
                FramePanel.AutoScrollMinSize = new Size(TM.GetTextureWidth(currentTexture), TM.GetTextureHeight(currentTexture));
            }
            FDurationUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].duration;
            if (currTool == 0 && Animations[currAnimation].Frames[playFrame].DrawX > 0)
            {
                comboBox1.Text = Animations[currAnimation].Frames[playFrame].Event;
                XUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].DrawX;
                YUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].DrawY;
                WidthUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].DrawRight;
                HeightUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].DrawBottom;
            }
            if (currTool == 1 && Animations[currAnimation].Frames[playFrame].CollisionX > 0)
            {
                comboBox1.Text = Animations[currAnimation].Frames[playFrame].Event;
                XUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].CollisionX;
                YUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].CollisionY;
                WidthUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].CollisionRight;
                HeightUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].CollisionBottom;
            }
            if (currTool == 2 && Animations[currAnimation].Frames[playFrame].AnchorX > 0)
            {
                comboBox1.Text = Animations[currAnimation].Frames[playFrame].Event;
                XUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].AnchorX;
                YUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].AnchorY;
            }
            if (currTool == 3 && Animations[currAnimation].Frames[playFrame].EventX > 0)
            {

                comboBox1.Text = Animations[currAnimation].Frames[playFrame].Event;
                XUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].EventX;
                YUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].EventY;
                WidthUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].EventRight;
                HeightUD.Value = (decimal)Animations[currAnimation].Frames[playFrame].EventBottom;
            }
            Animname.Text = Animations[currAnimation].Name;
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            Animations[currAnimation].Frames[playFrame].Event = comboBox1.Text;
        }

        private void comboBox1_TextChanged(object sender, EventArgs e)
        {
            Animations[currAnimation].Frames[playFrame].Event = comboBox1.Text;
        }



    }
}
