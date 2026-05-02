using System;
using System.Security.AccessControl;
using Vectora;

namespace Sandbox
{
    public class Camera : Entity
    {
        public Entity otherEntity;
        public float DistanceFromPlayer = 5.0f;

        private Entity m_Player;

        void OnCreate()
        {
            m_Player = FindEntityByName("Player");
        }

        void OnUpdate(float ts)
        {
            if (m_Player != null)
                Translation = new Vector3(m_Player.Translation.XY, DistanceFromPlayer);

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
