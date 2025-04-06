using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public class VirusScript : MonoBase
    {
     
        private Transform VirusTransform;
        private RigidBody VirusRigid;
        private Collider VirusCollider;
        private Vector3 Vrotate = new Vector3(0, 2, 0);
        float delta;
        public int order = 1;
        public bool following = false;
        private AudioComponent AudioComp;

        private StaticMeshComponent mesh;

        GameObject player;
        GameObject data_Red;
        GameObject data_Yellow;
        GameObject data_Blue;

        public Color currentcolor;
        
        Vector3 targetPos;
        List<posRec> playerPosList;

        public bool firsttick = true;

        void OnStart()
        {
            player = gameObject.FindWithTag("Surge");

            VirusTransform = gameObject.GetComponent<Transform>();
            VirusRigid = gameObject.GetComponent<RigidBody>();
            VirusCollider = gameObject.GetComponent<Collider>();
            targetPos = transform.Translation;
            AudioComp = gameObject.GetComponent<AudioComponent>();
            mesh = gameObject.GetComponent<StaticMeshComponent>();

            data_Red = gameObject.FindWithTag("Data_Red");
            data_Yellow = gameObject.FindWithTag("Data_Yellow");
            data_Blue = gameObject.FindWithTag("Data_Blue");

            string test = gameObject.name;

            if (test == "Data_Red")
            {
                currentcolor = Color.Red;
                //Log.LogInfo("Red");
            }

            if (test == "Data_Yellow")
            {
                currentcolor = Color.Yellow;
                //Log.LogInfo("Yellow");
            }

            if (test == "Data_Blue")
            {
                currentcolor = Color.Blue;
                //Log.LogInfo("Blue");
            }

            PlayerMovement pm = player.GetComponent<PlayerMovement>();
            playerPosList = new List<posRec>();
        }

        void OnUpdate()
        {
            if (targetPos != null)
            {
                //VirusTransform.Translation = Vector3.MoveTowards(VirusTransform.Translation, targetPos, followSpeed * delta);

                //VirusRigid.SetAngularVelocity(Vrotate);
                //VirusRigid.SetRotate(Vrotate);

                //targetPos = player.GetComponent<Transform>().Translation;
                float x = Lerp.LerpFloat(VirusTransform.Translation.X, targetPos.X, delta);
                float y = Lerp.LerpFloat(VirusTransform.Translation.Y, targetPos.Y, delta);
                float z = Lerp.LerpFloat(VirusTransform.Translation.Z, targetPos.Z, delta);

                VirusTransform.Translation = new Vector3(x, y, z);
            }

            if (following)
            {
                delta = Physics.GetFixedTimestep()*0.2f;

                int inx = VirusReorderScript.globalController.GetVirusIndex(this);
                order = inx + 1;

                int offset = order * 10;

                //PlayerMovement pm = player.GetComponent<PlayerMovement>();
                var pm = PlayerMovement.globalController.posList;
                var pm2 = PlayerMovement.globalController.rotList;

                var ix = offset - 1;

                if (ix < 0)
                    ix = 0;

                if (ix > pm.Count)
                    ix = pm.Count - 1;

                VirusTransform.Translation = pm[ix];
                VirusTransform.Rotation = pm2[ix];

                //Log.LogInfo("Virus Order:"+order.ToString() + " Pos:"+ ix + " offset:"+ offset+" total:" +pm.Count.ToString());

                //Log.LogInfo(pm.Count.ToString());

            }

            //if (!following) return;

            //if (playerPosList.Count == 0 || Vector3.Distance(playerPosList[playerPosList.Count - 1].position, player.transform.position) > followOffset)
            //{
            //    playerPosList.Add(new posRec(player.transform.position, player.transform.Rotation));

            //    while (playerPosList.Count > offsetTicks * order)
            //    {
            //        targetPos = playerPosList[0].position;
            //        transform.Rotation = playerPosList[0].rotation;
            //        playerPosList.RemoveAt(0);
            //    }
            //}




            if (firsttick == true)
            {
                firsttick = false;
            }
        }

        void OnFixedUpdate()
        {

            
        }

        void OnTriggerBegin(ulong entity)
        {
            if (!following && player.name == "Surge")
            {
                //Log.LogInfo("Entered");
                Obtain();
                AudioComp.PlaySFX("Got_Data.ogg");
            }
        }

        void OnTriggerStay(ulong entity)
        {

        }

        void OnTriggerEnd(ulong entity)
        {
            
        }
        public void InputIntoMechanic()
        {
            following = false;
            VirusReorderScript.globalController.RemoveVirus(this);
            gameObject.SetActive(false);
        }

        public void Corrupt()
        {
            if (currentcolor == Color.Red)
            {
                mesh.LoadModel("corrupted_data_red");
                currentcolor = Color.Corrupted_Red;
            }
            if (currentcolor == Color.Blue)
            {
                mesh.LoadModel("corrupted_data_blue");
                currentcolor = Color.Corrupted_Blue;
            }
            if (currentcolor == Color.Yellow)
            {
                mesh.LoadModel("corrupted_data_yellow");
                currentcolor = Color.Corrupted_Yellow;
            }
        }

        public void UnCorrupted()
        {
            if (currentcolor == Color.Corrupted_Red)
            {
                mesh.LoadModel("data_red");
                currentcolor = Color.Red;
            }
            if (currentcolor == Color.Corrupted_Blue)
            {
                mesh.LoadModel("data_blue");
                currentcolor = Color.Blue;
            }
            if (currentcolor == Color.Corrupted_Yellow)
            {
                mesh.LoadModel("data_yellow");
                currentcolor = Color.Yellow;
            }
        }

        public void Obtain()
        {
            if(firsttick == false)
            {
                gameObject.SetActive(true);
                //Log.LogInfo(this.currentcolor.ToString());
                VirusReorderScript.globalController.AddVirus(this);
                following = true;
            }
        }

        struct posRec
        {
            public Vector3 position;
            public Quaternion rotation;

            public posRec(Vector3 position, Quaternion rotation)
            {
                this.position = position;
                this.rotation = rotation;
            }
        }

    }
}
