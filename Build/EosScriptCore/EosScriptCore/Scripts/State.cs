using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public abstract class State : MonoBase
    {
        public abstract State RunCurrentState();

        void OnStart()
        {

        }
        void OnUpdate()
        {

        }

        void OnFixedUpdate()
        {

        }

        void OnTriggerBegin(ulong data)
        {

        }

        void onTriggerStay(ulong data)
        {

        }

        void OnTriggerEnd(ulong data)
        {

        }

    }
}
