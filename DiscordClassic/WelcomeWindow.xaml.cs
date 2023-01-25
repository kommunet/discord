using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Media;

namespace DiscordClassic
{
    public partial class WelcomeWindow : Window
    {
        public WelcomeWindow()
        {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            SoundPlayer player = new SoundPlayer(@"resources/audio/dsound_con.wav");
            player.Load(); player.PlaySync();

            // MainWindow mainWindow = new MainWindow();
            // mainWindow.Show();

            Close();
        }
    }
}
