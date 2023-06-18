using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WaterBoalController : MonoBehaviour
{
    private GameObject shotPoint;
    Vector3 delta;
    float x1;
    float x2;
    float alpha;
    MeshRenderer mesh;
    Rigidbody rb;

    void Awake()
    {
        shotPoint = GameObject.Find("ShotPoint");
        WaterShotter waterShotter = shotPoint.GetComponent<WaterShotter>();
        rb = GetComponent<Rigidbody>();
        rb.mass = Random.value + 0.7f;

        x1 = 0.25f;
        x2 = 0.3f;

        switch(waterShotter.waterDir){
            case 0:
                delta = new Vector3(0, 0, 0);
                break;
            case 1:
                delta = new Vector3(0, x1, x1);
                break;
            case 2:
                delta = new Vector3(0, x1, -x1);
                break;
            case 3:
                delta = new Vector3(0, -x1, x1);
                break;
            case 4:
                delta = new Vector3(0, -x1, -x1);
                break;
            case 5:
                delta = new Vector3(0, x2, 0);
                break;
            case 6:
                delta = new Vector3(0, -x2, 0);
                break;
            case 7:
                delta = new Vector3(0, 0, x2);
                break;
            case 8:
                delta = new Vector3(0, 0, -x2);
                break;
            case 9:
                delta = new Vector3(0, x1*0.5f, x1*0.5f);
                break;
            case 10:
                delta = new Vector3(0, x1*0.5f, -x1*0.5f);
                break;
            case 11:
                delta = new Vector3(0, -x1*0.5f, x1*0.5f);
                break;
            case 12:
                delta = new Vector3(0, -x1*0.5f, -x1*0.5f);
                break;
            case 13:
                delta = new Vector3(0, x2*0.5f, 0);
                break;
            case 14:
                delta = new Vector3(0, -x2*0.5f, 0);
                break;
            case 15:
                delta = new Vector3(0, 0, x2*0.5f);
                break;
            case 16:
                delta = new Vector3(0, 0, -x2*0.5f);
                waterShotter.waterDir = -1;
                break;
        }

        waterShotter.waterDir += 1;


        GetComponent<Rigidbody>().AddForce((shotPoint.transform.right + delta) * waterShotter.Intensity, ForceMode.Impulse);
    }

    void Start()
    {
        mesh = GetComponent<MeshRenderer>();
    }

    void Update()
    {
        alpha = 255-transform.position.x*8;
        if(alpha >= 0){
            mesh.material.color = new Color32(72, 133, 197, (byte)alpha);
        }else{
            mesh.material.color = new Color32(72, 133, 197, 0);
        }

        if(90 < transform.position.x || transform.position.y < -0.1 || transform.position.z < -50 || 50 < transform.position.z){
            Destroy(gameObject);
        }
        
    }

    void OnTriggerEnter(Collider coll)
    {
        if(coll.gameObject == GameObject.Find("Ground")){
            Destroy(gameObject);
        }
    }
}
