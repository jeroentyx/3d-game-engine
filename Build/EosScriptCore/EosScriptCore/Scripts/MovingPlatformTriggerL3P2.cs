using System;
using System.Collections.Generic;
using System.Text;
using EosScriptCore;

namespace EosScriptCore.Scripts
{
    public class MovingPlatformTriggerL3P2 : MonoBase
    {
        public static MovingPlatformTriggerL3P2 globalController;
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
                triggered = true;
                Log.LogInfo("Player entered trigger");

        }

        void OnTriggerEnd(ulong data)
        {
                triggered = false;
                Log.LogInfo("Player exited trigger");
        }
    }
}
