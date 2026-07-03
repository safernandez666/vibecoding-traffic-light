// semaforo_server.ino  -  Semaforo + buzzer (ESP32 / Arduino)
// Red: Tamborini1D  |  IP fija: 192.168.1.10
// LEDs: rojo=25  amarillo=27  verde=26   Buzzer: 33
//
// /alerta -> Claude pide atencion:
//    * 2 tandas de pitidos al principio (para llamarte)
//    * luego el amarillo sigue titilando EN SILENCIO hasta los 30s
// Todo NO BLOQUEANTE: el server sigue respondiendo mientras corre.

#include <WiFi.h>
#include <WebServer.h>

const char* SSID = "Tamborini1D";
const char* PASSWORD = "Santillana720";
IPAddress ip(192,168,1,10), gateway(192,168,1,1), subnet(255,255,255,0), dns(192,168,1,1);

const int ROJO=25, AMARILLO=27, VERDE=26, BUZZER=33;
const int T_ROJO=1500, T_AMARILLO=2000, T_VERDE=2600;

// ---- parametros de la alerta ----
const unsigned long ALERTA_MS = 30000;  // duracion total (titileo del LED)
const unsigned long BLINK_MS  = 350;    // velocidad del titileo
const int           TANDAS    = 2;      // cuantas tandas de buzz al principio
const int           PITIDOS   = 3;      // pitidos por tanda
const int           PITIDO_MS = 120;    // duracion de cada pitido
const int           GAP_MS    = 90;     // silencio entre pitidos
const int           TANDA_GAP = 500;    // silencio entre tandas

// ---- estado (no bloqueante) ----
bool          alerta_on   = false;
unsigned long alerta_fin  = 0;
unsigned long blink_next  = 0;
bool          blink_fase  = false;

WebServer server(80);

void escribir(bool r,bool a,bool v){ digitalWrite(ROJO,r); digitalWrite(AMARILLO,a); digitalWrite(VERDE,v); }
void beep(int f,int ms){ tone(BUZZER,f); delay(ms); noTone(BUZZER); }
void done_chime(){ beep(1600,150); beep(2000,150); beep(2500,500); }

// Las 2 tandas de pitidos suenan una sola vez al iniciar la alerta.
// Son cortas (~2s en total) asi que las hago aca directo; el resto del
// tiempo (titileo silencioso) es lo que corre no bloqueante en loop().
void tandas_iniciales(){
  for (int t=0; t<TANDAS; t++){
    for (int p=0; p<PITIDOS; p++){
      digitalWrite(AMARILLO, HIGH); tone(BUZZER, T_AMARILLO);
      delay(PITIDO_MS);
      digitalWrite(AMARILLO, LOW);  noTone(BUZZER);
      delay(GAP_MS);
    }
    delay(TANDA_GAP);
  }
}

void iniciar_alerta(){
  escribir(0,0,0);
  tandas_iniciales();          // 2 tandas de sonido para llamarte
  alerta_on  = true;
  alerta_fin = millis() + ALERTA_MS;
  blink_next = 0;
  blink_fase = false;
}
void cortar_alerta(){
  alerta_on = false;
  digitalWrite(AMARILLO, LOW);
  noTone(BUZZER);
}

void setup(){
  Serial.begin(115200);
  pinMode(ROJO,OUTPUT); pinMode(AMARILLO,OUTPUT); pinMode(VERDE,OUTPUT);
  escribir(0,0,0); noTone(BUZZER);

  WiFi.mode(WIFI_STA);
  WiFi.config(ip,gateway,subnet,dns);
  WiFi.begin(SSID,PASSWORD);
  while(WiFi.status()!=WL_CONNECTED){ delay(300); Serial.print("."); }
  Serial.print("\nIP del modulo: "); Serial.println(WiFi.localIP());

  server.on("/alerta",        HTTP_POST, [](){ iniciar_alerta(); server.send(200,"text/plain","ok alerta\n"); });
  server.on("/solo/amarillo", HTTP_POST, [](){ iniciar_alerta(); server.send(200,"text/plain","ok alerta\n"); });
  server.on("/solo/rojo",  HTTP_POST, [](){ cortar_alerta(); escribir(1,0,0); beep(T_ROJO,120); server.send(200,"text/plain","ok solo rojo\n"); });
  server.on("/solo/verde", HTTP_POST, [](){ cortar_alerta(); escribir(0,0,1); done_chime();      server.send(200,"text/plain","ok solo verde\n"); });
  server.on("/off",        HTTP_POST, [](){ cortar_alerta(); escribir(0,0,0); server.send(200,"text/plain","ok off\n"); });

  server.begin();
  Serial.println("Servidor listo en http://192.168.1.10");
}

void loop(){
  server.handleClient();

  // titileo SILENCIOSO del amarillo hasta cumplir los 30s
  if (alerta_on){
    if (millis() >= alerta_fin){
      cortar_alerta();
    } else if (millis() >= blink_next){
      blink_fase = !blink_fase;
      blink_next = millis() + BLINK_MS;
      digitalWrite(AMARILLO, blink_fase ? HIGH : LOW);   // sin tone(): mudo
    }
  }
}
