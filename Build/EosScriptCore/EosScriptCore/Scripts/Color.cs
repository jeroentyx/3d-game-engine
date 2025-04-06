using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public enum Color : int
    {
        Red = 0,
        Yellow = 1,
        Blue = 2,
        Corrupted_Red = 3,
        Corrupted_Yellow = 4,
        Corrupted_Blue = 5,
        White = 6
    }

    public class ColorVec
    {
        public static Vector3 red => new Vector3(1, 0, 0);
        public static Vector3 blue => new Vector3(0, 0, 1);
        public static Vector3 green => new Vector3(0, 1, 0);
        public static Vector3 white => new Vector3(1, 1, 1);
        public static Vector3 black => new Vector3(0, 0, 0);
        public static Vector3 cyan => new Vector3(0, 1, 1);
        public static Vector3 gray => new Vector3(0.5f, 0.5f, 0.5f);
        public static Vector3 magenta => new Vector3(1, 0, 1);
        public static Vector3 yellow => new Vector3(1, 0.92f, 0.016f);
        public static Vector3 toxic => new Vector3(0, 1.0f, 0);
        public static Vector3 elec => new Vector3(0, 1.0f, 1.0f);
        public static Vector3 fire => new Vector3(1, 0, 0);

    }
}
