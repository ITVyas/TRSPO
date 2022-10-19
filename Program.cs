using System;
using System.Threading;

namespace TaskTRSPO
{
    class CollatzNumber
    {
        public int Start;
        public int Current;
        public int IterationsAmount;

        public CollatzNumber(int start)
        {
            Start = start;
            Current = start;
            IterationsAmount = 0;
        }

        public void Print()
        {
            Console.WriteLine($"Start: {Start}, IterationsAmount: {IterationsAmount}");
        }
    }
    class Program
    {
        const int N = 10;
        const int ThreadsAmount = 5;
        static int ActiveThreadsAmount = 5;
        static Queue<CollatzNumber> numbers = new Queue<CollatzNumber>();
        static List<CollatzNumber> result = new List<CollatzNumber>();
        static Mutex NumbersMtx = new Mutex();
        static Mutex ResultMtx  = new Mutex();

        static void DoNextCollatzNumbers()
        {
            CollatzNumber number;

            while (result.Count < N - 1)
            {
                NumbersMtx.WaitOne();
                number = numbers.Dequeue();
                NumbersMtx.ReleaseMutex();

                number.Current = (number.Current % 2 == 0) ? number.Current / 2 : 3 * number.Current + 1;
                number.IterationsAmount += 1;
                
                

                if (number.Current == 1)
                {

                    ResultMtx.WaitOne();
                    result.Add(number);
                    number.Print();
                    if (N - 1 - result.Count < ActiveThreadsAmount)
                    {
                        ActiveThreadsAmount--;
                        ResultMtx.ReleaseMutex();
                        break;
                    }
                    ResultMtx.ReleaseMutex();
                                   
                }
                else
                {
                    NumbersMtx.WaitOne();
                    numbers.Enqueue(number);
                    NumbersMtx.ReleaseMutex();
                }



            } 
        }



        static int Main(string[] args)
        {
            
            for (int i = 0; i < N-1; i++)
                numbers.Enqueue(new CollatzNumber(i+2));

            Thread[] threads = new Thread[ThreadsAmount];

            for (int i = 0; i < ThreadsAmount; i++)
                threads[i] = new Thread(DoNextCollatzNumbers);


            for (int i = 0; i < ThreadsAmount; i++)
                threads[i].Start();

            for (int i = 0; i < ThreadsAmount; i++)
                threads[i].Join();

             return 0;
        }
    }
}