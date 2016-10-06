using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;
using System.Xml.Serialization;
using System.Xml.Linq;
using System.Xml; 

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        int currentTexture = -1; 
        SGP.CSGP_Direct3D D3D = SGP.CSGP_Direct3D.GetInstance();
        
        SGP.CSGP_TextureManager TM = SGP.CSGP_TextureManager.GetInstance();

        Stopwatch stopWatch = new Stopwatch();
        //float timer;

        char[] tempParticleName; 
        char[] ParticleTextureName;
        Bitmap image; 
        Emitter m_eParticleEmitter = new Emitter();
        Flyweight m_fwFlyweight = new Flyweight();
        List<Particle> m_prtParticleList; 

        public Form1()
        {
            InitializeComponent();
            this.Text = "Nightengale Particle Editor"; 
            D3D.Initialize(panel1, true);

            D3D.AddRenderTarget(panel2);

            TM.Initialize(D3D.Device, D3D.Sprite);

            //Initialize emitter's text boxes
            textBoxEmitHeight.Text = "1";
            textBoxEmitSpawnRate.Text = "0.25";
            textBoxEmitWidth.Text = "1";
            textBoxEmitXPos.Text = "150";
            textBoxEmitYPos.Text = "250";

            //Initialize flyweight textbox properties
            textBoxEndingImageScale.Text = "1";
            textBoxMaxLife.Text = "5";
            textBoxMinLife.Text = "3";
            textBoxNumParticles.Text = "3000";
            textBoxRotationRateofChange.Text = "1";
            textBoxRotationStart.Text = "1";
            textBoxStartingImageScale.Text = "1";
            textBoxEndingImageScale.Text = "1"; 
            textBoxStartingHeight.Text = "10";
            textBoxStartingWidth.Text = "10";


            //Randomize Velocities
            TextboxStartingXVel.Text = "10.0";
            textBoxStartingYVel.Text = "10.0";

            textBoxVelRateofChange.Text = "1";
             
            m_eParticleEmitter.IsLooping = false; 
            m_eParticleEmitter.EmitterPosX = float.Parse(textBoxEmitXPos.Text);
            m_eParticleEmitter.EmitterPosY = float.Parse(textBoxEmitYPos.Text);
            m_eParticleEmitter.EmitterWidth = float.Parse(textBoxEmitWidth.Text);
            m_eParticleEmitter.EmitterHeight = float.Parse(textBoxEmitHeight.Text); 
            m_eParticleEmitter.MaxNumParticles = int.Parse(textBoxNumParticles.Text);
            m_eParticleEmitter.SpawnRate = float.Parse(textBoxEmitSpawnRate.Text);

            m_fwFlyweight.MaxLifeSpan = float.Parse(textBoxMaxLife.Text);
            m_fwFlyweight.MinLifeSpan = float.Parse(textBoxMinLife.Text);
            m_fwFlyweight.RotationRateOfChange = float.Parse(textBoxRotationRateofChange.Text);
            m_fwFlyweight.StartingHeight = float.Parse(textBoxStartingHeight.Text);
            m_fwFlyweight.StartingWidth = float.Parse(textBoxStartingWidth.Text);
            m_fwFlyweight.StartingRotation = float.Parse(textBoxRotationStart.Text);
            m_fwFlyweight.StartingScale = float.Parse(textBoxStartingImageScale.Text);
            m_fwFlyweight.VelocityRateOfChange = float.Parse(textBoxVelRateofChange.Text);
            m_fwFlyweight.VelocityX = float.Parse(TextboxStartingXVel.Text);
            m_fwFlyweight.VelocityY = float.Parse(textBoxStartingYVel.Text);
            m_fwFlyweight.StartingColor = Color.White;
            m_fwFlyweight.EndingColor = Color.White; 


            m_prtParticleList = new List<Particle>(); 

           // timer = 0.0f;

            //OpenFileDialog dlg = new OpenFileDialog();
            //// Message
            //if (DialogResult.OK == dlg.ShowDialog())
            //{
            //    currentTexture = TM.LoadTexture(dlg.FileName);
            //}
            //else
            //{
            //    currentTexture = TM.LoadTexture("..\\..\\particle.bmp");
            //}

            comboBoxEmitShape.DataSource = Enum.GetValues(typeof(EmitterShape));

            comboBoxEmitShape.SelectedItem = 1; 
            stopWatch.Start();

            // panel1.AutoScrollMinSize = new Size(TM.GetTextureWidth(currentTexture), TM.GetTextureHeight(currentTexture));
            m_eParticleEmitter.CurrNumParticles = 0; 
            //panel1.AutoScrollMinSize.Height = 
        }

    bool looping = true;

    public bool Looping
        {
            get { return looping; }
            set { looping = value; }
        } 

    public new void Update()
        {
            //Update all parameters
            m_eParticleEmitter.SpawnRate -= (float)stopWatch.ElapsedMilliseconds / 1000;
            if (m_eParticleEmitter.SpawnRate <= 0 && m_eParticleEmitter.MaxNumParticles > m_eParticleEmitter.CurrNumParticles && currentTexture != -1) //Create a new particle
            {
                Particle pa = new Particle(); 

                //New particle's velocity
                Random randVelX = new Random();
                Random randVelY = new Random(); 

                pa.CurrVelocityX = (float)randVelX.NextDouble()*100;
                pa.CurrVelocityY = (float)randVelY.NextDouble() * 100;

                //New particle's randomly generated lifespan
                Random randLifeSpan = new Random(); 
                pa.CurrLifeSpan = (float)randLifeSpan.Next((int)m_fwFlyweight.MinLifeSpan, (int)m_fwFlyweight.MaxLifeSpan);

                //New particle's size
                pa.CurrParticleWidth = m_fwFlyweight.StartingWidth;
                pa.CurrParticleHeight = m_fwFlyweight.StartingHeight;

                //New particle's rotation
                pa.ParticleCurrRotation = m_fwFlyweight.StartingRotation; 

                //New particle's position
                pa.ParticleCurrPosX = m_eParticleEmitter.EmitterPosX;
                pa.ParticleCurrPosY = m_eParticleEmitter.EmitterPosY;

                //New particle's size scale
                pa.CurrXScaleValue = m_fwFlyweight.StartingScale; 
                pa.CurrYScaleValue = m_fwFlyweight.StartingScale; 
                pa.CurrScale = m_fwFlyweight.StartingScale; 
                float scaleIntervals = pa.CurrLifeSpan/0.1f;
                pa.ScaleIntervals = (int)scaleIntervals;
                pa.ChangeInScale = (m_fwFlyweight.EndingScale - m_fwFlyweight.StartingScale) / pa.ScaleIntervals; 



                //New particle's ARGB info
                pa.CurrARGB = m_fwFlyweight.StartARGB;
                float tempInterval = pa.CurrLifeSpan / 0.1f;
                pa.ColorIntervals = (int)tempInterval ;
                pa.ChangeInColorARGB = (m_fwFlyweight.EndARGB - m_fwFlyweight.StartARGB) / pa.ColorIntervals; 




                //Add the new particle to the list
                m_prtParticleList.Add(pa);

                m_eParticleEmitter.CurrNumParticles++; 
                m_eParticleEmitter.SpawnRate = float.Parse(textBoxEmitSpawnRate.Text);
            }

            for (int CurrentParticle = 0; CurrentParticle < m_prtParticleList.Count(); CurrentParticle++)
            {
                //Update the lifespans
                //float elapsedTime = 
                m_prtParticleList[CurrentParticle].CurrLifeSpan -= ((float)stopWatch.ElapsedMilliseconds / 1000000);

                //If the particle is still alive
                if (m_prtParticleList[CurrentParticle].CurrLifeSpan > 0)
                {
                    //Update the velocities
                    m_prtParticleList[CurrentParticle].CurrVelocityX *= m_fwFlyweight.VelocityRateOfChange;
                    m_prtParticleList[CurrentParticle].CurrVelocityY *= m_fwFlyweight.VelocityRateOfChange;

                    //Update the position based on the velocity
                    m_prtParticleList[CurrentParticle].ParticleCurrPosX += m_prtParticleList[CurrentParticle].CurrVelocityX * ((float)stopWatch.ElapsedMilliseconds / 1000000);
                    m_prtParticleList[CurrentParticle].ParticleCurrPosY += m_prtParticleList[CurrentParticle].CurrVelocityY * ((float)stopWatch.ElapsedMilliseconds / 1000000);

                    //Update the rotation
                    m_prtParticleList[CurrentParticle].ParticleCurrRotation += m_fwFlyweight.RotationRateOfChange * ((float)stopWatch.ElapsedMilliseconds / 1000000);

                    //Update the color ARGB
                    m_prtParticleList[CurrentParticle].CurrARGB += (int)(m_prtParticleList[CurrentParticle].ChangeInColorARGB); //* ((float)stopWatch.ElapsedMilliseconds / 1000000));

                    //Update the scale
                    m_prtParticleList[CurrentParticle].CurrScale += m_prtParticleList[CurrentParticle].ChangeInScale;
                    m_prtParticleList[CurrentParticle].CurrXScaleValue += m_prtParticleList[CurrentParticle].ChangeInScale * ((float)stopWatch.ElapsedMilliseconds / 1000000);
                    m_prtParticleList[CurrentParticle].CurrYScaleValue += m_prtParticleList[CurrentParticle].ChangeInScale * ((float)stopWatch.ElapsedMilliseconds / 1000000);

                }
                else if (m_prtParticleList[CurrentParticle].CurrLifeSpan <= 0)//Particle is dead
                {
                    m_prtParticleList.RemoveAt(CurrentParticle);

                    if (m_prtParticleList.Count() == 0 && m_eParticleEmitter.CurrNumParticles == m_eParticleEmitter.MaxNumParticles)//Emitter is dead
                    {
                        //Check to see if it should loop
                        if (true == m_eParticleEmitter.IsLooping)
                            m_eParticleEmitter.CurrNumParticles = 0;
                        else
                            m_eParticleEmitter.SpawnRate = -1; 
                    }
                }
            }
    
        }

    public void Render()
        {

            D3D.Clear(panel1, Color.Black); 

            D3D.DeviceBegin();
            D3D.SpriteBegin();

            //Draw the emitter shape 
            //Draw the emitter as a point
            if (m_eParticleEmitter.Shape == EmitterShape.Point)
            {
                D3D.DrawRect(new Rectangle((int)m_eParticleEmitter.EmitterPosX, (int)m_eParticleEmitter.EmitterPosY, 1, 1), Color.White);
            }
            else if (m_eParticleEmitter.Shape == EmitterShape.Rectangle)
            {
                D3D.DrawRect(new Rectangle((int)m_eParticleEmitter.EmitterPosX, (int)m_eParticleEmitter.EmitterPosY, (int)m_eParticleEmitter.EmitterWidth, (int)m_eParticleEmitter.EmitterHeight), Color.White);
            }
            else if (m_eParticleEmitter.Shape == EmitterShape.Line)
            {
                D3D.DrawLine((int)m_eParticleEmitter.EmitterPosX, (int)m_eParticleEmitter.EmitterPosY, 
                    (int)m_eParticleEmitter.EmitterPosX +(int)m_eParticleEmitter.EmitterWidth, 
                    (int)m_eParticleEmitter.EmitterPosY + (int)m_eParticleEmitter.EmitterHeight, Color.White, 5);
            }

            //D3D.DrawRect(new Rectangle(20, 20, 10, 10), Color.AliceBlue);
           // D3D.DrawText("Hello World", 10, 10, Color.Peru);

            //TM.Draw(currentTexture, panel1.AutoScrollPosition.X, panel1.AutoScrollPosition.Y); 
            if (currentTexture != -1) //Make sure a texture is set
            {
                for (int i = 0; i < m_prtParticleList.Count(); i++)
                {
                    Color col = Color.FromArgb(m_prtParticleList[i].CurrARGB);

                    TM.Draw(currentTexture, (int)m_prtParticleList[i].ParticleCurrPosX, (int)m_prtParticleList[i].ParticleCurrPosY,
                        m_prtParticleList[i].CurrXScaleValue, m_prtParticleList[i].CurrYScaleValue, Rectangle.Empty, m_prtParticleList[i].CurrParticleWidth / 2, m_prtParticleList[i].CurrParticleHeight / 2,
                        m_prtParticleList[i].ParticleCurrRotation, col);

                } 
            }
            D3D.SpriteEnd();
            D3D.DeviceEnd();

            D3D.Present(); 



            //panel 2
            D3D.Clear(panel2, Color.Black);

            D3D.DeviceBegin();
            D3D.SpriteBegin();

           // D3D.DrawRect(new Rectangle(20, 20, 10, 10), Color.AliceBlue);
           // D3D.DrawText("Hello World", 10, 10, Color.IndianRed);

            if (currentTexture != -1)
            {
                TM.Draw(currentTexture, 0, 0 /* panel1.AutoScrollPosition.X, panel1.AutoScrollPosition.Y*/);  
            }
            D3D.SpriteEnd();
            D3D.DeviceEnd();

            D3D.Present(); 
        }





    private void Form1_FormClosing(object sender, EventArgs e)
        {
            looping = false; 
        }

    private void Form1_Load(object sender, EventArgs e)
        {

        }

    private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            looping = false; 
        }

    private void fIleToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

    private void panel1_Resize(object sender, EventArgs e)
        {

        }

    private void label1_Click(object sender, EventArgs e)
        {

        }

    private void groupBox3_Enter(object sender, EventArgs e)
        {

        }

    private void button1_Click(object sender, EventArgs e)
        {
            ColorDialog dlg = new ColorDialog();
            dlg.Color = m_fwFlyweight.StartingColor;

            if (DialogResult.OK == dlg.ShowDialog())
            {
                m_fwFlyweight.StartingColor = dlg.Color;
            }
            m_fwFlyweight.StartARGB = dlg.Color.ToArgb(); 

        }

    private void EndingColor_Click(object sender, EventArgs e)
        {
            ColorDialog dlg = new ColorDialog();

            dlg.Color = m_fwFlyweight.EndingColor; 

            if (DialogResult.OK == dlg.ShowDialog())
	        {
                m_fwFlyweight.EndingColor = dlg.Color; 
	        }
            m_fwFlyweight.EndARGB = dlg.Color.ToArgb(); 
        }

    private void textureImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();

            if (DialogResult.OK == dlg.ShowDialog())
            {
                currentTexture = TM.LoadTexture(dlg.FileName);
                image = new Bitmap(dlg.FileName); 
                tempParticleName = dlg.FileName.ToCharArray(); // Application.StartupPath;  
                int lastBackslash = 0;
	            int characterCount = 0;
                for (int i = 0; i < tempParticleName.Length; i++)
	            {
                    if (tempParticleName[i] == '\\' || tempParticleName[i] == '/')
	            		lastBackslash = i + 1; 
                
	            	characterCount++; 
	            }
                ParticleTextureName = new char[characterCount - lastBackslash];
               for (int i = lastBackslash; i < characterCount; i++)
                   ParticleTextureName[i - lastBackslash] = tempParticleName[i]; 

                image = new Bitmap(dlg.FileName); 
            }

        }

    private void comboBoxEmitShape_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

    private void comboBoxEmitShape_Click(object sender, EventArgs e)
        {
            comboBoxEmitShape.DataSource = Enum.GetValues(typeof(EmitterShape));
        }

    private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            currentTexture = -1;
            textBoxMaxLife.Text = " ";
            
        }

    private void textBoxEmitXPos_Leave(object sender, EventArgs e)
        {
            m_eParticleEmitter.EmitterPosX = float.Parse(textBoxEmitXPos.Text);
        }

    private void textBoxEmitXPos_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar >= '0' && e.KeyChar <= '9' || e.KeyChar =='.' )
            {
                e.Handled = false;
            }
            else
                e.Handled = true; 
        }

    private void textBoxEmitYPos_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar >= '0' && e.KeyChar <= '9' || e.KeyChar == '.')
            {
                e.Handled = false;
            }
            else
                e.Handled = true; 
        }

    private void textBoxEmitWidth_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar >= '0' && e.KeyChar <= '9' || e.KeyChar == '.')
            {
                e.Handled = false;
            }
            else
                e.Handled = true; 
        }

    private void textBoxEmitHeight_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar >= '0' && e.KeyChar <= '9' || e.KeyChar == '.')
            {
                e.Handled = false;
            }
            else
                e.Handled = true; 
        }

    private void TextboxStartingXVel_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar >= '0' && e.KeyChar <= '9' || e.KeyChar == '.')
            {
                e.Handled = false;
            }
            else
                e.Handled = true; 
        }

    private void textBoxStartingYVel_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar >= '0' && e.KeyChar <= '9' || e.KeyChar == '.')
            {
                e.Handled = false;
            }
            else
                e.Handled = true; 
        }

    private void textBoxVelRateofChange_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar >= '0' && e.KeyChar <= '9' || e.KeyChar == '.')
            {
                e.Handled = false;
            }
            else
                e.Handled = true; 
        }

    private void textBoxMinLife_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar >= '0' && e.KeyChar <= '9' || e.KeyChar == '.')
            {
                e.Handled = false;
            }
            else
                e.Handled = true; 
        }

    private void textBoxMaxLife_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar >= '0' && e.KeyChar <= '9' || e.KeyChar == '.')
            {
                e.Handled = false;
            }
            else
                e.Handled = true; 
        }

    private void textBoxNumParticles_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar >= '0' && e.KeyChar <= '9')
            {
                e.Handled = false;
            }
            else
                e.Handled = true; 
        }

    private void textBoxRotationStart_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar >= '0' && e.KeyChar <= '9' || e.KeyChar == '.')
            {
                e.Handled = false;
            }
            else
                e.Handled = true; 
        }

    private void textBoxRotationRateofChange_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar >= '0' && e.KeyChar <= '9' || e.KeyChar == '.')
            {
                e.Handled = false;
            }
            else
                e.Handled = true; 
        }

    private void textBoxEmitYPos_Leave(object sender, EventArgs e)
        {
            m_eParticleEmitter.EmitterPosY = float.Parse(textBoxEmitYPos.Text);
        }

    private void textBoxEmitWidth_Leave(object sender, EventArgs e)
        {
            m_eParticleEmitter.EmitterWidth = float.Parse(textBoxEmitWidth.Text); 
        }

    private void textBoxEmitHeight_Leave(object sender, EventArgs e)
        {
            m_eParticleEmitter.EmitterHeight = float.Parse(textBoxEmitHeight.Text); 
        }

    private void TextboxStartingXVel_Leave(object sender, EventArgs e)
        {
            m_fwFlyweight.VelocityX = float.Parse(TextboxStartingXVel.Text);
            for (int CurrentParticle = 0; CurrentParticle < m_prtParticleList.Count(); CurrentParticle++)
            {
                if (m_prtParticleList[CurrentParticle].CurrLifeSpan > 0)
                {
                    m_prtParticleList[CurrentParticle].CurrVelocityX = float.Parse(TextboxStartingXVel.Text);
                }
            }
        }

    private void textBoxStartingYVel_Leave(object sender, EventArgs e)
        {
            m_fwFlyweight.VelocityY = float.Parse(textBoxEmitYPos.Text);
            for (int CurrentParticle = 0; CurrentParticle < m_prtParticleList.Count(); CurrentParticle++)
            {
                if (m_prtParticleList[CurrentParticle].CurrLifeSpan > 0)
                {
                    m_prtParticleList[CurrentParticle].CurrVelocityY = float.Parse(textBoxStartingYVel.Text);
                }
            }
        }

    private void textBoxVelRateofChange_Leave(object sender, EventArgs e)
    {
        m_fwFlyweight.VelocityRateOfChange = float.Parse(textBoxVelRateofChange.Text); 
    }

    private void textBoxMinLife_Leave(object sender, EventArgs e)
    {
        m_fwFlyweight.MinLifeSpan = float.Parse(textBoxMinLife.Text); 
    }

    private void textBoxMaxLife_Leave(object sender, EventArgs e)
    {
        m_fwFlyweight.MaxLifeSpan = float.Parse(textBoxMaxLife.Text); 
    }

    private void textBoxNumParticles_Leave(object sender, EventArgs e)
    {
        m_eParticleEmitter.MaxNumParticles = int.Parse(textBoxNumParticles.Text);
    }

    private void textBoxRotationStart_Leave(object sender, EventArgs e)
    {
        m_fwFlyweight.StartingRotation = float.Parse(textBoxRotationStart.Text); 
    }

    private void textBoxRotationRateofChange_Leave(object sender, EventArgs e)
    {
        m_fwFlyweight.RotationRateOfChange = float.Parse(textBoxRotationRateofChange.Text);
    }

    private void label18_Click(object sender, EventArgs e)
        {

        }

    private void textBoxEndingImageScale_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar >= '0' && e.KeyChar <= '9' || e.KeyChar == '.')
            {
                e.Handled = false;
            }
            else
                e.Handled = true; 
        }

    private void textBoxStartingImageScale_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar >= '0' && e.KeyChar <= '9' || e.KeyChar == '.')
            {
                e.Handled = false;
            }
            else
                e.Handled = true; 
        }

    private void textBoxStartingImageScale_Leave(object sender, EventArgs e)
        {
            m_fwFlyweight.StartingScale = float.Parse(textBoxStartingImageScale.Text);
            for (int i = 0; i < m_prtParticleList.Count(); i++)
            {
                m_prtParticleList[i].ChangeInScale = (m_fwFlyweight.EndingScale - m_fwFlyweight.StartingScale) / m_prtParticleList[i].ScaleIntervals;
            }
        }

    private void textBoxEndingImageScale_Leave(object sender, EventArgs e)
        {
            m_fwFlyweight.EndingScale = float.Parse(textBoxEndingImageScale.Text);
            for (int i = 0; i < m_prtParticleList.Count(); i++)
            {
                m_prtParticleList[i].ChangeInScale = (m_fwFlyweight.EndingScale - m_fwFlyweight.StartingScale) / m_prtParticleList[i].ScaleIntervals;
            }
        }

    private void textBoxStartingParticleWidth_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar >= '0' && e.KeyChar <= '9' || e.KeyChar == '.')
            {
                e.Handled = false;
            }
            else
                e.Handled = true; 
        }

    private void textBoxStartingParticleHeight_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar >= '0' && e.KeyChar <= '9' || e.KeyChar == '.')
            {
                e.Handled = false;
            }
            else
                e.Handled = true; 
        }

    private void textBoxStartingParticleWidth_Leave(object sender, EventArgs e)
        {
           // m_fwFlyweight.StartingWidth = float.Parse(textBoxStartingParticleWidth.Text);
        }

    private void textBoxStartingParticleHeight_Leave(object sender, EventArgs e)
        {
           // 
        }

    private void textBoxEmitSpawnRate_KeyDown(object sender, KeyEventArgs e)
        {

        }

    private void textBoxEmitSpawnRate_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar >= '0' && e.KeyChar <= '9' || e.KeyChar == '.')
            {
                e.Handled = false;
            }
            else
                e.Handled = true; 
        }

    private void textBoxEmitSpawnRate_Leave(object sender, EventArgs e)
        {
            m_eParticleEmitter.SpawnRate = float.Parse(textBoxEmitSpawnRate.Text); 
        }

    private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            m_eParticleEmitter.IsLooping = !m_eParticleEmitter.IsLooping;
        }

    private void label8_Click(object sender, EventArgs e)
        {

        }

    private void minLifespanToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Random rand = new Random(); 
             m_fwFlyweight.MinLifeSpan = rand.Next(1, (int)m_fwFlyweight.MaxLifeSpan - 1);
        }

    private void maxLifespanToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Random rand = new Random();
            m_fwFlyweight.MaxLifeSpan = rand.Next((int)m_fwFlyweight.MinLifeSpan + 1, (int)m_fwFlyweight.MinLifeSpan * 20);
        }

    private void textBoxStartingHeight_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar >= '0' && e.KeyChar <= '9' || e.KeyChar == '.')
            {
                e.Handled = false;
            }
            else
                e.Handled = true; 
        }

    private void textBoxStartingWidth_Leave(object sender, EventArgs e)
        {
            m_fwFlyweight.StartingWidth = float.Parse(textBoxStartingWidth.Text);

        }

    private void textBoxStartingWidth_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar >= '0' && e.KeyChar <= '9' || e.KeyChar == '.')
            {
                e.Handled = false;
            }
            else
                e.Handled = true; 
        }

    private void textBoxStartingHeight_Leave(object sender, EventArgs e)
        {
            m_fwFlyweight.StartingHeight = float.Parse(textBoxStartingHeight.Text);
        }

    private void spawnOnCircleToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_eParticleEmitter.Shape = EmitterShape.Circle;
            comboBoxEmitShape.SelectedItem = 2; 
        }

    private void pointToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_eParticleEmitter.Shape = EmitterShape.Point;
            comboBoxEmitShape.SelectedItem = 1; 
        }

    private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
    {

    }

    private void openToolStripMenuItem_Click(object sender, EventArgs e)
    {

    }

    private void xmlImageToolStripMenuItem_Click(object sender, EventArgs e)
    {

    }

    private void panel1_MouseDown(object sender, MouseEventArgs e)
    {
        m_eParticleEmitter.EmitterPosX = e.X;
        m_eParticleEmitter.EmitterPosY = e.Y; 
    }

    private void panel1_MouseMove(object sender, MouseEventArgs e)
        {
            m_eParticleEmitter.EmitterPosX = e.X;
            m_eParticleEmitter.EmitterPosY = e.Y;
            textBoxEmitXPos.Text = m_eParticleEmitter.EmitterPosX.ToString();
            textBoxEmitYPos.Text = m_eParticleEmitter.EmitterPosY.ToString(); 
        }

    private void selectFolderToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dlg = new FolderBrowserDialog();
            if (DialogResult.OK == dlg.ShowDialog())
            {
                string xml = dlg.SelectedPath + "\\ParticleList.xml";
                string particleImage = dlg.SelectedPath + "\\ParticleImage.png";

                SavingXML().Save(xml);
                if (false == System.IO.File.Exists(particleImage))
                     image.Save(particleImage);     
            }         
        }

    private void relativePathToolStripMenuItem_Click(object sender, EventArgs e)
    {
            string xmlRelative = Application.StartupPath + "\\ParticleList.xml";
            string imageRelative = Application.StartupPath + "\\ParticleImage.png";
            SavingXML().Save(xmlRelative);
            if (false == System.IO.File.Exists(imageRelative))
                image.Save(imageRelative);           
               
    }

        private void selectFolderToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            if (DialogResult.OK == dlg.ShowDialog())
                 LoadXML(dlg.FileName); 
        }


        private XmlDocument SavingXML()
        {
            XmlDocument doc = new XmlDocument();
            XmlElement root = doc.CreateElement("ParticleManager_Info");
            doc.PrependChild(root);

            XmlElement colorEle = doc.CreateElement("particle_color_values");

            //Starting colors
            XmlAttribute attribute = doc.CreateAttribute("startalpha");
            attribute.Value = m_fwFlyweight.StartingColor.A.ToString();
            colorEle.Attributes.Append(attribute);

            attribute = doc.CreateAttribute("startred");
            attribute.Value = m_fwFlyweight.StartingColor.R.ToString();
            colorEle.Attributes.Append(attribute);

            attribute = doc.CreateAttribute("startgreen");
            attribute.Value = m_fwFlyweight.StartingColor.G.ToString();
            colorEle.Attributes.Append(attribute);

            attribute = doc.CreateAttribute("startblue");
            attribute.Value = m_fwFlyweight.StartingColor.B.ToString();
            colorEle.Attributes.Append(attribute);

            //Ending colors
            attribute = doc.CreateAttribute("endalpha");
            attribute.Value = m_fwFlyweight.EndingColor.A.ToString();
            colorEle.Attributes.Append(attribute);

            attribute = doc.CreateAttribute("endred");
            attribute.Value = m_fwFlyweight.EndingColor.R.ToString();
            colorEle.Attributes.Append(attribute);

            attribute = doc.CreateAttribute("endgreen");
            attribute.Value = m_fwFlyweight.EndingColor.G.ToString();
            colorEle.Attributes.Append(attribute);

            attribute = doc.CreateAttribute("endblue");
            attribute.Value = m_fwFlyweight.EndingColor.B.ToString();
            colorEle.Attributes.Append(attribute);

            root.PrependChild(colorEle);

            //Flyweight information
            XmlElement partEle = doc.CreateElement("particle_values");

            attribute = doc.CreateAttribute("maxlifespan");
            attribute.Value = m_fwFlyweight.MaxLifeSpan.ToString();
            partEle.Attributes.Append(attribute);

            attribute = doc.CreateAttribute("minlifespan");
            attribute.Value = m_fwFlyweight.MinLifeSpan.ToString();
            partEle.Attributes.Append(attribute);

            attribute = doc.CreateAttribute("startrotation");
            attribute.Value = m_fwFlyweight.StartingRotation.ToString();
            partEle.Attributes.Append(attribute);

            attribute = doc.CreateAttribute("rotationrateofchange");
            attribute.Value = m_fwFlyweight.RotationRateOfChange.ToString();
            partEle.Attributes.Append(attribute);

            attribute = doc.CreateAttribute("startvelx");
            attribute.Value = m_fwFlyweight.VelocityX.ToString();
            partEle.Attributes.Append(attribute);

            attribute = doc.CreateAttribute("startvely");
            attribute.Value = m_fwFlyweight.VelocityY.ToString();
            partEle.Attributes.Append(attribute);

            attribute = doc.CreateAttribute("changevel");
            attribute.Value = m_fwFlyweight.VelocityRateOfChange.ToString();
            partEle.Attributes.Append(attribute);

            attribute = doc.CreateAttribute("width");
            attribute.Value = m_fwFlyweight.StartingWidth.ToString();
            partEle.Attributes.Append(attribute);

            attribute = doc.CreateAttribute("height");
            attribute.Value = m_fwFlyweight.StartingHeight.ToString();
            partEle.Attributes.Append(attribute);

            attribute = doc.CreateAttribute("sizerateofchange");
            attribute.Value = m_fwFlyweight.EndingScale.ToString();
            partEle.Attributes.Append(attribute);

            root.PrependChild(partEle);

            //Emitter informaiton
            XmlElement emitEle = doc.CreateElement("Emit_values");

            attribute = doc.CreateAttribute("maxnumparticles");
            attribute.Value = m_eParticleEmitter.MaxNumParticles.ToString();
            emitEle.Attributes.Append(attribute);

            attribute = doc.CreateAttribute("shape");
            int shapeInt = 0;
            if (m_eParticleEmitter.Shape == EmitterShape.Point)
                shapeInt = 0;
            else if (m_eParticleEmitter.Shape == EmitterShape.Rectangle)
                shapeInt = 1;
            else if (m_eParticleEmitter.Shape == EmitterShape.Line)
                shapeInt = 2;
            //else if (m_eParticleEmitter.Shape == EmitterShape.Circle)
            //    shapeInt = 3; 

            attribute.Value = shapeInt.ToString();
            emitEle.Attributes.Append(attribute);

            attribute = doc.CreateAttribute("emitx");
            attribute.Value = m_eParticleEmitter.EmitterPosX.ToString();
            emitEle.Attributes.Append(attribute);

            attribute = doc.CreateAttribute("emity");
            attribute.Value = m_eParticleEmitter.EmitterPosY.ToString();
            emitEle.Attributes.Append(attribute);

            attribute = doc.CreateAttribute("emitwidth");
            attribute.Value = m_eParticleEmitter.EmitterWidth.ToString();
            emitEle.Attributes.Append(attribute);

            attribute = doc.CreateAttribute("emitheight");
            attribute.Value = m_eParticleEmitter.EmitterHeight.ToString();
            emitEle.Attributes.Append(attribute);

            attribute = doc.CreateAttribute("emitlifespan");
            attribute.Value = m_eParticleEmitter.EmitterLifespan.ToString();
            emitEle.Attributes.Append(attribute);

            int tempLoop = 0;
            if (m_eParticleEmitter.IsLooping == true )
                tempLoop = 1;
            else if (m_eParticleEmitter.IsLooping == false)
               tempLoop = 0; 

            attribute = doc.CreateAttribute("emitlooping");
            attribute.Value = tempLoop.ToString();
            emitEle.Attributes.Append(attribute);


            attribute = doc.CreateAttribute("emitspawnrate");
            attribute.Value = textBoxEmitSpawnRate.Text; //m_eParticleEmitter.SpawnRate.ToString();
            emitEle.Attributes.Append(attribute);

            root.PrependChild(emitEle);

            //Texture information
            XmlElement currChild = doc.CreateElement("Texture_Name");

            attribute = doc.CreateAttribute("Name");
            string imgname = "ParticleImage.png";
            attribute.Value = imgname.ToString();

            currChild.Attributes.Append(attribute);
            root.PrependChild(currChild);

            return doc;
        }

        private void relativePathToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            string xmlRelative = Application.StartupPath + "\\ParticleList.xml";
            string imageRelative = Application.StartupPath + "\\ParticleImage.png";
            LoadXML(xmlRelative); 
        }

        private void LoadXML(string path)
        {
                XmlDocument doc = new XmlDocument();
                doc.Load(path);
                XmlElement root = doc.GetElementById("ParticleManager_Info");

                XmlNode info = doc.FirstChild.FirstChild;

                //Get the name
                string tempName = info.Attributes["Name"].Value;
                info = info.NextSibling;
           
                //Remove the xml doc name from the filepath

                int lastBackslash = 0;
                int characterCount = 0;
                char[] tempFilename = path.ToCharArray();

                for (int i = 0; i < tempFilename.Length; i++)
                {
                    if (tempFilename[i] == '\\' || tempFilename[i] == '/')
                        lastBackslash = i + 1;

                    characterCount++;
                }
                char[] filename = new char[lastBackslash];
                for (int i = 0; i < lastBackslash; i++)
                    filename[i] = tempFilename[i];

                string imgName = new string(filename); // +tempName; 
                imgName += tempName;



                currentTexture = TM.LoadTexture(imgName);
                image = new Bitmap(imgName); 

                //Get the emitter info 
                int tempPartNum = int.Parse(info.Attributes["maxnumparticles"].Value);
                string tempShape = info.Attributes["shape"].Value;

                float tempEmitX = float.Parse(info.Attributes["emitx"].Value);
                float tempEmitY = float.Parse(info.Attributes["emity"].Value);

                float tempEmitWidth = float.Parse(info.Attributes["emitwidth"].Value);
                float tempEmitHeight = float.Parse(info.Attributes["emitheight"].Value);

                float tempEmitLifespan = float.Parse(info.Attributes["emitlifespan"].Value);
                string tempIsLooping = info.Attributes["emitlooping"].Value;

                float tempSpawnRate = float.Parse(info.Attributes["emitspawnrate"].Value);

                //Particle info 
                info = info.NextSibling;
                float tempMaxLife = float.Parse(info.Attributes["maxlifespan"].Value);
                float tempMinLife = float.Parse(info.Attributes["minlifespan"].Value);

                float tempRotationStart = float.Parse(info.Attributes["startrotation"].Value);
                float tempRotationRoC = float.Parse(info.Attributes["rotationrateofchange"].Value);

                float tempStartVelX = float.Parse(info.Attributes["startvelx"].Value);
                float tempStartVelY = float.Parse(info.Attributes["startvely"].Value);

                float tempChangeVel = float.Parse(info.Attributes["changevel"].Value);

                float tempPartWidth = float.Parse(info.Attributes["width"].Value);
                float tempPartHeight = float.Parse(info.Attributes["height"].Value);

                float tempSizeRoC = float.Parse(info.Attributes["sizerateofchange"].Value);

                //Particle color info 
                info = info.NextSibling;
                int tempStartA = int.Parse(info.Attributes["startalpha"].Value);
                int tempStartR = int.Parse(info.Attributes["startred"].Value);
                int tempStartG = int.Parse(info.Attributes["startgreen"].Value);
                int tempStartB = int.Parse(info.Attributes["startblue"].Value);

                int tempEndA = int.Parse(info.Attributes["endalpha"].Value);
                int tempEndR = int.Parse(info.Attributes["endred"].Value);
                int tempEndG = int.Parse(info.Attributes["endgreen"].Value);
                int tempEndB = int.Parse(info.Attributes["endblue"].Value);

                //Flyweight 
                m_fwFlyweight.StartingColor = Color.FromArgb(tempStartA, tempStartR, tempStartG, tempStartB);
                m_fwFlyweight.EndingColor = Color.FromArgb(tempEndA, tempEndR, tempEndG, tempEndB);

                m_fwFlyweight.MaxLifeSpan = tempMaxLife;
                m_fwFlyweight.MinLifeSpan = tempMinLife;

                m_fwFlyweight.RotationRateOfChange = tempRotationRoC;
                m_fwFlyweight.StartingRotation = tempRotationStart;

                m_fwFlyweight.EndingScale = tempSizeRoC;


                //Emitter
                m_eParticleEmitter.EmitterHeight = tempEmitHeight;
                m_eParticleEmitter.EmitterWidth = tempEmitWidth;
                m_eParticleEmitter.EmitterLifespan = tempEmitLifespan;
                m_eParticleEmitter.EmitterPosX = tempEmitX;
                m_eParticleEmitter.EmitterPosY = tempEmitY;
                m_eParticleEmitter.SpawnRate = tempSpawnRate;
                if ("true" == tempIsLooping)
                    m_eParticleEmitter.IsLooping = true;
                else if ("false" == tempIsLooping)
                    m_eParticleEmitter.IsLooping = false;

                if ("Point" == tempShape)
                    m_eParticleEmitter.Shape = EmitterShape.Point;
                else if ("Rectangle" == tempShape)
                    m_eParticleEmitter.Shape = EmitterShape.Rectangle;
                else if ("Line" == tempShape)
                    m_eParticleEmitter.Shape = EmitterShape.Line;
                else if ("Circle" == tempShape)
                    m_eParticleEmitter.Shape = EmitterShape.Circle;


                textBoxEmitSpawnRate.Text = m_eParticleEmitter.SpawnRate.ToString();
                textBoxEmitHeight.Text = m_eParticleEmitter.EmitterHeight.ToString();
                textBoxEmitWidth.Text = m_eParticleEmitter.EmitterWidth.ToString();
                textBoxEmitXPos.Text = m_eParticleEmitter.EmitterPosX.ToString();
                textBoxEmitYPos.Text = m_eParticleEmitter.EmitterPosY.ToString();
                textBoxMaxLife.Text = m_fwFlyweight.MaxLifeSpan.ToString();
                textBoxMinLife.Text = m_fwFlyweight.MinLifeSpan.ToString();
                TextboxStartingXVel.Text = m_fwFlyweight.VelocityX.ToString();
                textBoxStartingYVel.Text = m_fwFlyweight.VelocityY.ToString();
                textBoxVelRateofChange.Text = m_fwFlyweight.VelocityRateOfChange.ToString();
                textBoxStartingImageScale.Text = m_fwFlyweight.StartingScale.ToString();
                textBoxRotationStart.Text = m_fwFlyweight.StartingRotation.ToString();
                textBoxRotationRateofChange.Text = m_fwFlyweight.RotationRateOfChange.ToString();
            }
        }


    }

    