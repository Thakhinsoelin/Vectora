using System;
using System.Security.AccessControl;
using Vectora;

namespace Sandbox
{
    public class Player : Entity
    {
        private TransformComponent m_transform;
        private Rigidbody2DComponent m_Rigidbody;

        public float Speed;
        public float Time = 0.0f;
        void OnCreate()
        {
            Console.WriteLine($"Player.Oncreate - id {ID}");
            Console.WriteLine("Recompiled version");
            m_transform = GetComponent<TransformComponent>();
            m_Rigidbody = GetComponent<Rigidbody2DComponent>();
            m_transform.Translation = new Vector3(0.0f);
        }

        void OnUpdate(float ts)
        {
            Time += ts;
            Console.WriteLine($"Player on update: {ts}");
            float speed = Speed;
            Vector3 velocity = Vector3.Zero;

            if(Input.IsKeyDown(KeyCodes.W))
                velocity.Y += 1.0f;
            else if(Input.IsKeyDown(KeyCodes.S))
                velocity.Y -= 1.0f;

            if (Input.IsKeyDown(KeyCodes.A))
                velocity.X -= 1.0f;
            else if (Input.IsKeyDown(KeyCodes.D))
                velocity.X += 1.0f;

            Entity cameraEntity = FindEntityByName("Camera");
            if (cameraEntity != null)
            {
                Camera camera = cameraEntity.As<Camera>();

                if (Input.IsKeyDown(KeyCodes.Q))
                    camera.DistanceFromPlayer += speed * 2.0f * ts;
                else if (Input.IsKeyDown(KeyCodes.E))
                    camera.DistanceFromPlayer -= speed * 2.0f * ts;
            }

            velocity *= speed * ts;

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
