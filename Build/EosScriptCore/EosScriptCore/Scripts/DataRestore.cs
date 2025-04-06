using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public class DataRestore : MonoBase
    {
        public GameObject Player;
        public GameObject BinCollider;
        public List<Vector3> poslist = new List<Vector3>();

        void onStart()
        {
            Player = gameObject.FindWithTag("Surge");
            BinCollider = gameObject.FindWithTag("Bin_Collider");

        }

        void OnUpdate()
        {
            
        }

        private void OnCollisionBegin(ulong data)
        {
            //if (Player.GetComponent<Collider>().oncollisionbegin && BinCollider.GetComponent<Collider>().oncollisionbegin)
            //{
            //      if (all the virus is affect or 1 is affect)
            //      {
            //          change all the virus in the array from corrupt back to normal
            //      }
            //      else
            //      { 
            //          do nothing
            //      }
            //}
        }
        private void OnCollisionEnd(ulong data)
        {
            //if (!Player.GetComponent<Collider>().oncollisionbegin && BinCollider.GetComponent<Collider>().oncollisionbegin)
            //{
            //  // Do nothing

            //}

        }
    }
}
