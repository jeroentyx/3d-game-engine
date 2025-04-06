using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace EosScriptCore
{
    public static class Audio
    {
        public static void IncreaseBGMvol() 
        {
            IncreaseBGMvol_Native();
        }

        public static void DecreaseBGMvol() 
        {
            DecreaseBGMvol_Native();
        }

        public static void IncreaseSFXvol() 
        {
            IncreaseSFXvol_Native();
        }

        public static void DecreaseSFXvol()
        {
            DecreaseSFXvol_Native();
        }

        public static void IncreaseMastervol()
        {
            IncreaseMastervol_Native();
        }

        public static void DecreaseMastervol()
        {
            DecreaseMastervol_Native();
        }

        public static void Mute()
        {
            Mute_Native();
        }

        public static void Unmute()
        {
            Unmute_Native();
        }

        public static void SetSFXVol(float vol)
        {
            SetSFXvol_Native(vol);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetSFXvol_Native(float vol);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void IncreaseBGMvol_Native();
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void DecreaseBGMvol_Native();
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void IncreaseSFXvol_Native();
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void DecreaseSFXvol_Native();
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void IncreaseMastervol_Native();
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void DecreaseMastervol_Native();
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Mute_Native();
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Unmute_Native();
    }
}