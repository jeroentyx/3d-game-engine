using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public class Bin_Collider : MonoBase
    {
        private bool displayRestored = false;
        private GameObject player;
        private GameObject restoredImage;
        private float timer = 0f;
        private bool timerStart = false;
        private float delta;
        //public Vector3 temp;

        // Numeber of datas in the current map
        //public GameObject normal_data_red1;
        //public GameObject normal_data_red2;
        //public GameObject normal_data_yellow1;
        //public GameObject normal_data_blue1;
        //public GameObject normal_data_blue2;

        //public GameObject corrupted_data_red1;
        //public GameObject corrupted_data_red2;
        //public GameObject corrupted_data_blue1;
        //public GameObject corrupted_data_blue2;
        //public GameObject corrupted_data_yellow1;


        void OnStart()
        {
            player = gameObject.FindWithTag("Surge");
            restoredImage = gameObject.FindWithTag("RestoredImage");
            restoredImage.SetActive(false);

            //normal_data_red1 = gameObject.FindWithTag("Data_red1");
            //normal_data_red2 = gameObject.FindWithTag("Data_red2");
            //normal_data_blue1 = gameObject.FindWithTag("Data_blue1");
            //normal_data_blue2 = gameObject.FindWithTag("Data_blue2");
            //normal_data_yellow1 = gameObject.FindWithTag("Data_yellow1");

            //corrupted_data_red1 = gameObject.FindWithTag("corrupted_Data_red1");
            //corrupted_data_yellow1 = gameObject.FindWithTag("corrupted_Data_yellow1");
            //corrupted_data_blue1 = gameObject.FindWithTag("corrupted_Data_blue1");
            //corrupted_data_red2 = gameObject.FindWithTag("corrupted_Data_red2");
            //corrupted_data_blue2 = gameObject.FindWithTag("corrupted_Data_blue2");

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
