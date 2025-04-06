using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Collections.Generic;



namespace EosScriptCore
{
    [StructLayout(LayoutKind.Sequential)]
    public class MonoComponent : Object
    {
        public MonoComponent()
            : base()
        {

        }

        internal MonoComponent(Entity _entity)
            : base(_entity)
        {

        }

        internal void Init(Entity _entity)
        {
            entity = _entity;
        }

        public GameObject gameObject
        {
            get
            {
                return new GameObject(entity);
            }
        }

        public Transform transform
        {
            get
            {
                return gameObject.transform;
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void Destroy_Native(Entity item);

        public static void Destroy(Object item)
        {
            Destroy_Native(item.entity);
        }


    }

    [StructLayout(LayoutKind.Sequential)]
    public class Transform : MonoComponent
    {
        internal Vector3 position;

        public Transform() : base()
        {
        }

        internal Transform(Entity _entity)
            : base(_entity)
        {
        }

     

        public Vector3 Translation
        {
            get
            {
                GetTranslation_Native(entity, out Vector3 result);
                return result;
            }

            set
            {
                SetTranslation_Native(entity, ref value);
            }
        }

        public Quaternion Rotation
        {
            get
            {
                GetRotation_Native(entity, out Quaternion result);
                return result;
            }

            set
            {
                SetRotation_Native(entity, ref value);
            }
        }

        public Vector3 Scale
        {
            get
            {
                GetScale_Native(entity, out Vector3 result);
                return result;
            }

            set
            {
                SetScale_Native(entity, ref value);
            }
        }


        public Vector3 right { get { return Rotation * Vector3.Right; } set { Rotation = Quaternion.FromToRotation(Vector3.Right, value); } }

        public Vector3 up { get { return Rotation * Vector3.Up; } set { Rotation = Quaternion.FromToRotation(Vector3.Up, value); } }
        public Vector3 forward { get { return Rotation * Vector3.Forward; } set { Rotation = Quaternion.LookRotation(value); } }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern Entity GetParent_Native(Entity item);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void SetParent_Native(Entity item, Entity parent);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void DetachParent_Native(Entity item);

        public Transform parent
        {
            get
            {
                return new Transform(GetParent_Native(entity));
            }
            set
            {
                SetParent_Native(entity, value.entity);
            }
        }

        public void SetParent(Transform parent)
        {
            SetParent_Native(entity, parent.entity);
        }

        public void DetachParent()
        {
            DetachParent_Native(entity);
        }


        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void GetTranslation_Native(Entity entity, out Vector3 outTranslation);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetTranslation_Native(Entity entity, ref Vector3 inTranslation);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void GetRotation_Native(Entity entity, out Quaternion outRotation);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetRotation_Native(Entity entity, ref Quaternion inRotation);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void GetScale_Native(Entity entity, out Vector3 outScale);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetScale_Native(Entity entity, ref Vector3 inScale);
    }

    [StructLayout(LayoutKind.Sequential)]
    public class Collider : MonoComponent
    {
        public Collider()
            : base()
        {
        }

        internal Collider(Entity _entity)
            : base(_entity)
        {
            entity = _entity;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern bool GetIsTrigger(Entity item);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void SetIsTrigger(Entity item, bool isTrigger);
        public bool isTrigger
        {
            get
            {
                return GetIsTrigger(entity);
            }
            set
            {
                SetIsTrigger(entity, value);
            }
        }
    }


    [StructLayout(LayoutKind.Sequential)]
    public class RigidBody : MonoComponent
    {
        public enum Type
        {
            STATIC,
            DYANMIC
        }

        public Type BodyType
        {
            get
            {
                return GetBodyType_Native(entity);
            }

        }

        public float Mass
        {
            get { return GetMass_Native(entity); }
            set { SetMass_Native(entity, value); }
        }

        public Vector3 LinearVelocity
        {
            get
            {
                GetLinearVelocity_Native(entity, out Vector3 velocity);
                return velocity;
            }

            set { SetLinearVelocity_Native(entity, ref value); }
        }

        public Vector3 AngularVelocity
        {
            get
            {
                GetAngularVelocity_Native(entity, out Vector3 velocity);
                return velocity;
            }

            set { SetAngularVelocity_Native(entity, ref value); }
        }

        public void AddForce(Vector3 force)
        {
            AddForce_Native(entity, ref force);
        }

        public void MovePosition(Vector3 pos)
        {
            MovePosition_Native(entity, ref pos);
        }

        public void RotateAlongAxis(Vector3 angle)
        {
            RotateAlongAxis_Native(entity, ref angle);
        }

        public Vector3 GetLinearVelocity()
        {
            GetLinearVelocity_Native(entity, out Vector3 velocity);
            return velocity;
        }

        public void SetLinearVelocity(Vector3 velocity)
        {
            SetLinearVelocity_Native(entity, ref velocity);
        }

        public Vector3 GetAngularVelocity()
        {
            GetAngularVelocity_Native(entity, out Vector3 velocity);
            return velocity;
        }

        public void SetAngularVelocity(Vector3 velocity)
        {
            SetAngularVelocity_Native(entity, ref velocity);
        }

        public Vector3 GetRotate()
        {
            GetRotate_Native(entity, out Vector3 rotation);
            return rotation;
        }

        public void SetRotate(Vector3 Rotation)
        {
            SetRotate_Native(entity, ref Rotation);
        }


        public void UseGravity(bool use)
        {
            UseGravity_Native(entity, use);
        }


        public void SetGravity(Vector3 gravity)
        {
            SetGravity_Native(entity, ref gravity);
        }

        public Vector3 GetGravity()
        {
            GetGravity_Native(entity, out Vector3 velocity);
            return velocity;
        }


        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetStatic_Native(Entity entity, ref bool use);
        public void SetStatic(bool use)
        {
            SetStatic_Native(entity, ref use);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetDynamic_Native(Entity entity, ref bool use);
        public void SetDynamic(bool use)
        {
            SetDynamic_Native(entity, ref use);
        }


        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetKinematics_Native(Entity entity, ref bool use);
        public void SetKinematics(bool use)
        {
            SetKinematics_Native(entity, ref use);
        }

        


        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void AddForce_Native(Entity entity, ref Vector3 force);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void MovePosition_Native(Entity entity, ref Vector3 pos);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void GetRotate_Native(Entity entity, out Vector3 rotate);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetRotate_Native(Entity entity, ref Vector3 rotate);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void GetLinearVelocity_Native(Entity entity, out Vector3 velocity);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetLinearVelocity_Native(Entity entity, ref Vector3 velocity);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void GetAngularVelocity_Native(Entity entity, out Vector3 velocity);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetAngularVelocity_Native(Entity entity, ref Vector3 velocity);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float GetMass_Native(Entity entity);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float SetMass_Native(Entity entity, float mass);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern Type GetBodyType_Native(Entity entityD);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void UseGravity_Native(Entity entity, bool use);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void GetGravity_Native(Entity entity, out Vector3 velocity);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetGravity_Native(Entity entity, ref Vector3 velocity);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void RotateAlongAxis_Native(Entity entity, ref Vector3 angle);
    }

    //As for now, not component based
    [StructLayout(LayoutKind.Sequential)]
    public class Camera : MonoComponent
    {

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void GetCameraPosition_Native(Entity entity, out Vector3 Pos);
        public Vector3 GetCameraPosition()
        {
            GetCameraPosition_Native(entity, out Vector3 Pos);
            return Pos;
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetCameraPosition_Native(Entity entity, ref Vector3 Up);
        public void SetCameraPosition(Vector3 pos)
        {
            SetCameraPosition_Native(entity, ref pos);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void GetTargetPosition_Native(Entity entity, out Vector3 Tpos);
        public Vector3 GetTargetPosition()
        {
            GetTargetPosition_Native(entity, out Vector3 Tpos);
            return Tpos;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetTargetPosition_Native(Entity entity, ref Vector3 Up);
        public void SetTargetPosition(Vector3 pos)
        {
            SetTargetPosition_Native(entity, ref pos);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void GetTargetRotation_Native(Entity entity, out Vector3 Tpos);
        public Vector3 GetTargetRotation()
        {
            GetTargetRotation_Native(entity, out Vector3 Tpos);
            return Tpos;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetTargetRotation_Native(Entity entity, ref Vector3 Up);
        public void SetTargetRotation(Vector3 pos)
        {
            SetTargetRotation_Native(entity, ref pos);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern int GetScreenSizeX_Native();
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern int GetScreenSizeY_Native();

        public Vector2 GetScreenWidthHeight()
        {
            return new Vector2((float)GetScreenSizeX_Native(), (float)GetScreenSizeY_Native());
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool GetActive_Native(Entity entity);
        public bool GetActive()
        {
            return GetActive_Native(entity);
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern int SetActive_Native(Entity entity, ref bool active);
        public void SetActive(bool active)
        {
            SetActive_Native(entity, ref active);
        }



        //[MethodImpl(MethodImplOptions.InternalCall)]
        //internal static extern void GetMainCameraCenter_Native(out Vector3 center);
        //public Vector3 GetMainCameraCenter()
        //{
        //    GetMainCameraCenter_Native(out Vector3 center);
        //    return center;
        //}
        //[MethodImpl(MethodImplOptions.InternalCall)]
        //internal static extern void SetMainCameraCenter_Native(ref Vector3 center);
        //public void SetMainCameraCenter(Vector3 center)
        //{
        //    SetMainCameraCenter_Native(ref center);
        //}

        //[MethodImpl(MethodImplOptions.InternalCall)]
        //internal static extern void GetMainCameraEye_Native(out Vector3 eye);

        //public Vector3 GetMainCameraEye()
        //{
        //    GetMainCameraEye_Native(out Vector3 eye);
        //    return eye;
        //}
        //[MethodImpl(MethodImplOptions.InternalCall)]
        //internal static extern void SetMainCameraEye_Native(ref Vector3 eye);
        //public void SetMainCameraEye(Vector3 eye)
        //{
        //    SetMainCameraEye_Native(ref eye);
        //}

        //// /////////////////////////////////////////////////////////////////////////

        //[MethodImpl(MethodImplOptions.InternalCall)]
        //internal static extern void GetMainCameraUp_Native(Entity entity,out Vector3 Up);

        //public Vector3 GetMainCameraUp()
        //{
        //    GetMainCameraUp_Native(entity, out Vector3 Up);
        //    return Up;
        //}
        //[MethodImpl(MethodImplOptions.InternalCall)]
        //internal static extern void SetMainCameraUp_Native(Entity entity, ref Vector3 Up);

        //public void SetMainCameraUp(Vector3 eye)
        //{
        //    SetMainCameraUp_Native(entity, ref eye);
        //}

    }


    [StructLayout(LayoutKind.Sequential)]
    public class Animation : MonoComponent
    {
        public enum Anistate
        {
            Idle,
            Walk,
            Finish
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern string GetAnimateState_Native(Entity entity);
        public string GetAnimateState()
        {
            return GetAnimateState_Native(entity);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetAnimateState_Native(Entity entity, string state);
        public void SetAnimateState(string state)
        {
            SetAnimateState_Native(entity, state);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool IsAnimationCompleted_Native(Entity entity);
        public bool IsAnimationCompleted()
        {
            return IsAnimationCompleted_Native(entity);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void ResetCompletedAnimation_Native(Entity entity);
        public void ResetCompletedAnimation()
        {
            IsAnimationCompleted_Native(entity);
        }


    }

    [StructLayout(LayoutKind.Sequential)]
    public class AudioComponent : MonoComponent
    {
        public void PlayBGM(string audioName)
        {
            PlayBGM_Native(entity, audioName);
        }

        public void PlaySFX(string audioName)
        {
            PlaySFX_Native(entity, audioName);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void PlayBGM_Native(Entity entity, string audioName);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void PlaySFX_Native(Entity entity, string audioName);
    }


    [StructLayout(LayoutKind.Sequential)]
    public class UIComponent : MonoComponent
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float GetOpacity_Native(Entity entity);

        public float GetOpacity()
        {
            return GetOpacity_Native(entity);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetOpacity_Native(Entity entity, float opaque);
        public void SetOpacity(float opaque)
        {
            SetOpacity_Native(entity, opaque);
        }


        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void GetColor_Native(Entity entity, out Vector3 color);

        public Vector3 GetColor()
        {
            GetColor_Native(entity, out Vector3 color);
            return color;
        }


        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetColor_Native(Entity entity, ref Vector3 audioName);
        public void SetColor(Vector3 color)
        {
            SetColor_Native(entity, ref color);
        }

        #region not sure if this is working,not tested yet
        #endregion


        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetDepth_Native(Entity entity, float d_value);
        public void SetDepth(float d_value)
        {
            SetDepth_Native(entity, d_value);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern string GetFilePath_Native(Entity entity);

        public string GetFilePath()
        {
            return GetFilePath_Native(entity);
        }


        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetFilePath_Native(Entity entity, string path);

        public void SetFilePath(string path)
        {
            SetFilePath_Native(entity, path);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float GetDepth_Native(Entity entity);

        public float GetDepth()
        {
            return GetDepth_Native(entity);
        }

    }

    [StructLayout(LayoutKind.Sequential)]
    public class ParticleComponent : MonoComponent
    {

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetParticleNumber_Native(Entity entity, int d_value);
        public void SetParticleNumber(int size)
        {
            SetParticleNumber_Native(entity, size);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern int GetParticleNumber_Native(Entity entity);

        public int GetParticleNumber()
        {
            return GetParticleNumber_Native(entity);
        }


        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetParticleSize_Native(Entity entity, float d_value);
        public void SetParticleSize(float size)
        {
            SetParticleSize_Native(entity, size);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float GetParticleSize_Native(Entity entity);

        public float GetParticleSize()
        {
            return GetParticleSize_Native(entity);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetParticleLife_Native(Entity entity, float d_value);
        public void SetParticleLife(float size)
        {
            SetParticleLife_Native(entity, size);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float GetParticleLife_Native(Entity entity);

        public float GetParticleLife()
        {
            return GetParticleLife_Native(entity);
        }


    }

    [StructLayout(LayoutKind.Sequential)]
    public class StaticMeshComponent : MonoComponent
    {

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void LoadModel_Native(Entity entity, string file);

        public void LoadModel(string file)
        {
            LoadModel_Native(entity, file);
        }

    }


}



