using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;

namespace AnimationEditor
{
    class Frame
    {
        public string Event;

        private int drawBottom;

        public int DrawBottom
        {
            get { return drawBottom; }
            set { drawBottom = value; }
        }

        private int drawRight;

        public int DrawRight
        {
            get { return drawRight; }
            set { drawRight = value; }
        }
        private int drawx = -1;

        public int DrawX
        {
            get { return drawx; }
            set { drawx = value; }
        }
        private int drawy = -1;

        public int DrawY
        {
            get { return drawy; }
            set { drawy = value; }
        }
        public Point drawlocation
        {
            get { return new Point(DrawX, DrawY); }
            set { drawlocation = value; }
        }

        public Size drawSize
        {
            get { return new Size(DrawRight - DrawX, DrawBottom - DrawY); }
            set { drawSize = value; }
        }

        public Rectangle DrawRect
        {
            get { return new Rectangle(drawlocation, drawSize); }
            set { DrawRect = value; }
        }


        /// <summary>
        /// //////////////////////////
        /// </summary>
        /// 
        private int collisionBottom;

        public int CollisionBottom
        {
            get { return collisionBottom; }
            set { collisionBottom = value; }
        }

        private int collisionRight;

        public int CollisionRight
        {
            get { return collisionRight; }
            set { collisionRight = value; }
        }
        private int collisionx = -1;

        public int CollisionX
        {
            get { return collisionx; }
            set { collisionx = value; }
        }
        private int collisiony = -1;

        public int CollisionY
        {
            get { return collisiony; }
            set { collisiony = value; }
        }
        public Point Collisionlocation
        {
            get { return new Point(CollisionX, CollisionY); }
            set { Collisionlocation = value; }
        }

        public Size CollisionSize
        {
            get { return new Size(CollisionRight - CollisionX, CollisionBottom - CollisionY); }
            set { CollisionSize = value; }
        }
        Rectangle collisionRect;

        public Rectangle CollisionRect
        {
            get { return new Rectangle(Collisionlocation, CollisionSize); }
            set { collisionRect = value; }
        }


        private int eventBottom;

        public int EventBottom
        {
            get { return eventBottom; }
            set { eventBottom = value; }
        }

        private int eventRight;

        public int EventRight
        {
            get { return eventRight; }
            set { eventRight = value; }
        }
        private int eventx = -1;

        public int EventX
        {
            get { return eventx; }
            set { eventx = value; }
        }
        private int eventy = -1;

        public int EventY
        {
            get { return eventy; }
            set { eventy = value; }
        }
        public Point Eventlocation
        {
            get { return new Point(EventX, EventY); }
            set { Eventlocation = value; }
        }

        public Size EventSize
        {
            get { return new Size(EventRight - EventX, EventBottom - EventY); }
            set { EventSize = value; }
        }
        Rectangle eventRect;

        public Rectangle EventRect
        {
            get { return new Rectangle(Eventlocation, EventSize); }
            set { eventRect = value; }
        }

        private int anchorx = -1;

        public int AnchorX
        {
            get { return anchorx; }
            set { anchorx = value; }
        }
        private int anchory = -1;

        public int AnchorY
        {
            get { return anchory; }
            set { anchory = value; }
        }
       

        public Point AnchorPoint
        {
            get { return new Point( AnchorX,AnchorY );}
        }
        public float duration;
    }
    class Animation
    {
        private string name;
        public string Name
        {
            get { return name; }
            set { name = value; }
        }
        public List<Frame> Frames;
        public bool looping = true;
        

    }
}
