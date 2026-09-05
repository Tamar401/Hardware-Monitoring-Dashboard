using System.IO;
using System.Net.Sockets;
using System.Threading.Channels;
using System.Threading.Tasks;

namespace AdhDashboard
{
    public class NetworkHardwareReader
    {
        public ChannelReader<string> StartReading()
        {
            var channel = Channel.CreateUnbounded<string>();
            
            Task.Run(async () =>
            {
                using TcpClient client = new TcpClient();
                await client.ConnectAsync("127.0.0.1", 5000);
                using StreamReader reader = new StreamReader(client.GetStream());

                while (true)
                {
                    string? data = await reader.ReadLineAsync();
                    
                    if (!string.IsNullOrWhiteSpace(data))
                    {
                        await channel.Writer.WriteAsync(data);
                    }
                }
            });
            
            return channel.Reader;
        }
    }
}