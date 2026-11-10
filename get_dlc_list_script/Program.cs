namespace get_dlc_list_script
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Game game = new Game(); // test of dlc responce
            game.GameName = "The Witcher 3: Wild Hunt"; // Game name for The Witcher 3: Wild Hunt
            game.SteamGameAppId = "292030"; // AppID for The Witcher 3: Wild Hunt

            string[] dlcArrayTest = new string[100]; // test of dlc responce
            dlcArrayTest = new Steam().GetDlcListFromSteamStoreApiAsync(game.SteamGameAppId).Result; // test of dlc responce

            Console.WriteLine($"DLC for {game.GameName} (AppID: {game.SteamGameAppId}):"); // Test of dlc responce
            Console.WriteLine(string.Join(", ", dlcArrayTest)); // test of dlc responce
        }
    }
}
