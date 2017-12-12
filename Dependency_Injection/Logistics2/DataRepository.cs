using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Logistics
{
    public class DataRepository
    {
        public DataContext dataBase;
        IFill fillDataBase;
        public static int key = 0;

        public DataRepository(DataContext dataBase, IFill fillDataBase)
        {
            this.dataBase = dataBase;
            this.fillDataBase = fillDataBase;
        }

        public void FillDataBase()
        {
            fillDataBase.Fill(dataBase);
        }

        public void Create(Register obj)
        {
            dataBase.collectionRegister.Add(obj);
        }
        public void Create(Catalog obj)
        {
            dataBase.collectionCatalog.Add(key++, obj);
        }
        public void Create(StateInfo obj)
        {
            dataBase.collectionStateInfo.Add(obj);
        }
        public void Create(Event obj)
        {
            dataBase.collectionEvent.Add(obj);
        }

        //Register C.R.U.D methods

        public Register GetRegister(int idI)
        {
            if (dataBase.collectionRegister.ElementAtOrDefault(idI) != null)
            {
                return dataBase.collectionRegister[idI];
            }
            else
            {
                Console.WriteLine("There is no such Instance");
                return null;
            }
        }

        public string GetRegisterName(string nameI)
        {
            var b = dataBase.collectionRegister.FirstOrDefault(a => a.clientName == nameI).clientName;
            if (b != null)
            {
                return b;
            }
            else
            {
                Console.WriteLine("There is no such Instance");
                return null;
            }
        }

        public void UpdateRegister(string nameI, string newNameI)
        {
            var obj = dataBase.collectionRegister.FirstOrDefault(a => a.clientName == nameI);
            if (obj != null)
            {
                obj.clientName = newNameI;
            }
            else
            {
                Console.WriteLine("There is no such instance");
            }
        }

        public void DeleteRegister(string nameI)
        {
            var obj = dataBase.collectionRegister.FirstOrDefault(a => a.clientName == nameI);
            if (obj != null)
            {
                dataBase.collectionRegister.Remove(obj);
            }
            else
            {
                Console.WriteLine("There is no such instance");
            }
        }

        public void DeleteRegisterIdx(int idx)
        {
            try
            {
                dataBase.collectionRegister.RemoveAt(idx);
            }
            catch (ArgumentOutOfRangeException)
            {
                Console.WriteLine("There is no such instance");
            }
        }

        public void ReadAllRegister()
        {
            if (dataBase.collectionRegister != null)
            {
                foreach (var obj in dataBase.collectionRegister)
                {
                    Console.WriteLine(obj + ";");
                }
            }
            else
            {
                Console.WriteLine("List is empty");
            }
        }

        //Catalog C.R.U.D methods

        public Catalog GetCatalog(int idI)
        {
            Catalog cat;
            if (dataBase.collectionCatalog.TryGetValue(idI, out cat))
            {
                return cat;
            }
            else
            {
                Console.WriteLine("There is no such Instance");
                return null;
            }
        }

        public string GetCatalogInfo(int idI)
        {
            Catalog cat;
            if (dataBase.collectionCatalog.TryGetValue(idI, out cat))
            {
                return cat.info;
            }
            else
            {
                Console.WriteLine("There is no such Instance");
                return null;
            }
        }

        public void UpdateCatalog(int idI, string infoI)
        {
            Catalog cat;
            if (dataBase.collectionCatalog.TryGetValue(idI, out cat))
            {
                cat.info = infoI;
            }
            else
            {
                Console.WriteLine("There is no such Instance");
            }
        }

        public void DeleteCatalog(int idI)
        {
            Catalog cat;
            if (dataBase.collectionCatalog.TryGetValue(idI, out cat))
            {
                dataBase.collectionCatalog.Remove(idI);
            }
            else
            {
                Console.WriteLine("There is no such Instance");
            }
        }

        public void ReadAllCatalog()
        {
            if (dataBase.collectionCatalog != null)
            {
                foreach (var obj in dataBase.collectionCatalog)
                {
                    Console.WriteLine("Key = {0}, Value = {1}", obj.Key ,obj.Value);
                }
            }
            else
            {
                Console.WriteLine("List is empty");
            }
        }

        //StateInfo C.R.U.D methods

        public StateInfo GetStateInfo(int idI)
        {
            if (dataBase.collectionStateInfo.ElementAtOrDefault(idI) != null)
            {
                return dataBase.collectionStateInfo[idI];
            }
            else
            {
                Console.WriteLine("There is no such Instance");
                return null;
            }
        }

        public void UpdateStateInfo(int idI, int amountI, int priceI, DateTime purchaseDateI, int taxI)
        {
            var obj = dataBase.collectionStateInfo[idI];
            if (obj != null)
            {
                obj.amount = amountI;
                obj.price = priceI;
                obj.purchaseDate = purchaseDateI;
                obj.tax = taxI;
            }
            else
            {
                Console.WriteLine("There is no such Instance");
            }
        }

        public void DeleteStateInfo(int idI)
        {
            var obj = dataBase.collectionStateInfo[idI];
            if (obj != null)
            {
                dataBase.collectionStateInfo.Remove(obj);
            }
            else
            {
                Console.WriteLine("There is no such Instance");
            }
        }

        public IEnumerable<StateInfo> ReadAllStateInfoCollection()
        {
            if (dataBase.collectionStateInfo != null)
            {
                return dataBase.collectionStateInfo;
            }
            else
            {
                Console.WriteLine("List is empty");
                return null;
            }
        }

        //Event C.R.U.D methods

        public Event GetEvent(int idI)
        {
            if (dataBase.collectionEvent.ElementAtOrDefault(idI) != null)
            {
                return dataBase.collectionEvent[idI];
            }
            else
            {
                Console.WriteLine("There is no such Instance");
                return null;
            }
        }

        public string GetEventName(string nameI)
        {
            var b = dataBase.collectionEvent.FirstOrDefault(a => a.name == nameI).name;
            if (b != null)
            {
                return b;
            }
            else
            {
                Console.WriteLine("There is no such Instance");
                return null;
            }
        }

        public void UpdateEvent(string nameI, string newNameI)
        {
            var obj = dataBase.collectionEvent.FirstOrDefault(a => a.name == nameI);
            if (obj != null)
            {
                obj.name = newNameI;
            }
            else
            {
                Console.WriteLine("There is no such instance");
            }
        }

        public void DeleteEvent(string nameI)
        {
            var obj = dataBase.collectionEvent.FirstOrDefault(a => a.name == nameI);
            if (obj != null)
            {
                dataBase.collectionEvent.Remove(obj);
            }
            else
            {
                Console.WriteLine("There is no such instance");
            }
        }

        public void DeleteEventIdx(int idx)
        {
            try
            {
                dataBase.collectionEvent.RemoveAt(idx);
            }
            catch(ArgumentOutOfRangeException)
            {
                Console.WriteLine("There is no such instance");
            }

        }

        public void ReadAllEvent()
        {
            if (dataBase.collectionEvent != null)
            {
                foreach (var obj in dataBase.collectionEvent)
                {
                    Console.WriteLine(obj + ";");
                }
            }
            else
            {
                Console.WriteLine("List is empty");
            }
        }
    }
}