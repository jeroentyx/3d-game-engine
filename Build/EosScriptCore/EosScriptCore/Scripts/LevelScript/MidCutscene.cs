using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public class MidCutscene: MonoBase
    {
        private GameObject mid1;

        private UIComponent UI1;

        private AudioComponent audiocomp;

        bool clickedonce;

        //The blackscreen that involves the fade in/ out
        private GameObject blackscreen;
        private UIComponent blackscreenUI;

        public float val;

        public int pressed = 0;
        public bool IsNextState = false;

        void OnStart()
        {
            mid1  = gameObject.FindWithTag("mid1");
            UI1 = mid1.GetComponent<UIComponent>();

            audiocomp = gameObject.GetComponent<AudioComponent>();
            audiocomp.PlayBGM("MainMenu03_V2_harmony_bass_synth_only.wav");


            blackscreen = gameObject.FindWithTag("BlackScreen");
            blackscreenUI = blackscreen.GetComponent<UIComponent>();
            blackscreen.SetActive(true);
            blackscreenUI.SetOpacity(1.0f);
        }

        void OnUpdate()
        {
            //Pressed once check
            if ((Input.IsKeyPressed(KeyCode.Space) || Input.IsControllerButtonPressed(ControllerCode.A)) && !clickedonce)
            {
                clickedonce = true;
                pressed += 1;
                //do whatever enter is supposed to do
            }

            if ((Input.IsKeyPressed(KeyCode.Space) || Input.IsControllerButtonPressed(ControllerCode.A)) && clickedonce)
            {
                //do nothing
            }

            if (!(Input.IsKeyPressed(KeyCode.Space) || Input.IsControllerButtonPressed(ControllerCode.A)) && clickedonce)
            {
                clickedonce = false;
            }

            // Cutscenes
            if (pressed == 1)
            {
                blackscreen.SetActive(false);
                mid1.SetActive(true);
            }

            else if (pressed == 2)
            {
                UI1.SetFilePath("Mid 2.png");
            }

            else if (pressed == 3)
            {
                UI1.SetFilePath("Mid 3.png");
            }

            else if (pressed >= 4)
            {
                UI1.SetFilePath("Loading bkgd lv4.png");
                Transition();
            }
        }

        void Transition()
        {
            val += 0.01f;

            if (val > 1.0f)
            {
                Log.LogInfo("Going into next level");
                GameStateManager.SetGameState(GameStateManager.ENGINESTATE.STOP);
                SceneMananger.LoadNextScene("Assets/Data/Level_4.json");
            }
        }
    }
}
