using UnityEngine;
using System.IO.Ports;
using System;

public class SerialController : MonoBehaviour
{
    public static String ComPort = "COM5";
    SerialPort serialPort = new SerialPort(ComPort, 115200); 

    void Start()
    {
        try
        {
            serialPort.Open();
            serialPort.ReadTimeout = 100;
            Debug.Log("Serial port opened successfully.");
        }
        catch (Exception e)
        {
            Debug.LogError("Failed to open serial port: " + e.Message);
        }
    }

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.J)) {
            serialPort.Write("1");
            Debug.Log("Sent: 1");
        }
        else if (Input.GetKeyDown(KeyCode.K)) {
            serialPort.Write("2");
            Debug.Log("Sent: 2");
        }
        else if (Input.GetKeyDown(KeyCode.L)) {
            serialPort.Write("3");
            Debug.Log("Sent: 3");
        }
        else if (Input.GetKeyDown(KeyCode.Semicolon)){
            serialPort.Write("4");
            Debug.Log("Sent: 4");
        }
        else if (Input.GetKeyDown(KeyCode.Space))
        {
            serialPort.Write("5");
            Debug.Log("Sent: 5");
        }
        else if (Input.GetKeyDown(KeyCode.Alpha6)) { 
            serialPort.Write("6");
            Debug.Log("Sent: 6");
        }
        else if (Input.GetKeyDown(KeyCode.Alpha0))
        {
            serialPort.Write("0");
            Debug.Log("Sent: 0");
        }

    }

    void OnApplicationQuit()
    {
        if (serialPort != null && serialPort.IsOpen)
        {
            serialPort.Close();
        }
    }
}
