#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <set>
#include <list>
#include <cmath>
#include <utility>

#include <chrono>

using namespace std;

const int SIZE = 7;
const char EMPTY = 'e';
const char PLAYER = 'o';
const char INVALID = 'x';
bool impreso = false;

enum class Action;

struct Position
{
    int x;
    int y;
    Position(int x, int y) : x(x), y(y) {}

    bool operator<(const Position &p) const
    {
        return (x < p.x) || (x == p.x && y < p.y);
    }
};

enum class Action
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

std::ostream &operator<<(std::ostream &os, const Action &action)
{
    switch (action)
    {
    case Action::UP:
        os << "Arriba";
        break;
    case Action::DOWN:
        os << "Abajo";
        break;
    case Action::LEFT:
        os << "Izquierda";
        break;
    case Action::RIGHT:
        os << "Derecha";
        break;
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const Position &position)
{
    os << "(" << position.x << ", " << position.y << ")";
    return os;
}

std::ostream &operator<<(std::ostream &os, const std::pair<Action, Position> &pair)
{
    os << "[" << pair.first << ", " << pair.second << "]";
    return os;
}

bool isValidMove(vector<vector<char>> &board, const Position &pos, const Action &a)
{ // FUNCIÓN FACTIBILIDAD
    bool valid = false;
    if (board[pos.x][pos.y] == PLAYER) // SI LA POSICIÓN ACTUAL ES UNA FICHA
    {
        switch (a)
        {
        case Action::UP:
            if (pos.x - 2 >= 0 && pos.x - 2 != INVALID)
            { // SIGO EN EL TABLERO TRAS LA ACCIÓN
                if (board[pos.x - 1][pos.y] == PLAYER && board[pos.x - 2][pos.y] == EMPTY)
                { // HAY UNA FICHO EN EL SIGUIENTE MOVIMIENTO, Y DESPUÉS HAY UN ESPACIO VACÍO
                    valid = true;
                }
            }
            break;
        case Action::DOWN:
            if (pos.x + 2 < SIZE && pos.x + 2 != INVALID)
            { // SIGO EN EL TABLERO TRAS LA ACCIÓN
                if (board[pos.x + 1][pos.y] == PLAYER && board[pos.x + 2][pos.y] == EMPTY)
                { // HAY UNA FICHO EN EL SIGUIENTE MOVIMIENTO, Y DESPUÉS HAY UN ESPACIO VACÍO
                    valid = true;
                }
            }
            break;
        case Action::LEFT:
            if (pos.y - 2 >= 0 && pos.y - 2 != INVALID)
            { // SIGO EN EL TABLERO TRAS LA ACCIÓN
                if (board[pos.x][pos.y - 1] == PLAYER && board[pos.x][pos.y - 2] == EMPTY)
                { // HAY UNA FICHO EN EL SIGUIENTE MOVIMIENTO, Y DESPUÉS HAY UN ESPACIO VACÍO
                    valid = true;
                }
            }
            break;
        case Action::RIGHT:
            if (pos.y + 2 < SIZE && pos.y + 2 != INVALID)
            { // SIGO EN EL TABLERO TRAS LA ACCIÓN
                if (board[pos.x][pos.y + 1] == PLAYER && board[pos.x][pos.y + 2] == EMPTY)
                { // HAY UNA FICHO EN EL SIGUIENTE MOVIMIENTO, Y DESPUÉS HAY UN ESPACIO VACÍO
                    valid = true;
                }
            }
            break;
        }
    }
    return valid;
}

void executeMove(vector<vector<char>> &board, Position &pos, const Action &a)
{ // FUNCIÓN EJECUCIÓN
    switch (a)
    {
    case Action::UP:
        board[pos.x][pos.y] = EMPTY;
        board[pos.x - 1][pos.y] = EMPTY;
        board[pos.x - 2][pos.y] = PLAYER;
        pos.x = pos.x - 2;
        break;
    case Action::DOWN:
        board[pos.x][pos.y] = EMPTY;
        board[pos.x + 1][pos.y] = EMPTY;
        board[pos.x + 2][pos.y] = PLAYER;
        pos.x = pos.x + 2;
        break;
    case Action::LEFT:
        board[pos.x][pos.y] = EMPTY;
        board[pos.x][pos.y - 1] = EMPTY;
        board[pos.x][pos.y - 2] = PLAYER;
        pos.y = pos.y - 2;
        break;
    case Action::RIGHT:
        board[pos.x][pos.y] = EMPTY;
        board[pos.x][pos.y + 1] = EMPTY;
        board[pos.x][pos.y + 2] = PLAYER;
        pos.y = pos.y + 2;
        break;
    }
}

void rollBack(vector<vector<char>> &board, Position &pos, const Action &a)
{ // FUNCIÓN RETROCESO
    switch (a)
    {
    case Action::UP:
        board[pos.x][pos.y] = PLAYER;
        board[pos.x - 1][pos.y] = PLAYER;
        board[pos.x - 2][pos.y] = EMPTY;
        pos.x = pos.x - 2;
        break;
    case Action::DOWN:
        board[pos.x][pos.y] = PLAYER;
        board[pos.x + 1][pos.y] = PLAYER;
        board[pos.x + 2][pos.y] = EMPTY;
        pos.x = pos.x + 2;
        break;
    case Action::LEFT:
        board[pos.x][pos.y] = PLAYER;
        board[pos.x][pos.y - 1] = PLAYER;
        board[pos.x][pos.y - 2] = EMPTY;
        pos.y = pos.y - 2;
        break;
    case Action::RIGHT:
        board[pos.x][pos.y] = PLAYER;
        board[pos.x][pos.y + 1] = PLAYER;
        board[pos.x][pos.y + 2] = EMPTY;
        pos.y = pos.y + 2;
        break;
    }
}

void imprimeTablero(vector<vector<char>> &board)
{ // FUNCIÓN IMPRESIÓN
    for (int i = 0; i < SIZE; i++)
    {
        cout << " ";
        for (int j = 0; j < SIZE; j++)
        {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

int cuentaFichas(vector<vector<char>> &board)
{
    int contador = 0;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (board[i][j] == PLAYER)
            {
                contador++;
            }
        }
    }
    return contador;
}

void imprimePlan(vector<pair<Action, Position>> &decisiones)
{ // FUNCIÓN IMPRESIÓN
    for (int i = 0; i < decisiones.size(); i++)
    {
        cout << "Movimiento " << i + 1 << ": ";
        switch (decisiones[i].first)
        {
        case Action::UP:
            cout << "UP";
            break;
        case Action::DOWN:
            cout << "DOWN";
            break;
        case Action::LEFT:
            cout << "LEFT";
            break;
        case Action::RIGHT:
            cout << "RIGHT";
            break;
        }
        cout << " desde [" << decisiones[i].second.x << "," << decisiones[i].second.y << "]" << endl;
    }
}

bool backTrackingSolitaire(vector<pair<Action, Position>> &decisiones, vector<vector<char>> &board, int contador)
{
    // Caso base: se ha alcanzado la condición de éxito (quedó una sola pieza en el centro)

    // Recorremos el tablero
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (board[i][j] == PLAYER)
            {
                // Recorremos las direcciones
                Position current = {i, j};
                for (Action direccion : {Action::UP, Action::DOWN, Action::LEFT, Action::RIGHT})
                {

                    // Si el movimiento es válido
                    if (isValidMove(board, current, direccion))
                    {
                        // Realizar el movimiento
                        Position old = current;
                        executeMove(board, current, direccion);

                        // imprimeTablero(board);

                        // Agregar la acción y la posición al vector de decisiones
                        decisiones.push_back(make_pair(direccion, old));
                        // cout << "Decisiones: " << decisiones.size() << endl;
                        // cout << "Movemos la casilla [" << old.x << "][" << old.y << "] en la dirección " << direccion << endl;

                        // Llamada recursiva
                        if (backTrackingSolitaire(decisiones, board, contador))
                            return true;

                        // Retroceso
                        rollBack(board, old, direccion);
                        decisiones.pop_back();
                    }
                }
            }
            if (decisiones.size() == 31 && board[SIZE / 2][SIZE / 2] == PLAYER && impreso == false)
            {
            
                impreso = true;
                return true; // Éxito
            }
            else if (decisiones.size() > 31)
                return false;
        }
    }
}

