using System;
using System.Runtime.InteropServices;

namespace EosScriptCore
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector3
    {
        public static Vector3 Zero = new Vector3(0, 0, 0);

        public static Vector3 Forward = new Vector3(0, 0, -1);
        public static Vector3 Right = new Vector3(-1, 0, 0);
        public static Vector3 Up = new Vector3(0, 1, 0);
        public static Vector3 positiveInfinity = new Vector3(float.PositiveInfinity, float.PositiveInfinity, float.PositiveInfinity);

        public float X;
        public float Y;
        public float Z;

        public const float kEpsilon = 0.00001F;
        public const float kEpsilonNormalSqrt = 1e-15F;
        
        public float sqrMagnitude 
        {
            get { return X * X + Y * Y + Z * Z; } 
        }

        public Vector3(float scalar)
        {
            X = Y = Z = scalar;
        }

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public Vector3(float x, Vector2 yz)
        {
            X = x;
            Y = yz.X;
            Z = yz.Y;
        }

        public Vector3(Vector2 vector)
        {
            X = vector.X;
            Y = vector.Y;
            Z = 0.0f;
        }

        public Vector3(Vector4 vector)
        {
            X = vector.X;
            Y = vector.Y;
            Z = vector.Z;
        }

        public void Clamp(Vector3 min, Vector3 max)
        {
            X = Mathf.Clamp(X, min.X, max.X);
            Y = Mathf.Clamp(Y, min.Y, max.Y);
            Z = Mathf.Clamp(Z, min.Z, max.Z);
        }

        public float Length()
        {
            return (float)Math.Sqrt(X * X + Y * Y + Z * Z);
        }

        public static float Distance(Vector3 first, Vector3 second)
        {
            Vector3 res = second - first;
            return res.Length();
        }


        public Vector3 Normalized()
        {
            float length = Length();
            float x = X / length;
            float y = Y / length;
            float z = Z / length;
            return new Vector3(x, y, z);
        }

        public void Normalize()
        {
            float length = Length();
            X = X / length;
            Y = Y / length;
            Z = Z / length;
        }

        public bool MoreThanInXCoordinate(Vector3 vector)
        {
            if (X > vector.X)
            {
                return true;
            }


            return false;
        }
        public bool MoreThanInYCoordinate(Vector3 vector)
        {
            if (Y > vector.Y)
            {
                return true;
            }


            return false;
        }
        public bool MoreThanInZCoordinate(Vector3 vector)
        {
            if (Z > vector.Z)
            {
                return true;
            }


            return false;
        }

        public static Vector3 Normalize(Vector3 vector)
        {
            float length = vector.Length();
            Vector3 res;
            res.X = vector.X / length;
            res.Y = vector.Y / length;
            res.Z = vector.Z / length;

            return res;
        }

        //Flatten Normalize
        public static Vector3 Normalize(Vector2 vector , float last)
        {
            float length = vector.Length();
            Vector3 res;
            res.X = vector.X / length;
            res.Y = last;
            res.Z = vector.Y / length;

            return res;
        }

        public static Vector3 Cross(Vector3 first, Vector3 second)
        {
            Vector3 res;
            res.X = (first.Y * second.Z) - (first.Z * second.Y);
            res.Y = (first.Z * second.X) - (first.X * second.Z);
            res.Z = (first.X * second.Y) - (first.Y * second.X);
            return res;
        }

        public static float Angle(Vector3 from, Vector3 to)
        {
            // sqrt(a) * sqrt(b) = sqrt(a * b) -- valid for real numbers
            float denominator = (float)Math.Sqrt(from.sqrMagnitude * to.sqrMagnitude);
            if (denominator < kEpsilonNormalSqrt)
                return 0F;

            float dot = Mathf.Clamp(Dot(from, to) / denominator, -1F, 1F);
            return ((float)Math.Acos(dot)) * Mathf.Rad2Deg;
        }

        public static bool operator ==(Vector3 left, Vector3 right)
        {
            // Returns false in the presence of NaN values.
            float diff_x = left.X - right.X;
            float diff_y = left.Y - right.Y;
            float diff_z = left.Z - right.Z;
            float sqrmag = diff_x * diff_x + diff_y * diff_y + diff_z * diff_z;
            return sqrmag < kEpsilon * kEpsilon;
        }

        public static bool operator !=(Vector3 left, Vector3 right)
        {
            return !(left == right);
        }

        public static Vector3 MoveTowards(Vector3 current, Vector3 target, float maxDistanceDelta)
        {
            // avoid vector ops because current scripting backends are terrible at inlining
            float toVector_x = target.X - current.X;
            float toVector_y = target.Y - current.Y;
            float toVector_z = target.Z - current.Z;

            float sqdist = toVector_x * toVector_x + toVector_y * toVector_y + toVector_z * toVector_z;

            if (sqdist == 0 || (maxDistanceDelta >= 0 && sqdist <= maxDistanceDelta * maxDistanceDelta))
                return target;
            var dist = (float)Math.Sqrt(sqdist);

            return new Vector3(current.X + toVector_x / dist * maxDistanceDelta,
                current.Y + toVector_y / dist * maxDistanceDelta,
                current.Z + toVector_z / dist * maxDistanceDelta);
        }

        public static float Dot(Vector3 lhs, Vector3 rhs) { return lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z; }
        public static Vector3 operator *(Vector3 left, float scalar)
        {
            return new Vector3(left.X * scalar, left.Y * scalar, left.Z * scalar);
        }
        public static Vector3 operator *(float scalar, Vector3 right)
        {
            return new Vector3(scalar * right.X, scalar * right.Y, scalar * right.Z);
        }

        public static Vector3 operator +(Vector3 left, Vector3 right)
        {
            return new Vector3(left.X + right.X, left.Y + right.Y, left.Z + right.Z);
        }

        public static Vector3 operator *(Vector3 left, Vector3 right)
        {
            return new Vector3(left.X * right.X, left.Y * right.Y, left.Z * right.Z);
        }

        public static Vector3 operator -(Vector3 left, float right)
        {
            return new Vector3(left.X - right, left.Y - right, left.Z - right);
        }

        public static Vector3 operator +(Vector3 left, float right)
        {
            return new Vector3(left.X + right, left.Y + right, left.Z + right);
        }

        public static Vector3 operator -(Vector3 left, Vector3 right)
        {
            return new Vector3(left.X - right.X, left.Y - right.Y, left.Z - right.Z);
        }

        public static Vector3 operator /(Vector3 left, Vector3 right)
        {
            return new Vector3(left.X / right.X, left.Y / right.Y, left.Z / right.Z);
        }

        public static Vector3 operator /(Vector3 left, float scalar)
        {
            return new Vector3(left.X / scalar, left.Y / scalar, left.Z / scalar);
        }

        public static Vector3 operator -(Vector3 vector)
        {
            return new Vector3(-vector.X, -vector.Y, -vector.Z);
        }

        public static Vector3 Cos(Vector3 vector)
        {
            return new Vector3((float)Math.Cos(vector.X), (float)Math.Cos(vector.Y), (float)Math.Cos(vector.Z));
        }

        public static Vector3 Sin(Vector3 vector)
        {
            return new Vector3((float)Math.Sin(vector.X), (float)Math.Sin(vector.Y), (float)Math.Sin(vector.Z));
        }

        public static Vector3 Lerp(Vector3 a, Vector3 b, float t)
        {
            if (t < 0.0f) t = 0.0f;
            if (t > 1.0f) t = 1.0f;
            return (1.0f - t) * a + t * b;
        }

        public override string ToString()
        {
            return "Vector3[" + X + ", " + Y + ", " + Z + "]";
        }

        public Vector2 XY
        {
            get { return new Vector2(X, Y); }
            set { X = value.X; Y = value.Y; }
        }
        public Vector2 XZ
        {
            get { return new Vector2(X, Z); }
            set { X = value.X; Z = value.Y; }
        }
        public Vector2 YZ
        {
            get { return new Vector2(Y, Z); }
            set { Y = value.X; Z = value.Y; }
        }

    }
}
