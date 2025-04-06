using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace EosScriptCore.Scripts
{
    [StructLayout(LayoutKind.Sequential)]
    public class PlayerMovement : MonoBase
    {
        public static PlayerMovement playerController;
        public List<Vector3> posList = new List<Vector3>();
        public List<Quaternion> rotList = new List<Quaternion>();
        Vector3 lastPos = Vector3.positiveInfinity;


        public static PlayerMovement globalController;
        public Transform surgeTransform;
        private RigidBody surgeRigid;
        private Animation animate;
        private GameObject CamObj;
       
        private AudioComponent BGM;
        private Collider surgeCollider;

        //Delta time
        float delta;

        Vector3 velocity = Vector3.Zero;
        private Vector3 m_Force;
        public float moveAccel = 125f;
        public float moveSpeed;// = 5f;
        public float frictionFactor = 0.98f;

        bool surgeDie = false;

        public bool teleported = false;

        private bool firstLoop = true;

        //Flags for interactions
        public bool bridgeInsert = false;
        public bool teleportInsert = false;

        void OnStart()
        {
            CamObj = gameObject.FindWithTag("Camera");
            //the current gamobject alr contains the surge transform
            surgeTransform = gameObject.GetComponent<Transform>();
            surgeRigid = gameObject.GetComponent<RigidBody>();
            surgeCollider = gameObject.GetComponent<Collider>();
  
            animate = gameObject.GetComponent<Animation>();
            animate.ResetCompletedAnimation();
            // ***** This audio will be put in a different script *****
            BGM = gameObject.GetComponent<AudioComponent>();
            globalController = this;
            
            bridgeInsert = false;
            teleportInsert = false;

            Input.SetCursorMode(Input.Cursor.Locked);
        }

        void OnUpdate()
        {
            

            //Log.LogInfo(moveSpeed.ToString());

            //This will ensure the updated surge tranform location
            //surgeTransform.Translation = surgeRigid.transform.Translation;


            //if the menu is not open
            {
                //This line determines the forward direction of the player based in the camera update
                var PlayerMovementForward = new Vector3(CamObj.transform.forward.X, 0, CamObj.transform.forward.Z).Normalized();

                //var PlayerMovementForward = new Vector3(0f, 0f, 1f);

                //Log.LogInfo("Camera Front: " + CamObj.transform.forward.ToString());

                var PlayerMovementRight = new Vector3(CamObj.transform.right.X, 0, CamObj.transform.right.Z).Normalized();

                //var PlayerMovementRight = new Vector3(1f, 0f, 0f);

                //Log.LogInfo("Camera Right: " + CamObj.transform.right.ToString());

                //Collect the input from the player
                //The GetAxisRaw will only return 0, -1, 1 depending on the input
                //This will get the normalized vector of the player is facing based on input and camera direction
                var Axis = PlayerMovementForward * Input.GetAxisRaw("Vertical")
                      + PlayerMovementRight * Input.GetAxisRaw("Horizontal");

                if (Axis.Length() > 0)
                    animate.SetAnimateState("Walk");
                
                else
                {
                    animate.SetAnimateState("Idle");
                }


                float AxisLength = Axis.Length();
                if (AxisLength > 1.0f)
                    Axis /= AxisLength;

                //Apply the Force to the player 
                m_Force += Axis * moveAccel * Time.deltaTime;

                //Log.LogInfo(Time.deltaTime.ToString());

                if (Axis.Length() > 0.5f)
                    surgeTransform.Rotation = Quaternion.LookRotation(Axis, Vector3.Up);


                //Log.LogInfo(surgeTransform.Rotation.ToString());

            }
       

            //Cheat Codes
            if (Input.IsKeyPressed(KeyCode.O))
            {
                surgeTransform.Translation = new Vector3(0, 0, 0);
            }
            if (Input.IsKeyPressed(KeyCode.P))
            {
                surgeRigid.UseGravity(false);
            }
            if (Input.IsKeyPressed(KeyCode.K))
            {
                surgeRigid.UseGravity(true);
            }

            if(Input.IsKeyPressed(KeyCode.Space))
            {
                surgeTransform.Translation = new Vector3(0, 50, 0);
            }

            if (Input.IsKeyPressed(KeyCode.N))
            {
                GameStateManager.SetGameState(GameStateManager.ENGINESTATE.STOP);
                SceneMananger.LoadNextScene("Assets/Data/Level_4.json");
            }

            if (Input.IsKeyPressed(KeyCode.M))
            {
                GameStateManager.SetGameState(GameStateManager.ENGINESTATE.STOP);
                SceneMananger.LoadNextScene("Assets/Data/Level_5.json");
            }

            if (Input.IsKeyPressed(KeyCode.B))
            {
                GameStateManager.SetGameState(GameStateManager.ENGINESTATE.STOP);
                SceneMananger.LoadNextScene("Assets/Data/MainMenu.json");
            }

            //Cheat Codes


            if (Vector3.Distance(lastPos, surgeTransform.Translation) > 0.1f || lastPos == Vector3.positiveInfinity)
            {
                posList.Insert(0, surgeTransform.Translation);
                rotList.Insert(0, surgeTransform.Rotation);
                if (posList.Count > 300)
                {
                    posList.RemoveAt(posList.Count - 1);
                    rotList.RemoveAt(rotList.Count - 1);
                }
                lastPos = surgeTransform.Translation;
            }

        }

        void OnFixedUpdate()
        {
            delta = Physics.GetFixedTimestep();
            m_Force *= frictionFactor;
            surgeRigid.AddForce(m_Force * 50.0f);

            m_Force = Vector3.Zero;

            //Taking the facing direction of the camera to determine forward movement of the character

        }

        public void HalfSpeed(bool mode)
        {
            if(mode == true)
            {
                moveAccel /= 2;
            }
            else if(mode == false)
            {
                moveAccel = 125f;
            }

        }

        public void SurgeDie(bool mode) 
        {
            surgeDie = mode;
        }

    }
}
