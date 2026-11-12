using System;
using System.Net.Http;
using System.Text.Json;

namespace get_dlc_list_script
{
    class SteamDlcParser
    {

        private Game gameEx = new Game();


        public async Task<string[]> GetDlcListFromSteamStoreApiAsync(string appId) // this method retrieves the list of DLCs for a given Steam game using the Steam Store API
        {
            string url = $"https://store.steampowered.com/api/appdetails?appids={appId}";

            using var client = new HttpClient();
            string response = await client.GetStringAsync(url);
            using var doc = JsonDocument.Parse(response);

            // 1.check if the response contains the appId and success is true
            if (!doc.RootElement.TryGetProperty(appId, out var appNode) ||
                !appNode.TryGetProperty("success", out var successNode) ||
                !successNode.GetBoolean())
            {
                return Array.Empty<string>();
            }

            // 2. safely check if the "data" property exists and contains "dlc"
            if (!appNode.TryGetProperty("data", out var dataNode) ||
                !dataNode.TryGetProperty("dlc", out var dlcArray))
            {
                return Array.Empty<string>(); // if game doesnt have dlc
            }

            // 3. using a List<string> to collect the DLC IDs
            var result = new List<string>();
            foreach (var dlc in dlcArray.EnumerateArray())
            {
                result.Add(dlc.GetInt32().ToString());
            }

            return result.ToArray();
        }


        public SteamDlcParser( Game game )
        {
            gameEx = game;
        }
    }

}