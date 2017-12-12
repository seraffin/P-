using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Logistics
{
    public class StateInfo
    {
        public int amount;
        public double price;
        public DateTimeOffset purchaseDate;
        public int tax;
        public Catalog catalog { get; set; }

        public StateInfo(int amount, double price, DateTimeOffset purchaseDate, int tax)
        {
            this.amount = amount;
            this.price = price;
            this.purchaseDate = purchaseDate;
            this.tax = tax;
        }
    }
}
