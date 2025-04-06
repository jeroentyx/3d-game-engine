using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public class ChasingState : State
    {
        public GettingBack gettingBackState;
        public bool CollidedWithPlayerColliderBox;
        public override State RunCurrentState()
        {
            Log.LogDebug("Chasing Player!");

            if (!CollidedWithPlayerColliderBox)
            {
                return gettingBackState;
            }
            else
            {
                // infect player data here if collided
                // put logic for infecting first data of player here
                // return this is for not getting error for abstract class
                return this;
            }
        }
    }
}
