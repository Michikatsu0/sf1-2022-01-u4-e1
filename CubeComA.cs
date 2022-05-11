﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CubeComA : MonoBehaviour
{
    [SerializeField] SerialController serialController;
    Renderer render;
    Vector3 vecScale = new Vector3(0, 0, 0);
    // Start is called before the first frame update
    void Start()
    {
        render = GetComponent<Renderer>();
        render.material.color = Color.blue;
    }

    // Update is called once per frame
    void Update()
    {
        string[] microRqst;
        string message = serialController.ReadSerialMessage();
        if (message == null) return;
        else
        {
            microRqst = message.Split(',');
            if(microRqst.Length == 2)
            {
                //Size
                float scale = float.Parse(microRqst[1]);
                vecScale.x = scale;
                vecScale.y = scale;
                vecScale.z = scale;
                transform.localScale = vecScale;
            }
            else
            {
                //Color
                int evColor = int.Parse(microRqst[0]);
                if (evColor == 0) 
                {
                    render.material.color = Color.red;
                }
                else
                {
                    render.material.color = Color.blue;
                }

            }
        }
        Debug.Log(message);
    }

    public void ButtonA()
    {
        serialController.SendSerialMessage("S");
        Debug.Log("Enviado");
    }
}
