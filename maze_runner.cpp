/*
	Universidade Federal de Minas Gerais
	Exercício Computacional 2 - Maze Runner
	Aluno: Bernardo de Souza Silva
	Matrícula: 2020084290
	Disciplina: Automação em Tempo Real

	Para rodar: ./maze_runner ../data/maze.txt
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <thread>
#include <chrono>

using namespace std;

// Matriz de char representando o labirinto
vector<vector<char>> maze;

// Número de linhas e colunas do labirinto
int num_rows;
int num_cols;

// Variável global de saída encontrada
bool exit_found = false;

// Representação de uma posição
struct pos_t {
    int i;
    int j;
};

// Função que lê o labirinto de um arquivo texto, carrega em memória e retorna a posição inicial
pos_t load_maze(const char* file_name) {
    pos_t initial_pos;
    // Abre o arquivo para leitura
    ifstream file(file_name);

    // Lê o numero de linhas e colunas e salva em num_rows e num_cols
    file >> num_rows >> num_cols;

    // Inicializa a matriz maze com '.' (pontos)
    maze.resize(num_rows, vector<char>(num_cols, '.'));

    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            char character;
            // Lê o caractere da posição i, j do arquivo
            file >> character;

            // Salva o caractere na matriz maze
            maze[i][j] = character;

            // Verifica se o caractere é 'e' (entrada)
            if (character == 'e') {
                initial_pos = {i, j};
            }
        }
    }
    return initial_pos;
}

// Função que imprime o labirinto
void print_maze() {
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            if (maze[i][j] == 'o') {
                // Impressão do caractere 'o' em vermelho
                cout << "\033[1;31mo\033[0m";
            } else if (maze[i][j] == '.') {
                // Impressão do caractere '.' em vermelho
                cout << "\033[1;31m.\033[0m";
            } else {
                cout << maze[i][j];
            }
        }
        cout << endl;
    }
    // Delay de 50 ms em cada passo
    this_thread::sleep_for(chrono::milliseconds(50));
}

// Função responsável pela navegação
void walk(pos_t pos) {
    pos_t next_pos;
    stack<pos_t> valid_positions;
    valid_positions.push(pos);

    while (!valid_positions.empty()) {        
        valid_positions.pop();
        maze[pos.i][pos.j] = 'o';

        // Delay de 50 ms para melhor visualização
        this_thread::sleep_for(chrono::milliseconds(50));

        maze[pos.i][pos.j] = '.';

        // Verifica se a saída foi encontrada à esquerda
        if (maze[pos.i][pos.j - 1] == 's' && pos.j - 1 >= 0) {
            print_maze();
            exit_found = true;
        } else if (maze[pos.i][pos.j - 1] == 'x' && pos.j - 1 >= 0) {
            next_pos = {pos.i, pos.j - 1};
            valid_positions.push(next_pos);
        }

        // Verifica se a saída foi encontrada à direita
        if (maze[pos.i][pos.j + 1] == 's' && pos.j + 1 < num_cols) {
            print_maze();
            exit_found = true;
        } else if (maze[pos.i][pos.j + 1] == 'x' && pos.j + 1 < num_cols) {
            next_pos = {pos.i, pos.j + 1};
            valid_positions.push(next_pos);
        }

        // Verifica se a saída foi encontrada abaixo
        if (pos.i + 1 < num_rows) {
            if (maze[pos.i + 1][pos.j] == 's') {
                print_maze();
                exit_found = true;
            } else if (maze[pos.i + 1][pos.j] == 'x') {
                next_pos = {pos.i + 1, pos.j};
                valid_positions.push(next_pos);
            }
        }

        // Verifica se a saída foi encontrada acima
        if (pos.i - 1 >= 0) {
            if (maze[pos.i - 1][pos.j] == 's') {
                print_maze();
                exit_found = true;
            } else if (maze[pos.i - 1][pos.j] == 'x') {
                next_pos = {pos.i - 1, pos.j};
                valid_positions.push(next_pos);
            }
        }
        
        // Verifica o tamanho da pilha para determinar o comportamento
        if (valid_positions.size() == 1) {
            pos = {valid_positions.top().i , valid_positions.top().j};
        } else if (valid_positions.size() == 2) {
            pos = {valid_positions.top().i , valid_positions.top().j};
            valid_positions.pop();
            next_pos = {valid_positions.top().i, valid_positions.top().j};
            // Cria uma thread separada para explorar a próxima posição
            thread f(walk, next_pos);
            f.detach();
        } else if (valid_positions.size() == 3) {
            pos = {valid_positions.top().i , valid_positions.top().j};
            valid_positions.pop();
            next_pos = {valid_positions.top().i, valid_positions.top().j};
            // Cria uma thread separada para explorar a próxima posição
            thread f(walk, next_pos);
            f.detach();

            valid_positions.pop();
            next_pos = {valid_positions.top().i, valid_positions.top().j};
            // Cria outra thread separada para explorar a próxima posição
            thread g(walk, next_pos);
            g.detach();
        }
    }
}

int main(int argc, char* argv[]) {
    // Carrega o labirinto com o nome do arquivo recebido como argumento
    pos_t initial_pos = load_maze(argv[1]);

    // Cria thread detached passando a função de navegação
    thread t(walk, initial_pos);
    t.detach();

    while (!exit_found) {
        print_maze();
        system("clear");
    }

    print_maze();
    if (!exit_found) {
        cout << "Não foi possível encontrar a saída!" << endl;
    } else {
        cout << "Saída encontrada!" << endl;
    }

    return 0;
}