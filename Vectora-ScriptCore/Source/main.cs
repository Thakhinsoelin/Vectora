using System;

namespace Vectora
{
    public class Main
    {
        public float FloatVar { get; set; }
        public Main()
        {
            Console.WriteLine("Main constructor called");
        }

        public void PrintCustomMessage(string msg)
        {
            Console.WriteLine($"C# says normal {msg}");
        }

        public void PrintCustomInt(int value)
        {
            Console.WriteLine($"Custom message from Main class {value}");
        }
    }
}
