/*
	Universidade Federal de Minas Gerais
	Exercício Computacional 1 - Maze Runner
	Aluno: Bernardo de Souza Silva
	Matrícula: 2020084290
	Disciplina: Automação em Tempo Real

	Para rodar: ./maze_runner ../data/maze.txt
*/

#include <stdio.h>
#include <stdlib.h>
#include <stack>
#include <unistd.h>

// Matriz de char representando o labirinto
char** maze;

// Número de linhas e colunas do labirinto
int num_rows;
int num_cols;

// Representação de uma posição
struct pos_t {
	int i;
	int j;
};

// Estrutura de dados contendo as próximas posicões a serem exploradas no labirinto
std::stack<pos_t> valid_positions;

// Função que lê o labirinto de um arquivo texto, carrega em memória e retorna a posição inicial
pos_t load_maze(const char* file_name) {
	pos_t initial_pos;
	// Abre o arquivo para leitura (fopen)
	FILE *file = fopen(file_name, "r");

	// Lê o numero de linhas e colunas (fscanf) e salva em num_rows e num_cols
	fscanf(file, "%i%i", &num_rows, &num_cols);

	// Aloca a matriz maze (malloc)
	maze = (char **) malloc(num_rows * sizeof(char *));
	for (int i = 0; i < num_rows; ++i)
		// Aloca cada linha da matriz
		maze[i] = (char *) malloc(num_cols * sizeof(char));
	
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			// Lê o caractere da posição i+1, j do arquivo
			char character;
			fscanf(file, " %c", &character);

			// Salva o caractere na matriz maze
			maze[i][j] = character;

			// Verifica se o caractere é 'e' (entrada)
			if (character == 'e') {
				initial_pos.i = i;
				initial_pos.j = j;
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
                printf("\033[1;31mo\033[0m");
            } else if (maze[i][j] == '.') {
                // Impressão do caractere '.' em vermelho
                printf("\033[1;31m.\033[0m");
            } else {
                printf("%c", maze[i][j]);
            }
        }
        printf("\n");
    }
    // Delay de 80 ms em cada passo
    usleep(80000);
}

// Função responsável pela navegação: recebe como entrada a posição inicial e retorna um boolean indicando se a saída foi encontrada
bool walk(pos_t pos) {
	// Repita até que a saída seja encontrada ou não existam mais posições não exploradas
	while (!valid_positions.empty()) {
		// Verificar se a saída foi encontrada
		bool exitFound = false;

		// Marcar a posição atual com o caractere 'o'
		maze[pos.i][pos.j] = 'o';

		// Limpar a tela (opcional, depende do sistema)
		system("clear");

		// Imprimir o labirinto
		print_maze();
		
		// Verificar as posições adjacentes e adiciona à pilha se forem válidas
		if (pos.i > 0 && maze[pos.i - 1][pos.j] == 's') {
			exitFound = true;
		// Verifica se a posição acima da posição atual é válida e se ela contém um caminho 'x' não visitado
		} else if (pos.i > 0 && maze[pos.i - 1][pos.j] == 'x') {
			pos_t next_pos = {pos.i - 1, pos.j};
			valid_positions.push(next_pos);
		}

		if (pos.i < num_rows - 1 && maze[pos.i + 1][pos.j] == 's') {
			exitFound = true;
		// Verifica se a posição abaixo da posição atual é válida e se ela contém um caminho 'x' não visitado
		} else if (pos.i < num_rows - 1 && maze[pos.i + 1][pos.j] == 'x') {
			pos_t next_pos = {pos.i + 1, pos.j};
			valid_positions.push(next_pos);
		}

		if (pos.j > 0 && maze[pos.i][pos.j - 1] == 's') {
			exitFound = true;
		// Verifica se a posição à esquerda da posição atual é válida e se ela contém um caminho 'x' não visitado
		} else if (pos.j > 0 && maze[pos.i][pos.j - 1] == 'x') {
			pos_t next_pos = {pos.i, pos.j - 1};
			valid_positions.push(next_pos);
		}

		if (pos.j < num_cols - 1 && maze[pos.i][pos.j + 1] == 's') {
			exitFound = true;
		// Verifica se a posição à direita da posição atual é válida e se ela contém um caminho 'x' não visitado
		} else if (pos.j < num_cols - 1 && maze[pos.i][pos.j + 1] == 'x') {
			pos_t next_pos = {pos.i, pos.j + 1};
			valid_positions.push(next_pos);
		}

		maze[pos.i][pos.j] = '.';

		if (exitFound) {
			return true;
		}

		// Se a pilha não estiver vazia, obter a próxima posição e continuar
		if (!valid_positions.empty()) {
			pos = valid_positions.top();
			valid_positions.pop();
		}
	}

	// Caso a pilha esteja vazia e a saída não tenha sido encontrada
	return false;
}

int main(int argc, char* argv[]) {
	// Carregar o labirinto com o nome do arquivo recebido como argumento
	pos_t initial_pos = load_maze(argv[1]);

	// Inserir a posição inicial na pilha
	valid_positions.push(initial_pos);

	// Chamar a função de navegação
	bool exit_found = walk(initial_pos);

	// Tratar o retorno: imprimir mensagem
	if (exit_found) {
		printf("Saída encontrada!\n");
	} else {
		printf("Não foi possível encontrar a saída!\n");
	}

	// Liberar memória da matriz maze
	for (int i = 0; i < num_rows; ++i) {
		free(maze[i]);
	}
	free(maze);

	return 0;
}