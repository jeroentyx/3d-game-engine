using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public class EndingCutscene: MonoBase
    {
        private GameObject end1;
   
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
            end1 = gameObject.FindWithTag("end1");
            UI1 = end1.GetComponent<UIComponent>();

            audiocomp = gameObject.GetComponent<AudioComponent>();
            audiocomp.PlayBGM("Easy_Level_Looping.wav");

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
                    end1.SetActive(true);
                }

                else if (pressed == 2)
                {
                    UI1.SetFilePath("Ending 2.png");
                }

                else if (pressed == 3)
                {
                    UI1.SetFilePath("Ending 3.png");
                }

                else if (pressed == 4)
                {
                    UI1.SetFilePath("Ending 4.png");
                }

                else if (pressed == 5)
                {
                    UI1.SetFilePath("Ending 5.png");
                }

                else if (pressed == 6)
                {
                    UI1.SetFilePath("Ending 6.png");
                    audiocomp.PlaySFX("User Finally.wav");
                }

                else if (pressed == 7)
                {
                    UI1.SetFilePath("Ending 7.png");
                }

                else if (pressed == 8)
                {
                    UI1.SetFilePath("Ending 8.png");
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

            else if (pressed >= 9)
            {
                UI1.SetFilePath("Ending 9.png");
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
                SceneMananger.LoadNextScene("Assets/Data/MainMenu.json");
            }
        }

    }
}
