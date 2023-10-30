using System.Windows;
using WPFWeather.App.Services;
using WPFWeather.App.ViewModels;

namespace WPFWeather.App.Windows
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();

            WeatherViewModel weather_vm = new WeatherViewModel(new WeatherDiskService());
            weather_vm.DownloadWeatherCommand.Execute("Brno");
            DataContext = weather_vm;
        }
    }
}