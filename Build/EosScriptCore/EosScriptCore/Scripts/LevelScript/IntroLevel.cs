using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public class IntroLevel: MonoBase
    {
        private GameObject intro1;

        private UIComponent UI1;

        private AudioComponent audiocomp;

        bool clickedonce;

        //The blackscreen that involves the fade in/ out
        private GameObject blackscreen;
        private UIComponent blackscreenUI;

        public int pressed = 0;
        public bool IsNextState = false;

        public float val = 0f;


    void OnStart()
        {
            audiocomp = gameObject.GetComponent<AudioComponent>();
            intro1  = gameObject.FindWithTag("intro1");

            audiocomp.PlayBGM("MainMenu03_V2_harmony_bass_synth_only.wav");

            UI1  = intro1.GetComponent<UIComponent>();
            

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
                // Cutscenes
                if (pressed == 1)
                {
                    blackscreen.SetActive(false);
                    intro1.SetActive(true);
                }

                else if (pressed == 2)
                {
                    UI1.SetFilePath("Intro 2.png");
                }

                else if (pressed == 3)
                {
                    UI1.SetFilePath("Intro 3.png");
                }

                else if (pressed == 4)
                {
                    UI1.SetFilePath("Intro 4.png");
                    audiocomp.PlaySFX("User Intro.wav");
                }

                else if (pressed == 5)
                {
                    UI1.SetFilePath("Intro 5.png");
                }

                else if (pressed == 6)
                {
                    UI1.SetFilePath("Intro 6.png");
                }

                else if (pressed == 7)
                {
                    UI1.SetFilePath("Intro 7.png");
                }

                else if (pressed == 8)
                {
                    UI1.SetFilePath("Intro 8.png");
                }

                else if (pressed == 9)
                {
                    UI1.SetFilePath("Intro 9.png");
                }

                else if (pressed == 10)
                {
                    UI1.SetFilePath("Intro 10.png");
                }

                else if (pressed == 11)
                {
                    UI1.SetFilePath("Intro 11.png");
                }

                else if (pressed == 12)
                {
                    UI1.SetFilePath("Intro 12.png");
                }
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

            else if (pressed >= 13)
            {
                UI1.SetFilePath("Loading bkgd lv1.png");
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
                SceneMananger.LoadNextScene("Assets/Data/Level_1.json");
            }
        }
    }
}
