using System;
using System.Collections.Generic;
using System.Text;
using EosScriptCore;

namespace EosScriptCore.Scripts
{
    public class MovingPlatformL3P1 : MonoBase
    {
        //Grab it from a gameobject for now
        public Transform destinationTransform;


        Vector3 startPos;

        public float moveSpeed = 250f;
        public float stopTime = 1f;
        float stopTimer = 0;
        int direction = -1;
        public Transform transformComponent;
        private RigidBody rb;
        private RigidBody PlayerBody;
        private Transform PlayerTransform;
        private GameObject destObject;
        private GameObject player;
        private Vector3 newPos;
        private bool onplat;
       

        void OnStart()
        {
            transformComponent = gameObject.GetComponent<Transform>();
            startPos = gameObject.GetComponent<Transform>().Translation;
            rb = gameObject.GetComponent<RigidBody>();
            destObject = gameObject.FindWithTag("Destination_L3P1");
            player = gameObject.FindWithTag("Surge");
            PlayerBody = player.GetComponent<RigidBody>();
            PlayerTransform = player.GetComponent<Transform>();
            destinationTransform = destObject.GetComponent<Transform>();

           

        }

        void OnUpdate()
        {
           
       

        }


        void OnFixedUpdate()
        {

            if (stopTimer > 0)
            {
                stopTimer -= Physics.GetFixedTimestep();
                return;
            }

            if (direction == 1)
            {

                newPos = Vector3.MoveTowards(transformComponent.Translation, destinationTransform.Translation, moveSpeed * Physics.GetFixedTimestep());
                rb.AddForce((newPos - transformComponent.Translation) * moveSpeed);

                if (MovingPlatformTriggerL3P1.globalController.triggered)
                {
                    PlayerBody.SetLinearVelocity(PlayerBody.GetLinearVelocity() + (newPos - transformComponent.Translation) / 8.0f);
                }

                if (transformComponent.Translation.Z <= destinationTransform.Translation.Z)
                {
                    //Log.LogInfo("Changed direction");
                    stopTimer = stopTime;
                    direction = -1;
                }

                //rb.AddForce(appliedForceZ);
            }

            if (direction == -1)
            {

                newPos = Vector3.MoveTowards(transformComponent.Translation, startPos, moveSpeed * Physics.GetFixedTimestep());
                rb.AddForce((newPos - transformComponent.Translation) * moveSpeed);

                if (MovingPlatformTriggerL3P1.globalController.triggered)
                {
                    PlayerBody.SetLinearVelocity(PlayerBody.GetLinearVelocity() + (newPos - transformComponent.Translation) / 8.0f);
                }

                if (transformComponent.Translation.Z >= startPos.Z)
                {
                    stopTimer = stopTime;
                    direction = 1;
                }

                //rb.AddForce(-appliedForceZ);
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
