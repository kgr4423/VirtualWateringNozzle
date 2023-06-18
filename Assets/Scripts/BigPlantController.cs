using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BigPlantController : MonoBehaviour
{
    private GameObject flower;
    private GameObject stem;
    bool SEisColled;
    
    void Start()
    {
        flower = transform.Find("Flower").gameObject;
        stem = transform.Find("Stem").gameObject;
        SEisColled = false;
    }

    void Update()
    {
        Vector3 stemPos = stem.gameObject.transform.position;
        if(stemPos.y >= 3.0f){
            if(!SEisColled){
                GameObject.Find("BloomSE").GetComponent<SEController>().genSE();
                SEisColled = true;
            }
            flower.SetActive(true);
        }
    }

    void OnTriggerEnter(Collider coll)
    {
        if(coll.gameObject != GameObject.Find("Ground")){
            Vector3 stemPos = stem.gameObject.transform.position;
            if(stemPos.y < 3.0f){
                stem.gameObject.transform.position = new Vector3(stemPos.x, stemPos.y+0.1f, stemPos.z);
            }
            Destroy(coll.gameObject);
        }
    }
}
