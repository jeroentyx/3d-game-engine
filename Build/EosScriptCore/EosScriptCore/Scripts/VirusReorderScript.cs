using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public class VirusReorderScript : MonoBase
    {
        public static VirusReorderScript globalController;
        public AudioComponent audio;
        public List<VirusScript> playerViruses = new List<VirusScript>();
        public List<GameObject> nodes = new List<GameObject>();

        public GameObject Data1;
        public GameObject Data2; 
        public GameObject Data3;
        public GameObject Data4;
        public GameObject Data5;
        public GameObject Data6;

        public GameObject indicator;
        int selection = 0;
        float timer = 0;
        bool selectionMode = true;

        private bool pressOnceQ = false;
        private bool pressOnceE = false;
        private bool pressOnceX = false;
        private bool pressOnceC = false;

        bool uiActive = false;

        

        Vector3 openPos, closedPos;

        // Start is called before the first frame update
        void OnStart()
        {
            openPos = transform.Translation;
            closedPos = transform.Translation + new Vector3(0, 1000, 0);

            globalController = this;

            audio = gameObject.GetComponent<AudioComponent>();

            indicator = gameObject.FindWithTag("UIIndicator");

            Data1 = gameObject.FindWithTag("UIData1");
            Data2 = gameObject.FindWithTag("UIData2");
            Data3 = gameObject.FindWithTag("UIData3");
            Data4 = gameObject.FindWithTag("UIData4");
            Data5 = gameObject.FindWithTag("UIData5");
            Data6 = gameObject.FindWithTag("UIData6");
            
            nodes.Add(Data1);
            nodes.Add(Data2);
            nodes.Add(Data3);
            nodes.Add(Data4);
            nodes.Add(Data5);
            nodes.Add(Data6);
            indicator.SetActive(false);
        }


        void OnUpdate()
        {
            if (uiActive)
            {
                if (playerViruses.Count > 0)
                {
                    Vector3 selectedPos = new Vector3(nodes[selection].transform.Translation.X,
                        nodes[selection].transform.Translation.Y,
                        indicator.transform.Translation.Z);

                    indicator.transform.Translation = selectedPos;

                    if (Input.IsKeyPressed(KeyCode.Q) || Input.IsControllerButtonPressed(ControllerCode.LEFT))
                    {
                        if (!pressOnceQ)
                        {
                            if (selectionMode)
                            {
                                //Move indicator left
                                --selection;
                                if (selection < 0)
                                    selection = playerViruses.Count - 1;
                                if (selection >= playerViruses.Count)
                                    selection = 0;
                            }
                            else
                            {
                                //After locking on to a data, swap it with the left node
                                int targetSlot = selection - 1;
                                if (targetSlot < 0)
                                    targetSlot = playerViruses.Count - 1;
                                if (targetSlot >= playerViruses.Count)
                                    targetSlot = 0;
                                VirusScript temp = playerViruses[targetSlot];
                                playerViruses[targetSlot] = playerViruses[selection];
                                playerViruses[selection] = temp;
                                selection = targetSlot;
                            }
                            audio.PlaySFX("Swapping_Data_UI.ogg");
                            UpdateUIList();
                            pressOnceQ = true;
                        }
                    }

                    else if (Input.IsKeyPressed(KeyCode.E) || Input.IsControllerButtonPressed(ControllerCode.RIGHT))
                    {
                        if (!pressOnceE)
                        {
                            if (selectionMode)
                            {
                                //Move indicator left
                                ++selection;
                                if (selection < 0)
                                    selection = playerViruses.Count - 1;
                                if (selection >= playerViruses.Count)
                                    selection = 0;
                            }
                            else
                            {
                                //After locking on to a data, swap it with the left node
                                int targetSlot = selection + 1;
                                if (targetSlot < 0)
                                    targetSlot = playerViruses.Count - 1;
                                if (targetSlot >= playerViruses.Count)
                                    targetSlot = 0;
                                VirusScript temp = playerViruses[targetSlot];
                                playerViruses[targetSlot] = playerViruses[selection];
                                playerViruses[selection] = temp;
                                selection = targetSlot;
                            }
                            audio.PlaySFX("Swapping_Data_UI.ogg");
                            UpdateUIList();
                            pressOnceE = true;
                        }
                            
                    }

                    else if (Input.IsKeyPressed(KeyCode.X) || Input.IsControllerButtonPressed(ControllerCode.X))
                    {
                        if (!pressOnceX)
                        {
                            selectionMode = !selectionMode;
                            Vector3 temp = indicator.GetComponent<Transform>().Scale;
                            indicator.GetComponent<Transform>().Scale = new Vector3(temp.X, -temp.Y, temp.Z);
                            pressOnceX = true;
                        }
                    }
                    else 
                    {
                        pressOnceE = false;
                        pressOnceQ = false;
                        pressOnceX = false;
                    }

                }
                else
                {
                    indicator.SetActive(false);
                }

            }
            else //If UI not active, do original swap 
            {
                /*
                if (selection < 0)
                    selection = playerViruses.Count - 1;
                if (selection >= playerViruses.Count)
                    selection = 0;
                */
                if (Input.IsKeyPressed(KeyCode.Q) || Input.IsControllerButtonPressed(ControllerCode.LEFT))
                {
                    if (!pressOnceQ)
                    {
                        if (playerViruses.Count > 0)
                        {
                            VirusScript v = playerViruses[0];
                            playerViruses.Remove(v);
                            playerViruses.Insert(playerViruses.Count, v);
                        }
                        audio.PlaySFX("Swapping_Data_UI.ogg");
                        pressOnceQ = true;
                    }

                }

                else if (Input.IsKeyPressed(KeyCode.E) || Input.IsControllerButtonPressed(ControllerCode.RIGHT))
                {
                    if (!pressOnceE)
                    {
                        if (playerViruses.Count > 0)
                        {
                            VirusScript v = playerViruses[playerViruses.Count - 1];
                            playerViruses.Remove(v);
                            playerViruses.Insert(0, v);
                        }
                        audio.PlaySFX("Swapping_Data_UI.ogg");
                        pressOnceE = true;
                    }
                }
                else
                {
                    pressOnceQ = false;
                    pressOnceE = false;
                }
            }

            if (Input.IsKeyPressed(KeyCode.C) || Input.IsControllerButtonPressed(ControllerCode.Y))
            {
                if (!pressOnceC)
                {
                    uiActive = !uiActive;
                    pressOnceC = true;
                }
            }
            else 
            {
                pressOnceC = false;
            }
            transform.Translation = uiActive ? openPos : closedPos;
            Data1.SetActive(uiActive);
            Data2.SetActive(uiActive);
            Data3.SetActive(uiActive);
            Data4.SetActive(uiActive);
            Data5.SetActive(uiActive);
            Data6.SetActive(uiActive);
            if (playerViruses.Count > 0)
            {
               indicator.SetActive(uiActive);
            }
                
        }


        void OnFixedUpdate()
        {

        }

        void OnTriggerBegin(ulong data)
        {
            
        }

        void OnTriggerStay(ulong data)
        {

        }

        void OnTriggerEnd(ulong data)
        {

        }
        public void AddVirus(VirusScript v)
        {
            playerViruses.Add(v);
            UpdateUIList();
        }

        public void RemoveVirus(VirusScript v)
        {
            playerViruses.Remove(v);
            UpdateUIList();
        }


        public int GetVirusIndex(VirusScript v)
        {
            return playerViruses.FindIndex(a => a == v);
        }

        public void UpdateUIList()
        {
            for(int i = 0; i < nodes.Count; ++i)
            {
                UIComponent currNode = nodes[i].GetComponent<UIComponent>();
                if (i < playerViruses.Count)
                {
                    VirusScript v = playerViruses[i];
                    switch (v.currentcolor) 
                    {
                        case Color.Blue:
                            currNode.SetColor(ColorVec.blue);
                            break;
                        case Color.Yellow:
                            currNode.SetColor(ColorVec.yellow);
                            break;
                        case Color.Red:
                            currNode.SetColor(ColorVec.red);
                            break;
                    }
                }
                else 
                {
                    currNode.SetColor(ColorVec.gray);
                }
            }
        }


    }
}
