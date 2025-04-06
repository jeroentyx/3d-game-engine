using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public class Level4 : MonoBase
    {
        //public GameObject cam;
        private AudioComponent audiocomp;
        private UIComponent uicomp;
        private GameObject blackscreen;
        private UIComponent blackscreenUI;
        bool loadingState = true;

        private GameObject player;

        public float val;
        void OnStart()
        {
            audiocomp = gameObject.GetComponent<AudioComponent>();
            uicomp = gameObject.GetComponent<UIComponent>();
            audiocomp.PlayBGM("Medium_Level_Looping.wav");
            player = gameObject.FindWithTag("Surge");

            blackscreen = gameObject.FindWithTag("BlackScreen");
            blackscreenUI = blackscreen.GetComponent<UIComponent>();
            blackscreen.SetActive(true);
            blackscreenUI.SetOpacity(1.0f);
            val = blackscreenUI.GetOpacity();
        }
        void OnUpdate()
        {

            if (loadingState)
            {

                val -= 0.01f;
                blackscreenUI.SetOpacity(val);

            }

            if (blackscreenUI.GetOpacity() <= 0.0f)
            {
                blackscreen.SetActive(false);
                loadingState = false;
            }
        }
    }
}
