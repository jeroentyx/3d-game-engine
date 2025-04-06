using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace EosScriptCore
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Lerp
    {
        public static float LerpFloat(float a, float b, float f)
        {
            return (a * (1.0f - f)) + (b * f);
        }
    }
}
