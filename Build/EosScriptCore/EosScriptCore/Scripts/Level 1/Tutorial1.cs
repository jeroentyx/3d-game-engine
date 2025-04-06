using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    class Tutorial1 : MonoBase
    {
        private bool displayTutorial = false;
        private GameObject player;
        private GameObject tutorialImage;

        public float scaledfactor;
        private float scaleCap;


        void OnStart()
        {
            player = gameObject.FindWithTag("Surge");
            tutorialImage = gameObject.FindWithTag("TutorialImage1");
            tutorialImage.SetActive(false);
           


        }

        void OnUpdate()
        {
            if (displayTutorial == true)
            {
                tutorialImage.SetActive(true);
                Enlarge();
            }
            else
            {
                Shrink();
                
            }
        }

        void OnTriggerBegin(ulong entity)
        {
            if (player.GetComponent<Collider>().isTrigger)
            {
                displayTutorial = true;
                //SceneCamera.SceneController.ActivateCutscene();
            }
        }

        void OnTriggerEnd(ulong entity)
        {
            if (!player.GetComponent<Collider>().isTrigger)
            {
                displayTutorial = false;
                //SceneCamera.SceneController.DeactivateCutscene();
            }
        }

        void Shrink()
        {
            if (tutorialImage.transform.Scale.X >= 0.0f || tutorialImage.transform.Scale.Y >= 0.0f)
            {
                tutorialImage.transform.Scale -= scaledfactor * Time.deltaTime;
                Log.LogInfo(tutorialImage.transform.Scale.ToString());
            }

            if(tutorialImage.transform.Scale.X <= 0.0f || tutorialImage.transform.Scale.Y <= 0.0f)
            {
                tutorialImage.SetActive(false);
            }
        }


        void Enlarge()
        {
            if (tutorialImage.transform.Scale.X <= 0.75f || tutorialImage.transform.Scale.Y <= 0.75f)
            {
                tutorialImage.transform.Scale += scaledfactor * Time.deltaTime;
               // Log.LogInfo(tutorialImage.transform.Scale.ToString());
            }
        }
    }
}
