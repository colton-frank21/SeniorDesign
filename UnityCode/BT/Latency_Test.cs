using System;             //DateTime library
using System.Diagnostics; //Stopwatch library
using System.IO;          //Writing to text file
using System.IO.Ports;    //Serial Port library
using System.Text;    
using UnityEngine;

public class Latency_Test : MonoBehaviour
{
    SerialPort serialPort = new SerialPort("COM4", 115200); //Serial port configuration (ESP32 Port and baud rate)
    private int packetSize = 5; // Size of the packet to send
    private int maxPacketSize = 1250; // Maximum packet size
    private Stopwatch latency = new Stopwatch(); // Timer to measure latency
    private bool firstPacketSent = false; // Flag to check if the first packet has been sent
    private int testCount = 0; // Counter for the number of tests conducted


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
        // Warm up by sending two packets before starting the test
        SendPacket(GeneratePacketData(packetSize), latency);
        SendPacket(GeneratePacketData(packetSize), latency);
        latency.Reset();
    }

    //Called once per frame (Kind of like loop())
    void Update()
    {

        if (!firstPacketSent)
        {
            SendPacket(GeneratePacketData(packetSize), latency); // Send the first packet
            firstPacketSent = true;
        }
            
        if (serialPort.IsOpen)
        {
            try
            {
                string response = serialPort.ReadLine();
                if (!string.IsNullOrEmpty(response) && packetSize <= maxPacketSize && testCount <= 10) //Waiting for a response from the ESP32
                {
                    latency.Stop(); // Stop the timer when a response is received
                    double roundTripTime = latency.Elapsed.TotalMilliseconds;
                    WriteLatencyToFile(packetSize, roundTripTime);
                    latency.Reset();

                    // Increase packet size for next test
                    packetSize += 5;
                    string packetData = GeneratePacketData(packetSize);
                    SendPacket(packetData, latency);
                    
                }
                if(packetSize > maxPacketSize)
                {
                    packetSize = 5; // Reset packet size after reaching max
                    testCount++;
                }
            }
            catch (TimeoutException)
            {
                //Do nothing, just wait for the next frame
            }
        }
    }
   

    // Send a packet to the ESP32 and start the timer
    private void SendPacket(string data,Stopwatch stopwatch)
    {
        serialPort.Write(data);
        serialPort.BaseStream.Flush(); // Ensure data is sent immediately
        stopwatch.Start();
    }

    // Create a packet of specified size
    private string GeneratePacketData(int size)
    {
        string data = new string('1', size); // Generate a string of '1's of the specified size
        data += "\n"; // Append newline character to indicate end of packet
        return data;
    }

    // Write round-trip latency to a file
    private void WriteLatencyToFile(int packetSize, double latency)
    {
        StreamWriter writer = new StreamWriter(@"C:\Senior_Design\latency_results.txt", true);
        writer.WriteLine($"Packet Size: {packetSize}, Latency: {latency}");
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
