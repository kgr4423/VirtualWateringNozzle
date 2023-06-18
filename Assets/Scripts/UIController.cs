using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UIController : MonoBehaviour
{
    GameObject AngleUDText;
    GameObject AngleLRText;
    GameObject IntensityText;
    GameObject ModeText;
    GameObject commandText;
    GameObject sens00Text;
    GameObject sens01Text;
    GameObject sens02Text;
    GameObject sens03Text;
    private HandleController handleController;
    private WaterShotter waterShotter;
    private Client client;
    private TestScript testScript;

    void Start()
    {
        AngleUDText = GameObject.Find("AngleUD");
        AngleLRText = GameObject.Find("AngleLR");
        IntensityText = GameObject.Find("Intensity");
        ModeText = GameObject.Find("Mode");
        commandText = GameObject.Find("command");
        sens00Text = GameObject.Find("sens00");
        sens01Text = GameObject.Find("sens01");
        sens02Text = GameObject.Find("sens02");
        sens03Text = GameObject.Find("sens03");
    }

    void Update()
    {
        handleController = GameObject.Find("Handle").GetComponent<HandleController>();
        waterShotter = GameObject.Find("ShotPoint").GetComponent<WaterShotter>();
        client = GameObject.Find("Client").GetComponent<Client>();
        testScript = GameObject.Find("Test").GetComponent<TestScript>();
        AngleUDText.GetComponent<Text>().text = "上下角度： " + handleController.HandleAngleZ.ToString("D2");
        AngleLRText.GetComponent<Text>().text = "左右角度： " + handleController.HandleAngleY.ToString("D2");
        IntensityText.GetComponent<Text>().text = "水量　　： " + waterShotter.Intensity.ToString("N0");
        ModeText.GetComponent<Text>().text = "モード　： " + waterShotter.Mode;
        commandText.GetComponent<Text>().text = "指令値　： " + testScript.Torque.ToString("D3");
        sens00Text.GetComponent<Text>().text = "センサ０： " + client.num[0].ToString("D3");
        sens01Text.GetComponent<Text>().text = "センサ１： " + client.num[1].ToString("D3");
        sens02Text.GetComponent<Text>().text = "センサ２： " + client.num[2].ToString("D3");
        sens03Text.GetComponent<Text>().text = "センサ３： " + client.num[3].ToString("D3");
    }
}
