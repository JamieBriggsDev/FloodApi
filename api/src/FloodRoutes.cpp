//
// Created by Jamie Briggs on 10/07/2025.
//

#include "../include/FloodRoutes.h"

#include <Update.h>
#include <aWOT.h>
#include <cJSON.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "Printer.h"
#include "wifi_settings.h"


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
  auto start = std::chrono::system_clock::now();
  response.set("Content-Type", "application/json");
  delay(25);
  auto end = std::chrono::system_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  response.set("Request-Time", std::to_string(duration.count()).c_str());
  response.println("{ \"readings\": [] }");
}

void FloodRoutes::riverStation(Request& request, Response& response)
{
  response.set("Content-Type", "application/json");
}

// Add static member definition
Printer* FloodRoutes::s_printer = nullptr;

static std::string getMethodName(Request::MethodType method)
{
  switch (method)
  {
  case Request::GET:
    return "GET";
  case Request::POST:
    return "PST";
  case Request::PUT:
    return "PUT";
  case Request::PATCH:
    return "PTC";
  case Request::DELETE:
    return "DEL";
  default:
    return "UNK";
  }
}

void FloodRoutes::logRequest(Request& req, Response& res)
{
  std::ostringstream messageRowOne;
  messageRowOne << "Request: ";
  std::ostringstream messageRowTwo;
  messageRowTwo << getMethodName(req.method()) << ": " << req.path();
  s_printer->println(messageRowOne.str().c_str(), messageRowTwo.str().c_str(), FLASH);
}

FloodRoutes::FloodRoutes(Printer* printer) : m_server(PORT)
{
  s_printer = printer;

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    s_printer->println("Connecting..", FLASH);
  }
  // Display IP and PORT number
  std::ostringstream portMessage;
  portMessage << "Port: " << std::to_string(PORT);
  s_printer->println(WiFi.localIP().toString().c_str(), portMessage.str().c_str(), STICKY);

  // m_app.header("Expect", m_expectHeader, 20);
  //  Loging request middleware
  m_app.use(&logRequest);

  m_app.get("/", &index);
  m_app.post("/update", &update);
  // River API endpoints
  m_app.get("/river", &river);
  m_app.post("/river", &river);
  m_app.patch("/river", &river);
  m_app.put("/river", &river);
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
