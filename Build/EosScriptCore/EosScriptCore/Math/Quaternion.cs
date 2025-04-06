using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;
using System.Numerics;

namespace EosScriptCore
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Quaternion
    {
        public float x;
        public float y;
        public float z;
        public float w;

        public Quaternion(Vector3 v, float w)
        {
            this.x = v.X;
            this.y = v.Y;
            this.z = v.Z;
            this.w = w;
        }

        public Quaternion(Vector3 v)
        {
            this.x = v.X;
            this.y = v.Y;
            this.z = v.Z;
            this.w = 0.0f;
        }
        public Quaternion(Vector4 v)
        {
            this.x = v.X;
            this.y = v.Y;
            this.z = v.Z;
            this.w = v.W;
        }

        public Quaternion(float x, float y, float z, float w) { this.x = x; this.y = y; this.z = z; this.w = w; }
        static readonly Quaternion identityQuaternion = new Quaternion(0F, 0F, 0F, 1F);

        public float LengthSquared
        {
            get
            {
                return x * x + y * y + z * z + w * w;
            }
        }
        public static Quaternion identity
        {
            get
            {
                return identityQuaternion;
            }
        }

        public static float Dot(Quaternion a, Quaternion b)
        {
            return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern Quaternion QuatCast_Native(Vector3 Rot);
        public static Quaternion QuatCast(Vector3 Rot)
        {
            return QuatCast_Native(Rot);
        }


        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern Quaternion Euler_Native(Vector3 euler);
        public static Quaternion Euler(float X, float Y, float Z)
        {
            return Euler_Native(new Vector3(X, Y, Z) * Mathf.Deg2Rad);
        }


        public static Quaternion Normalize(Quaternion q)
        {
            float mag = Mathf.Sqrt(Dot(q, q));

            if (mag < Mathf.Epsilon)
                return Quaternion.identity;

            return new Quaternion(q.x / mag, q.y / mag, q.z / mag, q.w / mag);
        }
        public static Quaternion operator *(Quaternion lhs, Quaternion rhs)
        {
            return new Quaternion(
                lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
                lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z,
                lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x,
                lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z);
        }

        public static Vector3 operator *(Quaternion rotation, Vector3 point)
        {
            float x = rotation.x * 2F;
            float y = rotation.y * 2F;
            float z = rotation.z * 2F;
            float xx = rotation.x * x;
            float yy = rotation.y * y;
            float zz = rotation.z * z;
            float xy = rotation.x * y;
            float xz = rotation.x * z;
            float yz = rotation.y * z;
            float wx = rotation.w * x;
            float wy = rotation.w * y;
            float wz = rotation.w * z;

            Vector3 res;
            res.X = (1F - (yy + zz)) * point.X + (xy - wz) * point.Y + (xz + wy) * point.Z;
            res.Y = (xy + wz) * point.X + (1F - (xx + zz)) * point.Y + (yz - wx) * point.Z;
            res.Z = (xz - wy) * point.X + (yz + wx) * point.Y + (1F - (xx + yy)) * point.Z;
            return res;
        }

        public static Quaternion LookRotation(Vector3 forward)
        {
            Vector3 up = Vector3.Up;
            return Quaternion.LookRotation(forward, up);
        }
        public static Quaternion LookRotation(Vector3 forward, Vector3 up)
        {

            forward.Normalized(); 
            Vector3 right = Vector3.Normalize(Vector3.Cross(up, forward));
            up = Vector3.Cross(forward, right);
            var m00 = right.X;
            var m01 = right.Y;
            var m02 = right.Z;
            var m10 = up.X;
            var m11 = up.Y;
            var m12 = up.Z;
            var m20 = forward.X;
            var m21 = forward.Y;
            var m22 = forward.Z;


            float num8 = (m00 + m11) + m22;
            var quaternion = new Quaternion();
            if (num8 > 0f)
            {
                var num = (float)System.Math.Sqrt(num8 + 1f);
                quaternion.w = num * 0.5f;
                num = 0.5f / num;
                quaternion.x = (m12 - m21) * num;
                quaternion.y = (m20 - m02) * num;
                quaternion.z = (m01 - m10) * num;
                return quaternion;
            }
            if ((m00 >= m11) && (m00 >= m22))
            {
                var num7 = (float)System.Math.Sqrt(((1f + m00) - m11) - m22);
                var num4 = 0.5f / num7;
                quaternion.x = 0.5f * num7;
                quaternion.y = (m01 + m10) * num4;
                quaternion.z = (m02 + m20) * num4;
                quaternion.w = (m12 - m21) * num4;
                return quaternion;
            }
            if (m11 > m22)
            {
                var num6 = (float)System.Math.Sqrt(((1f + m11) - m00) - m22);
                var num3 = 0.5f / num6;
                quaternion.x = (m10 + m01) * num3;
                quaternion.y = 0.5f * num6;
                quaternion.z = (m21 + m12) * num3;
                quaternion.w = (m20 - m02) * num3;
                return quaternion;
            }
            var num5 = (float)System.Math.Sqrt(((1f + m22) - m00) - m11);
            var num2 = 0.5f / num5;
            quaternion.x = (m20 + m02) * num2;
            quaternion.y = (m21 + m12) * num2;
            quaternion.z = 0.5f * num5;
            quaternion.w = (m01 - m10) * num2;
            return quaternion;
        }
        public static Quaternion FromToRotation(Vector3 aFrom, Vector3 aTo)
        {

            Vector3 axis = Vector3.Cross(aFrom, aTo);
            float angle = Vector3.Angle(aFrom, aTo);
            return Quaternion.AngleAxis(angle, axis.Normalized());
        }
        public static Quaternion AngleAxis(float aAngle, Vector3 aAxis)
        {
            aAxis.Normalize();
            float rad = aAngle * Mathf.Deg2Rad * 0.5f;
            aAxis *= Mathf.Sin(rad);
            return new Quaternion(aAxis.X, aAxis.Y, aAxis.Z, Mathf.Cos(rad));
        }


        public static Quaternion Slerp(Quaternion a, Quaternion b, float t)
        {
            if (t > 1) t = 1;
            if (t < 0) t = 0;
            return SlerpUnclamped(a, b, t);
        }

        public static Quaternion SlerpUnclamped(Quaternion a, Quaternion b, float t)
        {
            if (a.LengthSquared == 0.0f)
            {
                if (b.LengthSquared == 0.0f)
                {
                    return identity;
                }
                return b;
            }
            else if (b.LengthSquared == 0.0f)
            {
                return a;
            }


            float cosHalfAngle = a.w * b.w + Vector3.Dot(a.XYZ, b.XYZ);

            if (cosHalfAngle >= 1.0f || cosHalfAngle <= -1.0f)
            {
                // angle = 0.0f, so just return one input.
                return a;
            }
            else if (cosHalfAngle < 0.0f)
            {
                b.XYZ = -b.XYZ;
                b.w = -b.w;
                cosHalfAngle = -cosHalfAngle;
            }

            float blendA;
            float blendB;
            if (cosHalfAngle < 0.99f)
            {
                // do proper slerp for big angles
                float halfAngle = (float)System.Math.Acos(cosHalfAngle);
                float sinHalfAngle = (float)System.Math.Sin(halfAngle);
                float oneOverSinHalfAngle = 1.0f / sinHalfAngle;
                blendA = (float)System.Math.Sin(halfAngle * (1.0f - t)) * oneOverSinHalfAngle;
                blendB = (float)System.Math.Sin(halfAngle * t) * oneOverSinHalfAngle;
            }
            else
            {
                // do lerp if angle is really small.
                blendA = 1.0f - t;
                blendB = t;
            }

            Quaternion result = new Quaternion(blendA * a.XYZ + blendB * b.XYZ, blendA * a.w + blendB * b.w);
            if (result.LengthSquared > 0.0f)
                return Normalize(result);
            else
                return identity;
        }



        public Vector3 XYZ
        {
            get { return new Vector3(x, y, z); }
            set { x = value.X; y = value.Y; z = value.Z ; }
        }

        public override string ToString()
        {
            return "Quaternion[" + x + ", " + y + ", " + z + ", " + w + "]";
        }
    }
}
