using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public class Lvl4Gate01 : MonoBase
    {
        private List<Color> gateRequirement = new List<Color>();

        private GameObject gateFence;
        private GameObject gateFenceCollider;
        private GameObject player;
        private AudioComponent audio;

        private Color pos1, pos2, pos3, pos4, pos5, pos6;

        private bool allChecked = false;
        private bool gateUnlock = false;

        private int gateKey = 0;

        void OnStart()
        {
            gateFence = gameObject.FindWithTag("GateFence01");
            gateFenceCollider = gameObject.FindWithTag("Gate01_Fence_Collider");
            player = gameObject.FindWithTag("Surge");
            audio = gameObject.GetComponent<AudioComponent>();

            pos1 = Color.Red;
            pos2 = Color.Yellow;
            pos3 = Color.Blue;
            pos4 = Color.Yellow;
            pos5 = Color.Yellow;
            pos6 = Color.Red;

            gateRequirement.Add(pos1);
            gateRequirement.Add(pos2);
            gateRequirement.Add(pos3);
            gateRequirement.Add(pos4);
            gateRequirement.Add(pos5);
            gateRequirement.Add(pos6);
        }

        void OnUpdate()
        {
            if (gateKey == 0)
            {
                GateKey(gateRequirement);
            }

            if (gateUnlock == true)
            {
                gateFence.SetActive(false);
                gateFenceCollider.SetActive(false);
            }
        }

        void OnFixedUpdate()
        {

        }

        void GateKey(List<Color> gateRequirement)
        {
            for (int i = 0; i < gateRequirement.Count; i++)
            {
                gateKey++;
            }

            Log.LogInfo("Gatekey" + gateKey.ToString());
        }

        void OnTriggerBegin(ulong data)
        {
            {
                if (VirusReorderScript.globalController.playerViruses.Count >= gateKey)
                {
                    Log.LogInfo("Checking2");

                    for (int i = 0; i < gateKey; i++)
                    {
                        if ((int)VirusReorderScript.globalController.playerViruses[i].currentcolor == (int)gateRequirement[i]) //colour matches
                        {
                            allChecked = true;
                            Log.LogInfo("Checking3");
                            Audio.SetSFXVol(0.1f);
                            audio.PlaySFX("Gate_Open.ogg");
                            Audio.SetSFXVol(0.7f);
                        }
                        else
                        {
                            Log.LogInfo("Checking4");
                            allChecked = false;
                            audio.PlaySFX("Gate_Deny.ogg");
                            break;
                        }
                    }
                }

                if (allChecked == true)
                {
                    gateUnlock = true;
                }
            }
        }

        void OnTriggerStay(ulong data)
        {

        }

        void OnTriggerEnd(ulong data)
        {

        }

    }
}
