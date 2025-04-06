using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public class Goal : MonoBase
    {
        private GameObject player;
        private Animation aniOnj;
        private GameObject EndScreen;
        private GameObject BlackScreen;
        private UIComponent EndScreenUI;
        private UIComponent BlackScreenUI;

        private GameObject lvl4loadingScreen;
        private UIComponent l4loadUI;

        private AudioComponent Audio;
        private bool IsNextState;
        private bool Triggered = false;
        private bool Teleport = false;
        public float val;


        private Vector3 rotating = new Vector3(0, 1, 0);

        void OnStart()
        {
            player = gameObject.FindWithTag("Surge");
            aniOnj = player.GetComponent<Animation>();

            EndScreen = gameObject.FindWithTag("EndScreen");     // Create, rename tag, and insert tag here
            EndScreenUI = EndScreen.GetComponent<UIComponent>();

            BlackScreen = gameObject.FindWithTag("BlackScreen");
            BlackScreenUI = BlackScreen.GetComponent<UIComponent>();

            lvl4loadingScreen = gameObject.FindWithTag("lvl4loadingScreen");
            l4loadUI = lvl4loadingScreen.GetComponent<UIComponent>();
            lvl4loadingScreen.SetActive(false);

            Audio = gameObject.GetComponent<AudioComponent>();
            BlackScreen.SetActive(false);
            BlackScreenUI.SetOpacity(0.0f);

        }
        void OnUpdate()
        {
            if (Triggered)
            {

                Transition();
            }
            Teleporting();
        }

        void OnTriggerBegin(ulong entity)
        {
            if (player.GetComponent<Collider>().isTrigger && gameObject.GetComponent<Collider>().isTrigger)
            {
                // Get UI element and set active
                Teleport = true;
                
            }
        }

        void OnTriggerStay(ulong entity)
        {
            if (player.GetComponent<Collider>().isTrigger && gameObject.GetComponent<Collider>().isTrigger)
            {
                
                //Set it to play the animation first 
                if (aniOnj.IsAnimationCompleted())
                {
                    EndScreen.SetActive(true);
                    Audio.PlaySFX("Victory_SFX.ogg");
                    Triggered = true;
                }
            }
        }


        void Teleporting()
        {
            if (Teleport)
            {
                aniOnj.SetAnimateState("Teleport");
                player.transform.Translation = gameObject.transform.Translation;
            }
        }

        void Transition()
        {
            if (Input.IsKeyPressed(KeyCode.Space))
            {
                
                IsNextState = true;
                BlackScreen.SetActive(true);
                lvl4loadingScreen.SetActive(true);
                EndScreenUI.SetDepth(0.9f);
            }
            if (IsNextState)
            {
                val += 0.01f;
                BlackScreenUI.SetOpacity(val);
            }

            if (BlackScreenUI.GetOpacity() > 1.0f)
            {
                BlackScreen.SetActive(false);


            }
            if (val >= 1.5f)
            {
                GameStateManager.SetGameState(GameStateManager.ENGINESTATE.STOP);
                SceneMananger.LoadNextScene("Assets/Data/MidCutScene.json");
            }

        }
    }
}
