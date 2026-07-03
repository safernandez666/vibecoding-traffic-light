# Semaforo de Vibecoding 🚦

Un semáforo físico conectado a Claude Code.

- **Amarillo titilando + pitidos** → Claude necesita tu atención.
- **Rojo fijo** → Claude está pensando.
- **Verde fijo** → Claude terminó; seguí codeando.

<p align="center">
  <img src="docs/demo.gif" alt="Semaforo de Vibecoding en accion" width="640">
</p>

<p align="center">
  <a href="https://youtu.be/8MrP0tenx98">▶ Ver con audio</a>
</p>

---

## Hardware

- ESP32 DevKit
- LED rojo → GPIO 25
- LED amarillo → GPIO 27
- LED verde → GPIO 26
- Buzzer pasivo → GPIO 33
- Resistencias de 220 Ω para cada LED

## Instalación

1. Abrí `semaforo_server/semaforo_server.ino` en el IDE de Arduino.
2. Configurá tus credenciales de WiFi y una IP fija libre en tu router:

```cpp
const char* SSID     = "TU_RED";      // cambia por tu WiFi
const char* PASSWORD = "TU_PASS";     // cambia por tu password
IPAddress ip(192, 168, 1, 10);        // IP fija libre en tu red
```

3. Seleccioná la placa **ESP32 Dev Module** y el puerto serie correcto.
4. Subí el sketch (`Ctrl+U` / `Cmd+U`).
5. Abrí el Monitor Serie a 115200 baudios y anotá la IP que imprime.

## Configuración de Claude

El archivo `claude-config.json` contiene los hooks para que Claude controle el semáforo.

1. Copiá su contenido en tu configuración de Claude Code.
2. **Cambiá la IP** (`192.168.1.10`) por la que te haya dado el router o la que hayas configurado como fija.
3. Asegurate de que tu computadora y la ESP32 estén en la misma red.

Endpoints del servidor web:

| Endpoint          | Método | Efecto                                        |
|-------------------|--------|-----------------------------------------------|
| `/alerta`         | POST   | 2 tandas de pitidos + amarillo titilando 30 s |
| `/solo/amarillo`  | POST   | Igual que `/alerta`                           |
| `/solo/rojo`      | POST   | LED rojo fijo + pitido corto                  |
| `/solo/verde`     | POST   | LED verde fijo + chime de "listo"             |
| `/off`            | POST   | Apaga todo                                    |

## Integración con Claude Code

Claude usa `curl` para hablarle al semáforo en estos momentos:

- `Notification` → `/alerta` (Claude te necesita)
- `UserPromptSubmit` → `/solo/rojo` (Claude está pensando)
- `Stop` → `/solo/verde` (Claude terminó)

## Importante: usá una IP fija libre

El código usa `192.168.1.10` por defecto. Antes de usarlo:

1. Entrá a tu router.
2. Verificá que `192.168.1.10` no esté asignada a otro dispositivo, o cambiala por una IP libre dentro de tu rango.
3. (Opcional pero recomendado) Reservá esa IP para la MAC de tu ESP32 en el DHCP del router.

## Modelos de impresión 3D

Próximamente en `/models`.

## Licencia

MIT

---

Do you speak English? Find the English version at [`README.md`](README.md).
