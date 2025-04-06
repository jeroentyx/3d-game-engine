using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public class MainMenu : MonoBase
    {
        //public GameObject cam;
        private AudioComponent audiocomp;
        private UIComponent uicomp;
        private GameObject blackscreen;
        private GameObject mm;
        private GameObject gtt;
        private GameObject gtx;
        private UIComponent blackscreenUI;
        private UIComponent mmUI;
        private UIComponent gttUI;
        private UIComponent gtxUI;
        private GameObject lvl1loadingScreen;
        private UIComponent l1loadUI;
        bool changeState = false;

        bool loadnext = false;
        bool loadnow = false;

        public float val;
        public float vol = 1.0f;
        void OnStart()
        {
            audiocomp = gameObject.GetComponent<AudioComponent>();
            uicomp = gameObject.GetComponent<UIComponent>();
            audiocomp.PlayBGM("Main_Menu.ogg");


            blackscreen = gameObject.FindWithTag("BlackScreen");
            blackscreenUI = blackscreen.GetComponent<UIComponent>();
            val = blackscreenUI.GetOpacity();

            mm = gameObject.FindWithTag("MainMenu");
            mmUI = mm.GetComponent<UIComponent>();

            gtt = gameObject.FindWithTag("GameTitle");
            gttUI = mm.GetComponent<UIComponent>();

            gtx = gameObject.FindWithTag("GameText");
            gtxUI = mm.GetComponent<UIComponent>();

            lvl1loadingScreen = gameObject.FindWithTag("lvl1loadingScreen");
            l1loadUI = lvl1loadingScreen.GetComponent<UIComponent>();
            lvl1loadingScreen.SetActive(false);
        }
        void OnUpdate()
        {
            if (Input.IsKeyPressed(KeyCode.Space))
            {
                changeState = true;
                blackscreen.SetActive(true);
                lvl1loadingScreen.SetActive(true);
            }
            if(changeState)
            {
                Log.LogInfo("Opaque: " + val.ToString());
                val += 0.01f;
                blackscreenUI.SetOpacity(val);
            }

            if (blackscreenUI.GetOpacity() > 1.0f)
            {
                blackscreen.SetActive(false);
                mmUI.SetDepth(0.9f);
                gtt.SetActive(false);
                gtx.SetActive(false);
            }

            if (val >= 1.5f)
            {

                GameStateManager.SetGameState(GameStateManager.ENGINESTATE.STOP);
                SceneMananger.LoadNextScene("Assets/Data/IntroCutScene.json");

            }
        }
    }
}

