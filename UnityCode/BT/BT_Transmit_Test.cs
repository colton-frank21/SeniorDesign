using System;             //DateTime library
using System.Collections.Generic;
using System.Diagnostics; //Stopwatch library
using System.IO;          //Writing to text file
using System.IO.Ports;    //Serial Port library
using System.Text;
using UnityEngine;
using UnityEngine.Rendering.LookDev;

public class BT_Transmit_Test : MonoBehaviour
{
    SerialPort serialPort = new SerialPort("COM5", 115200); //Serial port configuration (ESP32 Port and baud rate)
    private const int packetSize = 10; // Size of the packet to send
    int MessagesSent = 0; // Counter for the number of tests conducted
    int maxTests = 9999; // Maximum number of tests to conduct


    //initialization
    void Start()
    {
        try //Try to open the serial port
        {
            serialPort.Open();
            serialPort.ReadTimeout = 100;
            UnityEngine.Debug.Log("Serial port opened successfully.");
        }
        catch (Exception e) //Could not open the port
        {
            UnityEngine.Debug.LogError("Failed to open serial port: " + e.Message);
        }

    }

    //Called once per frame (Kind of like loop())
    void Update()
    {
        if (serialPort.IsOpen)
        {
            if (MessagesSent < maxTests)
            {
                SendPacket(packetSize); //Send the packet and start the timer
            }
        }
    }


    // Send a packet to the ESP32 and start the timer
    private void SendPacket(int size)
    {
        MessagesSent++;
        Stopwatch latency = new Stopwatch();
        latency.Start(); // Start the timer
        string packetData = new string('A', size); // Create packet data of specified size minus 1 for "ACK" and number
        string message = packetData + "\n"; // Append packet number and newline character
        serialPort.Write(message); // Send the packet over serial
        
    }

    // Clean up when the application quits
    void OnApplicationQuit()
    {
        if (serialPort != null && serialPort.IsOpen)
        {
            serialPort.Close();
        }
    }
}
