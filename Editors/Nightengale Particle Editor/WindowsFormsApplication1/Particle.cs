using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WindowsFormsApplication1
{
    public class Particle
    {
        //LifeSpan
        float currLifeSpan;
        public float CurrLifeSpan
        {
            get { return currLifeSpan; }
            set { currLifeSpan = value; }
        }

        //Color values; 
        float currAlpha;
        public float CurrAlpha
        {
            get { return currAlpha; }
            set { currAlpha = value; }
        }

        float currRed;
        public float CurrRed
        {
            get { return currRed; }
            set { currRed = value; }
        }

        float currGreen;
        public float CurrGreen
        {
            get { return currGreen; }
            set { currGreen = value; }
        }

        float currBlue;
        public float CurrBlue
        {
            get { return currBlue; }
            set { currBlue = value; }
        }

        //Color ARGB
        int currARGB;
        public int CurrARGB
        {
            get { return currARGB; }
            set { currARGB = value; }
        }

        int colorIntervals;
        public int ColorIntervals
        {
            get { return colorIntervals; }
            set { colorIntervals = value; }
        }

        float changeInColorARGB;
        public float ChangeInColorARGB
        {
            get { return changeInColorARGB; }
            set { changeInColorARGB = value; }
        } 


        //Particle's current velocity
        float currVelocityX;
        public float CurrVelocityX
        {
            get { return currVelocityX; }
            set { currVelocityX = value; }
        }

        float currVelocityY;
        public float CurrVelocityY
        {
            get { return currVelocityY; }
            set { currVelocityY = value; }
        }

        //Particle's current lifespan
        float particleCurrLifeSpan;
        public float ParticleCurrLifeSpan
        {
            get { return particleCurrLifeSpan; }
            set { particleCurrLifeSpan = value; }
        }

        //Particle's current rotation
        float particleCurrRotation;
        public float ParticleCurrRotation
        {
            get { return particleCurrRotation; }
            set { particleCurrRotation = value; }
        } 

        //Particle's current location 
        float particleCurrPosX;
        public float ParticleCurrPosX
        {
            get { return particleCurrPosX; }
            set { particleCurrPosX = value; }
        }

        float particleCurrPosY;
        public float ParticleCurrPosY
        {
            get { return particleCurrPosY; }
            set { particleCurrPosY = value; }
        }

        //Particle size info
        float currParticleWidth;
        public float CurrParticleWidth
        {
            get { return currParticleWidth; }
            set { currParticleWidth = value; }
        }

        float currParticleHeight;
        public float CurrParticleHeight
        {
            get { return currParticleHeight; }
            set { currParticleHeight = value; }
        } 

        //Particle scale info
        float currXScaleValue;
        public float CurrXScaleValue
        {
            get { return currXScaleValue; }
            set { currXScaleValue = value; }
        }

        float currYScaleValue;
        public float CurrYScaleValue
        {
            get { return currYScaleValue; }
            set { currYScaleValue = value; }
        }

        float currScale;
        public float CurrScale
        {
            get { return currScale; }
            set { currScale = value; }
        }

        int scaleIntervals;
        public int ScaleIntervals
        {
            get { return scaleIntervals; }
            set { scaleIntervals = value; }
        }

        float changeInScale;
        public float ChangeInScale
        {
            get { return changeInScale; }
            set { changeInScale = value; }
        } 

    }
}
