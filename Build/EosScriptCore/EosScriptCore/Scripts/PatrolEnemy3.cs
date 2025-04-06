using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public class PatrolEnemy3 : MonoBase
    {
        public List<Transform> points = new List<Transform>();
        public StateMachine sm = new StateMachine();

        int current;
        public float speed = 10f;

        private bool displayCorrupted3 = false;
        private GameObject CorruptedImage3;

        //public GameObject virusTrigger;

        public GameObject point1;
        public GameObject point2;
        public GameObject point3;
        public GameObject point4;

        private GameObject Player;
        private GameObject big_Collider2;

        public bool alert = false;
        float timer = 0f;
        bool timerStart = false;

        //public Stance behaviour = Stance.Patrol;
        float delta;

        //public enum Stance
        //{
        //    Patrol = 0,
        //    Chasing = 1,
        //    GettingBack = 2,
        //}

        public GameObject corrupted_data_red;
        public GameObject corrupted_data_blue;

        public GameObject normal_data_red;
        public GameObject normal_data_blue;


        // Start is called before the first frame update
        void OnStart()
        {
            current = 0;
            //behaviour = Stance.Patrol;

            //virusTrigger = gameObject.FindWithTag("Virus01_Trigger");
            Player = gameObject.FindWithTag("Surge");
            big_Collider2 = gameObject.FindWithTag("Big_Collider2");
            CorruptedImage3 = gameObject.FindWithTag("CorruptedImage3");
            CorruptedImage3.SetActive(false);

            point1 = gameObject.FindWithTag("Point5");
            point2 = gameObject.FindWithTag("Point6");

            normal_data_red = gameObject.FindWithTag("Data_Red");
            normal_data_blue = gameObject.FindWithTag("Data_Blue");

            points.Add(point1.transform);
            points.Add(point2.transform);
        }

        // Update is called once per frame
        void OnUpdate()
        {
            if (sm.CurrentState == ProcessState.Patrol)
            {
                if (gameObject.transform.Translation != points[current].Translation)
                {
                    gameObject.transform.Translation = Vector3.MoveTowards(transform.Translation, points[current].Translation, speed * delta);
                }

                else
                {
                    current = (current + 1) % points.Count;
                }
                //Log.LogInfo("Enemey 3 patrolling...");
            }

            if (sm.CurrentState == ProcessState.Chasing)
            {
                gameObject.transform.Translation = Vector3.MoveTowards(transform.Translation, Player.transform.Translation, (speed * 2) * delta);
                //Log.LogInfo("Enemy 3 chasing you!");
            }

            if (alert == true)
            {
                sm.MoveNext(Command.Begin);
            }

            if (Player.GetComponent<Collider>().isTrigger && big_Collider2.GetComponent<Collider>().isTrigger)
            {
                gameObject.transform.Translation = Vector3.MoveTowards(transform.Translation, Player.transform.Translation, (speed * 2) * delta);
                //Log.LogInfo("Enemy 3 chasing you!");
            }

            if (displayCorrupted3 == true)
            {
                CorruptedImage3.SetActive(true);
                Log.LogInfo("Corrupted Image3 is out!");
            }
            if (displayCorrupted3 == false)
            {
                CorruptedImage3.SetActive(false);
                Log.LogInfo("Corrupted Image3 is not out!");
            }
            if (timer >= 3)
            {
                timerStart = false;
                timer = 0;
                displayCorrupted3 = false;
            }
        }

        void OnFixedUpdate()
        {
            delta = Physics.GetFixedTimestep() * 0.2f;
        }

        // Self collision
        private void OnTriggerBegin(ulong data)
        {
            if (Player.GetComponent<Collider>().isTrigger && gameObject.GetComponent<Collider>().isTrigger)
            {
                displayCorrupted3 = true;
               //timerStart = true;

                for (int i = 0; i < VirusReorderScript.globalController.playerViruses.Count; ++i)
                {
                    if (VirusReorderScript.globalController.playerViruses[i].currentcolor == Color.Blue)
                    {
                        // Swap position with available Corrupted_Blue data
                        VirusReorderScript.globalController.playerViruses[i].Corrupt();
                        Log.LogInfo("blue data/blue corrupted data");
                        // Destroy object or just replace position not too sure yet?
                        //Destroy(normal_data_blue1);
                        //Destroy(normal_data_blue2);
                        //Destroy(VirusReorderScript.globalController.playerViruses[i]);
                    }

                    if (VirusReorderScript.globalController.playerViruses[i].currentcolor == Color.Red)
                    {
                        // Swap position with available Corrupted_Red data
                        VirusReorderScript.globalController.playerViruses[i].Corrupt();
                        Log.LogInfo("red data/red corrupted data");
                        // Destroy object or just replace position not too sure yet?
                        //Destroy(normal_data_red1);
                        //Destroy(normal_data_red2);
                    }
                    if (VirusReorderScript.globalController.playerViruses[i].currentcolor == Color.Yellow)
                    {
                        VirusReorderScript.globalController.playerViruses[i].Corrupt();
                        Log.LogInfo("yellow data/yellow corrupted data");
                        // Swap position with available Corrupted_Yellow data
                        // Destroy object or just replace position not too sure yet?
                        //Destroy(normal_data_yellow1);
                        //Destroy(VirusReorderScript.globalController.playerViruses[i]);
                    }


                }
            }
        }
        private void OnTriggerEnd(ulong data)
        {
            //if (Player.GetComponent<Collider>().isTrigger && virusTrigger.GetComponent<Collider>().isTrigger)
            //{
            //    alert = false;
            //    behaviour = Stance.Patrol;
            //}
            if (Player.name == "Surge")
            {
                displayCorrupted3 = false;
            }
        }
    }
}

