//#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = {192, 168, 178, 42};
byte weather[] = {146, 185, 181, 89};
//byte gateway[] = {192, 186,178,1};
//byte subnet[] = {255,255,255,0};

String request;

EthernetServer server(80); // Web server
EthernetClient apiClient;

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
          //client.println("Connection: close");  // the connection will be closed after completion of the response
          //client.println("Refresh: 10"); // refresh the page automatically every 5 sec
          client.println();
          // CSS
          /*
            client.println("<style>");
            client.println("body { background-color: #d3d3d3;}");
            client.println("input[type=text], select {");
            client.println("width: 100%;");
            client.println("box-sizing: border-box;");
            client.println("font-size: 150%;}");
            client.println("input[type=submit], select {");
            client.println("width: 100%;");
            client.println("background-color: #4CAF50;");
            client.println("color: white;}");
            client.println("input[type=submit]:hover {");
            client.println("background-color: #45a049;}");
            client.println("div {");
            client.println("width: 50%;");
            client.println("margin: auto;");
            client.println("background-color: #5381ac;");
            client.println("padding: 20px;}");
            client.println("</style>");
            // END OF CSS
          */
          // BODY
          client.println("<body><br/><br/><div>");
          client.println("<h1>Welcome to the LED-Matrix webserver</h1>");
          // INPUT FORM
          client.println("<div><h4>Enter Text</h4>");
          client.println("<form action=\"http://192.168.178.42\" method=\"GET\">");
          client.println("<input type=\"text\" id=\"led\" name=\"text\">");
          client.println("<input type=\"submit\" value=\"Submit\"/>");
          client.println("<h4>Functions</h4>");
          client.println("<input type=\"submit\" name=\"time\" value=\"Time\">");
          client.println("<input type=\"submit\" name=\"weather\" value=\"Weather\"></form><div>");
          // END OF INPUT FORM
          client.println("</div></body></html>");
          // END OF BODY
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

// --------------------------

// PARSER

void parseRequest(String *request) {
  int startIndex = request->indexOf("/"); // 'GET /?text=jahui HTTP/1.1\n'
  if (!(request->charAt(startIndex + 1) == '?')) {
    // no get request, nothing to parse
    return;
  }
  startIndex += 7; // 'GET /?text=jahui HTTP/1.1\n'
  int endIndex = request->indexOf("\n") - 10; // 'GET /?text=jahui HTTP/1.1\n'
  char message[endIndex - startIndex + 1] = "";
  if (request->charAt(startIndex) == '&') {
    // time request
    if (request->charAt(startIndex + 1) == 't') {
      // CALL TIME API HERE
      printTime();
      //setLongText("4:20");
      return;
    }
    // weather request
    if (request->charAt(startIndex + 1) == 'w') {
      // CALL WEATHER API HERE
      //setLongText("kalt");
      printWeather();
      return;
    }
  }
  // print text
  // substring doesn't work
  for (int i = 0; i < endIndex - startIndex; i++) {
    message[i] = request->charAt(startIndex + i);
    if (request->charAt(startIndex + i) == '+') message[i] = ' ';
    if (request->charAt(startIndex + i) == '%' ||
        request->charAt(startIndex + i + 1) == '3' ||
        request->charAt(startIndex + i + 2) == 'F') {
      message[i] = '?';
      message[i + 1] = 0;
      message[i + 2] = 0;
      i += 2;
    }
  }
  message[endIndex - startIndex] = 0; // last character has to be zero, or the string won't "end" there
  // black magic (it removes some weird characters, we don't know why either)
  String str(message);
  Serial.println("------------");
  Serial.println(message);
  Serial.println("------------");
  setLongText(message);
}

// ---------------------

// TIME REQUEST

void printTime() {
  if (apiClient.connect(weather, 80)) {
    Serial.println("connected to time");
    apiClient.println("GET /data/2.5/weather?q=Basel&APPID=3410a8375afbfb13baeeff03f2472b6b");
    apiClient.println();

    // converting the string into a char array and printing it onto the screen
    char *answer = searchStreamForKeyWord("dt", 2, 2, 10);

    Serial.println();
    int x = 0;
    for ( int i = 0; i < 10; i++) {
      Serial.print(answer[i]);
      //x += (answer[i] - '0') * pow(10, 10 - i);
    }

    //Serial.println(x);

    apiClient.stop();

  } else {
    Serial.println("connection to weather failed");
    setLongText("Nuclear winter");
  }
}

// WEATHER REQUEST

void printWeather() {
  //String searchTags[2] = {"temp:\"","description:\""};

  if (apiClient.connect(weather, 80)) {
    Serial.println("connected to weather");
    apiClient.println("GET /data/2.5/weather?q=Basel&APPID=3410a8375afbfb13baeeff03f2472b6b");
    apiClient.println();

    // converting the string into a char array and printing it onto the screen
    char *answer = searchStreamForKeyWord("temp", 4, 2, 6);

    // set to celsius
    float kelvin = 0.f;
    kelvin += (answer[0] - 48) * 100;
    kelvin += (answer[1] - 48) * 10;
    kelvin += answer[2] - 48;
    kelvin += (answer[4] - 48) * 0.1;
    kelvin += (answer[5] - 48) * 0.01;
    Serial.println("kelvin");
    Serial.println(kelvin);
    float cel = kelvin - 273.15;
    Serial.println("celsius");
    Serial.println(cel);

    char celsius[6];
    dtostrf(cel, 6, 2, celsius);
    char temp[8];
    for (int i = 0; i < 6; i++) {
      temp[i] = celsius[i];
    }
    temp[6] = '@';
    temp[7] = 'C';

    // converting the string into a char array and printing it onto the screen
    //setLongText(temperature.c_str());
    //setLongText(celsius.c_str());
    setLongText(temp);
    apiClient.stop();

  } else {
    Serial.println("connection to weather failed");
    setLongText("Nuclear winter");
  }
}

// *******************

// returns a char array which immediatly trails the keyword of length

char *searchStreamForKeyWord(char *search, int listenerSize, int gapLength, int searchLength) {
  char listener[listenerSize] = ""; // listens for search
  int listenerPos = 0;

  Serial.println(search);

  int found = 0;

  // first we want to find where the temperature lies
  while (apiClient.connected()) {
    if (apiClient.available()) {
      char c = apiClient.read();
      Serial.print(c);

      // we fill the listener buffer
      if (listenerPos >= listenerSize) { // buffer is full - we have to shift the data to the left
        for (int i = 0; i < listenerSize - 1; i++) {
          listener[i] = listener[i + 1];
        }
        listener[listenerSize - 1] = c;
      } else { // buffer not yet full, we continue to fill it where we left the last time
        listener[listenerPos] = c;
        listenerPos++;
      }

      // here we check wheter we encountered the string we need
      found = 1;
      for (int i = 0; i < listenerSize; i++) {
        if (listener[i] != search[i]) { // not all characters match the search mask -> found = 0
          found = 0;
          //break;
        }
      }
      if (found == 1) { // all characters match -> leave loop
        break;
      }
    }
  }

  // now that we know where the temperature is, we want to extract it
  if (found == 1) {
    char answer[searchLength + 1] = ""; // here we build the answer
    int i = -gapLength;

    while (apiClient.connected()) {
      if (apiClient.available()) {
        char c = apiClient.read();
        Serial.print(c);

        if (i >= 0) {
          answer[i] = c;
        }
        i++;
        if (i >= searchLength) {
          answer[searchLength] = 0;
          Serial.println();
          return answer;
        }
      }
    }
  } else {
    return "-";
  }
}

