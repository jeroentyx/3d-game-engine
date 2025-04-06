using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    class Level1Dialog3 : MonoBase
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
                SceneCamera.SceneController.ActivateCutscene();
                Log.LogInfo("Entered");
                Audio.SetSFXVol(0.5f);
                AudioComp.PlaySFX("Level1B_Surge_E.wav");
                Audio.SetSFXVol(1.0f);
                Trigger = true;
            }

          
        }

        void OnTriggerEnd(ulong entity)
        {
            if (Trigger == true && !player.GetComponent<Collider>().isTrigger)
            {
                SceneCamera.SceneController.DeactivateCutscene();
                //Trigger = false;
            }
        }
    }
}
