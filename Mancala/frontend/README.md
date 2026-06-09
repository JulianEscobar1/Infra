# Mancala AI Frontend Console

Esta es la consola interactiva **frontend** para interactuar con el motor del juego Mancala (Kalah 6,4). Permite a los usuarios visualizar gráficamente el tablero, modificar las semillas de las fosas, seleccionar el lado activo y los parámetros de búsqueda, y visualizar en tiempo real las estadísticas de cómputo.

## Características

* **Tablero Interactivo**: Modificación dinámica de la cantidad de semillas en cada hoyo pulsando en ellos o mediante controles individuales (+/-).
* **Parámetros del Algoritmo**: Configuración de profundidad de búsqueda (`depth`), número de hilos OpenMP (`threads`) y el lado actual a maximizar (`south`/`north`).
* **Visualización del Movimiento Óptimo**: El hoyo que representa el mejor movimiento se resalta automáticamente en verde y parpadea de forma fluida.
* **Telemetría en Tiempo Real**: Exposición directa de las estadísticas de cómputo del motor (`elapsed_ms`, `nodes`, `prunes`).
* **Gestión de Conectividad**: Monitor de disponibilidad permanente del backend mediante sondeo del endpoint `/readyz`.

## Tecnologías Utilizadas

* **React 18**
* **Vite**
* **TypeScript**
* **CSS Puro** (Diseño premium con soporte responsivo y dark-mode)

## Guía de Desarrollo Local

### 1. Requisitos
Asegúrate de tener instalado [Node.js](https://nodejs.org/) (versión 18 o superior).

### 2. Instalar dependencias
```bash
npm install
```

### 3. Ejecutar en modo desarrollo
```bash
npm run dev
```
*El servidor de desarrollo correrá por defecto en `http://localhost:3000`. Cualquier solicitud a `/api` se redirigirá automáticamente a `http://localhost:8000` (el backend FastAPI) mediante la configuración de proxy de Vite.*

### 4. Compilar para producción
```bash
npm run build
```
Los archivos optimizados se generarán en la carpeta `dist/`.
