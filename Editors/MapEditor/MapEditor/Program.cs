﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MapEditor
{
    static class Program {
        [STAThread]

        static void Main() {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            
            MapEditor myform = new MapEditor();
            myform.Show();
            while (myform.Looping) {                
                Application.DoEvents();
            }
        }
    }
}
