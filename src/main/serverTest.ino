//#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = {192, 168, 178, 42};
//byte gateway[] = {192, 186,178,1};
//byte subnet[] = {255,255,255,0};

String request;

EthernetServer server(80); // Web server

// Http data
String reqData; // Request from Smartphone
String header;
int contentSize = -1;
String CONTENT_LENGTH_TXT = "Content-Length: ";

void setupServer() {
    // disable the SD card by switching pin 4 high 
    pinMode(4, OUTPUT); 
    digitalWrite(4, HIGH); 

    // setup internet
    Serial.println("Initiaizing ethernet...");

    Ethernet.begin(mac, ip);//, gateway, subnet);
    // give the card a second to initialize 
    delay(1000);
    
    server.begin();
    Serial.println("Server set up at");
    Serial.println(Ethernet.localIP());
}

void serverLoop() {
  // listen for incoming clients 
  EthernetClient client = server.available(); 
  if (client) { 
    Serial.println("new client"); 

    request = "";
    
    // an http request ends with a blank line 
    boolean currentLineIsBlank = true; 
    int i = 0;
    while (client.connected()) { 
      if (client.available()) { 
        char c = client.read(); 
        Serial.write(c);
        if (i < 160) request += c;
        i++;
        
        // if you've gotten to the end of the line (received a newline 
        // character) and the line is blank, the http request has ended, 
        // so you can send a reply 
        if (c == '\n' && currentLineIsBlank) { 
          //if (request.startsWith("GET /?text=")) break;
          // SEND HTTP RESPONSE HEADER 
          client.println("HTTP/1.1 200 OK"); 
          client.println("Content-Type: text/html"); 
          client.println("Connection: close");  // the connection will be closed after completion of the response 
          //client.println("Refresh: 10"); // refresh the page automatically every 5 sec 
          client.println(); 
          client.println("<!DOCTYPE HTML>");
          // CSS
          client.println("<style>");
          client.println("body { background-color: #d3d3d3;}");
          client.println("input[type=text], select {");
          client.println("width: 100%;");
          client.println("padding: 12px 20px;");
          client.println("margin: 8px 0;");
          client.println("box-sizing: border-box;");
          client.println("font-size: 150%;}");
          client.println("input[type=submit], select {");
          client.println("width: 100%;");
          client.println("background-color: #4CAF50;");
          client.println("color: white;");
          client.println("padding: 14px 0;");
          client.println("margin: 8px 0;}");
          client.println("input[type=submit]:hover {");
          client.println("background-color: #45a049;}");
          client.println("div {");
          client.println("width: 50%;");
          client.println("margin: auto;");
          client.println("background-color: #5381ac;");
          client.println("padding: 20px;}");
          client.println("</style>");
          // END OF CSS


          // BODY
          client.println("<body><br/><br/><div>");
          client.println("<h1>Welcome to the LED-Matrix webserver</h1>");
          // INPUT FORM
          client.println("<div><h4>Enter Text</h4>");
          client.println("<form action=\"http://192.168.178.42\" method=\"GET\">");
          client.println("<input type=\"text\" id=\"led\" name=\"text\">");
          client.println("<input type=\"submit\" value=\"Submit\"/></form></div>");
          // END OF INPUT FORM
          /*
          // buttons form
          client.println("<div>");
          client.println("<h4>Functions</h4>");
          client.println("<form action=\"http://192.168.178.42\" method=\"GET\">");
          
          client.println("<form action=\"http://192.168.178.42\" method=\"GET\">");
          client.println("<input type=\"submit\" name=\"weather\"></form>");
          client.println("</form></div>");
          // end of buttons form
          */
          client.println("</div></body></html>");
          // END OF HTML
          break; 
        } 
        if (c == '\n') { 
          // you're starting a new line 
          currentLineIsBlank = true; 
        } else if (c != '\r') { 
          // you've gotten a character on the current line 
          currentLineIsBlank = false; 
        } 
      } 
    } 
    // give the web browser time to receive the data 
    delay(1); 
    // close the connection: 
    client.stop(); 
    Serial.println("client request:");
    Serial.println(request);
    parseRequest(&request);
    Serial.println("client disconnected");
  }
}

void parseRequest(String *request) {
  /*
   * int startIndex = request->indexOf("/"); // 'GET /?text=jahui HTTP/1.1\n'
  char *message;
  char start = request->charAt(startIndex + 1);
  if (start == '?') {
    startIndex += 7; // 'GET /?text=jahui HTTP/1.1\n'
    int endIndex = request->indexOf("\n") - 10; // 'GET /?text=jahui HTTP/1.1\n'
    char message[endIndex - startIndex];
    // substring doesn't work
    for(int i = 0; i < endIndex - startIndex; i++) {
      message[i] = request->charAt(startIndex + i);
    }
    // black magic (it removes some weird characters, we don't
    // know why either)
    String str(message);
  } else if (start == 't') {
    char message = "04:20";
  } else if (start == 'w') {
    char message = "kalt";
  } else return;
  // else if for movie (disable shifteText, reenable after movie
   */
  int startIndex = request->indexOf("/"); // 'GET /?text=jahui HTTP/1.1\n'
  if (!(request->charAt(startIndex + 1) == '?')) {
    return;
  }
  startIndex += 7; // 'GET /?text=jahui HTTP/1.1\n'
  int endIndex = request->indexOf("\n") - 10; // 'GET /?text=jahui HTTP/1.1\n'
  char message[endIndex - startIndex + 1] = "";
  // substring doesn't work
  for(int i = 0; i < endIndex - startIndex; i++) {
    message[i] = request->charAt(startIndex + i);
  }
  message[endIndex - startIndex] = 0; // last character has to be zero, or the string won't "end" there
  // black magic (it removes some weird characters, we don't
  // know why either)
  Serial.println("#####");
  Serial.println(startIndex);
  Serial.println(endIndex);
  String str(message);
  Serial.println("------------");
  Serial.println(message);
  Serial.println("------------");
  //writeText(message,0);
  setLongText(message);
}
