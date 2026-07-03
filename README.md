# Vibecoding Traffic Light 🚦

Un semáforo físico conectado a Claude Code. Cuando Claude necesita llamarte, el semáforo parpadea en amarillo y pita; cuando está pensando, se pone rojo; cuando termina, verde.

A physical traffic light hooked up to Claude Code. When Claude needs you, the light blinks yellow and beeps; while it's thinking, it turns red; when it's done, green.

<p align="center">
  <a href="https://youtu.be/8MrP0tenx98">
    <img src="https://img.youtube.com/vi/8MrP0tenx98/0.jpg" alt="Vibecoding Traffic Light" width="640">
  </a>
</p>

<p align="center">
  <a href="https://youtu.be/8MrP0tenx98">▶ Ver en acción / Watch it in action</a>
</p>

---

## Hardware

- ESP32 DevKit
- LED rojo / red LED → GPIO 25
- LED amarillo / yellow LED → GPIO 27
- LED verde / green LED → GPIO 26
- Buzzer pasivo / passive buzzer → GPIO 33
- Resistencias de 220 Ω para cada LED / 220 Ω resistors for each LED

## Instalación / Installation

1. Abrí `semaforo_server/semaforo_server.ino` en el IDE de Arduino.  
   Open `semaforo_server/semaforo_server.ino` in the Arduino IDE.
2. Configurá tu red WiFi y una IP fija libre en tu router.  
   Set your WiFi network and a free static IP on your router.

```cpp
const char* SSID     = "TU_RED";      // cambia por tu WiFi / change to your WiFi
const char* PASSWORD = "TU_PASS";     // cambia por tu password / change to your password
IPAddress ip(192,168,1,10);           // IP fija libre / free static IP
```

3. Seleccioná la placa **ESP32 Dev Module** y el puerto correspondiente.  
   Select the **ESP32 Dev Module** board and the right port.
4. Subí el código (`Ctrl+U` / `Cmd+U`).  
   Upload the code (`Ctrl+U` / `Cmd+U`).
5. Abrí el Monitor Serie a 115200 bps y anotá la IP que imprime.  
   Open the Serial Monitor at 115200 bps and note the printed IP.

## Configuración de Claude / Claude Configuration

El archivo `claude-config.json` contiene los hooks para que Claude controle el semáforo.  
The `claude-config.json` file has the hooks so Claude can control the traffic light.

1. Copiá su contenido en tu configuración de Claude Code.  
   Copy its contents into your Claude Code configuration.
2. **Cambiá la IP** (`192.168.1.10`) por la que te haya dado el router o la que hayas configurado como fija.  
   **Change the IP** (`192.168.1.10`) to the one your router assigned or the static one you configured.
3. Asegurate de que tu computadora y la ESP32 estén en la misma red.  
   Make sure your computer and the ESP32 are on the same network.

Endpoints del servidor web / Web server endpoints:

| Endpoint          | Método | Efecto / Effect                             |
|-------------------|--------|---------------------------------------------|
| `/alerta`         | POST   | 2 tandas de pitidos + amarillo titilando / 2 beep bursts + yellow blinking |
| `/solo/amarillo`  | POST   | Igual que `/alerta` / Same as `/alerta`     |
| `/solo/rojo`      | POST   | LED rojo fijo + pitido corto / Red solid + short beep |
| `/solo/verde`     | POST   | LED verde fijo + chime de "listo" / Green solid + "done" chime |
| `/off`            | POST   | Apaga todo / Turn everything off            |

## Conexión con Claude Code / Claude Code Integration

Claude usa `curl` para hablarle al semáforo en estos momentos:  
Claude uses `curl` to talk to the traffic light at these moments:

- `Notification` → `/alerta` (Claude te necesita / Claude needs you)
- `UserPromptSubmit` → `/solo/rojo` (Claude está pensando / Claude is thinking)
- `Stop` → `/solo/verde` (Claude terminó / Claude is done)

## Importante: IP fija libre / Important: free static IP

El código usa `192.168.1.10` por defecto. Antes de usarlo:  
The code uses `192.168.1.10` by default. Before using it:

1. Entrá a tu router. / Log into your router.
2. Verificá que `192.168.1.10` no esté asignada a otro dispositivo, o cambiala por una IP libre dentro de tu rango.  
   Check that `192.168.1.10` is not assigned to another device, or change it to a free IP in your range.
3. (Opcional pero recomendado) Reservá esa IP para la MAC de tu ESP32 en el DHCP del router.  
   (Optional but recommended) Reserve that IP for your ESP32's MAC in the router's DHCP.

## Modelos de impresión 3D / 3D Print Models

Próximamente en `/models`.  
Coming soon to `/models`.

## Licencia / License

MIT
