using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public class CorruptTile : MonoBase
    {
        public GameObject player;
        public float maxTime = 3;
        public float timer = 0;

        void OnStart()
        {
            player = gameObject.FindWithTag("Surge");
        }

        void OnTriggerBegin(ulong data)
        {
            Log.LogInfo(data.ToString());
            if (player.GetComponent<Collider>().isTrigger && gameObject.GetComponent<Collider>().isTrigger)
            {
                //Sfx.PlaySFX("Alternate_Tile_Change.wav");
                PlayerMovement.globalController.HalfSpeed(true);
                timer+=Physics.GetFixedTimestep();
                if(timer > maxTime)
                {
                    //Die
                    PlayerMovement.globalController.SurgeDie(true);
                    timer = 0;
                }
                //Particle Effects here

            }
    
        }

        void OnTriggerEnd(ulong data)
        {
            PlayerMovement.globalController.HalfSpeed(false);
            //Reset Timer
            timer = 0;
        }
    }

}