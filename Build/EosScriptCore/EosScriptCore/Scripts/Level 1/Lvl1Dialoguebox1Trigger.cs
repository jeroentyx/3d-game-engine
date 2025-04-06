using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public class Lvl1Dialoguebox1Trigger : MonoBase
    {
        private bool displayDialogueBox= false;
        private GameObject player;
        private GameObject DialogueBox;

        void OnStart()
        {
            player = gameObject.FindWithTag("Surge");
            DialogueBox = gameObject.FindWithTag("DialogueBox1");
            DialogueBox.SetActive(false);
        }

        void OnUpdate()
        {
        }

        void OnFixedUpdate()
        {
        }

        void OnTriggerBegin(ulong entity)
        {
            if (player.GetComponent<Collider>().isTrigger && gameObject.GetComponent<Collider>().isTrigger)
            {
                DialogueBox.SetActive(true);
                Log.LogInfo("Lvl1Dialoguebox1 is showing!");

                if(Input.IsKeyPressed(KeyCode.T))
                {
                    DialogueBox.SetActive(false);
                    Log.LogInfo("Lvl1Dialoguebox1 is not showing!");
                }
            }
        }

        void OnTriggerStay(ulong entity)
        {

        }

        void OnTriggerEnd(ulong entity)
        {

        }
    }

}
