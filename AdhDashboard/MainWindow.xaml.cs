using System.Collections.Generic;
using System.Linq;
using System.Windows;

namespace AdhDashboard
{
    public partial class MainWindow : Window
    {
        // שימוש בקורא הרשת שמתחבר ל-AdhServer דרך TCP
        private NetworkHardwareReader _reader;
        
        private Queue<int> _distanceSamples = new Queue<int>();
        private const int FilterSize = 10;

        public MainWindow()
        {
            InitializeComponent();
            _reader = new NetworkHardwareReader(); // אתחול קורא הרשת
            Loaded += MainWindow_Loaded;
        }

        private async void MainWindow_Loaded(object sender, RoutedEventArgs e)
        {
            // קריאה מהרשת לא דורשת הגדרת COM Port במסך
            var channelReader = _reader.StartReading(); 

            await foreach (var data in channelReader.ReadAllAsync())
            {
                var parts = data.Split(',');
                int rawDistance = int.Parse(parts[0].Split(':')[1]);
                string adc = parts[1].Split(':')[1];

                // ממוצע נע לסינון רעשי החיישן
                _distanceSamples.Enqueue(rawDistance);
                if (_distanceSamples.Count > FilterSize)
                {
                    _distanceSamples.Dequeue();
                }
                
                int smoothedDistance = (int)_distanceSamples.Average();

                // עדכון התצוגה והסקאלה הגרפית
                Dispatcher.Invoke(() =>
                {
                    DistanceText.Text = $"Distance: {smoothedDistance} cm";
                    DistanceBar.Value = smoothedDistance; 
                    AdcText.Text = $"ADC Value: {adc}";
                });
            }
        }
    }
}