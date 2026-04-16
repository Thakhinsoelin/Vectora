using System;
using System.Runtime.CompilerServices;

namespace Vectora
{
    public struct Vector3
    {
        public float X;
        public float Y;
        public float Z;

        public Vector3(float x, float y, float z)
        {
            X = x; Y = y; Z = z;
        }

    }
    public class Main
    {
        public float FloatVar { get; set; }
        public Main()
        {
            Console.WriteLine("Main constructor called");
            NativeLog("Soegyi", 969);

            Vector3 position = new Vector3(4, 2, 1);
            Log(ref position);
        }

        public void PrintCustomMessage(string msg)
        {
            Console.WriteLine($"C# says normal {msg}");
        }

        public void PrintCustomInt(int value)
        {
            Console.WriteLine($"Custom message from Main class {value}");
        }

        private void Log(string text, int param)
        {
            NativeLog(text, param);
        }

        private void Log(ref Vector3 param)
        {
            NativeLog_Vector(ref param);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void NativeLog(string text, int parameter);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void NativeLog_Vector(ref Vector3 parameter);
    }
}
