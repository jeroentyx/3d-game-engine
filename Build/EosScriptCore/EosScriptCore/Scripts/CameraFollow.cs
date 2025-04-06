using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public class CameraFollow : MonoBase
    {

        public Vector3 target;
        private Vector3 newPos;
        public Transform camTransform;
        public GameObject player;
        private Camera CamComp;

        private float Y_angle_min = 0f;
        private float Y_angle_max = 45f;

        private float distance = 15f;
        private float currentX = 0f;
        private float currentY = 45f;
        private Vector2 current;
        private Vector2  midcam = new Vector2( 0f, 0f );
        private Vector2 prevPos = new Vector2(0f, 0f);

        void OnStart()
        {
            
            player = gameObject.FindWithTag("Surge");
            CamComp = gameObject.GetComponent<Camera>();
            Input.SetCursorMode(Input.Cursor.Locked);
           
            Input.SetMousePosition(midcam);
        }

      
        void OnUpdate()
        {
            target = player.transform.Translation;
            
            float sens = 0.05f;
            //Log.LogInfo("Mouse Position: " + Input.GetMousePosition().ToString());
            if (Input.IsJoystickConnected())
            {
                current = Input.CameraControllerMovement();
            
                currentX -= current.X * sens;
                currentY += current.Y * sens; 
            }
            else
            {

                currentX -= Input.GetAxisRaw("MouseX") * sens;   
                currentY += Input.GetAxisRaw("MouseY") * sens;
            
            }
            
            //commented to show code
            currentY = Mathf.Clamp(currentY, Y_angle_min, Y_angle_max);
            Vector3 dir = new Vector3(0, 0, -distance);
            Quaternion rotation = Quaternion.Euler(currentY, currentX, 0);

            newPos = target + rotation * dir;

            CamComp.SetCameraPosition(newPos);
            CamComp.SetTargetPosition(target);
            CamComp.transform.Rotation = rotation;

        }

        void OnFixedUpdate()
        {

        }


        
    }
}
