using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;
using System.Text;

namespace EosScriptCore
{
    

    [StructLayout(LayoutKind.Sequential)]
    public class GameStateManager
    {

        private static GameStateManager instance;

        public static StateType currentGameState;

        public static GameStateManager Instance
        {
            get
            {
                if (instance == null)
                {
                    instance = new GameStateManager();
                }

                return instance;
            }
        }

        public void Pause()
        { 
            SetGameState(ENGINESTATE.PAUSE);
        }

        public void Play()
        {
            SetGameState(ENGINESTATE.PLAY);
        }



        public enum ENGINESTATE
        {
            PLAY,
            PAUSE,
            STOP
        };

        public enum StateType
        {
            MainMenu,
            Level1,
            Level2,
            Leve13,
            Level4,
            Level5,
            Level6,
            Credits,
            PauseMenu,
            Options

            //Currently have is level 2 and level 4 - Bridging
        }

        public static void ChangeGameState(StateType gamestate)
        {
            switch(gamestate)
            {
                case StateType.MainMenu:
                    break;
                case StateType.Level1:
                    SceneMananger.LoadNextScene("Assets/Data/Level_1.json");
                    currentGameState = StateType.Level1;
                    break;
                case StateType.Level2:
                    break;
                case StateType.Leve13:
                    break;
                case StateType.Level4:
                    break;
                case StateType.Level5:
                    break;
                case StateType.Level6:
                    break;
                case StateType.Credits:
                    break;
                case StateType.PauseMenu:
                    break;
                case StateType.Options:
                    break;
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern ENGINESTATE GetGameState_Native();

        public static ENGINESTATE GetGameState()
        {
            return GetGameState_Native();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetGameState_Native(ENGINESTATE state);

        public static void SetGameState(ENGINESTATE state)
        {
            SetGameState_Native(state);
        }


        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void LoadGameState_Native();
        public static void LoadGameState()
        {
            LoadGameState_Native();
        }


        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void UnloadGameState_Native();
        public static void UnloadGameState()
        {
            UnloadGameState_Native();
        }



       




    }
   
}
