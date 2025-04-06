using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public class SceneCamera : MonoBase
    {

        //For the SceneObject to be looked at
        public Vector3 TargetCamera;


        public static SceneCamera SceneController;
        private GameObject PlayerCamera;
        private Camera PlayerCameraCom;
        private GameObject Scene1;
        private Camera Scene1Cam;
        

        private GameObject Frame;
        private float moveSpeed;

        private GameObject BlackScreen;
        private UIComponent BlackScreenUI;

        private Vector3 direction;

        public bool activateCutscene =  false;
        public bool blackin =  false;

        public float StopTimer = 5.0f;

        public float CameraSpeed = 250;
        public float val = 0.0f;


        private GameObject CutSceneBar1; 
        private GameObject CutSceneBar2;

        void OnStart()
        {
            SceneController = this;
            CutSceneBar1 = gameObject.FindWithTag("CutSceneBar1");
            CutSceneBar2 = gameObject.FindWithTag("CutSceneBar2");



            PlayerCamera = gameObject.FindWithTag("Camera");
            PlayerCameraCom = PlayerCamera.GetComponent<Camera>();
            Scene1 = gameObject.FindWithTag("SceneCamera1");
            Scene1Cam = Scene1.GetComponent<Camera>();
           


            Frame = gameObject.FindWithTag("KeyFrame");

            

            BlackScreen = gameObject.FindWithTag("BlackScreen");
            BlackScreenUI = BlackScreen.GetComponent<UIComponent>();
            

        }
        void OnUpdate()
        {

            if(StopTimer > 0)
            {
                StopTimer -= Time.deltaTime;
                return;
            }

            if (activateCutscene)
            {
                CameraTransition();
                Blackout();
            }
            else
            {

            }
          

        }

        public void ActivateCutscene()
        {
            //Should Deactive Surge from moving


            activateCutscene = true;
            Scene1.SetActive(true);
            PlayerCamera.SetActive(false);
            Scene1Cam.SetTargetPosition(TargetCamera);
            BlackScreen.SetActive(true);
            BlackScreenUI.SetOpacity(0.0f);
            CutSceneBar1.SetActive(true);
            CutSceneBar2.SetActive(true);
        }

        public void DeactivateCutscene()
        {
            activateCutscene = false;
            PlayerCamera.SetActive(true);
            Scene1.SetActive(false);
            BlackScreen.SetActive(false);
            CutSceneBar1.SetActive(false);
            CutSceneBar2.SetActive(false);
        }


        public void CameraTransition()
        {
            //Scene1.transform.Translation = Scene1.transform.Translation * CameraSpeed * Time.deltaTime;

            Scene1.transform.Translation = Vector3.MoveTowards(Scene1.transform.Translation, Frame.transform.Translation, CameraSpeed * Time.deltaTime);
            Log.LogInfo(Scene1.transform.Translation.ToString());


            Scene1Cam.SetCameraPosition(Scene1.transform.Translation);
            
        }



        public void Blackout()
        {
            val += 0.01f;
            BlackScreenUI.SetOpacity(val);
            //if (BlackScreenUI.GetOpacity() > 1.0f)
            //{
            //    blackin = true;
            //}
        }

        public void BlackIn()
        {
            val -= 0.01f;
            BlackScreenUI.SetOpacity(val);
            if (BlackScreenUI.GetOpacity() <= 0.0f)
            {
                
            }
            Log.LogInfo(val.ToString());
        }

        public void CameraTransitionFromData()
        {

        }
    }
}
