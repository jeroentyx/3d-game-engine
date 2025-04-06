using System;
using System.Collections.Generic;
using System.Text;
using EosScriptCore;

namespace EosScriptCore.Scripts
{
    public class MovingPlatformTrigger3 : MonoBase
    {
        public static MovingPlatformTrigger3 globalController;
        public bool triggered = false;
        private GameObject player;

        void OnStart()
        {
            player = gameObject.FindWithTag("Surge");
            globalController = this;
        }

        void OnUpdate()
        {
        }


        void OnFixedUpdate()
        {
            
        }

        void OnTriggerBegin(ulong data)
        {
            if (player.GetComponent<Collider>().isTrigger)
                triggered = true;
      
        }

        void OnTriggerEnd(ulong data)
        {

         
                triggered = false;

        }
    }
}
