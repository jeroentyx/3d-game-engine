using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace EosScriptCore
{

   
    [StructLayout(LayoutKind.Sequential)]
    public static class Physics
    {

    
        public static void SetFixedTimestep(float fixedTimestep)
        {
            SetFixedTimestep_Native(fixedTimestep);
        }
        public static float GetFixedTimestep()
        {
            return GetFixedTimestep_Native();
        }
        public static void SetGravity(Vector3 inGravity)
        {
            SetGravity_Native(ref inGravity);
        }
        public static Vector3 GetGravity()
        {
            GetGravity_Native(out Vector3 gravity);
            return gravity;
        }
        public static void SetSolverIterations(uint solverIterations)
        {
            SetSolverIterations_Native(solverIterations);
        }
        public static uint GetSolverIterations()
        {
            return GetSolverIterations_Native();
        }
        public static void SetSolverVelocityIterations(uint solverVelocityIterations)
        {
            SetSolverVelocityIterations_Native(solverVelocityIterations);
        }
        public static uint GetSolverVelocityIterations()
        {
            return GetSolverVelocityIterations_Native();
        }


        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetFixedTimestep_Native(float fixedTimestep);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float GetFixedTimestep_Native();
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetGravity_Native(ref Vector3 inGravity);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void GetGravity_Native(out Vector3 outGravity);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetSolverIterations_Native(uint solverIterations);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern uint GetSolverIterations_Native();
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetSolverVelocityIterations_Native(uint solverVelocityIterations);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern uint GetSolverVelocityIterations_Native();
    }



}
