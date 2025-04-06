using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace EosScriptCore
{
    public class Time
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float GetDeltaTime_Native();

        public static float deltaTime
        {
            get
            {
                return GetDeltaTime_Native();
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float GetTime_Native();
        public static float time
        {
            get
            {
                return GetTime_Native();
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float GetFixedDeltaTime_Native();
        public static float fixedDeltaTime
        {
            get
            {
                return GetFixedDeltaTime_Native();
            }
        }

    }
}
