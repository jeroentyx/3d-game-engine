﻿using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public class Gate02 : MonoBase
    {


        private List<Color> gateRequirement = new List<Color>(); //gate colour

        private GameObject gateFence;
        private GameObject gateCollider;
        private GameObject player;
        private AudioComponent audio;

        private Color pos1;

        private bool allChecked = false;
        private bool gateUnlock = false;

        private int gateKey = 0;

        // Start is called before the first frame update
        void OnStart()
        {
            gateFence = gameObject.FindWithTag("Gate02_Fence");
            gateCollider = gameObject.FindWithTag("FenceCollider02");
            player = gameObject.FindWithTag("Surge");
            audio = gameObject.GetComponent<AudioComponent>();

            pos1 = Color.Blue;

            gateRequirement.Add(pos1);
        }

        // Update is called once per frame
        void OnUpdate()
        {
            if (gateKey == 0)
            {
                GateKey(gateRequirement);
            }

            if (gateUnlock == true)
            {
                gateFence.SetActive(false);
                gateCollider.SetActive(false);
            }
        }

        void OnFixedUpdate()
        {

        }

        void GateKey(List<Color> gateRequirement)
        {
            for (int i = 0; i < gateRequirement.Count; i++)
            {
                // need to update later for the corrupted data
                
                gateKey++;
            }

            //Log.LogInfo("Gatekey"+gateKey.ToString());
        }

        void OnTriggerBegin(ulong data)
        {
            {
                if (VirusReorderScript.globalController.playerViruses.Count >= gateKey) //if player has more data than the gate require
                {
                   // Log.LogInfo("Checking2");

                    for (int i = 0; i < gateKey; i++)
                    {
                        if ((int)VirusReorderScript.globalController.playerViruses[i].currentcolor == (int)gateRequirement[i]) //colour matches
                        {
                            allChecked = true;
                            //Log.LogInfo("Checking3");
                            Audio.DecreaseSFXvol();
                            Audio.DecreaseSFXvol();
                            audio.PlaySFX("Gate_Open.ogg");
                            Audio.IncreaseSFXvol();
                            Audio.IncreaseSFXvol();
                        }
                        else
                        {
                            //Log.LogInfo("Checking4");
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



