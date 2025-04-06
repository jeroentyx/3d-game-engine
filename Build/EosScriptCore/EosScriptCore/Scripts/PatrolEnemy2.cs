using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public class PatrolEnemy2 : MonoBase
    {
        public List<Transform> points = new List<Transform>();
        public StateMachine sm = new StateMachine();
        int current;
        public float speed = 10f;

        private bool displayCorrupted2 = false;
        private GameObject CorruptedImage2;

        //public GameObject virusTrigger;

        public GameObject point1;
        public GameObject point2;
        public GameObject point3;
        public GameObject point4;

        private GameObject Player;
        private GameObject big_Collider;

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
            big_Collider = gameObject.FindWithTag("Big_Collider");
            CorruptedImage2 = gameObject.FindWithTag("CorruptedImage2");
            CorruptedImage2.SetActive(false);


            point1 = gameObject.FindWithTag("Point1");
            point2 = gameObject.FindWithTag("Point2");
            point3 = gameObject.FindWithTag("Point3");
            point4 = gameObject.FindWithTag("Point4");

            normal_data_red = gameObject.FindWithTag("Data_Red");
            normal_data_blue = gameObject.FindWithTag("Data_Blue");

            points.Add(point3.transform);
            points.Add(point4.transform);
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
                //Log.LogInfo("Enemey 2 patrolling...");
            }

            if (sm.CurrentState == ProcessState.Chasing)
            {
                gameObject.transform.Translation = Vector3.MoveTowards(transform.Translation, Player.transform.Translation, (speed * 1.5f) * delta);
                //Log.LogInfo("Enemy 2 chasing you!");
            }

            if (alert == true)
            {
                sm.MoveNext(Command.Begin);
            }

            if (Player.GetComponent<Collider>().isTrigger && big_Collider.GetComponent<Collider>().isTrigger)
            {
                gameObject.transform.Translation = Vector3.MoveTowards(transform.Translation, Player.transform.Translation, (speed * 1.5f) * delta);
                //Log.LogInfo("Enemy 2 chasing you!");
            }

            if (displayCorrupted2 == true)
            {
                CorruptedImage2.SetActive(true);
                Log.LogInfo("Corrupted Image2 is out!");
            }
            if (displayCorrupted2 == false)
            {
                CorruptedImage2.SetActive(false);
                Log.LogInfo("Corrupted Image2 is not out!");
            }
            if (timer >= 3)
            {
                timerStart = false;
                timer = 0;
                displayCorrupted2 = false;
            }
        }

        void OnFixedUpdate()
        {
            delta = Physics.GetFixedTimestep() * 0.2f;

            if (timerStart = true)
            {
                timer += Physics.GetFixedTimestep();
            }
        }


        private void OnTriggerBegin(ulong data)
        {
            if (Player.GetComponent<Collider>().isTrigger && gameObject.GetComponent<Collider>().isTrigger)
            {
                displayCorrupted2 = true;
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
            if (Player.name == "Surge")
            {
                displayCorrupted2 = false;
            }
        }
    }
}
