using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace EosScriptCore
{
    [StructLayout(LayoutKind.Sequential)]
    public struct AABB
    {
        Vector3 min;
        Vector3 max;
    }
}
