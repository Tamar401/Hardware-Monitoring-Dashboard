using System;
using System.IO;
using System.Net.Sockets;
using System.Threading.Channels;
using System.Threading.Tasks;

namespace AdhDashboard
{
    public class NetworkHardwareReader
    {
        private const string Host = "127.0.0.1";
        private const int Port = 5000;
        private static readonly TimeSpan ReconnectDelay = TimeSpan.FromSeconds(2);

        public ChannelReader<string> StartReading()
        {
            var channel = Channel.CreateUnbounded<string>();

            Task.Run(async () =>
            {
                while (true) // לולאת Reconnect חיצונית - חיה כל עוד האפליקציה רצה
                {
                    try
                    {
                        using TcpClient client = new TcpClient();
                        await client.ConnectAsync(Host, Port);
                        using StreamReader reader = new StreamReader(client.GetStream());

                        while (true)
                        {
                            string? data = await reader.ReadLineAsync();

                            if (data is null)
                                throw new IOException("החיבור נסגר על ידי השרת.");

                            if (!string.IsNullOrWhiteSpace(data))
                                await channel.Writer.WriteAsync(data);
                        }
                    }
                    catch (Exception)
                    {
                        // כאן אפשר להוסיף לוג/עדכון UI שמראה "מתחבר מחדש..."
                    }

                    await Task.Delay(ReconnectDelay); // המתנה לפני ניסיון נוסף
                }
            });

            return channel.Reader;
        }
    }
}
