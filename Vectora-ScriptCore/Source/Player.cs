using System;
using System.Security.AccessControl;
using Vectora;

namespace Sandbox
{
    public class Player : Entity
    {
        private TransformComponent m_transform;
        private Rigidbody2DComponent m_Rigidbody;
        void OnCreate()
        {
            Console.WriteLine($"Player.Oncreate - id {ID}");
            m_transform = GetComponent<TransformComponent>();
            m_Rigidbody = GetComponent<Rigidbody2DComponent>();
            m_transform.Translation = new Vector3(0.0f);
        }

        

        void OnUpdate(float ts)
        {
            Console.WriteLine($"Player on update: {ts}");
            float speed = 0.01f;
            Vector3 velocity = Vector3.Zero;

            if(Input.IsKeyDown(KeyCodes.W))
                velocity.Y += 1.0f;
            else if(Input.IsKeyDown(KeyCodes.S))
                velocity.Y -= 1.0f;

            if (Input.IsKeyDown(KeyCodes.A))
                velocity.X -= 1.0f;
            else if (Input.IsKeyDown(KeyCodes.D))
                velocity.X += 1.0f;

            velocity *= speed;

            m_Rigidbody.ApplyLinerImpulse(velocity.XY, true);

            //Vector3 translation = m_transform.Translation;
            //translation += velocity * ts;
            //m_transform.Translation = translation;
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
