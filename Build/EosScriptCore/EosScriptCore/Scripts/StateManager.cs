using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public abstract class StateManager : MonoBase
    {
        public State currentState;

        void OnStart()
        {

        }

        void OnUpdate()
        {
            RunStateMachine();
        }

        void OnFixedUpdate()
        {

        }

        private void RunStateMachine()
        {
            State nextState = currentState.RunCurrentState();

            if (nextState != null)
            {
                SwitchToNextState(nextState);
            }
        }

        private void SwitchToNextState(State nextState)
        {
            currentState = nextState;
        }


    }
}
