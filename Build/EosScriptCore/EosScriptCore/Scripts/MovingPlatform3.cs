using System;
using System.Collections.Generic;
using System.Text;
using EosScriptCore;

namespace EosScriptCore.Scripts
{
    public class MovingPlatform3 : MonoBase
    {
        public Transform destinationTransform3;

        Vector3 startPos3;

        public float moveSpeed3 = 250f;
        public float stopTime3 = 1f;

        float stopTimer3 = 0;
        int direction3 = 1;

        public Transform transformComponent3;
        public RigidBody rb3;
        private RigidBody PlayerBody;
        private GameObject destObject3;
        private GameObject player;
        private Vector3 newPos;

        #region Newly-added variables
        #endregion
        private bool onplat;

        void OnStart()
        {
            transformComponent3 = gameObject.GetComponent<Transform>();
            startPos3 = gameObject.GetComponent<Transform>().Translation;
            rb3 = gameObject.GetComponent<RigidBody>();
            destObject3 = gameObject.FindWithTag("Destin_Target3");
            player = gameObject.FindWithTag("Surge");
            PlayerBody = player.GetComponent<RigidBody>();
            destinationTransform3 = destObject3.GetComponent<Transform>();
        }

        void OnUpdate()
        {  
        }

        void OnFixedUpdate()
        {
            if (stopTimer3 > 0)
            {
                stopTimer3 -= Physics.GetFixedTimestep();
                return;
            }

            if (direction3 == 1)
            {
               
                newPos = Vector3.MoveTowards(transformComponent3.Translation, destinationTransform3.Translation, moveSpeed3 * Physics.GetFixedTimestep());
                rb3.AddForce((newPos - transformComponent3.Translation) * moveSpeed3);

                if (MovingPlatformTrigger3.globalController.triggered)
                {
                    PlayerBody.SetLinearVelocity(PlayerBody.GetLinearVelocity() + (newPos - transformComponent3.Translation) / 10.5f);
                }

                if (transformComponent3.Translation.X <= 8.8f)
                {
                    Log.LogInfo("Changed direction3");
                    stopTimer3 = stopTime3;
                    direction3 = -1;
                }

    
            }

            if (direction3 == -1)
            {
                //Log.LogInfo("Reversed3");
                newPos = Vector3.MoveTowards(transformComponent3.Translation, startPos3, moveSpeed3 * Physics.GetFixedTimestep());
                rb3.AddForce((newPos - transformComponent3.Translation) * moveSpeed3);

                if (MovingPlatformTrigger3.globalController.triggered)
                {
                    PlayerBody.SetLinearVelocity(PlayerBody.GetLinearVelocity() + (newPos - transformComponent3.Translation) / 10.5f);
                }

                if (transformComponent3.Translation.X >= 26.3f)
                {
                    stopTimer3 = stopTime3;
                    direction3 = 1;
                }
            }
        }

        void OnTriggerBegin(ulong data)
        {
            if (player.GetComponent<Collider>().isTrigger)
            {
                Log.LogInfo("High");
                PlayerBody.UseGravity(false);
                onplat = true;

            }
        }

        void OnTriggerEnd(ulong data)
        {
            if (!player.GetComponent<Collider>().isTrigger)
            {
                Log.LogInfo("Left2");
                PlayerBody.UseGravity(true);
                onplat = false;
            }
        }
    }
}
