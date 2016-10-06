using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing; 

namespace WindowsFormsApplication1
{
    enum EmitterShape {Point, Rectangle, Circle, Line};
    public class Emitter
    {
        //Emitter Position
        float emitterPosX;
        public float EmitterPosX
        {
            get { return emitterPosX; }
            set { emitterPosX = value; }
        }

        float emitterPosY;
        public float EmitterPosY
        {
            get { return emitterPosY; }
            set { emitterPosY = value; }
        } 

        //Does the emitter loop through particles
        bool isLooping;
        public bool IsLooping
        {
            get { return isLooping; }
            set { isLooping = value; }
        } 


        //Spawn Rate
        float spawnRate;
        public float SpawnRate
        {
            get { return spawnRate; }
            set { spawnRate = value; }
        }

        //Emitter shape
        EmitterShape shape;
        internal EmitterShape Shape
        {
            get { return shape; }
            set { shape = value; }
        }

        //Emitter width
        float emitterWidth;
        public float EmitterWidth
        {
            get { return emitterWidth; }
            set { emitterWidth = value; }
        }

        //Emitter height
        float emitterHeight;
        public float EmitterHeight
        {
            get { return emitterHeight; }
            set { emitterHeight = value; }
        } 

        //Current number of particles
        int currNumParticles;
        public int CurrNumParticles
        {
            get { return currNumParticles; }
            set { currNumParticles = value; }
        }

        //Max possible number of particles
        int maxNumParticles;
        public int MaxNumParticles
        {
            get { return maxNumParticles; }
            set { maxNumParticles = value; }
        } 

        //Emitter's lifespan
        float emitterLifespan;
        public float EmitterLifespan
        {
            get { return emitterLifespan; }
            set { emitterLifespan = value; }
        } 



    }
}
