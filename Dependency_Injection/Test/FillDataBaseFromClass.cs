using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Logistics
{
    public class FillDataBaseFromClass : IFill
    {
        public int nrOfElements { get; set; }

        public void Fill(DataContext dataContext)
        {
            for (int i = 0; i < nrOfElements; i++)
            {
                Register newReg = NewRegister("name" + i);
                Catalog newCat = NewCatalog("name" + i);
                // 05/01/2008 8:06:32 AM is 633,452,259,920,000,000 ticks
                StateInfo newSt = NewStateInfo(i, (double)i, new DateTimeOffset(633452259920000000+i, 
                                                                                new TimeSpan(1, 0, 0)), i);
                Event newEve = NewEvent("name" + i);
                
                newSt.catalog = newCat;
                newEve.register = newReg;
                newEve.stateInfo = newSt;
                
                dataContext.collectionRegister.Add(newReg);
                dataContext.collectionCatalog.Add(i, newCat);
                dataContext.collectionStateInfo.Add(newSt);
                dataContext.collectionEvent.Add(newEve);
           }
        }

        private Register NewRegister(string name)
        {
            Register reg = new Register(name);
            return reg;
        }
        private Catalog NewCatalog(string name)
        {
            Catalog cat = new Catalog(name);
            return cat;
        }
        private StateInfo NewStateInfo(int amount, double price, DateTimeOffset purchaseDate, int tax)
        {
            StateInfo si = new StateInfo(amount, price, purchaseDate, tax);
            return si;
        }
        private Event NewEvent(string name)
        {
            Event eve = new Event(name);
            return eve;
        }
    }
}
