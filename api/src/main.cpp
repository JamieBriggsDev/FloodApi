//
// Created by Jamie Briggs on 10/07/2025.
//

#include <Arduino.h>
#include <WiFi.h>
#include <Update.h>
#include <aWOT.h>

#include "credentials.h"
#include "Printer.h"


// You can also use cURL: curl http://192.168.1.1/update --data-binary "@sketch.bin"

WiFiServer server(80);
Application app;

char expectHeader[20] {};
bool shouldRestart = false;

void index(Request &req, Response &res) {
    res.set("Content-Type", "text/html");

    res.println("<html>");
    res.println("<body>");
    res.println("  <h1>");
    res.println("    Compiled: " __DATE__ " " __TIME__);
    res.println("  </h1>");
    res.println("  <form id='form'>");
    res.println("    <input id='file' type='file'>");
    res.println("    <input type='submit' value='Send' />");
    res.println("  </form>");
    res.println("</body>");
    res.println("<script>");
    res.println("  const form = document.getElementById('form');");
    res.println("  form.onsubmit = function(e) {");
    res.println("    e.preventDefault();");
    res.println("    const body = document.getElementById('file').files[0];");
    res.println("    fetch('/update', { method: 'POST', body }).then((response) => {");
    res.println("      if (!response.ok) {");
    res.println("        return alert('File upload failed');");
    res.println("      }");
    res.println("      alert('File upload succeeded');");
    res.println("    });");
    res.println("  }");
    res.println("</script>");
    res.println("</html>");
}

void update(Request &req, Response &res) {
    int contentLength = req.left();

    if (strcmp(req.get("Expect"), "100-continue") == 0) {
        res.status(100);
    }

    if (!Update.begin(contentLength)) {
        res.status(500);
        return Update.printError(req);
    }

    unsigned long start = millis();
    while (!req.available() && millis() - start <= 5000) {}

    if (!req.available()) {
        return res.sendStatus(408);
    }

    if (Update.writeStream(req) != contentLength) {
        res.status(400);
        return Update.printError(req);
    }

    if (!Update.end(true)) {
        res.status(500);
        return Update.printError(req);
    }

    shouldRestart = true;
    res.sendStatus(204);
}


Printer printer;

void setup() {
    Serial.begin(115200);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        printer.println("Still connecting..", STICKY);
    }
    printer.println(WiFi.localIP().toString().c_str(), STICKY);

    app.header("Expect", expectHeader, 20);
    app.get("/", &index);
    app.post("/update", &update);
    server.begin();
}

void loop() {
    WiFiClient client = server.available();

    if (client.connected()) {
        app.process(&client);
        client.stop();
    }

    if (shouldRestart) {
        delay(1000);
        ESP.restart();
    }
}

/*void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
}

void loop() {
    printer.println("Hello World", FLASH);
    printer.println("Hello World", SCROLL);
    printer.println("Hello", "World", FLASH);
    printer.println("Hello", "World", SCROLL);

}*/