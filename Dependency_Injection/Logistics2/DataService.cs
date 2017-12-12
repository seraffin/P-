using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Logistics
{
    class DataService
    {
        DataRepository dataRepo;

        public DataService(DataRepository dataRepo)
        {
            this.dataRepo = dataRepo;
            dataRepo.dataBase.collectionEvent.CollectionChanged += new System.Collections.Specialized.NotifyCollectionChangedEventHandler
                (CollectionChangedMethod);
        }

        private void CollectionChangedMethod(object sender, NotifyCollectionChangedEventArgs e)
        {
            if (e.Action == NotifyCollectionChangedAction.Add)
            {
                Console.WriteLine("Item added to Events");
            }
            if (e.Action == NotifyCollectionChangedAction.Replace)
            {
                //
            }
            if (e.Action == NotifyCollectionChangedAction.Remove)
            {
                Console.WriteLine("Item removed from Events");
            }
            if (e.Action == NotifyCollectionChangedAction.Move)
            {
                //
            }
        }

        //Printing 

        void printCollection(List<Register> collection)
        {
            foreach(Register obj in collection)
            {
                Console.WriteLine(obj.clientName);
            }
        }

        void printCollection(Dictionary<int, Catalog> collection)
        {
            foreach(KeyValuePair<int, Catalog> obj in collection)
            {
                Console.WriteLine(obj.Value.info);
            }
        }

        void printCollection(ObservableCollection<StateInfo> collection)
        {
            foreach (StateInfo obj in collection)
            {
                Console.WriteLine("{0}, {1}, {2}, {3}", obj.amount, obj.price, obj.purchaseDate, obj.tax);
            }
        }

        void printCollection(ObservableCollection<Event> collection)
        {
            foreach (Event obj in collection)
            {
                Console.WriteLine(obj.name);
            }
        }

        //Filtering

        IEnumerable<Event> GetEventsByRegister(Register reg)
        {
            return dataRepo.dataBase.collectionEvent.Where(a => a.register == reg);
        }

        IEnumerable<Event> GetEventsByCatalog(Catalog cat)
        {
            return dataRepo.dataBase.collectionEvent.Where(a => a.stateInfo.catalog == cat);
        }

        IEnumerable<Event> GetEventsByPurchaseDate(DateTimeOffset min, DateTimeOffset max)
        {
            return dataRepo.dataBase.collectionEvent.Where(a => (a.stateInfo.purchaseDate > min && a.stateInfo.purchaseDate < max));
        }

        //Modyficattions

        void AddEventWithRegisterAndStateInfo(string eventName, Register reg, StateInfo state)
        {
            dataRepo.Create(reg);
            dataRepo.Create(state);

            Event myEvent = new Event(eventName);
            myEvent.stateInfo = state;
            myEvent.register = reg;

            dataRepo.Create(myEvent);
        }

        void DeleteEventByRegister(Register reg)
        {
            var list = dataRepo.dataBase.collectionEvent.Where(a => a.register == reg);

            foreach (Event obj in list)
            {
                dataRepo.dataBase.collectionEvent.Remove(obj);
            }
        }
        
    }
}