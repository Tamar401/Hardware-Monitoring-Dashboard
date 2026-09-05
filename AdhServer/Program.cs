using System;
using System.IO.Ports;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

class Program
{
    static async Task Main()
    {
        using SerialPort serialPort = new SerialPort("COM3", 115200);
        serialPort.Open();

        TcpListener listener = new TcpListener(IPAddress.Any, 5000);
        listener.Start();
        Console.WriteLine("AdhServer started. Waiting for Dashboard on port 5000...");

        while (true) // לולאה חיצונית - מאפשרת חיבור מחדש של הדשבורד
        {
            using TcpClient client = await listener.AcceptTcpClientAsync();
            Console.WriteLine("Dashboard connected! Streaming hardware data...");

            try
            {
                using var networkStream = client.GetStream();
                while (client.Connected)
                {
                    string data = serialPort.ReadLine() + "\n";
                    byte[] bytes = Encoding.UTF8.GetBytes(data);
                    await networkStream.WriteAsync(bytes, 0, bytes.Length);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Dashboard disconnected: {ex.Message}. Waiting for reconnection...");
            }
        }
    }
}
