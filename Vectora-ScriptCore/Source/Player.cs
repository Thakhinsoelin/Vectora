using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Vectora;

namespace Sandbox
{
    public class Player : Entity
    {
        void OnCreate()
        {
            Console.WriteLine("Player.OncReate");
        }

        void OnUpdate(float ts)
        {
            Console.WriteLine($"Player on update: {ts}");
        }
    }
}
