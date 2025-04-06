using System.Collections;
using System.Collections.Generic;

namespace EosScriptCore.Scripts
{
    public class TeleportMultiple : MonoBase
    {
        public static TeleportMultiple globalController;
        public GameObject player;

        //public Transform teleportTargetRed;
        //public Transform teleportTargetBlue;
        //public Transform teleportTargetYellow;

        public GameObject teleportTargetRed;
        public GameObject teleportTargetBlue;
        public GameObject teleportTargetYellow;

        public GameObject PlatformRedMode;
        public GameObject PlatformBlueMode;
        public GameObject PlatformYellowMode;
        public GameObject PlatformNULLMode;


        public bool red;
        public bool yellow;
        public bool blue;

        public float timer = 3000f;
        public bool countdownTimer = false;

        //MeshRenderer renderer;

        void OnStart() 
        {
            globalController = this;
            player = gameObject.FindWithTag("Surge");
            teleportTargetRed = gameObject.FindWithTag("Teleport_Red");
            teleportTargetBlue = gameObject.FindWithTag("Teleport_Blue");
            teleportTargetYellow = gameObject.FindWithTag("Teleport_Yellow");

            PlatformRedMode = gameObject.FindWithTag("Teleport_BaseRed");
            PlatformBlueMode = gameObject.FindWithTag("Teleport_BaseBlue");
            PlatformYellowMode = gameObject.FindWithTag("Teleport_BaseYellow");
            PlatformNULLMode = gameObject.FindWithTag("Teleport_Base01");
        }

        public void UpdateColor(Color uc)
        {
            //Material c = renderer.material;
            //c.color = uc;
            //renderer.material = c;
            PlatformNULLMode.SetActive(false);
            PlatformYellowMode.SetActive(false);
            PlatformBlueMode.SetActive(false);
            PlatformRedMode.SetActive(false);

            if (uc == Color.White)
            {
                PlatformNULLMode.SetActive(true);
            }
            else if (uc == Color.Red)
            {
                PlatformRedMode.SetActive(true);
            }
            else if (uc == Color.Blue) 
            {
                PlatformBlueMode.SetActive(true);
            }
            else if (uc == Color.Yellow)
            {
                PlatformYellowMode.SetActive(true);
            }

        }

        public void OnUpdate() 
        {
            if (countdownTimer == true)
            {
                timer -= Time.deltaTime;
            }

            if (countdownTimer == false)
            {
                timer = 3000f;
            }
            //Log.LogInfo(timer.ToString());
        }

        void OnTriggerBegin(ulong data)
        {
            if (player.GetComponent<Collider>().isTrigger)
            {
                if (PlayerMovement.globalController.teleported == false) 
                {
                    if (red == true)
                    {
                        //countdownTimer = true;
                        PlayerMovement.globalController.teleported = true;
                        player.GetComponent<RigidBody>().MovePosition(teleportTargetRed.transform.Translation);
                        
                        if (timer <= 0)
                        {

                        }
                    }

                    else if (yellow == true)
                    {
                        //countdownTimer = true;
                        PlayerMovement.globalController.teleported = true;
                        player.GetComponent<RigidBody>().MovePosition(teleportTargetYellow.transform.Translation);
                        
                        if (timer <= 0)
                        {

                        }
                    }
                    else if (blue == true)
                    {
                        //countdownTimer = true;
                        PlayerMovement.globalController.teleported = true;
                        player.GetComponent<RigidBody>().MovePosition(teleportTargetBlue.transform.Translation);
                        
                        if (timer <= 0)
                        {

                        }
                    }
                }
                else
                {
                    PlayerMovement.globalController.teleported = false;
                }
                

            }
        }

        void OnTriggerEnd(ulong data)
        {
            /*
            Log.LogInfo("OnTriggerExit Triggered MULTIPLE");
            countdownTimer = false;
            if (player.GetComponent<Collider>().isTrigger)
            {
                
            }
            */
            Log.LogInfo("OnTriggerExit Triggered MULTIPLE");
            if (PlayerMovement.globalController.teleported == true) 
            {
            }
            //enter = true;
        }

    }
}