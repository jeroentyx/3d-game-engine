using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.CompilerServices;

namespace EosScriptCore
{ 
    public class Input
    {

        public enum Cursor
        {
            Normal = 0,
		    Hidden = 1,
		    Locked = 2,
	    };

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool IsKeyPressed_Native(KeyCode key);
        public static bool IsKeyPressed(KeyCode key)
        { 
            return IsKeyPressed_Native(key);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool IsKeyTriggered_Native(KeyCode key);
        public static bool IsKeyTriggered(KeyCode key)
        {
            return IsKeyTriggered_Native(key);
        }


        public static bool IsMouseButtonPressed(MouseCode key)
        {
            return IsMouseButtonPressed_Native(key);
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetMousePosition_Native(out Vector2 outScale);
        public static Vector2 GetMousePosition()
        {
            //Will test this again in future
            GetMousePosition_Native(out Vector2 result);
            return result;
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetMousePosition_Native(ref Vector2 pos);
        public static void SetMousePosition(Vector2 pos)
        {
            SetMousePosition_Native(ref pos);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetCursorMode_Native(Cursor Mode);
        public static void SetCursorMode(Cursor Mode)
        {
            SetCursorMode_Native(Mode);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Cursor GetCursorMode_Native();
        public static Cursor GetCursorMode()
        {
            return GetCursorMode_Native();
        }

        public static float GetMouseWheel()
        {
            return GetMouseWheel_Native();
        }


        public static float GetAxisRaw(string axis)
        {
            return GetAxis_Native(axis);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern bool IsJoystickConnected_Native();
        public static bool IsJoystickConnected()
        {
            return IsJoystickConnected_Native();
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern bool IsControllerButtonPressed_Native(ControllerCode cBtn);
        public static bool IsControllerButtonPressed(ControllerCode cBtn)
        {
            return IsControllerButtonPressed_Native(cBtn);
        }
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void CameraControllerMovement_Native(out Vector2 pos);
        public static Vector2 CameraControllerMovement()
        {
            CameraControllerMovement_Native(out Vector2 result);
            return result;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void PlayerControllerMovement_Native(out Vector2 pos);
        public static Vector2 PlayerControllerMovement()
        {
            PlayerControllerMovement_Native(out Vector2 result);
            return result;
        }



        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern float GetAxis_Native(string axis);

        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool IsMouseButtonPressed_Native(MouseCode key);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetMouseWheel_Native();


    }
}
