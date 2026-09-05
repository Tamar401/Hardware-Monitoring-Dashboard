using System;
using System.Threading.Channels;
using System.Threading.Tasks;

namespace AdhDashboard
{
    public class MockHardwareReader
    {
        public ChannelReader<string> StartReading()
        {
            var channel = Channel.CreateUnbounded<string>();
            Task.Run(async () =>
            {
                var random = new Random();
                int currentDistance = 100; // מרחק התחלתי

                while (true)
                {
                    // שינוי הדרגתי של המרחק: זז בין -5 ל-+5 ס"מ בכל דגימה
                    currentDistance += random.Next(-5, 6);
                    currentDistance = Math.Clamp(currentDistance, 20, 200); // שומר בטווח הגיוני
                    
                    int adc = random.Next(2000, 2100); // ערך ADC יציב יותר
                    
                    await channel.Writer.WriteAsync($"DIST:{currentDistance},ADC:{adc}");
                    await Task.Delay(100); 
                }
            });
            return channel.Reader;
        }
    }
}