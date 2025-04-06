using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public class PatrollingEnemy1OnTrigger : MonoBase
    {
        public List<Transform> points = new List<Transform>();
        public StateMachine sm = new StateMachine();
        private GameObject big_Collider;

        public GameObject virusTrigger;
        private RigidBody virusRigid;
        private GameObject Player;
        public bool alert = false;
        public static PatrollingEnemy1OnTrigger patrollingEnemy1OnTrigger;

        float delta;
        int current;

        public float speed = 50f;
        private Vector3 newPos;

        public GameObject point1;
        public GameObject point2;
        public GameObject point3;
        public GameObject point4;

        // Start is called before the first frame update
        void OnStart()
        {
            patrollingEnemy1OnTrigger = this;   
            virusTrigger = gameObject.FindWithTag("Virus01_Trigger");
            virusRigid = gameObject.GetComponent<RigidBody>();
            Player = gameObject.FindWithTag("Surge");

            point1 = gameObject.FindWithTag("Point1");
            point2 = gameObject.FindWithTag("Point2");
            point3 = gameObject.FindWithTag("Point3");
            point4 = gameObject.FindWithTag("Point4");

            points.Add(point1.transform);
            points.Add(point2.transform);
            points.Add(point3.transform);
            points.Add(point4.transform);
        }

        // Update is called once per frame
        void OnUpdate()
        {
            //if (sm.CurrentState == ProcessState.Patrol)
            //{
            //    Log.LogInfo("Enemy Petrolling!");
            //    if (gameObject.transform.Translation != points[current].Translation)
            //    {
            //        Log.LogInfo("Enemy Petrolling!");
            //        newPos = Vector3.MoveTowards(transform.Translation, points[current].Translation, speed * delta);
            //        virusRigid.AddForce((newPos - transform.Translation) * speed);
            //    }

            //    else
            //    {
            //        current = (current + 1) % points.Count;
            //    }
            //}
            //if (sm.CurrentState == ProcessState.Chasing)
            //{
            //    gameObject.transform.Translation = Vector3.MoveTowards(transform.Translation, Player.transform.Translation, speed * delta);
            //    //virusRigid.AddForce((newPos - transform.Translation) * speed);
            //}


            //if (Player.GetComponent<Collider>().isTrigger && big_Collider.GetComponent<Collider>().isTrigger)
            //{
            //    gameObject.transform.Translation = Vector3.MoveTowards(transform.Translation, Player.transform.Translation, (speed * 2) * delta);
            //}


        }
        void OnFixedUpdate()
        {
            delta = Physics.GetFixedTimestep() * 0.2f;
        }

        private void OnTriggerBegin(ulong data)
        {
            if (Player.GetComponent<Collider>().isTrigger && virusTrigger.GetComponent<Collider>().isTrigger)
            {
                Log.LogInfo("Player collider colliding with virus trigger!");
                alert = true;
            }
        }

        private void OnTriggerEnd(ulong data)
        {
            if (!Player.GetComponent<Collider>().isTrigger && virusTrigger.GetComponent<Collider>().isTrigger)
            {
                Log.LogInfo("Player collider not colliding with virus trigger!");
                Player = null;
                alert = false;
                //sm.GetNext(Command.End);
                //behaviour = Stance.Patrol;
            }
        }

    }
}
