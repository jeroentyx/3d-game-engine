using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public class TeleportInput : MonoBase
    {
        public VirusScript input;

        public GameObject player;
        //public TeleportMultiple linkedTeleporter;
        //public GameObject linkedTeleporter;

        public Color red_color;
        public Color blue_color;
        public Color yellow_color;

        bool inputFlag = false;



        void OnStart()
        {
            player = gameObject.FindWithTag("Surge");
            red_color = Color.Red;
            blue_color = Color.Blue;
            yellow_color = Color.Yellow;

    }

        void UpdateTeleporter()
        {
            //linkedTeleporter.red = false;
            //linkedTeleporter.blue = false;
            //linkedTeleporter.yellow = false;
            TeleportMultiple.globalController.red = false;
            TeleportMultiple.globalController.blue = false;
            TeleportMultiple.globalController.yellow = false;

            if (input == null)
            {
                TeleportMultiple.globalController.UpdateColor(Color.White);
                return;
            }
            else if (input.currentcolor == red_color)
            {
                TeleportMultiple.globalController.red = true;
                TeleportMultiple.globalController.UpdateColor(input.currentcolor);
            }
            else if (input.currentcolor == blue_color)
            {
                TeleportMultiple.globalController.blue = true;
                TeleportMultiple.globalController.UpdateColor(input.currentcolor);
            }
            else if (input.currentcolor == yellow_color)
            {
                TeleportMultiple.globalController.yellow = true;
                TeleportMultiple.globalController.UpdateColor(input.currentcolor);
            }

        }
        void OnTriggerStay(ulong data)
        {
            
            if (player.GetComponent<Collider>().isTrigger)
            {
                if (!inputFlag)
                {
                    if ((Input.IsKeyPressed(KeyCode.R) || Input.IsControllerButtonPressed(ControllerCode.A)) && input == null)
                    {
                        VirusScript v = VirusReorderScript.globalController.playerViruses[0];
                        input = v;
                        v.gameObject.transform.Translation = transform.Translation;
                        v.InputIntoMechanic();
                        inputFlag = true;
                        UpdateTeleporter();
                    }
                    if ((Input.IsKeyPressed(KeyCode.T) || Input.IsControllerButtonPressed(ControllerCode.B)) && input != null)
                    {
                        VirusScript v = input;
                        input = null;
                        v.Obtain();
                        inputFlag = true;
                        UpdateTeleporter();
                    }
                }
                else 
                {
                    if (!(Input.IsKeyPressed(KeyCode.R) || Input.IsControllerButtonPressed(ControllerCode.A)) && !(Input.IsKeyPressed(KeyCode.T)|| Input.IsControllerButtonPressed(ControllerCode.B))) 
                    {
                        inputFlag = false;
                    }
                }
            }
        }
    }
}