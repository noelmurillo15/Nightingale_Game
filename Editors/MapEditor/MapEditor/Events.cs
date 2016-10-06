using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MapEditor
{
    class Events
    {

        Tile[] EventTiles = new Tile[0];

        int count = 0;

        string eventname = "none";

        public string Name
        {
            get { return eventname; }
            set { eventname = value; }
        }

        public void AddEventTiles(Tile t)
        {
            Array.Resize(ref EventTiles, EventTiles.Length + 1);
            t.Event = eventname;
            EventTiles[EventTiles.Length - 1] = t;
            count++;
        }

        public Tile GetEventTile(int n)
        {
            return EventTiles[n];
        }

        public int GetCount()
        {
            return count;
        }


        public Events(string name)
        {
            this.eventname = name;
        }
        
    }
}
