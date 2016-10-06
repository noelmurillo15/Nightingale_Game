using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MapEditor {
    class Tile {
        int column;
        int row;
        int type;
        bool col = false;
        bool bush = false;
        string _event = "none";
        string spawnname = "none";

        public int Column
        {
            get { return column; }
            set { column = value; }
        }

        public int Row
        {
            get { return row; }
            set { row = value; }
        }

        public int Type
        {
            get { return type; }
            set { type = value; }
        }

        public bool Collision
        {
            get { return col; }
            set { col = value; }
        }

        public bool Bush
        {
            get { return bush; }
            set { bush = value; }
        }

        public string Event{
            get { return _event; }
            set { _event = value; }
        }

        public string CreatureName
        {
            get { return spawnname; }
            set { spawnname = value; }
        }

        public Tile(int x, int y)
        {
            this.column = x;
            this.row = y;
        }
    }
}
