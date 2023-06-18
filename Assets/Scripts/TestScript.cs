using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TestScript : MonoBehaviour
{
    public int Torque;
    public int Angle01;
    public int Angle02;
    public int Angle03;
    public Client client;

    // Start is called before the first frame update
    void Start()
    {
        Torque = 0;
        Angle01 = 0;
        Angle02 = 0;
        Angle03 = 0;
    }

    // Update is called once per frame
    void Update()
    {
        if(Input.GetKey(KeyCode.Alpha1)){
            Torque -= 5;          
        }
        if(Input.GetKey(KeyCode.Alpha2)){
            Torque = 0;
        }
        if(Input.GetKey(KeyCode.Alpha3)){
            Torque += 5;
        }

        Angle01 = client.num[0];
        Angle02 = client.num[1];
        Angle03 = client.num[2];
        Debug.Log("Angle01 : " + Angle01);
        Debug.Log("Angle02 : " + Angle02);
        Debug.Log("Angle03 : " + Angle03);
    }
}
