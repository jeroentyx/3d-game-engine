using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public class Goal_Leve1 : MonoBase
    {
        private GameObject player;
        private Animation aniOnj;
        private GameObject EndScreen;
        private GameObject BlackScreen;
        private UIComponent EndScreenUI;
        private UIComponent BlackScreenUI;

        private GameObject lvl2loadingScreen;
        private UIComponent l2loadUI;


        private AudioComponent Audio;
        private RigidBody rigid;
        private bool IsNextState;
        private bool Triggered = false;
        private bool Teleport = false;
        public float val;

       

        void OnStart()
        {
            player = gameObject.FindWithTag("Surge");
            aniOnj = player.GetComponent<Animation>();

            EndScreen = gameObject.FindWithTag("EndScreen");     // Create, rename tag, and insert tag here
            EndScreenUI = EndScreen.GetComponent<UIComponent>();

            BlackScreen = gameObject.FindWithTag("BlackScreen");
            BlackScreenUI = BlackScreen.GetComponent<UIComponent>();

            lvl2loadingScreen = gameObject.FindWithTag("lvl2loadingScreen");
            l2loadUI = lvl2loadingScreen.GetComponent<UIComponent>();
            lvl2loadingScreen.SetActive(false);

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
                
                Teleport = true;
                
            }
        }

        void OnTriggerStay(ulong entity)
        {
            if (player.GetComponent<Collider>().isTrigger && gameObject.GetComponent<Collider>().isTrigger)
            {
                player.transform.Translation = gameObject.transform.Translation;
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
            //Teleporting Animation
            //Take note if animation has ended then set the end screen 
            if(Teleport)
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
                lvl2loadingScreen.SetActive(true);
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
                SceneMananger.LoadNextScene("Assets/Data/GreyBox_Naz_Lv.json");

            }
        }
    }
}
