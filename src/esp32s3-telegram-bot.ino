/*
  Arduino project to send a telegram message
*/

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "ucontroller"; // REPLACE_WITH_YOUR_SSID
const char* password = "soria975"; // REPLACE_WITH_YOUR_PASSWORD

// Initialize Telegram BOT
#define BOTtoken ""  // your Bot Token (Get from Botfather), REPLACE_WITH_YOUR_TOKEN

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "" // your Chat ID (Get from your chat), REPLACE_WITH_YOUR_TOKEN

String msg;
int counter = 0;
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

void setup() {
    Serial.begin(9600);
    while (!Serial); // Wait until Serial is ready
    Serial.println("Debug ready!");

    // Attempt to connect to Wifi network:
    Serial.print("Connecting Wifi: ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org

    while (WiFi.status() != WL_CONNECTED) {
    Serial.print(counter);
    Serial.println(" - Connecting ...");
    counter = counter + 1;
    delay(500);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    bot.sendMessage(CHAT_ID, "Bot started up", "");
    counter = 0;
}

void loop() {
    msg = notificationMessage(counter, "El nivel del cubo de basura ha alcanzado un", 0.5);
    bot.sendMessage(CHAT_ID, msg, "");
    Serial.print(counter);
    Serial.println(" - Message sent!");
    counter = counter + 1;
    delay(5000);
}


char* notificationMessage(const int cnt, const char* str, const float percentage) {
    char str1[16], str3[16];
    char* result;
    sprintf(str1, "%d", cnt);
    sprintf(str3, "%f", percentage);
    asprintf(&result, "%s - %s %s", str1, str, str3);
    return result;
}
