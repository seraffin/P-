using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Logistics
{
    class Program
    {
        static void Main(string[] args)
        {
            DataContext myDataContext = new DataContext();
            FillDataBaseFromTxt myFillDataBase = new FillDataBaseFromTxt();
            DataRepository myDataRepositry = new DataRepository(myDataContext, myFillDataBase);
        }
    }
}