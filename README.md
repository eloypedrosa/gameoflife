# Game Of Life - C++

Simulación del **Conway's Game of life** en C++.

## How to

1. Crear un archivo `config.txt` con el tablero inicial (`x` = vivo, `.` = muerto).
2. Compilar:

```bash
g++ -o game_of_life game_of_life.cpp
```

3. Ejecutar:

```bash
./game_of_life
```

4. Presiona `space` para detener la simulación.

## Qué hace

- Muestra el tablero en consola.

- Calcula la siguiente generación según las reglas de Conway.

- Guarda un registro de celdas vivas en grafico.txt.

