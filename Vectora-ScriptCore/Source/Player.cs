using System;
using Vectora;

namespace Sandbox
{
    public class Player : Entity
    {
        
        void OnCreate()
        {
            Console.WriteLine($"Player.Oncreate - id {ID}");
        }

        void OnUpdate(float ts)
        {
            Console.WriteLine($"Player on update: {ts}");
            float speed = 1.0f;
            Vector3 velocity = Vector3.Zero;

            if(Input.IsKeyDown(KeyCodes.W))
                velocity.Y += 1.0f;
            else if(Input.IsKeyDown(KeyCodes.S))
                velocity.Y -= 1.0f;

            if (Input.IsKeyDown(KeyCodes.A))
                velocity.X += 1.0f;
            else if (Input.IsKeyDown(KeyCodes.D))
                velocity.X -= 1.0f;

            velocity *= speed;
            Vector3 translation = this.Translation;
            translation += velocity * ts;
            this.Translation = translation;
        }

        // had to add this so visual studio don't gray out the methods above, since they are called from native code
 
        void do_nothing()
        {
            OnCreate();
            OnUpdate(0.1f);
            //do_nothing();
        }
    }
}
