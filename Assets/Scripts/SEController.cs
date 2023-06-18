using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SEController : MonoBehaviour
{
    public void genSE()
    {
        // サウンドを鳴らす
        GetComponent<AudioSource>().Play();
    }

    public void stopSE()
    {
        // サウンドを止める
        GetComponent<AudioSource>().Stop();
    }
}
