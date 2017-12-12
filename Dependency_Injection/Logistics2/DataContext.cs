using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Logistics
{
    public class DataContext
    {
        public List<Register> collectionRegister = new List<Register>();
        public Dictionary<int, Catalog> collectionCatalog = new Dictionary<int,Catalog>();
        
        public ObservableCollection<StateInfo> collectionStateInfo = new ObservableCollection<StateInfo>();
        public ObservableCollection<Event> collectionEvent = new ObservableCollection<Event>();
    }
}
