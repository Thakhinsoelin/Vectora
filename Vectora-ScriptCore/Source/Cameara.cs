using System;
using System.Security.AccessControl;
using Vectora;

namespace Sandbox
{
    public class Camera : Entity
    {
        
        void OnUpdate(float ts)
        {
            float speed = 1.0f;
            Vector3 velocity = Vector3.Zero;

            if(Input.IsKeyDown(KeyCodes.UP))
                velocity.Y += 1.0f;
            else if(Input.IsKeyDown(KeyCodes.DOWN))
                velocity.Y -= 1.0f;

            if (Input.IsKeyDown(KeyCodes.RIGHT))
                velocity.X += 1.0f;
            else if (Input.IsKeyDown(KeyCodes.LEFT))
                velocity.X -= 1.0f;

            velocity *= speed;



            Vector3 translation = Translation;
            translation += velocity * ts;
            Translation = translation;
        }

        // had to add this so visual studio don't gray out the methods above, since they are called from native code
 
        void do_nothing()
        {
            OnUpdate(0.1f);
            //do_nothing();
        }
    }
}