int main()
{

    vector<vector<char>> board(SIZE); // TABLERO DE JUEGO (7x7)
    for (int i = 0; i < SIZE; i++)
    {
        board[i].resize(SIZE);
    }

    for (int i = 0; i < SIZE; i++)
    { // RELLENO EL TABLERO
        for (int j = 0; j < SIZE; j++)
        {
            board[i][j] = PLAYER;
        }
    }

    board[3][3] = EMPTY; // POSICIÓN CENTRAL VACÍA

    board[0][0] = INVALID; // POSICIONES INVÁLIDAS
    board[0][1] = INVALID;
    board[1][0] = INVALID;
    board[1][1] = INVALID;

    board[0][5] = INVALID;
    board[0][6] = INVALID;
    board[1][5] = INVALID;
    board[1][6] = INVALID;

    board[5][0] = INVALID;
    board[5][1] = INVALID;
    board[6][0] = INVALID;
    board[6][1] = INVALID;

    board[5][5] = INVALID;
    board[5][6] = INVALID;
    board[6][5] = INVALID;
    board[6][6] = INVALID;

    for (int i = 0; i < SIZE; i++)
    { // IMPRIMO EL TABLERO
        for (int j = 0; j < SIZE; j++)
        {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }

    cout << endl;

    set<Position> positions;

    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            if (board[i][j] == 'o')
            {
                Position pos(i, j);
                positions.insert(pos);
            }
        }
    }

    vector<pair<Action, Position>> decisiones;

    bool solucion;
    //variables para mediciín de tiempo
    clock_t start, end;
    double cpu_time_used;

    do
    {
        start = clock();
        solucion = backTrackingSolitaire(decisiones, board, 31);
        end = clock();
    } while (!solucion);

    if (!solucion)
    {
        cout << "No se encontró solución" << endl;
    }
    else
    {
        for (pair<Action, Position> decision : decisiones)
        {
            cout << decision.first << " " << decision.second << endl;
        }
    }

    cout << endl;

    for (int i = 0; i < SIZE; i++)
    { // IMPRIMO EL TABLERO
        for (int j = 0; j < SIZE; j++)
        {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }

    cout << "Tiempo de ejecución: " << (double)(end - start) / CLOCKS_PER_SEC << " segundos" << endl;

    return 0;
}
