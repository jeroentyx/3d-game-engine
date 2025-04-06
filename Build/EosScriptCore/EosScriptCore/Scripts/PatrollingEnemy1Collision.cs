using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    class PatrollingEnemy1Collision : MonoBase
    {
        public GameObject virusCollider;
        private GameObject Player;
        //private StateMachine sm = new StateMachine();

        void OnStart()
        {
            virusCollider = gameObject.FindWithTag("Virus01_Collider");
            Player = gameObject.FindWithTag("Surge");
        }

        void OnUpdate()
        {

        }

        private void OnCollisionBegin(ulong data)
        {
            //if (Player.GetComponent<Collider>().oncollisionbegin && virusCollided.GetComponent<Collider>().oncollisionbegin)
            //{
            //    sm.GetNext(Command.Beging);
            //}
        }

        private void OnCollisionEnd(ulong data)
        {

        }


        void OnTriggerBegin(ulong data)
        {
            Log.LogInfo("I Entered");
        }

        void OnTriggerStay(ulong data)
        {
            Log.LogInfo("I Stayed");
        }

        void OnTriggerEnd(ulong data)
        {
            Log.LogInfo("I Left");
        }
    }

}

