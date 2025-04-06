using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
   public  class PatrolState : State
    {
        public ChasingState chasingState;
        public bool TriggeredWithPlayerTriggerBox;

        public override State RunCurrentState()
        {
            Log.LogDebug("Patrolling!");

            if (TriggeredWithPlayerTriggerBox)
            {
                return chasingState;
            }
            else
            {
                return this;
            }
        }
    }
}
