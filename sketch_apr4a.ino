#include <WiFi.h>
#include <WebServer.h>

// Configuración del Access Point
const char* ssid = "ESP32_MotorControl";
const char* password = "123456789";

// Pin del motor
const int motorPin = 5;  // GPIO5 (ajusta según tu conexión)

// Velocidades predefinidas (0-255)
const int speeds[6] = {0, 51, 102, 153, 204, 255}; // OFF, 1, 2, 3, 4, 5

WebServer server(80);
int currentSpeed = 0;

void setup() {
  Serial.begin(115200);
  
  // Configurar el pin del motor como salida
  pinMode(motorPin, OUTPUT);
  analogWrite(motorPin, 0); // Iniciar con motor apagado
  
  // Crear Access Point
  WiFi.softAP(ssid, password);
  Serial.println("Access Point creado");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  // Configurar rutas del servidor
  server.on("/", HTTP_GET, handleRoot);
  server.on("/setSpeed", HTTP_GET, handleSetSpeed);
  server.on("/getSpeed", HTTP_GET, handleGetSpeed);
  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = R"=====(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Control de Motor</title>
  <style>
    /* From Uiverse.io by Pradeepsaranbishnoi */ 
    .container {
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
      margin: 0;
      padding: 20px;
      box-sizing: border-box;
      background-color: #f5f5f5;
    }
    
    .origin {
      position: absolute;
      left: 50%;
      top: 50%;
      z-index: 111;
      width: 2px;
      height: 2px;
      margin: -1px 0 0 -1px;
      background-color: #f50;
    }

    .de {
      user-select: none;
      position: relative;
      width: 230px;
      height: 230px;
      border-radius: 100%;
      box-shadow: 0 0 25px rgba(0, 0, 0, .1);
      background-color: transparent;
    }

    .den, .dene, .denem, .deneme, .light, .dot {
      position: absolute;
      left: 50%;
      top: 50%;
    }

    .den {
      position: relative;
      width: 220px;
      height: 220px;
      margin: -110px 0 0 -110px;
      border-radius: 100%;
      box-shadow: inset 0 3px 10px rgba(0, 0, 0, .6), 0 2px 20px rgba(255, 255, 255, 1);
      background: #888888;
      background: -moz-radial-gradient(center, ellipse cover, #888888 0%, #333333 100%);
      background: -webkit-gradient(radial, center center, 0px, center center, 100%, color-stop(0%, #888888), color-stop(100%, #333333));
      background: -webkit-radial-gradient(center, ellipse cover, #888888 0%, #333333 100%);
      background: -o-radial-gradient(center, ellipse cover, #888888 0%, #333333 100%);
    }

    .dene {
      z-index: 4;
      width: 140px;
      height: 140px;
      margin: -70px 0 0 -70px;
      border-radius: 100%;
      box-shadow: inset 0 2px 2px rgba(255, 255, 255, .4), 0 3px 13px rgba(0, 0, 0, .85);
      background: #f2f6f5;
      background: -moz-linear-gradient(top, #f2f6f5 0%, #cbd5d6 100%);
      background: -webkit-gradient(linear, left top, left bottom, color-stop(0%, #f2f6f5), color-stop(100%, #cbd5d6));
      background: -webkit-linear-gradient(top, #f2f6f5 0%, #cbd5d6 100%);
      background: -o-linear-gradient(top, #f2f6f5 0%, #cbd5d6 100%);
    }

    .denem {
      width: 120px;
      height: 120px;
      margin: -60px 0 0 -60px;
      border-radius: 100%;
      background: #cbd5d6;
      background: -moz-linear-gradient(top, #cbd5d6 0%, #f2f6f5 100%);
      background: -webkit-gradient(linear, left top, left bottom, color-stop(0%, #cbd5d6), color-stop(100%, #f2f6f5));
      background: -webkit-linear-gradient(top, #cbd5d6 0%, #f2f6f5 100%);
      background: -o-linear-gradient(top, #cbd5d6 0%, #f2f6f5 100%);
    }

    .deneme {
      width: 100px;
      height: 100px;
      margin: -50px 0 0 -50px;
      border-radius: 100%;
      box-shadow: inset 0 2px 3px rgba(255, 255, 255, .6), 0 8px 20px rgba(0, 0, 0, .9);
      background: #eef7f6;
      background: -moz-linear-gradient(top, #eef7f6 0%, #8d989a 100%);
      background: -webkit-gradient(linear, left top, left bottom, color-stop(0%, #eef7f6), color-stop(100%, #8d989a));
      background: -webkit-linear-gradient(top, #eef7f6 0%, #8d989a 100%);
      background: -o-linear-gradient(top, #eef7f6 0%, #8d989a 100%);
    }

    .den .switch {
      z-index: 3;
      position: absolute;
      top: 0;
      left: 0;
      width: 100%;
      height: 100%;
    }

    .den .switch label {
      z-index: 2;
      position: absolute;
      top: 50%;
      left: 50%;
      width: 50%;
      height: 70px;
      margin-top: -35px;
      transform-origin: 0% 50%;
      -webkit-transform-origin: 0% 50%;
      -o-transform-origin: 0% 50%;
    }

    .den .switch label:after {
      content: "";
      position: absolute;
      top: 6px;
      left: 1px;
      width: 100%;
      height: 30px;
      transform: rotate(-30deg);
      -webkit-transform: rotate(-30deg);
      -o-transform: rotate(-30deg);
    }

    .den .switch label:before {
      content: "";
      position: absolute;
      bottom: 6px;
      left: 1px;
      width: 100%;
      height: 30px;
      transform: rotate(30deg);
      -webkit-transform: rotate(30deg);
      -o-transform: rotate(30deg);
    }

    .den .switch label span {
      z-index: 2;
      position: absolute;
      top: 0;
      right: 0;
      width: 40px;
      height: 100%;
      font-weight: bold;
      font-size: 15px;
      line-height: 70px;
      text-align: center;
      color: #eee;
      text-shadow: 0 1px 0 #444;
    }

    .den .switch label:nth-child(1) {
      transform: rotate(-90deg);
      -webkit-transform: rotate(-90deg);
      -o-transform: rotate(-90deg);
    }

    .den .switch label:nth-child(1) span {
      right: 2px;
      font-size: 13px;
      transform: rotate(90deg);
      -webkit-transform: rotate(90deg);
      -o-transform: rotate(90deg);
    }

    .den .switch label:nth-child(2) {
      transform: rotate(-30deg);
      -webkit-transform: rotate(-30deg);
      -o-transform: rotate(-30deg);
    }

    .den .switch label:nth-child(2) span {
      transform: rotate(30deg);
      -webkit-transform: rotate(30deg);
      -o-transform: rotate(30deg);
    }

    .den .switch label:nth-child(3) {
      transform: rotate(30deg);
      -webkit-transform: rotate(30deg);
      -o-transform: rotate(30deg);
    }

    .den .switch label:nth-child(3) span {
      transform: rotate(-30deg);
      -webkit-transform: rotate(-30deg);
      -o-transform: rotate(-30deg);
    }

    .den .switch label:nth-child(4) {
      transform: rotate(90deg);
      -webkit-transform: rotate(90deg);
      -o-transform: rotate(90deg);
    }

    .den .switch label:nth-child(4) span {
      transform: rotate(-90deg);
      -webkit-transform: rotate(-90deg);
      -o-transform: rotate(-90deg);
    }

    .den .switch label:nth-child(5) {
      transform: rotate(150deg);
      -webkit-transform: rotate(150deg);
      -o-transform: rotate(150deg);
    }

    .den .switch label:nth-child(5) span {
      transform: rotate(-150deg);
      -webkit-transform: rotate(-150deg);
      -o-transform: rotate(-150deg);
    }

    .den .switch label:nth-child(6) {
      transform: rotate(210deg);
      -webkit-transform: rotate(210deg);
      -o-transform: rotate(210deg);
    }

    .den .switch label:nth-child(6) span {
      transform: rotate(-210deg);
      -webkit-transform: rotate(-210deg);
      -o-transform: rotate(-210deg);
    }

    .den .switch input {
      position: absolute;
      opacity: 0;
      visibility: hidden;
    }

    /* SWITCH LIGHT */
    .den .light {
      z-index: 1;
      width: 50%;
      height: 100px;
      margin-top: -50px;
      transform-origin: 0% 50%;
      -webkit-transform-origin: 0% 50%;
      -o-transform-origin: 0% 50%;
      transform: rotate(-90deg);
      -webkit-transform: rotate(-90deg);
      -o-transform: rotate(-90deg);
      -webkit-transition: all .5s;
      transition: all .5s;
      -o-transition: all .5s;
    }

    .den .light span {
      opacity: .4;
      position: absolute;
      top: 0;
      left: 15px;
      width: 100px;
      height: 100px;
      background: -moz-radial-gradient(center, ellipse cover, rgba(184, 163, 204, 1) 0%, rgba(159, 197, 224, 0.42) 42%, rgba(111, 113, 179, 0) 72%, rgba(67, 34, 137, 0) 100%);
      background: -webkit-gradient(radial, center center, 0px, center center, 100%, color-stop(0%, rgba(184, 163, 204, 1)), color-stop(42%, rgba(159, 197, 224, 0.42)), color-stop(72%, rgba(111, 113, 179, 0)), color-stop(100%, rgba(67, 34, 137, 0)));
      background: -webkit-radial-gradient(center, ellipse cover, rgba(184, 163, 204, 1) 0%, rgba(159, 197, 224, 0.42) 42%, rgba(111, 113, 179, 0) 72%, rgba(67, 34, 137, 0) 100%);
      background: -o-radial-gradient(center, ellipse cover, rgba(184, 163, 204, 1) 0%, rgba(159, 197, 224, 0.42) 42%, rgba(111, 113, 179, 0) 72%, rgba(67, 34, 137, 0) 100%);
    }

    /* SWITCH LIGHT POSITION */
    .den #switch_off:checked ~ .light {
      transform: rotate(-90deg);
      -webkit-transform: rotate(-90deg);
      -o-transform: rotate(-90deg);
    }

    .den #switch_1:checked ~ .light {
      transform: rotate(-30deg);
      -webkit-transform: rotate(-30deg);
      -o-transform: rotate(-30deg);
    }

    .den #switch_2:checked ~ .light {
      transform: rotate(30deg);
      -webkit-transform: rotate(30deg);
      -o-transform: rotate(30deg);
    }

    .den #switch_3:checked ~ .light {
      transform: rotate(90deg);
      -webkit-transform: rotate(90deg);
      -o-transform: rotate(90deg);
    }

    .den #switch_4:checked ~ .light {
      transform: rotate(150deg);
      -webkit-transform: rotate(150deg);
      -o-transform: rotate(150deg);
    }

    .den #switch_5:checked ~ .light {
      transform: rotate(210deg);
      -webkit-transform: rotate(210deg);
      -o-transform: rotate(210deg);
    }

    /* SWITCH DOT */
    .den .dot {
      z-index: 6;
      width: 50%;
      height: 12px;
      margin-top: -6px;
      transform-origin: 0% 50%;
      -webkit-transform-origin: 0% 50%;
      -o-transform-origin: 0% 50%;
      transition: all .5s;
      -moz-transition: all .5s;
      -o-transition: all .5s;
    }

    .den .dot span {
      position: absolute;
      top: 0;
      left: 30px;
      width: 12px;
      height: 12px;
      border-radius: 100%;
      background: #dae2e4;
      background: -moz-linear-gradient(top, #dae2e4 0%, #ecf5f4 100%);
      background: -webkit-gradient(linear, left top, left bottom, color-stop(0%, #dae2e4), color-stop(100%, #ecf5f4));
      background: -webkit-linear-gradient(top, #dae2e4 0%, #ecf5f4 100%);
      background: -o-linear-gradient(top, #dae2e4 0%, #ecf5f4 100%);
    }

    /* DOT POSITIONS */
    .den #switch_off:checked ~ .dot {
      transform: rotate(-90deg);
      -webkit-transform: rotate(-90deg);
      -o-transform: rotate(-90deg);
    }

    .den #switch_off:checked ~ .dot span {
      transform: rotate(90deg);
      -webkit-transform: rotate(90deg);
      -o-transform: rotate(90deg);
    }

    .den #switch_1:checked ~ .dot {
      transform: rotate(-30deg);
      -webkit-transform: rotate(-30deg);
      -o-transform: rotate(-30deg);
    }

    .den #switch_1:checked ~ .dot span {
      opacity: .9;
      transform: rotate(30deg);
      -webkit-transform: rotate(30deg);
      -o-transform: rotate(30deg);
    }

    .den #switch_2:checked ~ .dot {
      transform: rotate(30deg);
      -webkit-transform: rotate(30deg);
      -o-transform: rotate(30deg);
    }

    .den #switch_2:checked ~ .dot span {
      opacity: .5;
      transform: rotate(-30deg);
      -webkit-transform: rotate(-30deg);
      -o-transform: rotate(-30deg);
    }

    .den #switch_3:checked ~ .dot {
      transform: rotate(90deg);
      -webkit-transform: rotate(90deg);
      -o-transform: rotate(90deg);
    }

    .den #switch_3:checked ~ .dot span {
      opacity: .4;
      transform: rotate(-90deg);
      -webkit-transform: rotate(-90deg);
      -o-transform: rotate(-90deg);
    }

    .den #switch_4:checked ~ .dot {
      transform: rotate(150deg);
      -webkit-transform: rotate(150deg);
      -o-transform: rotate(150deg);
    }

    .den #switch_4:checked ~ .dot span {
      opacity: .5;
      transform: rotate(-150deg);
      -webkit-transform: rotate(-150deg);
      -o-transform: rotate(-150deg);
    }

    .den #switch_5:checked ~ .dot {
      transform: rotate(210deg);
      -webkit-transform: rotate(210deg);
      -o-transform: rotate(210deg);
    }

    .den #switch_5:checked ~ .dot span {
      opacity: .9;
      transform: rotate(-210deg);
      -webkit-transform: rotate(-210deg);
      -o-transform: rotate(-210deg);
    }

    /* LINE */
    .den hr.line {
      z-index: 1;
      position: absolute;
      top: 50%;
      width: 100%;
      height: 0;
      margin-top: -1px;
      border-width: 1px 0;
      border-style: solid;
      border-top-color: #3c3d3f;
      border-bottom-color: #666769;
    }

    .den hr.line:nth-child(1) {
      transform: rotate(-60deg);
      -webkit-transform: rotate(-60deg);
      -o-transform: rotate(-60deg);
    }

    .den hr.line:nth-child(2) {
      transform: rotate(60deg);
      -webkit-transform: rotate(60deg);
      -o-transform: rotate(60deg);
    }

    /* Status display */
    .status {
      text-align: center;
      margin-top: 20px;
      font-family: Arial, sans-serif;
      color: #333;
    }
  </style>
</head>
<body>
  <div class="container">
    <div class="de">
      <div class="den">
        <hr class="line">
        <hr class="line">
        <hr class="line">
        
        <div class="switch">
          <label for="switch_off"><span>OFF</span></label>
          <label for="switch_1"><span>1</span></label>
          <label for="switch_2"><span>2</span></label>
          <label for="switch_3"><span>3</span></label>
          <label for="switch_4"><span>4</span></label>
          <label for="switch_5"><span>5</span></label>
          
          <input type="radio" checked name="switch" id="switch_off" onchange="setSpeed(0)">
          <input type="radio" name="switch" id="switch_1" onchange="setSpeed(1)">
          <input type="radio" name="switch" id="switch_2" onchange="setSpeed(2)">
          <input type="radio" name="switch" id="switch_3" onchange="setSpeed(3)">
          <input type="radio" name="switch" id="switch_4" onchange="setSpeed(4)">
          <input type="radio" name="switch" id="switch_5" onchange="setSpeed(5)">
          
          <div class="light"><span></span></div>
          <div class="dot"><span></span></div>
          
          <div class="dene">
            <div class="denem">
              <div class="deneme">
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>

  <div class="status">
    <h2>Control de Motor</h2>
    <p>Velocidad actual: <span id="currentSpeed">0</span>/255</p>
    <p>Conectado a: ESP32_MotorControl</p>
  </div>

  <script>
    function setSpeed(level) {
      fetch('/setSpeed?level=' + level)
        .then(response => response.text())
        .then(text => {
          document.getElementById('currentSpeed').textContent = text.split(':')[1];
        });
    }
    
    // Seleccionar el nivel actual al cargar la página
    document.addEventListener('DOMContentLoaded', function() {
      fetch('/getSpeed')
        .then(response => response.json())
        .then(data => {
          document.getElementById('currentSpeed').textContent = data.speed;
          if(data.level >= 0 && data.level <= 5) {
            document.getElementById('switch_' + (data.level === 0 ? 'off' : data.level)).checked = true;
          }
        });
    });
  </script>
</body>
</html>
)=====";

  server.send(200, "text/html", html);
}

void handleSetSpeed() {
  if (server.hasArg("level")) {
    int level = server.arg("level").toInt();
    if (level >= 0 && level <= 5) {
      currentSpeed = speeds[level];
      analogWrite(motorPin, currentSpeed);
      server.send(200, "text/plain", "Velocidad: " + String(currentSpeed));
      Serial.println("Velocidad establecida: " + String(currentSpeed));
    } else {
      server.send(400, "text/plain", "Nivel inválido");
    }
  } else {
    server.send(400, "text/plain", "Falta parámetro level");
  }
}

void handleGetSpeed() {
  int level = 0;
  for (int i = 0; i < 6; i++) {
    if (currentSpeed == speeds[i]) {
      level = i;
      break;
    }
  }
  String json = "{\"speed\":" + String(currentSpeed) + ",\"level\":" + String(level) + "}";
  server.send(200, "application/json", json);
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not found");
}