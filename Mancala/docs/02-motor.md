# 02 - Motor de Búsqueda (C++)

## Reglas de Kalah(6,4) Implementadas
*(Describir cómo el tablero de 14 espacios procesa el movimiento antihorario, salta el Kalaha rival y gestiona el turno extra/capturas)*.

## Función de Evaluación Heurística
La evaluación de la hoja del árbol Minimax se define matemáticamente como:

$$h(estado) = (kalaha \_ propio - kalaha \_ rival) + \alpha \cdot (semillas \_ lado \_ propio - semillas \_ lado \_ rival)$$

*(Mencione aquí el valor de $\alpha \in [0, 1]$ que eligió su grupo y el motivo de esa ponderación)*.

## Pseudocódigo: Minimax con Poda Alfa-Beta
```text
// Inserte aquí el pseudocódigo algorítmico básico de cómo implementaron Alfa-Beta en C++
```

## Pruebas de Corrección
*(Adjunte pruebas o discuta cómo verificaron que el árbol podado por Alfa-Beta produzca matemáticamente el mismo movimiento que un recorrido Minimax puro en las mismas posiciones de prueba)*.
