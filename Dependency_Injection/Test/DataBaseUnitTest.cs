using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.IO;
using System.Text;

namespace Logistics
{
    [TestClass]
    public class DataBaseUnitTest
    {
        
        [TestMethod]
        public void TestCreateEvent()
        {
            Event a = new Event("nowy");
            Event b = new Event("nowy2");

            DataRepository r = new DataRepository(new DataContext(), new FillDataBaseFromClass());
            r.Create(a);
            r.Create(b);
            Assert.AreEqual(r.dataBase.collectionEvent[0].name, "nowy");
            Assert.AreEqual(r.dataBase.collectionEvent[1].name, "nowy2");
        }

        [TestMethod]
        public void TestCreateCatalog()
        {
            Catalog a = new Catalog("nowy");
            Catalog b = new Catalog("nowy2");
            DataRepository r = new DataRepository(new DataContext(), new FillDataBaseFromClass());
            r.Create(a);
            r.Create(b);
            Assert.AreEqual(r.dataBase.collectionCatalog[0].info, "nowy");
            Assert.AreEqual(r.dataBase.collectionCatalog[1].info, "nowy2");
        }

        [TestMethod]
        public void TestCreateStateInfo()
        {
            DateTimeOffset data1 = new DateTimeOffset(new DateTime(2017, 1, 1));
            DateTimeOffset data2 = new DateTimeOffset(new DateTime(1000, 11, 13));
            StateInfo a = new StateInfo(1, 2.55, data1, 3);
            StateInfo b = new StateInfo(0, 10000.99, data2, 99);
            DataRepository r = new DataRepository(new DataContext(), new FillDataBaseFromClass());
            r.Create(a);
            r.Create(b);
            Assert.AreEqual(r.dataBase.collectionStateInfo[0].amount, 1);
            Assert.AreEqual(r.dataBase.collectionStateInfo[0].price, 2.55);
            Assert.AreEqual(r.dataBase.collectionStateInfo[0].purchaseDate, new DateTimeOffset(new DateTime(2017, 1, 1)));
            Assert.AreEqual(r.dataBase.collectionStateInfo[0].tax, 3);

            Assert.AreEqual(r.dataBase.collectionStateInfo[1].amount, 0);
            Assert.AreEqual(r.dataBase.collectionStateInfo[1].price, 10000.99);
            Assert.AreEqual(r.dataBase.collectionStateInfo[1].purchaseDate, new DateTimeOffset(new DateTime(1000, 11, 13)));
            Assert.AreEqual(r.dataBase.collectionStateInfo[1].tax, 99);
        }

        [TestMethod]
        public void TestCreateRegister()
        {
            Register a = new Register("nowy");
            Register b = new Register("nowy2");
            DataRepository r = new DataRepository(new DataContext(), new FillDataBaseFromClass());
            r.Create(a);
            r.Create(b);
            Assert.AreEqual(r.dataBase.collectionRegister[0].clientName, "nowy");
            Assert.AreEqual(r.dataBase.collectionRegister[1].clientName, "nowy2");
        }

        [TestMethod]
        public void TestUpdateEvent()
        {
            Event a = new Event("nowy");

            DataRepository r = new DataRepository(new DataContext(), new FillDataBaseFromClass());
            r.Create(a);
            r.UpdateEvent("nowy", "nowszy");

            Assert.AreEqual(r.dataBase.collectionEvent[0].name, "nowszy");
        }

        [TestMethod]
        public void TestDeleteEvent()
        {
            Event a = new Event("nowy");

            DataRepository r = new DataRepository(new DataContext(), new FillDataBaseFromClass());

            r.Create(a);
            Assert.AreEqual(r.dataBase.collectionEvent.Count, 1);

            r.DeleteEvent("nowy");
            Assert.AreEqual(r.dataBase.collectionEvent.Count, 0);
        }

        [TestMethod]
        public void TestDeleteWarehouseIdx()
        {
            Event a = new Event("nowy");
            DataRepository r = new DataRepository(new DataContext(), new FillDataBaseFromClass());

            r.Create(a);
            Assert.AreEqual(r.dataBase.collectionEvent.Count, 1);

            r.DeleteEventIdx(0);
            Assert.AreEqual(r.dataBase.collectionRegister.Count, 0);
        }

        [TestMethod]
        public void TestFillDataBaseFromClass()
        {
            FillDataBaseFromClass f = new FillDataBaseFromClass();
            f.nrOfElements = 100000;
            DataRepository r = new DataRepository(new DataContext(), f);

            r.FillDataBase();

            Assert.AreEqual(r.dataBase.collectionCatalog.Count, f.nrOfElements);
        }

        [TestMethod]
        public void FillTxt()
        {
            FillDataBaseFromTxt f = new FillDataBaseFromTxt();
            f.file = "data4";
            f.nrOfElements = (int)Math.Exp(4);
            f.FillFile();

            f.file = "data8";
            f.nrOfElements = (int)Math.Exp(8);
            f.FillFile();

            f.file = "data12";
            f.nrOfElements = (int)Math.Exp(12);
            f.FillFile();

            f.file = "data14";
            f.nrOfElements = (int)Math.Exp(14);
            f.FillFile();
        }

        [TestMethod]
        public void TestFillDataBaseFromTxt4()
        {
            FillDataBaseFromTxt f = new FillDataBaseFromTxt();
            f.file = "data4";
            StreamReader sr = new StreamReader(f.file, Encoding.Default);

            int nrOfLines = 0;
            while(sr.ReadLine() != null)
            {
                nrOfLines++;
            }
            DataRepository r = new DataRepository(new DataContext(), f);
            
            r.FillDataBase();

            Assert.AreEqual(r.dataBase.collectionCatalog.Count, nrOfLines);
        }

        [TestMethod]
        public void TestFillDataBaseFromTxt8()
        {
            FillDataBaseFromTxt f = new FillDataBaseFromTxt();
            f.file = "data8";
            StreamReader sr = new StreamReader(f.file, Encoding.Default);

            int nrOfLines = 0;
            while (sr.ReadLine() != null)
            {
                nrOfLines++;
            }
            DataRepository r = new DataRepository(new DataContext(), f);

            r.FillDataBase();

            Assert.AreEqual(r.dataBase.collectionCatalog.Count, nrOfLines);
        }

        [TestMethod]
        public void TestFillDataBaseFromTxt12()
        {
            FillDataBaseFromTxt f = new FillDataBaseFromTxt();
            f.file = "data12";
            StreamReader sr = new StreamReader(f.file, Encoding.Default);

            int nrOfLines = 0;
            while (sr.ReadLine() != null)
            {
                nrOfLines++;
            }
            DataRepository r = new DataRepository(new DataContext(), f);

            r.FillDataBase();

            Assert.AreEqual(r.dataBase.collectionCatalog.Count, nrOfLines);
        }

        [TestMethod]
        public void TestFillDataBaseFromTxt14()
        {
            FillDataBaseFromTxt f = new FillDataBaseFromTxt();
            f.file = "data14";
            StreamReader sr = new StreamReader(f.file, Encoding.Default);

            int nrOfLines = 0;
            while (sr.ReadLine() != null)
            {
                nrOfLines++;
            }
            DataRepository r = new DataRepository(new DataContext(), f);

            r.FillDataBase();

            Assert.AreEqual(r.dataBase.collectionCatalog.Count, nrOfLines);
        }

    }
    
}
