using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WaterShotter : MonoBehaviour
{
    public float Intensity;
    public string Mode;
    public GameObject waterBallPrefab;
    float r;
    public int waterDir;
    Vector3 delta;

    // Start is called before the first frame update
    void Start()
    {
        Intensity = 20;
        Mode = "ジェット";
    }

    // Update is called once per frame
    void Update()
    {
        //水の強弱設定
        if(0 < Intensity){
            if(Input.GetKey(KeyCode.L)){  
                Intensity -= 0.1f;
            }
            if(Input.GetKey(KeyCode.O)){
                Intensity += 0.1f;
            }
        }else if(20 < Intensity){
            if(Input.GetKey(KeyCode.L)){
                Intensity -= 0.1f;
            }
        }else{
            if(Input.GetKey(KeyCode.O)){
                Intensity += 0.1f;
            }
        }

        //発射モード設定
        if(Input.GetKeyDown(KeyCode.P)){
            if(Mode == "ジェット"){
                Mode = "ワイド";
            }else{
                Mode = "ジェット";
            }
        }

        //水を発射する
        if(Input.GetKey(KeyCode.Space) && 5 < Intensity){
            if(Mode == "ジェット"){
                JetWater();
                JetWater();
                JetWater();
                JetWater();
                JetWater();
            }else{
                for(int i=0; i<Intensity-4; ++i){
                    WideWater();
                }
            }
            
        }

        if(Input.GetKeyDown(KeyCode.Space) && 5 < Intensity){
            GameObject.Find("ShowerSE").GetComponent<SEController>().genSE();
        }
        if(Input.GetKeyUp(KeyCode.Space) && 5 < Intensity){
            GameObject.Find("ShowerSE").GetComponent<SEController>().stopSE();
        }
        
    }

    void JetWater(){
        waterDir = 0;
        r = (Random.value - 0.5f) * 0.2f;
        delta = new Vector3(0, r, r);
        Instantiate(waterBallPrefab, transform.position + delta, Quaternion.identity);
    }

    void WideWater(){
        r = (Random.value - 0.5f) * 0.1f;
        delta = new Vector3(0, r, r);
        Instantiate(waterBallPrefab, transform.position + delta, Quaternion.identity);

    }
}
