using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;
using System.Text;

namespace EosScriptCore
{
    [StructLayout(LayoutKind.Sequential)]
    public static class SceneMananger
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool LoadNextScene_Native(string sceneName);

        public static bool LoadNextScene(string sceneName)
        {
            return LoadNextScene_Native(sceneName);
        }
    }
}
