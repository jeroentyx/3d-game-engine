namespace EosScriptCore.Scripts
{
    public class TeleportSingle : MonoBase 
    {
        
        public GameObject player;
        public GameObject teleportMain;

        public float timer = 3000f;
        public bool countdownTimer = false;

        public bool red = false;
        public bool blue = false;
        public bool yellow = false;

        //Need OnStart to find player etc.
        void OnStart() 
        {
            player = gameObject.FindWithTag("Surge");
            teleportMain = gameObject.FindWithTag("Teleport_Base01");
        }

        void OnUpdate() 
        {
            if (countdownTimer == true) 
            {
                timer -= Time.deltaTime;
            }

            if (countdownTimer == false) 
            {
                timer = 3000f;
            }
        }

        void OnTriggerBegin(ulong data) 
        {
            bool mainActive = false;

            if (TeleportMultiple.globalController.red && red)
            {
                mainActive = true;
            }
            if (TeleportMultiple.globalController.blue && blue)
            {
                mainActive = true;
            }
            if (TeleportMultiple.globalController.yellow && yellow)
            {
                mainActive = true;
            }

            if (player.GetComponent<Collider>().isTrigger)
            {
                /*
                countdownTimer = true;

                if (timer <= 0 && mainActive)
                {
                    player.GetComponent<RigidBody>().MovePosition(teleportMain.transform.Translation);
                }
                */
                if (mainActive) 
                {
                    if (PlayerMovement.globalController.teleported == false)
                    {
                        PlayerMovement.globalController.teleported = true;
                        player.GetComponent<RigidBody>().MovePosition(teleportMain.transform.Translation);
                        
                    }
                    else 
                    {
                        PlayerMovement.globalController.teleported = false;
                    }
                    
                }

            }
        }

        void OnTriggerEnd(ulong data)
        {
            /*
            
            countdownTimer = false;
            if (player.GetComponent<Collider>().isTrigger)
            {
                
            }
            */
            Log.LogInfo("OnTriggerExit Triggered SINGLE");
        }
    }
}