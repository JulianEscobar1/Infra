# 07 - Análisis Comparativo Local vs. Nube

## Diseño del Experimento
Se utilizó una herramienta sintética (*wrk* o *k6*) enviando múltiples peticiones recurrentes con profundidad fija.

## Resultados Cuantitativos

### 1. Entorno Local (Escalado Vertical - Variando Hilos)
*(Se varía el parámetro `OMP_NUM_THREADS` usando 1 sola instancia del backend)*.

| Configuración (Hilos) | Latencia p50 | Latencia p95 | Throughput (Req/sec) |
| --------------------- | ------------ | ------------ | -------------------- |
| 1 Hilo                |              |              |                      |
| 2 Hilos               |              |              |                      |
| 4 Hilos               |              |              |                      |
| 8 Hilos               |              |              |                      |

### 2. Entorno Nube (Escalado Horizontal - Variando Réplicas)
*(Se fija el parámetro de hilos del motor, p.ej. en 2, y se varía el número de pods de backend en el clúster de K8s)*.

| Configuración (Pods)  | Latencia p50 | Latencia p95 | Throughput (Req/sec) |
| --------------------- | ------------ | ------------ | -------------------- |
| 1 Réplica             |              |              |                      |
| 3 Réplicas            |              |              |                      |

## Conclusión Cualitativa: Escalado Vertical vs. Horizontal
*(Cierre redactando de forma cualitativa en 2-3 frases cuándo conviene usar más hilos por pod y cuándo conviene tener más réplicas bajo un LoadBalancer, anclado estrictamente a los números de las tablas de arriba).*
