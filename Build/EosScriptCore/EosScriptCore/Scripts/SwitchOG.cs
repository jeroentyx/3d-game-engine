using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public class SwitchOG : MonoBase
    {

        public static bool enterEven;
        public static bool orangeActive;
        public static bool greenActive;
        public GameObject[] orangeTiles;
        public GameObject[] greenTiles;

        private GameObject player;
        private AudioComponent Sfx;
        void OnStart()
        {
            enterEven = false;
            orangeTiles = gameObject.FindGameObjectsWithTag("OrangeTile");
            greenTiles = gameObject.FindGameObjectsWithTag("GreenTile");
            player = gameObject.FindWithTag("Surge");
            Sfx = gameObject.GetComponent<AudioComponent>();

            orangeActive = false;
            greenActive = true;
        }

        void OnUpdate()
        {
            if (orangeActive == false && greenActive == true)
            {
                foreach (GameObject o in orangeTiles)
                {
                    o.SetActive(true);
                }

                foreach (GameObject o in greenTiles)
                {
                    o.SetActive(false);
                }

            }
            if(orangeActive == true && greenActive == false)
            {

                foreach (GameObject o in greenTiles)
                {
                    o.SetActive(true);
                }


                foreach (GameObject o in orangeTiles)
                {
                    o.SetActive(false);
                }
            }
        }

        void OnFixedUpdate()
        {

        }

        void OnTriggerBegin(ulong data)
        {
            Log.LogInfo(data.ToString());
            if (player.GetComponent<Collider>().isTrigger && gameObject.GetComponent<Collider>().isTrigger)
            {
                Sfx.PlaySFX("Alternate_Tile_Change.wav");

                if (orangeActive == false)
                {
                    orangeActive = true;
                    greenActive = false;
                }
                else if (greenActive == false)
                {
                    orangeActive = false;
                    greenActive = true;
                }
            }

        }

  
    }
}
