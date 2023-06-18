using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NozzleController : MonoBehaviour
{
    public int NozzleAngleY;

    void Start()
    {
        NozzleAngleY = 0;
    }

    void Update()
    {
        //左右の移動を制御
		if (-40 < NozzleAngleY && NozzleAngleY < 40){
			// (1)画面端じゃないとき
            if(Input.GetKey(KeyCode.A)){
                NozzleAngleY -= 1;          
            }
            if(Input.GetKey(KeyCode.D)){
                NozzleAngleY += 1;
            } 

		}else if(NozzleAngleY <= -30){
			//(2)画面左端にいるとき
            if(Input.GetKey(KeyCode.D)){
                NozzleAngleY += 1;
            }

		}else{
			//(3)画面右端にいるとき
			if(Input.GetKey(KeyCode.A)){
                NozzleAngleY -= 1;          
            }
		}
        transform.rotation = Quaternion.Euler(0, NozzleAngleY, 0);
    }
}
