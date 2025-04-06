using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Collections.Generic;

namespace EosScriptCore
{
    [StructLayout(LayoutKind.Sequential)]
    public class GameObject : Object
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool GetActive_Native(Entity entity);
        public bool GetActive()
        {
            return GetActive_Native(entity);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetActive_Native(Entity entity, bool active);
        public void SetActive(bool value)
        {
            SetActive_Native(entity, value);
        }
        internal GameObject(Entity _entity)
            : base(_entity)
        {
           
        }

        public Transform transform
        {
            get
            {
                return new Transform(entity);
            }
        }


        public T CreateComponent<T>() where T : MonoComponent, new()
        {
            CreateComponent_Native(entity, typeof(T));
            T comp = (T)(object)(new T());
            comp.Init(entity);
            return comp;
        }

        public bool HasComponent<T>() where T : MonoComponent, new()
        {
            Type type = typeof(T);
            return HasComponent_Native(entity, type.Name);
        }

        public T GetComponent<T>() where T : MonoComponent, new()
        {
            if (HasComponent<T>())
            {
                T comp = (T)(object)(new T());
                comp.Init(entity);
                return comp;
            }
            return null;
        }



        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Entity FindGameObjectByTag_Native(string tag);
        public GameObject FindWithTag(string tag)
        {
            return new GameObject(FindGameObjectByTag_Native(tag));
        }


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern Entity[] FindGameObjectsWithTag_Native(string name);
        public GameObject[] FindGameObjectsWithTag(string tag)
        {
            List<GameObject> retList = new List<GameObject>();
            Entity[] entities = FindGameObjectsWithTag_Native(tag);

            foreach (Entity e in entities)
            {
                retList.Add(new GameObject(e));
            }

            return retList.ToArray();
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern Entity[] GetAllChild_Native(Entity ent);
        public GameObject[] GetAllChild(GameObject ent)
        {
            List<GameObject> retList = new List<GameObject>();
            Entity[] entities = GetAllChild_Native(ent.entity);

            foreach (Entity e in entities)
            {
                retList.Add(new GameObject(e));
            }

            return retList.ToArray();
        }


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern string GetName_Native(Entity item);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void SetName_Native(Entity item, string value);
        virtual public string name
        {
            get { return GetName_Native(entity); }
            set { SetName_Native(entity, value); }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void Preload_Native(string name);
        public static void Preload(string name)
        {
            Preload_Native(name);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern Entity[] GetComponentsInChildren_Native(Entity entity, string name, int recursive, out int isScript);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern MonoComponent[] GetScriptsInChildren_Native(Entity entity, string name, int recursive);
        public T[] GetComponentsInChildren<T>() where T : MonoComponent, new()
        {
            List<T> retList = new List<T>();
            int isScript = 0;

            Entity[] entities = GetComponentsInChildren_Native(entity, typeof(T).Name, 1, out isScript);

            if(isScript == 0)
            {
                foreach(Entity entity in entities)
                {
                    T comp = (T)(object)(new T());
                    comp.Init(entity);
                    retList.Add(comp);
                }
            }
            else
            {
                MonoComponent[] scripts = GetScriptsInChildren_Native(entity, typeof(T).Name, 1);

                foreach (MonoComponent script in scripts)
                {
                    retList.Add((T)(object)(script));
                }
            }
    
            return retList.ToArray();
        }


        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint GetEntityID_Native();
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void CreateComponent_Native(Entity entity, Type type);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool HasComponent_Native(Entity entity, string typeOf);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool EntityExists_Native(Entity entity);

        
    }
}
