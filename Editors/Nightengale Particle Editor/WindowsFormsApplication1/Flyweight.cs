using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing; 

namespace WindowsFormsApplication1
{
    public class Flyweight
    {
        //Lifespan info (move into flyweight)
        float maxLifeSpan;
        public float MaxLifeSpan
        {
            get { return maxLifeSpan; }
            set { maxLifeSpan = value; }
        }
        float minLifeSpan;
        public float MinLifeSpan
        {
            get { return minLifeSpan; }
            set { minLifeSpan = value; }
        }

        //Texture info
        string textureName;
        public string TextureName
        {
            get { return textureName; }
            set { textureName = value; }
        }

        int CurrentTexture;
        public int CurrentTexture1
        {
            get { return CurrentTexture; }
            set { CurrentTexture = value; }
        }

        //Velocity variables
        float velocityX;
        public float VelocityX
        {
            get { return velocityX; }
            set { velocityX = value; }
        }

        float velocityY;
        public float VelocityY
        {
            get { return velocityY; }
            set { velocityY = value; }
        }

        //Velocity rate of change
        float velocityRateOfChange;
        public float VelocityRateOfChange
        {
            get { return velocityRateOfChange; }
            set { velocityRateOfChange = value; }
        }

        //Rotation Variables
        float startingRotation;
        public float StartingRotation
        {
            get { return startingRotation; }
            set { startingRotation = value; }
        }

        float rotationRateOfChange;
        public float RotationRateOfChange
        {
            get { return rotationRateOfChange; }
            set { rotationRateOfChange = value; }
        }

        //Color information 
        Color startingColor;
        public Color StartingColor
        {
            get { return startingColor; }
            set { startingColor = value; }
        }

        Color endingColor;
        public Color EndingColor
        {
            get { return endingColor; }
            set { endingColor = value; }
        } 

        //Scale information
        float startingScale;
        public float StartingScale
        {
            get { return startingScale; }
            set { startingScale = value; }
        }

        float endingScale;
        public float EndingScale
        {
            get { return endingScale; }
            set { endingScale = value; }
        } 

        //Starting sizes
        float startingWidth;
        public float StartingWidth
        {
            get { return startingWidth; }
            set { startingWidth = value; }
        }

        float startingHeight;
        public float StartingHeight
        {
            get { return startingHeight; }
            set { startingHeight = value; }
        } 

        //Color ARGBs
        int startARGB;
        public int StartARGB
        {
            get { return startARGB; }
            set { startARGB = value; }
        }

        int endARGB;
        public int EndARGB
        {
            get { return endARGB; }
            set { endARGB = value; }
        }



    }
}
