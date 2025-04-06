using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public class GettingBack : State
    {
        public override State RunCurrentState()
        {
            Log.LogDebug("Getting Back!");

            return this;
        }
     }
}
