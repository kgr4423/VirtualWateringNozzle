using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HandleController : MonoBehaviour
{
    public int HandleAngleY;
    public int HandleAngleZ;
    
    private NozzleController nozzle;
    

    void Start()
    {
        HandleAngleY = 0;
        HandleAngleZ = 20;
        
    }

    void Update()
    {
        nozzle = GameObject.Find("Nozzle").GetComponent<NozzleController>();
        HandleAngleY = nozzle.NozzleAngleY;

        //上下の移動を制御
        if(-10 < HandleAngleZ && HandleAngleZ < 40){
            if(Input.GetKey(KeyCode.S)){
                HandleAngleZ -= 1;          
            }
            if(Input.GetKey(KeyCode.W)){
                HandleAngleZ += 1;
            }
        }else if(HandleAngleZ <= -5){
            if(Input.GetKey(KeyCode.W)){
                HandleAngleZ += 1;
            }
        }else{
            if(Input.GetKey(KeyCode.S)){
                HandleAngleZ -= 1;          
            }
        }
        transform.rotation = Quaternion.Euler(0, HandleAngleY, HandleAngleZ);
        
    }

    
}
