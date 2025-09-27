#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>
#include <cstdlib>
#include <conio.h>
#include <thread>
using namespace std;

const char VIVO = 'x';
const char MUERTO = '.';

// cuenta los vecinos "vivos"
int contarVecinos(const vector<vector<char>> &tablero, int x, int y)
{

  // dimensiones de la tabla
  int filas = tablero.size();
  int columnas = tablero[0].size();

  // nuemro d vecinos vivos
  int vecinos = 0;

  // recorrer las 8 direcciones
  for (int dx = -1; dx <= 1; dx++)
  {
    for (int dy = -1; dy <= 1; dy++)
    {

      // si es donde se encuentra salta
      if (dx == 0 && dy == 0)
      {
        continue;
      }

      // coordenadas del vecino a comprobar
      int nx = x + dx;
      int ny = y + dy;

      // si se encuentra dentro del tablero
      if (nx >= 0 && nx < filas && ny >= 0 && ny < columnas)
      {

        // si esta vivo
        if (tablero[nx][ny] == VIVO)
          vecinos++;
      }
    }
  }

  // devolver los vecinos d esa celda
  return vecinos;
}

// muestra el tablero
void mostrar(const vector<vector<char>> &tablero)
{

  // limpiar pantalla
  // falta añadir compatibilidad para linux/mac
  system("cls");

  // fila a fila
  for (auto &fila : tablero)
  {
    for (auto celda : fila)
    {
      cout << celda;
    }
    cout << "\n";
  }
}

// contar celdas vivas
int contarVivas(const vector<vector<char>> &tablero)
{
  int vivas = 0;

  for (const auto &fila : tablero)
  {
    for (char c : fila)
    {
      if (c == VIVO)
      {
        vivas++;
      }
    }
  }
  return vivas;
}

// calcula la siguiente generacion
pair<int, int> siguienteGeneracion(vector<vector<char>> &tablero)
{
  // dimensiones del tablero
  int filas = tablero.size();
  int columnas = tablero[0].size();

  // contar muertas
  int muertasEnEstaIteracion = 0;
  int nacidasEnEstaIteracion = 0;

  // copia del tablero
  vector<vector<char>> copia = tablero;

  // recorrer cada celda
  for (int x = 0; x < filas; x++)
  {
    for (int y = 0; y < columnas; y++)
    {
      // contar vecinos de la celda
      int vecinos = contarVecinos(tablero, x, y);

      // esta vivo?
      if (tablero[x][y] == VIVO)
      {
        // muere por soledad o sobrepoblacion
        if (vecinos < 2 || vecinos > 3)
        {
          copia[x][y] = MUERTO;
          muertasEnEstaIteracion++;
        }
      }
      else
      {
        // nace por reproduccion
        if (vecinos == 3)
        {
          copia[x][y] = VIVO;
          nacidasEnEstaIteracion++;
        }
      }
    }
  }

  // actualizar el tablero
  tablero = copia;

  // devolvemos celdas muertas y vivas
  return {muertasEnEstaIteracion, nacidasEnEstaIteracion};
}

// lee el archivo inicial del tablero
vector<vector<char>> leerArchivo(const string &nombreArchivo)
{

  // abrir archivo
  ifstream archivo(nombreArchivo);
  vector<vector<char>> tablero;
  string linea;

  // comprobar si se ha abierto
  if (!archivo)
  {
    // mensaje d error
    cerr << "no se puede abrir el archivo" << nombreArchivo << "\n";
    exit(1);
  }

  bool primeraLinea = true;
  // leer linea a linea hasta acabar archivo
  while (getline(archivo, linea))
  {

    if (primeraLinea)
    {
      primeraLinea = false;
    }
    else
    {
      // comprobar que todas las lineas tienen la misma longitud
      if (linea.length() != tablero[0].size())
      {
        cerr << "todas las lineas del archivo deben tener la misma longitud\n";
        exit(1);
      }
    }

    // copiar linea al tablero
    vector<char> fila(linea.begin(), linea.end());
    tablero.push_back(fila);
  }

  // cerrar archivo
  archivo.close();
  return tablero;
}

int main()
{
  // leer el archivo de configuracion
  vector<vector<char>> tablero = leerArchivo("config.txt");
  int iteracion = 0;
  ofstream archivo("grafico.txt");

  while (true)
  {
    iteracion++;

    // mostrar tablero
    mostrar(tablero);
    // calcular siguiente generacion
    auto [muertasEnEstaIteracion, nacidasEnEstaIteracion] = siguienteGeneracion(tablero);

    // esperar un poco

    int vivasDespues = contarVivas(tablero);
    cout << "muertas: " << muertasEnEstaIteracion << "\n";
    cout << "nacidas: " << nacidasEnEstaIteracion << "\n";
    cout << "vivas ahora: " << vivasDespues << "\n";
    cout << "presiona \"space\" para acabar la simulacion \n";
    if (!archivo)
    {
      cerr << "no se ha podido abrir el archivo d grafico" << std::endl;
      exit(1);
    }

    for (int i = 0; i < vivasDespues; i++)
    {
      archivo << "x";
    }
    archivo << "\n";

    this_thread::sleep_for(chrono::milliseconds(50));

    // comprobar si se ha presionado una tecla
    if (_kbhit())
    {
      char tecla = _getch();
      // si se presiona espacio, salir del bucle
      if (tecla == ' ')
      {
        break;
      }
    }
  }

  // guardar grafico
  archivo.close();

  cout << "simulacion detenida por usuario\n";

  return 0;
}
