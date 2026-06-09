# Mancala Backend API Facade

Este es el componente **backend/ HTTP Facade** para el motor de Mancala (Kalah 6,4). Actúa como una pasarela HTTP intermedia que se comunica de forma asíncrona con el motor C++/OpenMP, además de validar y asegurar las solicitudes entrantes.

## Tecnologías Utilizadas

* **Python 3.12**
* **FastAPI**: Servidor web ágil e interactivo.
* **Pydantic**: Validaciones estrictas de datos en tiempo de ejecución.
* **HTTPX**: Llamadas asíncronas no bloqueantes al motor.
* **Uvicorn**: Servidor ASGI de alto rendimiento.
* **Pytest**: Entorno de testing unitario.
* **Prometheus Client**: Exposición de telemetría de rendimiento.

## Configuración y Variables de Entorno

Puedes modificar el comportamiento del backend definiendo las siguientes variables en tu entorno:

| Variable | Descripción | Valor por Defecto |
|----------|-------------|-------------------|
| `ENGINE_URL` | Dirección base del motor C++/OpenMP | `http://localhost:8080` |
| `ALLOWED_ORIGINS` | Orígenes permitidos por CORS (separados por comas) | `http://localhost:3000,http://127.0.0.1:3000` |
| `PORT` | Puerto en el que escucha el backend | `8000` |
| `HOST` | Interfaz de red de escucha | `0.0.0.0` |

---

## Ejecución en Local

### 1. Requisitos
Asegúrate de tener instalado Python 3.12.

### 2. Crear entorno virtual e instalar dependencias
```bash
python -m venv venv
source venv/bin/activate  # En Windows: venv\Scripts\activate
pip install -r requirements.txt
```

### 3. Ejecutar el servidor en desarrollo
```bash
uvicorn app.main:app --reload --host 0.0.0.0 --port 8000
```

### 4. Ejecutar las pruebas
```bash
pytest
```

---

## Estructura de Endpoints expuestos

* **`POST /move`**:
  * Envía el estado del tablero (`board`), el lado que juega (`side`), la profundidad de búsqueda (`depth`) y los hilos OpenMP (`threads`).
  * Valida que el tablero tenga exactamente 14 posiciones, y los enteros de configuración sean correctos (`depth > 0`, `threads > 0`, `side` en "south"/"north").
* **`GET /healthz`**: Liveness probe para orquestadores (retorna 200 OK).
* **`GET /readyz`**: Readiness probe. Retorna 200 OK únicamente si el motor de C++ responde. Si no, devuelve HTTP 503.
* **`GET /metrics`**: Expone métricas Prometheus de peticiones, latencias y errores de red.
