using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public class Bin_Collider3 : MonoBase
    {
        private bool displayRestored = false;
        private GameObject player;
        private GameObject restoredImage;
        private float timer = 0f;
        private bool timerStart = false;
        private float delta;

        void OnStart()
        {
            player = gameObject.FindWithTag("Surge");
            restoredImage = gameObject.FindWithTag("RestoredImage");
            restoredImage.SetActive(false);
        }

        void OnUpdate()
        {
            
        }

        void OnFixedUpdate()
        {
            delta = Physics.GetFixedTimestep() * 0.2f;

            if (timerStart == true)
            {
                timer += Physics.GetFixedTimestep();
            }

        }

        void OnTriggerBegin(ulong entity)
        {
            if (player.GetComponent<Collider>().isTrigger && gameObject.GetComponent<Collider>().isTrigger)
            {
                restoredImage.SetActive(true);

                for (int i = 0; i < VirusReorderScript.globalController.playerViruses.Count; ++i)
                {
                    if (VirusReorderScript.globalController.playerViruses[i].currentcolor == Color.Corrupted_Red)
                    {
                        // Swap position with available Red data
                        VirusReorderScript.globalController.playerViruses[i].UnCorrupted();
                        //Instantiate("red-data", normal_data_red.transform);
                        Log.LogInfo("Corrupted Red Data!");
                        //Destroy(VirusReorderScript.globalController.playerViruses[i]);
                    }
                    if (VirusReorderScript.globalController.playerViruses[i].currentcolor == Color.Corrupted_Yellow)
                    {
                        // Swap position with available Yellow data
                        VirusReorderScript.globalController.playerViruses[i].UnCorrupted();
                        //Instantiate("yellow-data", normal_data_yellow.transform);
                        Log.LogInfo("Corrupted Yellow Data!");
                        //Destroy(VirusReorderScript.globalController.playerViruses[i]);
                    }
                    if (VirusReorderScript.globalController.playerViruses[i].currentcolor == Color.Corrupted_Blue)
                    {
                        // Swap position with available Blue data
                        VirusReorderScript.globalController.playerViruses[i].UnCorrupted();
                        //Instantiate("blue-data", normal_data_blue.transform);
                        Log.LogInfo("Corrupted Blue Data!");
                        // Destroy(VirusReorderScript.globalController.playerViruses[i]);
                    }


                }
            }
        }

        void OnTriggerEnd(ulong entity)
        {
            if (player.name == "Surge")
            {
                restoredImage.SetActive(false);

            }
        }

    }
}
