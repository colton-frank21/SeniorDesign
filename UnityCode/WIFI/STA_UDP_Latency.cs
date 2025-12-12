using UnityEngine;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System;
using System.IO;
using System.Diagnostics;
using System.Collections.Generic; // For Stopwatch

public class STA_UDP_Latency : MonoBehaviour
{
    private UdpClient udp;
    private IPEndPoint espEndPoint;
    private Thread receiveThread;
    private bool acknowledged = false;

    // Configuration
    public string espIP = "10.0.0.6"; // ESP32 IP address
    public int espPort = 4211;         // ESP32 port
    public int laptopPort = 4210;      // Local port to listen for responses

    // Latency testing counters
    private int count = 0;
    private int loops = 1;

    //Dictionary to store latencies with packets sent
    private Dictionary<int, Stopwatch> latencyTimers = new Dictionary<int, Stopwatch>();
    private int packetsSent = 0;
    public int packetSize = 5;

    void Start()
    {
        udp = new UdpClient(laptopPort);
        espEndPoint = new IPEndPoint(IPAddress.Parse(espIP), espPort);

        receiveThread = new Thread(ReceiveData)
        {
            IsBackground = true
        };
        receiveThread.Start();

        UnityEngine.Debug.Log($"UDP initialized, listening for acknowledgements on port #{laptopPort}");
    }

    void Update()
    {
        // Send packets continuously
        if (count <= 50)
        {
            SendMessage(loops);
            count++;
        }
        else if (count > 50 && loops <= 250)
        {
            packetSize = 5 * loops;
            loops++;
            count = 0;
        }

        // Handle ACKs
        if (acknowledged)
        {
            UnityEngine.Debug.Log("Acknowledgment received from ESP32.");
            acknowledged = false;
        }
    }

    private void SendMessage(int loops)
    {
        byte[] message = new byte[packetSize];
        Array.Fill(message, (byte)1);
        udp.Send(message, message.Length, espEndPoint);

        packetsSent++;
        Stopwatch stopwatch = new Stopwatch();
        stopwatch.Start(); // Start timing
        latencyTimers[packetsSent] = stopwatch; // Store stopwatch for this packet

        UnityEngine.Debug.Log($"Sent message: {message.Length} bytes");
    }

    private void ReceiveData()
    {
        IPEndPoint remoteEndPoint = new IPEndPoint(IPAddress.Any, 0);

        while (true)
        {
            try
            {
                byte[] data = udp.Receive(ref remoteEndPoint);
                string ack = Encoding.UTF8.GetString(data);

                if (ack.Contains("ACK"))
                {
                    if (int.TryParse(ack.Substring(3), out int packetId))
                    {
                        if (latencyTimers.TryGetValue(packetId, out Stopwatch stopwatch))
                        {
                            stopwatch.Stop(); // Stop timing
                            float latencyMs = (float)stopwatch.Elapsed.TotalMilliseconds;
                            WriteToFile(packetSize, latencyMs);
                            UnityEngine.Debug.Log("Acknowledgment received from ESP32.");
                            latencyTimers.Remove(packetId); // Remove the entry after processing
                        }
                    }
                }
            }
            catch (Exception e)
            {
                UnityEngine.Debug.LogError($"Error receiving UDP data: {e.Message}");
            }
        }
    }

    private void WriteToFile(int packetSize, float latencyMs)
    {
        string path = @"C:\Senior_Design\udp_latency.txt";

        using (StreamWriter sw = new StreamWriter(path, true))
        {
            sw.WriteLine($"{packetSize} {latencyMs}");
        }
    }

    private void OnApplicationQuit()
    {
        if (receiveThread != null && receiveThread.IsAlive)
            receiveThread.Abort();

        udp?.Close();
    }
}
