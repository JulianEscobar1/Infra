# Proyecto Final: Motores paralelos de Mancala (Kalah) y despliegue en Kubernetes

Este repositorio contiene la implementación del proyecto final de la asignatura Infraestructuras Paralelas y Distribuidas. Consiste en un motor de inteligencia artificial en C++ (Minimax con Poda Alfa-Beta y OpenMP), una API REST en Python (FastAPI) y un Frontend en React (Nginx), orquestados a través de Docker y Kubernetes.

## 👥 Integrantes del Grupo

*(Rellenar con los datos reales antes de la entrega final)*

| Nombre Completo | Código Estudiantil | Correo Institucional |
| :--- | :--- | :--- |
| [Ricardo Zuluaga Pulgarín] | [2160051-3743] | [ricardo.zuluaga@correounivalle.edu.co] |
| [Julián Andrés Escobar Tangarife] | [2160362-3743] | [escobar.julian@correounivalle.edu.co] |

📄 **Enlace al informe técnico detallado:** [Ver Documentación en la carpeta /docs](docs/README.md)

---

## 🚀 Guía de Despliegue Local

El proyecto está diseñado bajo una arquitectura de microservicios estricta. Existen tres maneras principales de construir y ejecutar la aplicación en tu máquina local.

### Opción 1: Docker Compose (Recomendada)
Esta es la manera más rápida de levantar toda la infraestructura conectada automáticamente.

1. Abre una terminal en la raíz del proyecto.
2. Ejecuta el comando para construir las imágenes y levantar los servicios en segundo plano:
   ```bash
   docker compose up --build -d
   ```
3. Verifica que los tres contenedores estén corriendo (`mancala-motor`, `mancala-backend`, `mancala-frontend`).
4. Abre tu navegador y entra a: **http://localhost:8080**
5. Para detener y limpiar el entorno:
   ```bash
   docker compose down
   ```

### Opción 2: Kubernetes Local (Minikube o Docker Desktop)
Para simular el entorno de orquestación tolerante a fallos con réplicas (3 réplicas del backend).

1. Asegúrate de tener Kubernetes activado (ej. Docker Desktop Kubernetes) y asegúrate de **haber detenido** el Docker Compose (`docker compose down`) para liberar los puertos.
2. Construye las imágenes localmente primero para que Kubernetes pueda usarlas:
   ```bash
   docker compose build
   ```
3. Aplica todos los manifiestos YAML:
   ```bash
   kubectl apply -f deploy/local/
   ```
4. Verifica que todos los Pods (5 en total) estén en estado `Running`:
   ```bash
   kubectl get pods
   ```
5. Abre tu navegador y entra a: **http://localhost:8080**
6. Para apagar y eliminar los recursos de Kubernetes:
   ```bash
   kubectl delete -f deploy/local/
   ```

### Opción 3: Contenedores Individuales (Docker puro)
Si deseas construir y aislar manualmente cada imagen sin orquestadores automáticos:

**1. Construir las imágenes:**
```bash
docker build -t mancala-motor ./motor
docker build -t mancala-motor-backend ./backend
docker build -t mancala-frontend ./frontend
```

**2. Crear una red para que se comuniquen:**
```bash
docker network create mancala-net
```

**3. Levantar cada contenedor en la misma red:**
```bash
# 1. Motor (Puerto interno 8080)
docker run -d --name motor --net mancala-net mancala-motor

# 2. Backend (Expuesto en el puerto 8000)
docker run -d --name motor-backend --net mancala-net -p 8000:8000 -e ENGINE_URL="http://motor:8080" mancala-motor-backend

# 3. Frontend (Expuesto en el puerto 8080)
docker run -d --name frontend --net mancala-net -p 8080:80 mancala-frontend
```
4. Abre tu navegador en **http://localhost:8080**.
