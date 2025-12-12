using System;
using System.Collections.Generic;
using System.Data;
using System.Diagnostics;
using System.IO.Ports;
using System.Linq.Expressions;
using UnityEngine;
using UnityEngine.AI;

public class ClassicBT_Improved : MonoBehaviour
{
    public static String ComPort = "COM5";
    SerialPort serialPort = new SerialPort(ComPort, 115200); 
    byte[] fingerStates = new byte[5] { 0, 0, 0, 0, 0}; // Initial finger states
    byte[] lastSent = new byte[5] { 0, 0, 0, 0, 0}; // Last sent finger states
    Dictionary<int, Stopwatch> messagesSent = new Dictionary<int, Stopwatch>();
    int messageID = 0;
    const int ackTimeout = 70; // milliseconds



    void Start()
    {
        try
        {
            serialPort.Open();
            serialPort.ReadTimeout = 100;
            UnityEngine.Debug.Log("Serial port opened successfully.");
        }
        catch (Exception e)
        {
            UnityEngine.Debug.LogError("Failed to open serial port: " + e.Message);
        }
    }

    void Update()
    {
        fingerStates[0] = (byte)(Input.GetKey(KeyCode.Space) ? 1 : 0); // Thumb
        fingerStates[1] = (byte)(Input.GetKey(KeyCode.J) ? 1 : 0); // Index
        fingerStates[2] = (byte)(Input.GetKey(KeyCode.K) ? 1 : 0); // Middle
        fingerStates[3] = (byte)(Input.GetKey(KeyCode.L) ? 1 : 0); // Ring
        fingerStates[4] = (byte)(Input.GetKey(KeyCode.Semicolon) ? 1 : 0); // Pinky

        if (!AreArraysEqual(fingerStates, lastSent))
        {
            sendMessage(fingerStates, lastSent);
        }

        
        if (messageID > 0) // No messages sent, nothing to check
        { 
            CheckForAcks(); 
            //CheckForTimeOuts();
        } 
    }

    void sendMessage(byte[] arr1, byte[] arr2)
    {
        messageID++; // Increment message ID

        //Build message 5 bytes for fingers, 4 bytes for message ID, 1 byte for newline
        byte[] message = new byte[11];
        //Copy (SourceArray, SourceIndex, DestinationArray, DestinationIndex, Length)
        Array.Copy(fingerStates, 0, message, 0, 5);
        int checksum = fingerStates[0] + fingerStates[1] + fingerStates[2] + fingerStates[3] + fingerStates[4];
        message[5] = (byte)checksum;                              // Add checksum
        byte[] messageIdBytes = BitConverter.GetBytes(messageID); // Convert messageID to byte array
        Array.Copy(messageIdBytes, 0, message, 6, 4);             // Copy messageID bytes into message
        message[10] = (byte)'\n';                                 // Signal end of message
        serialPort.Write(message, 0, message.Length);             // Send message

        // Clean up for next send
        Array.Copy(fingerStates, lastSent, fingerStates.Length);  //Record Previous sent state

        // Start tracking this message for acknowledgment
        Stopwatch stopwatch = new Stopwatch();                    // Start timer for RTT measurement
        stopwatch.Start();
        messagesSent[messageID] = stopwatch;                      

        // Clean up previous message tracking
        if (messageID > 1) 
        {
            messagesSent[messageID - 1].Stop(); //Stop the timer for the previous message
            messagesSent.Remove(messageID - 1); //Remove previous message from tracking (Only care about latest message)
        }
    }

    void CheckForAcks()
    {
        // Check for acknowledgments
        try
        {
            string response = serialPort.ReadLine();
            if (!string.IsNullOrEmpty(response)) //Received something
            {
                response = response.Trim(); //Clean up whitespace/newline
                // ERR received, resend
                if (response == "ERR")
                {
                    sendMessage(fingerStates, lastSent); // Resend the latest message
                }
                //If acknowledgment is not an error
                else if (int.TryParse(response.Trim(), out int ackId)) //Try to parse acknowledgment ID
                {
                    if (messagesSent.ContainsKey(ackId)) // Valid acknowledgment
                    {
                        // Stop the timer for this message
                        Stopwatch stopwatch = messagesSent[ackId];
                        stopwatch.Stop();
                        // How long it took to get acknowledgment
                        long rtl = stopwatch.ElapsedMilliseconds;
                        UnityEngine.Debug.Log($"Message {ackId} acknowledged. RTL: {rtl} ms");
                        messagesSent.Remove(ackId);
                        messageID = 0; // Reset message ID after successful acknowledgment
                    }
                }
            }
        }
        catch (TimeoutException)
        {
            // No acknowledgment received within the timeout period

        }
    }

    void CheckForTimeOuts()
    {
        // if no acknowledgment received within timeout, resend
        if (messagesSent[messageID].ElapsedMilliseconds > ackTimeout)
        {
            //UnityEngine.Debug.LogWarning($"Message {messageID} timed out. Resending...");
            sendMessage(fingerStates, lastSent); // Resend the latest message
        }
    }



    bool AreArraysEqual(byte[] arr1, byte[] arr2)
    {
        //Could do array comparison here but want little overhead
        if (arr1.Length != arr2.Length) return false;
        if(arr1[0] != arr2[0]) return false;
        if(arr1[1] != arr2[1]) return false;
        if(arr1[2] != arr2[2]) return false;
        if(arr1[3] != arr2[3]) return false;
        if(arr1[4] != arr2[4]) return false;
        return true;
    }

    void OnApplicationQuit()
    {
        if (serialPort != null && serialPort.IsOpen)
        {
            serialPort.Close();
        }
    }
}
