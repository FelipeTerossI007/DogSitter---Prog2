#ifndef DOGSITTER_H
#define DOGSITTER_H

#include <stdio.h> // Necessário para o tipo size_t (e FILE*)

// --- ESTRUTURAS DE DADOS ---

typedef struct {
    int id;
    char nome[100];
    char telefone[20];
    float taxaPorDia;
} Dogsitter;

typedef struct {
    int id;
    char nome[100];
    char telefone[20];
    char nomeCachorro[100];
} Cliente;

typedef struct {
    int id;
    int idCliente;
    int idDogsitter;
    char dataInicio[11]; // Formato "DD/MM/AAAA"
    char dataFim[11];    // Formato "DD/MM/AAAA"
    float custoTotal;
    char status; // 'A' = Ativa, 'C' = Cancelada
} Reserva;


// --- PROTÓTIPOS DAS FUNÇÕES (DECLARAÇÕES) ---

// Funções de utilidade para entrada de dados
void limparBufferEntrada();
void lerString(const char *prompt, char *destino, int tamanho);
int lerInteiro(const char *prompt);
float lerFloat(const char *prompt);

// Funções de Menu (Protótipos de Tela)
void exibirMenuPrincipal();
void exibirSubMenu(const char *titulo);

// Funções de Gerenciamento de Dogsitters (CRUD)
void cadastrarDogsitter();
void listarDogsitters();
void atualizarDogsitter();
void excluirDogsitter();
Dogsitter* buscarDogsitterPorID(int id); // Função auxiliar

// Funções de Gerenciamento de Clientes (CRUD)
void cadastrarCliente();
void listarClientes();
void atualizarCliente();
void excluirCliente();
Cliente* buscarClientePorID(int id); // Função auxiliar

// Funções de Gerenciamento de Reservas (CRUD)
void criarReserva();
void listarReservas();
void cancelarReserva();
Reserva* buscarReservaPorID(int id); // Função auxiliar

// Função Recursiva (Requisito do projeto)
int encontrarProximoIDDisponivel(int* idsUsados, int count, int idAtual);
int getProximoIDDogsitter();
int getProximoIDCliente();
int getProximoIDReserva();

// Funções de Persistência (Leitura/Gravação em Disco .txt)
void carregarDogsittersTXT();
void salvarDogsittersTXT();
void carregarClientesTXT();
void salvarClientesTXT();
void carregarReservasTXT();
void salvarReservasTXT();

void carregarDados(); // Carrega tudo do disco na inicialização
void salvarDados();  // Salva tudo no disco ao sair e limpa a memória

#endif // DOGSITTER_H
