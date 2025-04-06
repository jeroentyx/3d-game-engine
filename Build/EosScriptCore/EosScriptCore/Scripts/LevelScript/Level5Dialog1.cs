using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    class Level5Dialog1 : MonoBase
    {
        private Collider DialogCollider;
        private AudioComponent AudioComp;
        private bool Trigger = false;

        GameObject player;

        void OnStart()
        {
            player = gameObject.FindWithTag("Surge");
            DialogCollider = gameObject.GetComponent<Collider>();
            AudioComp = gameObject.GetComponent<AudioComponent>();
        }

        void OnUpdate()
        {

        }

        void OnTriggerBegin(ulong entity)
        {
            if (Trigger == false && player.GetComponent<Collider>().isTrigger)
            {
                Log.LogInfo("Entered");
                Audio.DecreaseSFXvol();
                Audio.DecreaseSFXvol();
                AudioComp.PlaySFX("Level5_Surge_E.wav");
                Audio.IncreaseSFXvol();
                Audio.IncreaseSFXvol();
                Trigger = true;
            }
        }
    }
}
