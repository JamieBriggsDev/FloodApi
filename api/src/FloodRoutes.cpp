//
// Created by Jamie Briggs on 10/07/2025.
//

#include "../include/FloodRoutes.h"

#include <sstream>
#include <Update.h>
#include <cJSON.h>

#include "wifi_settings.h"
#include "Printer.h"
#include "Readings.h"


void FloodRoutes::index(Request& request, Response& response)
{
    response.set("Content-Type", "text/html");

    response.println("<html>");
    response.println("<body>");
    response.println("  <h1>");
    response.println("    Compiled: " __DATE__ " " __TIME__);
    response.println("  </h1>");
    response.println("  <form id='form'>");
    response.println("    <input id='file' type='file'>");
    response.println("    <input type='submit' value='Send' />");
    response.println("  </form>");
    response.println("</body>");
    response.println("<script>");
    response.println("  const form = document.getElementById('form');");
    response.println("  form.onsubmit = function(e) {");
    response.println("    e.preventDefault();");
    response.println("    const body = document.getElementById('file').files[0];");
    response.println("    fetch('/update', { method: 'POST', body }).then((response) => {");
    response.println("      if (!response.ok) {");
    response.println("        return alert('File upload failed');");
    response.println("      }");
    response.println("      alert('File upload succeeded');");
    response.println("    });");
    response.println("  }");
    response.println("</script>");
    response.println("</html>");
}

void FloodRoutes::update(Request& request, Response& response)
{
    int contentLength = request.left();

    if (strcmp(request.get("Expect"), "100-continue") == 0)
    {
        response.status(100);
    }

    if (!Update.begin(contentLength))
    {
        response.status(500);
        return Update.printError(request);
    }

    unsigned long start = millis();
    while (!request.available() && millis() - start <= 5000)
    {
    }

    if (!request.available())
    {
        return response.sendStatus(408);
    }

    if (Update.writeStream(request) != contentLength)
    {
        response.status(400);
        return Update.printError(request);
    }

    if (!Update.end(true))
    {
        response.status(500);
        return Update.printError(request);
    }

    response.sendStatus(204);
}

void FloodRoutes::river(Request& request, Response& response)
{
    response.set("Content-Type", "application/json");

    response.println("{ \"readings\": [] }");
}

void FloodRoutes::riverStation(Request& request, Response& response)
{
    response.set("Content-Type", "application/json");
}

FloodRoutes::FloodRoutes(Printer* printer) : m_server(PORT), m_printer(printer)
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        m_printer->println("Still connecting..", STICKY);
    }
    // Display IP and PORT number
    std::ostringstream portMessage;
    portMessage << "Port: " << std::to_string(PORT);
    m_printer->println(WiFi.localIP().toString().c_str(), portMessage.str().c_str(), STICKY);

    m_app.header("Expect", m_expectHeader, 20);
    m_app.get("/", &index);
    m_app.post("/update", &update);
    // River API endpoints
    m_app.get("/river", &river);
    m_app.get("/river/{station}", &riverStation);
    m_server.begin();
}

void FloodRoutes::loop()
{
    WiFiClient client = m_server.available();

    if (client.connected())
    {
        m_app.process(&client);
        client.stop();
    }
}
