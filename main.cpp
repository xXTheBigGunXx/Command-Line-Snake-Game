#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h>
#include <vector>
#include <windows.h>

using namespace std;

const int  length = 30;
const int width = 40;
const int time_miliseconds = 100;

const int number_of_cherries = 10;

void Sleep_win(int miliseconds);

void Grid_creation(char Grid[][width], vector<vector<int>>& cherrie_coordinations);

inline void Print(char Grid[][width]);

void Direction(int& x_direct, int& y_direct);

void Create_and_place_cherries(char Grid[][width], vector<vector<int>>& cherrie_coordinations, vector<vector<int>> Array_of_position);

bool Game_over(char Grid[][width], vector<vector<int>> Array_of_position, int x_direct, int y_direct);

struct Snakes_body
{
    char Grid[length][width];

    int x_start = width / 2;
    int y_start = length / 2;

    int snakes_length = 1;

    vector<vector<int>> Array_of_position;
    vector<int> temp;

    bool is_cherrie_hit = false;

    void Move_snake(int x_direct, int y_direct, char Grid[][width], vector<vector<int>>& cherrie_coordinations)
    {
        temp.push_back(Array_of_position[0][0] + y_direct);
        temp.push_back(Array_of_position[0][1] + x_direct);

        Array_of_position.insert(Array_of_position.begin(), temp);
        temp.clear();

        for (int i = 0; i < Array_of_position.size() - 1; i++)
        {
            swap(Grid[Array_of_position[i][0]][Array_of_position[i][1]], Grid[Array_of_position[i + 1][0]][Array_of_position[i + 1][1]]);
        }

        if (Grid[Array_of_position[Array_of_position.size() - 1][0]][Array_of_position[Array_of_position.size() - 1][1]] == 'X')
        {
            Grid[Array_of_position[Array_of_position.size() - 1][0]][Array_of_position[Array_of_position.size() - 1][1]] = '0';

            for (int i = 0; i < number_of_cherries; i++)
            {
                if (cherrie_coordinations[i][0] == Array_of_position[0][0] && cherrie_coordinations[i][1] == Array_of_position[0][1])
                {
                    cherrie_coordinations[i].clear();
                    break;
                }
            }

            Create_and_place_cherries(Grid, cherrie_coordinations, Array_of_position);
        }
        else
        {
            Array_of_position.pop_back();
        }
    }
};

int main()
{
    srand(time(0));
    vector<vector<int>> cherrie_coordinations;

    Snakes_body Snake;
    Grid_creation(Snake.Grid, cherrie_coordinations);

    Create_and_place_cherries(Snake.Grid, cherrie_coordinations, Snake.Array_of_position);

    int x_direct = 0;
    int y_direct = 0;

    Snake.temp.push_back(Snake.y_start);
    Snake.temp.push_back(Snake.x_start);
    Snake.Array_of_position.push_back(Snake.temp);
    Snake.Grid[Snake.y_start][Snake.x_start] = '0';
    Snake.temp.clear();

    bool Game_state = true;

    cout << "\033[?25l";

    while (Game_state == true)
    {
        Direction(x_direct, y_direct);

        Snake.Move_snake(x_direct, y_direct, Snake.Grid, cherrie_coordinations);

        Sleep_win(time_miliseconds);
        Print(Snake.Grid);

        Game_state = Game_over(Snake.Grid, Snake.Array_of_position, x_direct, y_direct);
    }
    system("pause");
    return 0;
}

void Sleep_win(int milisecond)
{
    //this_thread::sleep_for(chrono::milliseconds(milisecond));
    Sleep(milisecond);
}

inline void Grid_creation(char Grid[][width], vector<vector<int>>& cherrie_coordinations)
{
    for (int i = 0; i < length; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Grid[i][j] = '.';
        }
    }

    for (int i = 0; i < number_of_cherries; i++)
    {
        vector<int>temp;
        cherrie_coordinations.push_back(temp);
    }
}

void Print(char Grid[][width])
{
    cout << "\033[H";
    //cout << "\033[?25h";
    for (int i = 0; i < width * 2 + 1; i++)
    {
        cout << '-';
    }
    cout << "\n";

    for (int i = 0; i < length; i++)
    {
        cout << '|';
        for (int j = 0; j < width - 1; j++)
        {
            cout << Grid[i][j] << " ";
        }
        cout << Grid[i][width - 1] << '|' << "\n";
    }

    for (int i = 0; i < width * 2 + 1; i++)
    {
        cout << '-';
    }
    cout << "\n";
}

void Direction(int& x_direct, int& y_direct)
{
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'w':x_direct = 0; y_direct = -1; break;
        case 'd':x_direct = 1; y_direct = 0; break;
        case 's':x_direct = 0; y_direct = 1; break;
        case 'a':x_direct = -1; y_direct = 0; break;
        }
    }
}

void Create_and_place_cherries(char Grid[][width], vector<vector<int>>& cherrie_coordinations, vector<vector<int>> Array_of_position)
{
    bool temp_grid[length][width] = { 0 };

    for (const auto& vec : Array_of_position)
    {
        temp_grid[vec[0]][vec[1]] = true;
    }

    for (auto& vec : cherrie_coordinations)
    {
        if (vec.empty())
        {
            bool is_not_touching = true;

            while (is_not_touching)
            {
                int x = rand() % width;
                int y = rand() % length;

                if (!temp_grid[y][x])
                {
                    vec.push_back(y);
                    vec.push_back(x);
                    Grid[y][x] = 'X';
                    is_not_touching = false;
                }
            }
        }
    }
}

bool Game_over(char Grid[][width], vector<vector<int>> Array_of_position, int x_direct, int y_direct)
{
    if (Array_of_position[0][0] + y_direct < -1 || Array_of_position[0][1] + x_direct < -1)
    {
        return false;
    }
    else if (Array_of_position[0][0] + y_direct > length || Array_of_position[0][1] + x_direct > width)
    {
        return false;
    }

    for (int i = 1; i < Array_of_position.size(); i++)
    {
        if (Array_of_position[0][0] == Array_of_position[i][0] && Array_of_position[0][1] == Array_of_position[i][1])
        {
            return false;
        }
    }

    return true;
}
