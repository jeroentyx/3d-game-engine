using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Collections.Generic;

namespace EosScriptCore
{

    [StructLayout(LayoutKind.Sequential)]
    internal struct Entity
    {
        internal uint _id;
        internal uint _generations;

        public Entity(Entity entity)
        {
            this._id = entity._id;
            this._generations = entity._generations;
        }

        public bool IsNull { get { return (_id & 0xFFFFF) == 0xFFFFF; } }

        public static Entity Null { get { return new Entity { _id = 0xFFFFF }; } }


    }


    
}
