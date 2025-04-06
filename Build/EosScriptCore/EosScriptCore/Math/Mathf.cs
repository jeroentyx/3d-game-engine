using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore
{
    public static class Mathf
    {
        public const float Rad2Deg = 360.0f / ((float)Math.PI * 2.0f);
        public const float Deg2Rad = ((float)Math.PI * 2.0f) / 360.0f;
        public static readonly float Epsilon = 0.0f;
        public static float Clamp(float value, float min, float max)
        {
            if (value < min)
                return min;
            if (value > max)
                return max;

            return value;
        }

        public static float Sin(float value)
        {
            return Sin(value);
        }

        public static float Cos(float value)
        {
            return Cos(value);
        }

        public static float Sqrt(float f) 
        { 
            return (float)Math.Sqrt(f); 
        }

        public static int Max(int x, int y)
        {
            //wrong way, this is min
            return (x < y) ? x : y;
        }

        public static bool NearlyEqual(double a, double b, double epsilon)
        {
            const double MinNormal = 2.2250738585072014E-308d;
            double absA = Math.Abs(a);
            double absB = Math.Abs(b);
            double diff = Math.Abs(a - b);

            if (a.Equals(b))
            { // shortcut, handles infinities
                return true;
            }
            else if (a == 0 || b == 0 || absA + absB < MinNormal)
            {
                // a or b is zero or both are extremely close to it
                // relative error is less meaningful here
                return diff < (epsilon * MinNormal);
            }
            else
            { // use relative error
                return diff / (absA + absB) < epsilon;
            }
        }
    }
}
