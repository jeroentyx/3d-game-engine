using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public class CorruptData : MonoBase
    {
        public static CorruptData globalController;
        //public List<VirusDummy_Follow1> playerViruses;

        private GameObject player;
        //private AudioComponent Sfx;
        void onStart()
        {
            //playerViruses = new List<VirusDummy_Follow1>();
            player = gameObject.FindWithTag("Surge");
            //Sfx = gameObject.GetComponent<AudioComponent>();

        }

        private void OnTriggerBegin(ulong data)
        {
            if (player.GetComponent<Collider>().isTrigger && gameObject.GetComponent<Collider>().isTrigger)
            {
                //Sfx.PlaySFX("CorruptedData.wav");
                Log.LogInfo("The virus corrupted 1 of your data!");
                CorruptData.globalController.CorruptVirus();

            }
        }

        public void CorruptVirus()
        {
            //    foreach(VirusDummy_Follow1 v in playerViruses)
            //    if (!v.corruptedVariant)
            //    {
            //        v.Corrupt();
            //        return;
            //    }
            //
        }
    }
}
