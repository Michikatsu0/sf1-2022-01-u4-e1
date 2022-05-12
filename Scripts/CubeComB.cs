using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CubeComB : MonoBehaviour
{
    [SerializeField] SerialControllerCustomDelimiter serialControllerCustomDelimiter;
    Renderer render;
    Vector3 vecScale = new Vector3(0, 0, 0);
    float[] scale;

    void Start()
    {
        render = GetComponent<Renderer>();
        render.material.color = Color.yellow;
    }

    void Update()
    {
        byte[] message = serialControllerCustomDelimiter.ReadSerialMessage();
        if (message == null)
            return;
        else
        {
            Debug.Log(message.Length);
            if (message.Length == 4)
            {
                //Size
                scale = new float[3];
                for (uint i = 0; i < 3; i++)
                {
                    scale[i] = (byte)message[i + 1];
                }
                vecScale.x = scale[0];
                vecScale.y = scale[1];
                vecScale.z = scale[2];
                transform.localScale = vecScale;
            }
            else 
            {
                //Color
                if (message[0] == 0)
                {
                    render.material.color = Color.green;
                }
                else
                {
                    render.material.color = Color.yellow;
                }
            }
        }
    }
    
    public void ButtonB()
    {
        serialControllerCustomDelimiter.SendSerialMessage(new byte[] { 0x3e });
    }
}
