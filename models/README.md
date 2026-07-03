# Semáforo de estado para Claude Code (ESP32)

Semáforo físico (3 LEDs + buzzer) que reacciona a la terminal de Claude Code:
se controla por HTTP y los hooks de Claude le disparan los cambios.

- **Rojo** = Claude está trabajando
- **Amarillo** = Claude pide tu atención (permiso / idle) → titila + 2 tandas de buzz, 30 s
- **Verde** = Claude terminó → sonido largo

---

## Hardware

- Placa: **NodeMCU ESP32 (ESP-32S / WROOM-32)**, USB-C, chip USB-serie Silicon Labs CP2102.
- Puerto serie en la Mac: `/dev/cu.usbserial-0001` · 115200 baud.
- En Arduino IDE: placa **"ESP32 Dev Module"**.

### Pines

| Componente | GPIO |
|---|---|
| LED rojo | 25 |
| LED amarillo | 27 |
| LED verde | 26 |
| Buzzer (pasivo) | 33 |

Evitar como salida: 34/35/36/39 (solo entrada), 6–11 (flash), 0/2/5/12/15 (strapping).

### Cableado

- Cada LED: pata larga (+) → resistencia 220 Ω → su GPIO · pata corta (–) → GND.
- Buzzer: pata larga (+) → GPIO33 · pata corta (–) → GND. (Pasivo, se maneja con `tone()`.)
- Alimentación: USB-C 5V.

---

## Software (firmware)

Archivo: `semaforo_server.ino` (Arduino / ESP32). IP fija **192.168.1.10**.

### Endpoints (POST)

| Endpoint | Efecto |
|---|---|
| `/solo/rojo` | rojo (apaga el resto) + beep |
| `/solo/amarillo` o `/alerta` | ALERTA: amarillo titila + 2 tandas de buzz, 30 s |
| `/solo/verde` | verde + sonido largo (Claude terminó) |
| `/off` | apaga todo y corta la alerta |

Prueba rápida (Python):

```python
import requests
requests.post("http://192.168.1.10/alerta")
requests.post("http://192.168.1.10/solo/verde")
requests.post("http://192.168.1.10/off")
```

### Parámetros ajustables (arriba del .ino)

`ALERTA_MS` (30 s) · `BLINK_MS` (velocidad titileo) · `TANDAS` / `PITIDOS` (buzz inicial).

---

## Hooks de Claude Code

Archivo: `claude_hooks.json`. Va dentro de `~/.claude/settings.json`
(clave `hooks` de nivel superior; si ya hay `permissions`, agregar al lado, no pisar).

```json
{
  "hooks": {
    "Notification":     [ { "hooks": [ { "type": "command", "command": "curl -s --max-time 2 -X POST http://192.168.1.10/alerta" } ] } ],
    "Stop":             [ { "hooks": [ { "type": "command", "command": "curl -s --max-time 2 -X POST http://192.168.1.10/solo/verde" } ] } ],
    "UserPromptSubmit": [ { "hooks": [ { "type": "command", "command": "curl -s --max-time 2 -X POST http://192.168.1.10/solo/rojo" } ] } ]
  }
}
```

Después: reiniciar Claude Code · verificar con `/hooks` · validar JSON con
`python3 -m json.tool ~/.claude/settings.json`.

**Seguridad (pendiente opcional):** el endpoint hoy es abierto en la LAN.
Se puede pasar a hook tipo `http` con header `X-Token` y validar el token en el firmware.

---

## Caja 3D

- `caja_base.stl` — cuerpo (imprimir).
- `caja_tapa.stl` — tapa trasera.
- `caja_completa.scad` — fuente paramétrico.

Contiene: 3 agujeros para los LEDs del módulo + rejilla del buzzer al frente,
clip para el borde del monitor arriba, bahía para la ESP con ranura USB-C atrás,
y cámara interna para los cables soldados.

### Medidas a verificar antes de imprimir (en el .scad)

- `led_pitch` / `led_d` — separación y diámetro de los LEDs del módulo.
- `mon_thick` — espesor del borde del monitor (para el clip).
- `usb_y` / `usb_w` — posición/ancho de la ranura USB-C según cómo apoyes la ESP.

Reexportar: `openscad -D 'part="base"' -o caja_base.stl caja_completa.scad`
(y `part="tapa"` para la tapa).

### Renders (JPG)

`caja_v1.jpg`, `caja_v2.jpg` (vistas), `caja_cables.jpg`, `caja_back.jpg` (interior / bahía ESP).
