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
        Console.WriteLine("AdhServer started. Waiting for Dashboard on port 5000...");
        TcpListener listener = new TcpListener(IPAddress.Any, 5000);
        listener.Start();

        using TcpClient client = await listener.AcceptTcpClientAsync();
        Console.WriteLine("Dashboard connected! Streaming hardware data...");
        using var networkStream = client.GetStream();

        // פתיחת חיבור החומרה (עדכני את הפורט בהתאם לבקר שלך)
        using SerialPort serialPort = new SerialPort("COM3", 115200);
        serialPort.Open();

        while (true)
        {
            try
            {
                string data = serialPort.ReadLine() + "\n";
                byte[] bytes = Encoding.UTF8.GetBytes(data);
                await networkStream.WriteAsync(bytes, 0, bytes.Length);
            }
            catch
            {
                // התעלמות משגיאות רגעיות
            }
        }
    }
}