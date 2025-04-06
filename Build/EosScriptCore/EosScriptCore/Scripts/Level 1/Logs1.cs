using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    class Logs1 : MonoBase
    {
        private bool displayLogs = false;
        private bool displayInteract = false;
        private bool isInteracting = false;
        private GameObject player;
        private GameObject logsImage;
        private GameObject interactImage;

        void OnStart()
        {
            player = gameObject.FindWithTag("Surge");
            interactImage = gameObject.FindWithTag("interact_controller_a");
            logsImage = gameObject.FindWithTag("Logs1");
            logsImage.SetActive(false);
            interactImage.SetActive(false);
        }

        void OnUpdate()
        {
            if (displayLogs == true)
            {
                logsImage.SetActive(true);
            }
            else if(displayLogs == false)
            {
                logsImage.SetActive(false);
            }

            if (displayInteract == true)
            {
                interactImage.SetActive(true);
                if (Input.IsKeyPressed(KeyCode.R) && isInteracting == false)
                {
                    displayLogs = true;
                    isInteracting = true;
                }
                else if (Input.IsKeyPressed(KeyCode.R) && isInteracting == true)
                {
                    displayLogs = false;
                    isInteracting = false;
                }
            }
            else if (displayInteract == false)
            {
                interactImage.SetActive(false);
            }
        }

        void OnTriggerBegin(ulong entity)
        {
            if (player.GetComponent<Collider>().isTrigger)
            {
                displayInteract = true;
            }
        }

        void OnTriggerStay(ulong entity)
        {
        }

        void OnTriggerEnd(ulong entity)
        {
            if (player.name == "Surge")
            {
                displayLogs = false;
                displayInteract = false;
            }
        }
    }
}
