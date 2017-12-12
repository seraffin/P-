using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Logistics
{
    public class Event
    {
        public string name;
        public StateInfo stateInfo { get; set; }
        public Register register { get; set; }

        public Event(string name)
        {
            this.name = name;
        }
    }
}
