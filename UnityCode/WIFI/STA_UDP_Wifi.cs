using UnityEngine;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System;

public class STA_UDP_Wifi : MonoBehaviour
{
    UdpClient udp;
    IPEndPoint espEndPoint;
    Thread receiveThread;
    bool Acknowledged = false;

    // configuration
    public string espIP = "10.0.0.6"; // Set the ESP32 IP address here
    public int espPort = 4211; // Set the ESP32 port here
    public int laptopPort = 4210; // Local port to listen for responses

    //finger state array
    byte[] fingerStates = new byte[5] { 0, 0, 0, 0, 0}; // Initial finger states
    byte[] lastSent = new byte[5] { 0, 0, 0, 0, 0}; // Last sent finger states


    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        udp = new UdpClient(laptopPort);
        espEndPoint = new IPEndPoint(IPAddress.Parse(espIP), espPort);
        receiveThread = new Thread(ReceiveData);
        receiveThread.IsBackground = true;
        receiveThread.Start();

        Debug.Log("UDP initalized, and listening for acknowledgements on port #" + laptopPort);
    }

    // Update is called once per frame
    void Update()
    {
        fingerStates[0] = (byte)(Input.GetKey(KeyCode.Space) ? 1 : 0); // Thumb
        fingerStates[1] = (byte)(Input.GetKey(KeyCode.J) ? 1 : 0); // Index
        fingerStates[2] = (byte)(Input.GetKey(KeyCode.K) ? 1 : 0); // Middle
        fingerStates[3] = (byte)(Input.GetKey(KeyCode.L) ? 1 : 0); // Ring
        fingerStates[4] = (byte)(Input.GetKey(KeyCode.Semicolon) ? 1 : 0); // Pinky

        // Check if finger states have changed
        if(!AreArraysEqual(fingerStates, lastSent))
        {
            SendMessage();
            Array.Copy(fingerStates, lastSent, fingerStates.Length);
        }

        if(Acknowledged)
        {
            Debug.Log("Acknowledgment received from ESP32.");
            Acknowledged = false; // Reset acknowledgment flag
        }
    }

    bool AreArraysEqual(byte[] arr1, byte[] arr2)
    {
        if (arr1.Length != arr2.Length)
            return false;
        for (int i = 0; i < arr1.Length; i++)
        {
            if (arr1[i] != arr2[i])
                return false;
        }
        return true;
    }

    void SendMessage()
    {
        byte checksum = (byte)(fingerStates[0] + fingerStates[1] + fingerStates[2] + fingerStates[3] + fingerStates[4]);
        byte[] message = new byte[6] { fingerStates[0], fingerStates[1], fingerStates[2], fingerStates[3], fingerStates[4], checksum };
        udp.Send(message, message.Length, espEndPoint);

        Debug.Log("Sent message: " + string.Join(",", message));
    }

    void ReceiveData()
    {
        IPEndPoint remoteEndPoint = new IPEndPoint(IPAddress.Any, 0);
        while (true)
        {
            try
            {
                byte[] data = udp.Receive(ref remoteEndPoint);
                string ACK = Encoding.UTF8.GetString(data);
                if (ACK.Contains("ACK")) // Assuming 0xAC is the acknowledgment byte
                {
                    Acknowledged = true;
                }
            }
            catch (Exception e)
            {
                Debug.LogError("Error receiving UDP data: " + e.Message);
            }
        }
    }
    
    void OnApplicationQuit()
    {
        if (receiveThread != null && receiveThread.IsAlive)
        {
            receiveThread.Abort();
        }
        if (udp != null)
        {
            udp.Close();
        }
    }
}
