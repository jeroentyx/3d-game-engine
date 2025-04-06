using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    class Tutorial2 : MonoBase
    {
        private bool displayTutorial = false;
        private GameObject player;
        private GameObject tutorialImage2;

        void OnStart()
        {
            player = gameObject.FindWithTag("Surge");
            tutorialImage2 = gameObject.FindWithTag("TutorialImage2");
            tutorialImage2.SetActive(false);
        }

        void OnUpdate()
        {
            if (displayTutorial == true)
            {
                tutorialImage2.SetActive(true);
            }
            else
            {
                tutorialImage2.SetActive(false);
            }
        }

        void OnTriggerBegin(ulong entity)
        {
            if (player.GetComponent<Collider>().isTrigger)
            {
                //Log.LogInfo("Entered");
                displayTutorial = true;
            }
        }

        void OnTriggerEnd(ulong entity)
        {
            if (player.name == "Surge")
            {
                //Log.LogInfo("Exited");
                displayTutorial = false;

            }
        }
    }
}
