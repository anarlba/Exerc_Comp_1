#include <chrono>
#include <fstream>
#include <iostream>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include <vector>

// Representação do labirinto
using Maze = std::vector<std::vector<char>>;

// Struct para representar uma posição no labirinto
struct Position {
  int row; // linha
  int col; // coluna
};

// Variáveis globais
Maze maze; // Labirinto
int num_rows;
int num_cols;
std::stack<Position> valid_positions; // Pilha de posições válidas

// Função para ler o labirinto de um arquivo
Position load_maze(const std::string &file_name) {
  std::ifstream file(file_name); // Abre o arquivo
  if (!file.is_open()) {         // Trata erro ao abrir o arquivo
    std::cerr << "Erro ao abrir  o arquivo " << file_name << std::endl;
    exit(1);
  }
  // std::cout << "Arquivo aberto com sucesso!\n";

  Position start;               // Armazena posição inicial 'e' do labirinto
  file >> num_rows >> num_cols; // Lê o número de linhas e colunas do labirinto
  maze.resize(num_rows, std::vector<char>(num_cols));
  for (int i = 0; i < num_rows; i++) {
    for (int j = 0; j < num_cols; j++) {
      file >> maze[i][j];      // Lê cada posição da matriz do labirinto
      if (maze[i][j] == 'e') { // Encontra a posição inicial 'e'
        start.row = i;
        start.col = j;
      }
    }
  }
  return start;
}

void print_maze() {
  std::cout << "\033[H\033[J"; // Move o cursor para o topo e limpa a tela

  for (int i = 0; i < num_rows; i++) {
    for (int j = 0; j < num_cols; j++) {
      if (maze[i][j] == '#') {
        std::cout << "\033[1;37m" << maze[i][j] << " \033[0m"; // Branco
      } else if (maze[i][j] == 'o') {
        std::cout << "\033[1;32m" << maze[i][j] << " \033[0m"; // Verde
      } else if (maze[i][j] == 'S' || maze[i][j] == 's') {
        std::cout << "\033[1;34m" << maze[i][j] << " \033[0m"; // Azul
      } else {
        std::cout << maze[i][j] << " ";
      }
    }
    std::cout << '\n';
  }
}

bool is_valid_position(int row, int col) { // Função para verificar se a posição é válida
  if (row >= 0 && row < num_rows && col >= 0 && col < num_cols && (maze[row][col] == 'x'|| maze[row][col] == 's')) {
    return true;
  } else {
    return false;
  }
}

bool walk(Position pos) { // Função principal para navegar pelo labirinto
  if (maze[pos.row][pos.col] == 's') {
    return true;
  }
  maze[pos.row][pos.col] = 'o'; // Marca a posição atual como visitada
  print_maze();                 // Imprime o labirinto
  std::cout << '\n';
  std::this_thread::sleep_for(std::chrono::milliseconds(150)); // Aguarda 100ms
  if (is_valid_position(pos.row - 1, pos.col)) { // Posição acima válida?
    valid_positions.push({pos.row - 1, pos.col});
  }
  if (is_valid_position(pos.row, pos.col + 1)) { // Posição à direita válida?
    valid_positions.push({pos.row, pos.col + 1});
  }
  if (is_valid_position(pos.row + 1, pos.col)) { // Posição abaixo válida?
    valid_positions.push({pos.row + 1, pos.col});
  }
  if (is_valid_position(pos.row, pos.col - 1)) { // Posição à esquerda válida?
    valid_positions.push({pos.row, pos.col - 1});
  }
  if (!valid_positions.empty()) {
    Position next = valid_positions.top(); // Pega a próxima posição da pilha
    valid_positions.pop();                 // Remove a posição da pilha
    return walk(next);                     // Chama a função recursivamente
    return true;
  }
  if (valid_positions.empty() && maze[pos.row][pos.col] != 's') { // Se a pilha estiver vazia e a posição atual não for a saída 's'
    maze[pos.row][pos.col] = 'x';  // Marca a posição atual como inválida
    print_maze();
    std::cout << "Backtracking...\n";
    return false;
  }
  return false;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Uso: " << argv[0] << "arquivo_maze.txt\n" << std::endl;
    return 1;
  }

  Position initial_pos = load_maze(argv[1]); // Carrega o labirinto
  if (initial_pos.row == -1 || initial_pos.col == -1) {
    std::cerr << "Posição inicial não encontrada no labirinto\n" << std::endl;
    return 1;
  }

  bool exit_found =
      walk(initial_pos); // Chama a função para navegar pelo labirinto

  if (exit_found) {
    std::cout << "Saída encontrada!\n" << std::endl;
  } else {
    std::cout << "Saída não encontrada!\n" << std::endl;
  }

  return 0;
}
