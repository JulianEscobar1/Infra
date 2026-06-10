# 03 - Paralelización y Profiling

## Estrategia OpenMP Elegida
*(Explique aquí la estrategia utilizada: Root Parallelism, YBWC, o PVS. Discuta la sobrecarga de sincronización de las cotas $\alpha$ y $\beta$ entre hilos).*

## Métrica de Benchmarks Locales
A continuación, los resultados de la instrumentación del motor local, ejecutando el benchmark:

### Resultados (Profundidad = 8)
| Hilos ($p$) | Tiempo ($T_p$) | Speedup ($S_p$) | Eficiencia ($E_p$) | Nodos Explorados | Podas Efectuadas |
| ----------- | -------------- | --------------- | ------------------ | ---------------- | ---------------- |
| 1           |                | 1.0x            | 100%               |                  |                  |
| 2           |                |                 |                    |                  |                  |
| 4           |                |                 |                    |                  |                  |
| 8           |                |                 |                    |                  |                  |

*(Añada más tablas si lo considera necesario para profundidades mayores como la requerida depth=12).*

## Evidencia de Profiling
*(Pegue aquí las capturas de pantalla tomadas ejecutando el benchmark local usando las herramientas exigidas: htop, perf o /usr/bin/time).*

![Captura Profiling](ruta_a_la_imagen.jpg)

*(Breve discusión de los datos experimentales capturados)*.
