using System;             //DateTime library
using System.Collections.Generic;
using System.Diagnostics; //Stopwatch library
using System.IO;          //Writing to text file
using System.IO.Ports;    //Serial Port library
using System.Text;
using UnityEngine;
using UnityEngine.Rendering.LookDev;

public class Single_Value_Latency_Test : MonoBehaviour
{
    SerialPort serialPort = new SerialPort("COM4", 115200); //Serial port configuration (ESP32 Port and baud rate)
    private const int packetSize = 10; // Size of the packet to send
    int MessagesSent = 0; // Counter for the number of tests conducted
    int maxTests = 9999; // Maximum number of tests to conduct
    private Dictionary<int, Stopwatch> packetTimers = new Dictionary<int, Stopwatch>(); // Track stopwatches per packet


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
            try
            {
                while(serialPort.BytesToRead > 0)
                {
                    string AckData = serialPort.ReadLine().Trim(); //Read the incoming data until newline character
                    if(!string.IsNullOrEmpty(AckData) && AckData.StartsWith("ACK"))
                    {
                        int ackNumber;
                        if(int.TryParse(AckData.Substring(3), out ackNumber))
                        {
                            if(packetTimers.ContainsKey(ackNumber))
                            {
                                Stopwatch latency = packetTimers[ackNumber];
                                latency.Stop(); //Stop the timer
                                double roundTripTime = latency.Elapsed.TotalMilliseconds; //Get elapsed time in milliseconds
                                WriteLatencyToFile(ackNumber, roundTripTime); //Write the latency to a file
                                packetTimers.Remove(ackNumber); //Remove the stopwatch from the dictionary
                            }
                        }
                    }
                }
            }
            catch (TimeoutException)
            {
                // No data received within the timeout period
            }
        }
    }


    // Send a packet to the ESP32 and start the timer
    private void SendPacket(int size)
    {
        MessagesSent++;
        Stopwatch latency = new Stopwatch();
        latency.Start(); // Start the timer
        packetTimers[MessagesSent] = latency; // Store the stopwatch for this packet
        if (MessagesSent < 9)
        {
            string packetData = new string('A', size - 1); // Create packet data of specified size minus 1 for "ACK" and number
            string message = packetData + MessagesSent.ToString() + "\n"; // Append packet number and newline character
            serialPort.Write(message); // Send the packet over serial
        }
        else if (MessagesSent < 99)
        {
            string packetData = new string('A', size - 2); // Create packet data of specified size minus 1 for "ACK" and number
            string message = packetData + MessagesSent.ToString() + "\n"; // Append packet number and newline character
            serialPort.Write(message); // Send the packet over serial
        }
        else if (MessagesSent < 999)
        {
            string packetData = new string('A', size - 3); // Create packet data of specified size minus 1 for "ACK" and number
            string message = packetData + MessagesSent.ToString() + "\n"; // Append packet number and newline character
            serialPort.Write(message); // Send the packet over serial
        }
        else
        {
            string packetData = new string('A', size - 4); // Create packet data of specified size minus 1 for "ACK" and number
            string message = packetData + MessagesSent.ToString() + "\n"; // Append packet number and newline character
            serialPort.Write(message); // Send the packet over serial
        }
    }

    // Write round-trip latency to a file
    private void WriteLatencyToFile(int ID, double latency)
    {
        StreamWriter writer = new StreamWriter(@"C:\Senior_Design\latency_results_Single_Value10.csv", true);
        writer.WriteLine($"{ID},{latency}");
        writer.Close();
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
