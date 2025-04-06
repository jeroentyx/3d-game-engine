using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Collections.Generic;

namespace EosScriptCore
{

    [StructLayout(LayoutKind.Sequential)]
    public class Object
    {
        internal Entity entity;

        //Private Setting
        public uint id => entity._id;

        public Object()
        {
            entity = Entity.Null;
        }

        internal Object(Entity _entity)
        {
            entity = _entity;
        }

        public bool IsNull()
        {
            return entity.IsNull;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern Entity InstantiatePrefab_Native(string original, Vector3 position, Vector3 rotation, Entity parent);
        public static GameObject Instantiate(string original)
        {
            return new GameObject(InstantiatePrefab_Native(original, new Vector3(), new Vector3(), Entity.Null));
        }
        public static GameObject Instantiate(string original, Transform parent)
        {
            return new GameObject(InstantiatePrefab_Native(original, new Vector3(), new Vector3(), parent.entity));
        }
        public static GameObject Instantiate(string original, Vector3 position, Quaternion rotation)
        {
            return new GameObject(InstantiatePrefab_Native(original, position, rotation.XYZ, Entity.Null));
        }
        public static GameObject Instantiate(string original, Vector3 position, Quaternion rotation, Transform parent)
        {
            return new GameObject(InstantiatePrefab_Native(original, position, rotation.XYZ, parent.entity));
        }

    }
}
