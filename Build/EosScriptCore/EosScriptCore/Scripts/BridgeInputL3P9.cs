using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
	public class BridgeInputL3P9 : MonoBase
	{
		List<VirusScript> inputs = new List<VirusScript>();
		
		public BridgeSourceL3P8 linkedBridge;
		//List<Color> colorList;
		public GameObject detect;
		public GameObject player;

        bool canUpdate = false;
		bool inputFlag = false;
		
		void UpdateBridge()
		{
            List<Color> colorList = new List<Color>();

            for (int i = 0; i < inputs.Count; ++i)
            {
                colorList.Add(inputs[i].currentcolor);
            }

            //linkedBridge.BuildBridge(colorList);
            //Log.LogInfo("Updating Bridge");
			//List<Color> colorList = new List<Color>();
			//colorList.Add(Color.Blue);
			//colorList.Add(Color.Red);
			BridgeSourceL3P8.globalController.BuildBridge(colorList);
		}

		void OnStart()
		{
			player = gameObject.FindWithTag("Surge");
			//colorList = new List<Color>();
		}

		void OnUpdate()
		{
            if (canUpdate)
            {
                if (!inputFlag)
                {
                    if (Input.IsKeyPressed(KeyCode.R) || Input.IsControllerButtonPressed(ControllerCode.B))
                    {
                        if (VirusReorderScript.globalController.playerViruses.Count != 0)
                        {
                            VirusScript v = VirusReorderScript.globalController.playerViruses[0];
                            inputs.Add(v);
                            v.gameObject.transform.position = transform.position;
                            v.InputIntoMechanic();
                            inputFlag = true;
                            //colorList.Add(Color.Red);
                            UpdateBridge();
                        }
                        //Log.LogInfo("Z pressed");
                    }

                    if (Input.IsKeyPressed(KeyCode.T) || Input.IsControllerButtonPressed(ControllerCode.A))
                    {
                        if (inputs.Count != 0)
                        {
                            VirusScript v = inputs[inputs.Count - 1]; //get last input
                            inputs.Remove(v);
                            v.Obtain();
                            inputFlag = true;
                            //if (colorList.Count != 0)
                            //{
                            //	Color v = colorList[colorList.Count - 1];
                            //	colorList.Remove(v);
                            //}
                            //colorList.Add(Color.Blue);
                            UpdateBridge();
                        }
                        //Log.LogInfo("X pressed");
                    }
                }
                else
                {
                    if (!(Input.IsKeyPressed(KeyCode.R) || Input.IsControllerButtonPressed(ControllerCode.B)) && !(Input.IsKeyPressed(KeyCode.T) || Input.IsControllerButtonPressed(ControllerCode.A)))
                    {
                        inputFlag = false;
                    }
                }
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
            //if (player.GetComponent<Collider>().isTrigger)
            //{
            //    if (!inputFlag)
            //    {
            //        if (Input.IsKeyPressed(KeyCode.Z))
            //        {
            //            VirusScript v = VirusReorderScript.globalController.playerViruses[0];
            //            inputs.Add(v);
            //            v.gameObject.transform.position = transform.position;
            //            v.InputIntoMechanic();
            //            inputFlag = true;
            //            UpdateBridge();
            //        }

            //        if (Input.IsKeyPressed(KeyCode.X))
            //        {
            //            VirusScript v = inputs[inputs.Count - 1]; //get last input
            //            inputs.Remove(v);
            //            v.Obtain();
            //            inputFlag = true;
            //            UpdateBridge();
            //        }
            //    }
            //    else
            //    {
            //        if (!Input.IsKeyPressed(KeyCode.Z) && !Input.IsKeyPressed(KeyCode.X))
            //        {
            //            inputFlag = false;
            //        }
            //    }
            //}

            if (player.GetComponent<Collider>().isTrigger)
            {
                canUpdate = true;
            //    if (!inputFlag)
            //    {
            //        if (Input.IsKeyPressed(KeyCode.Z))
            //        {
            //            VirusScript v = VirusReorderScript.globalController.playerViruses[0];
            //            inputs.Add(v);
            //            v.gameObject.transform.position = transform.position;
            //            v.InputIntoMechanic();
            //            inputFlag = true;
            //            //colorList.Add(Color.Red);
            //            UpdateBridge();
            //            Log.LogInfo("Z pressed");
            //        }

            //        if (Input.IsKeyPressed(KeyCode.X))
            //        {
            //            VirusScript v = inputs[inputs.Count - 1]; //get last input
            //            inputs.Remove(v);
            //            v.Obtain();
            //            inputFlag = true;
            //            //if (colorList.Count != 0)
            //            //{
            //            //	Color v = colorList[colorList.Count - 1];
            //            //	colorList.Remove(v);
            //            //}
            //            //colorList.Add(Color.Blue);
            //            UpdateBridge();
            //            Log.LogInfo("X pressed");
            //        }
            //    }
            //    else
            //    {
            //        if (!Input.IsKeyPressed(KeyCode.Z) && !Input.IsKeyPressed(KeyCode.X))
            //        {
            //            inputFlag = false;
            //        }
            //    }
            }
        }

		void OnTriggerEnd(ulong data)
        {
            canUpdate = false;
        }
	}
}