using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public class HugeMovingBlock2 : MonoBase
    {
        public Transform destTransform;
        Vector3 startingPos;
        public float moveSpeed = 250f;
        public float stopTime = 2f;
        public float accel = 1.5f;
        float stopTimer = 0;
        int direction = 1;

        public Transform transformComponent;
        public RigidBody rb;
        private GameObject destinationObject;
        private Vector3 newPos;

        void OnStart()
        {
            transformComponent = gameObject.GetComponent<Transform>();
            startingPos = gameObject.GetComponent<Transform>().Translation;
            rb = gameObject.GetComponent<RigidBody>();
            destinationObject = gameObject.FindWithTag("Huge_BlockDest2");

            destTransform = destinationObject.GetComponent<Transform>();
        }

        void OnUpdate()
        {


        }

        void OnFixedUpdate()
        {
            if (stopTimer > 0)
            {
                stopTimer -= Time.deltaTime;
                return;
            }

            if (direction == 1)
            {
                newPos = Vector3.MoveTowards(transformComponent.Translation, destTransform.Translation, moveSpeed * Time.deltaTime);
                rb.AddForce((newPos - transformComponent.Translation) * moveSpeed * accel);

                if (transformComponent.Translation.X <= destTransform.Translation.X)
                {
                    //Log.LogInfo("Changed direction");
                    stopTimer = stopTime;
                    direction = -1;
                }

            }

            if (direction == -1)
            {
                newPos = Vector3.MoveTowards(transformComponent.Translation, startingPos, moveSpeed * Time.deltaTime);
                rb.AddForce((newPos - transformComponent.Translation) * moveSpeed * accel);

                if (transformComponent.Translation.X >= startingPos.X)
                {
                    stopTimer = stopTime;
                    direction = 1;
                }

     
            }
        }

    }
}
