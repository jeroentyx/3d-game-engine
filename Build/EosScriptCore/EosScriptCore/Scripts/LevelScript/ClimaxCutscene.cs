using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public class ClimaxCutscene: MonoBase
    {
        private GameObject climax1;

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
            climax1 = gameObject.FindWithTag("climax1");
            UI1 = climax1.GetComponent<UIComponent>();

            audiocomp = gameObject.GetComponent<AudioComponent>();
            audiocomp.PlayBGM("MainMenu03_V2_harmony_bass_synth_only.wav");

            blackscreen = gameObject.FindWithTag("BlackScreen");
            blackscreenUI = blackscreen.GetComponent<UIComponent>();
            blackscreen.SetActive(true);
            blackscreenUI.SetOpacity(1.0f);

            blackscreen.SetActive(true);
        }

        void OnUpdate()
        {
            //Pressed once check
            if ((Input.IsKeyPressed(KeyCode.Space) || Input.IsControllerButtonPressed(ControllerCode.A)) && !clickedonce)
            {
                clickedonce = true;
                pressed += 1;
                //do whatever enter is supposed to do
                // Cutscenes
                if (pressed == 1)
                {
                    blackscreen.SetActive(false);
                    climax1.SetActive(true);
                }

                else if (pressed == 2)
                {
                    UI1.SetFilePath("Climax 2.png");
                }

                else if (pressed == 3)
                {
                    UI1.SetFilePath("Climax 3.png");
                    audiocomp.PlaySFX("User Yes.wav");
                }

                else if (pressed == 4)
                {
                    UI1.SetFilePath("Climax 4.png");
                }

                else if (pressed == 5)
                {
                    UI1.SetFilePath("Climax 5.png");
                    audiocomp.PlaySFX("User Noo.wav");
                }

                else if (pressed == 6)
                {
                    UI1.SetFilePath("Climax 6.png");
                    audiocomp.PlaySFX("Cutscene2_Surge_E.wav");
                }

                else if (pressed == 7)
                {
                    UI1.SetFilePath("Climax 7.png");
                }

                else if (pressed == 8)
                {
                    UI1.SetFilePath("Climax 8.png");
                }

                else if (pressed == 9)
                {
                    UI1.SetFilePath("Climax 9.png");
                }

                else if (pressed == 10)
                {
                    UI1.SetFilePath("Climax 10.png");
                }

                else if (pressed == 11)
                {
                    UI1.SetFilePath("Climax 11.png");
                    audiocomp.PlaySFX("Cutscene2C_Surge_E.wav");
                }

                else if (pressed == 12)
                {
                    UI1.SetFilePath("Climax 12.png");
                }

                else if (pressed == 13)
                {
                    UI1.SetFilePath("Climax 13.png");
                }

                else if (pressed == 14)
                {
                    UI1.SetFilePath("Climax 14.png");
                }
            }

            if ((Input.IsKeyPressed(KeyCode.Space) || Input.IsControllerButtonPressed(ControllerCode.A)) && clickedonce)
            {
                //do nothing
            }

            if (!(Input.IsKeyPressed(KeyCode.Space) || Input.IsControllerButtonPressed(ControllerCode.A)) && clickedonce)
            {
                clickedonce = false;
            }

            else if (pressed >= 15)
            {
                UI1.SetFilePath("Loading bkgd lv5.png");
                Transition();
            }
            //climax2.SetActive(false);
            //climax3.SetActive(false);
            //climax4.SetActive(false);
            //climax5.SetActive(false);
            //climax6.SetActive(false);
            //climax7.SetActive(false);
            //climax8.SetActive(false);
            //climax9.SetActive(false);
            //climax10.SetActive(false);
            //climax11.SetActive(false);
            //climax12.SetActive(false);
            //climax13.SetActive(false);
            //climax14.SetActive(false);
        }

        void Transition()
        {
            val += 0.01f;

            if (val > 1.0f)
            {
                Log.LogInfo("Going into next level");
                GameStateManager.SetGameState(GameStateManager.ENGINESTATE.STOP);
                SceneMananger.LoadNextScene("Assets/Data/Level_5.json");
            }
        }

    }
}
