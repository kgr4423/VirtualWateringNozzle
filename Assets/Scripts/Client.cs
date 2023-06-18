using System;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using System.Net.Sockets;
using UnityEngine;

public class Client : MonoBehaviour
{
    public static string data;
    public static string resMsg;

    public static int SenserCount = 0;
    public static int[] SenserVal = new int[1024];

    public static int SenserNum = 4;
    public int[] num = new int[SenserNum];

    System.Net.Sockets.NetworkStream ns;
    System.Net.Sockets.TcpClient tcp;

    public TestScript testScript;

    void Start()
    {
        //サーバに送信するデータa
        data = "1234567";

        //サーバーのIPアドレスとポート番号
        string ipOrHost = "localhost";
        int port = 12345;

        //TcpClientを作成し、サーバーと接続
        tcp = new System.Net.Sockets.TcpClient(ipOrHost, port);
        Debug.Log("サーバー({0}:{1})と接続しました。" +
            ((System.Net.IPEndPoint)tcp.Client.RemoteEndPoint).Address + "," +
            ((System.Net.IPEndPoint)tcp.Client.RemoteEndPoint).Port + "," +
            ((System.Net.IPEndPoint)tcp.Client.LocalEndPoint).Address + "," +
            ((System.Net.IPEndPoint)tcp.Client.LocalEndPoint).Port);

        //NetworkStreamを取得
        ns = tcp.GetStream();
    }

    void Update()
    {
        //タイムアウト設定
        ns.ReadTimeout = 10000;
        ns.WriteTimeout = 10000;

        //サーバに送信するデータをここで設定
        data = testScript.Torque.ToString();
        //文字列をByte型配列に変換して、サーバにデータを送信
        System.Text.Encoding enc = System.Text.Encoding.UTF8;
        byte[] sendBytes = enc.GetBytes(data + '\n');
        ns.Write(sendBytes, 0, sendBytes.Length);

        //サーバからデータ受信の準備
        System.IO.MemoryStream ms = new System.IO.MemoryStream();
        byte[] resBytes = new byte[256];
        int resSize = 256;
        //データを受信
        resSize = ns.Read(resBytes, 0, resBytes.Length);
        //受信したデータを蓄積
        ms.Write(resBytes, 0, resSize);
        //受信したデータを文字列に変換
        resMsg = enc.GetString(ms.GetBuffer(), 0, (int)ms.Length);
        ms.Close();
        //末尾の\nを削除
        resMsg = resMsg.TrimEnd('\n');

        //データをセンサの数だけ分割
        string[] str = resMsg.Split(' ');
        //分割したデータをそれぞれint型に変換
        for (int i = 0; i < str.Length; i++)
        {
            num[i] = int.Parse(str[i]);
        }

        //スペース押すと閉じる
        if (Input.GetKey(KeyCode.Space))
        {
            ns.Close();
            tcp.Close();
            Debug.Log("切断しました。");
        }
    }
}