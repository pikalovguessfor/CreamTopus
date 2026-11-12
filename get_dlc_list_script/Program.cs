using System;
using System.IO;

namespace get_dlc_list_script
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Game game = new Game(); // test of dlc responce
            SteamDlcParser dlcParser = new SteamDlcParser(game);

            StreamReader sr = new StreamReader("temp/OutputMain.txt");
            StreamWriter sw = new StreamWriter("temp/InputMain.txt");

            game.SteamGameAppId = sr.ReadLine();

            game.GameDlcArrayString = dlcParser.GetDlcListFromSteamStoreApiAsync(game.SteamGameAppId).Result;

            sw.WriteLine(string.Join(",", game.GameDlcArrayString));

            sw.Close();
            sr.Close();

            return;
        }
    }
}
