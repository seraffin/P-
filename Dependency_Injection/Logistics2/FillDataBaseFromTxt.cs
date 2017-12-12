using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace Logistics
{
    public class FillDataBaseFromTxt : IFill
    {
        public int nrOfElements { get; set; }
        public string file { get; set; }
        public void Fill(DataContext dataContext)
        {

            using (StreamReader sr = new StreamReader(file, Encoding.Default))
            {

                string text = sr.ReadToEnd();
                string[] lines = text.Split('\r');
                int i = 0;
                foreach (string s in lines)
                {
                    string[] elements = s.Split(';');
                    if (elements.Count() == 7)
                    {
                        Register newReg = NewRegister(elements[0]);
                        Catalog newCat = NewCatalog(elements[1]);

                        StateInfo newSt = NewStateInfo(Convert.ToInt32(elements[2]),
                                                       Convert.ToDouble(elements[3]),
                                                       new DateTimeOffset(Convert.ToInt64(elements[4]),
                                                                          new TimeSpan(1, 0, 0)),
                                                       Convert.ToInt32(elements[5]));

                        Event newEve = NewEvent(elements[6]);

                        newSt.catalog = newCat;
                        newEve.register = newReg;
                        newEve.stateInfo = newSt;

                        dataContext.collectionRegister.Add(newReg);
                        dataContext.collectionCatalog.Add(i, newCat);
                        dataContext.collectionStateInfo.Add(newSt);
                        dataContext.collectionEvent.Add(newEve);

                        i++;
                    }
                }
            }
        }

        public void FillFile()
        {
            using (StreamWriter outputFile = new StreamWriter(file))
            {
                for (int i = 0; i < nrOfElements; i++)
                {
                    outputFile.WriteLine("name" + i + ";"
                                        + "name" + i + ";"
                                        + i + ";"
                                        + i + ";"
                                        + (633452259920000000 + i) + ";"
                                        + i + ";"
                                        + "name" + i);
                }
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
