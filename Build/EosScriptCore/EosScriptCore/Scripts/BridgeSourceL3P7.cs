using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
	public class BridgeSourceL3P7 : MonoBase
	{
		float blockWidth = 1.6f;
		public static BridgeSourceL3P7 globalController;
		public GameObject prefab_redTile;
		public GameObject prefab_blueTile;
		public GameObject prefab_yellowTile;

		public Color red_color;
		public Color blue_color;
		public Color yellow_color;

		public GameObject bridgeRoot;
		GameObject[] blocks = new GameObject[32];
		int blockCount = 0;
		
		void OnStart()
		{
			globalController = this;
			bridgeRoot = gameObject.FindWithTag("bridge source 7");
			red_color = Color.Red;
			blue_color = Color.Blue;
			yellow_color = Color.Yellow;
		}
		
		void OnUpdate()
		{
			
		}
		
		void OnFixedUpdate()
		{
			
		}
		
		public void ClearBridge()
		{
			if (blockCount != 0)
			{
				for (int i = 0; i < blockCount; ++i)
				{
					Destroy(blocks[i]); //does this exist?
				}
			}
			blockCount = 0;
        }
		
		public void BuildBridge(List<Color> colorList)
		{
			ClearBridge();
			Vector3 currentPos = bridgeRoot.transform.Translation; //need this pointer?
			currentPos += transform.forward * blockWidth;

			for (int i = 0; i < colorList.Count; ++i)
            {
				if (colorList[i] == red_color)
                {
					for (int j = 0; j < 3; ++j)
                    {
						blockCount++;
						blocks[i * 3 + j] = Instantiate("red_tile", currentPos, Quaternion.identity);
						currentPos += transform.forward * blockWidth;
					}
                }

				if (colorList[i] == blue_color)
				{
					for (int j = 0; j < 3; ++j)
					{
						blockCount++;
						blocks[i * 3 + j] = Instantiate("blue_tile", currentPos, Quaternion.identity);
						
						if (j < 2)
							currentPos += -transform.right * blockWidth;
					}
					currentPos += transform.forward * blockWidth;
				}

				if (colorList[i] == yellow_color)
				{
					for (int j = 0; j < 3; ++j)
					{
						blockCount++;
						blocks[i * 3 + j] = Instantiate("yellow_tile", currentPos, Quaternion.identity);

						if (j < 2)
							currentPos += transform.right * blockWidth;
					}
					currentPos += transform.forward * blockWidth;
				}
			}
		}
		
		void OnTriggerBegin(ulong data)
        {

        }

        void OnTriggerStay(ulong data)
        {

        }

        void OnTriggerEnd(ulong data)
        {

        }
	}
}