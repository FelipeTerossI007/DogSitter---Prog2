#include <stdio.h>
#include <stdlib.h>
#include "dogsitter.h" // Inclui nosso header

// --- FUNÇÕES DE LOOP DE SUB-MENU ---

void loopMenuDogsitters() {
    int opcao = -1;
    while (opcao != 0) {
        exibirSubMenu("Gerenciar Dogsitters");
        opcao = lerInteiro("Sua opcao: ");
        
        switch (opcao) {
            case 1: cadastrarDogsitter(); break;
            case 2: listarDogsitters(); break;
            case 3: atualizarDogsitter(); break;
            case 4: excluirDogsitter(); break;
            case 0: printf("Voltando ao menu principal...\n"); break;
            default: printf("Opcao invalida. Tente novamente.\n"); break;
        }
    }
}

void loopMenuClientes() {
    int opcao = -1;
    while (opcao != 0) {
        exibirSubMenu("Gerenciar Clientes");
        opcao = lerInteiro("Sua opcao: ");
        
        switch (opcao) {
            case 1: cadastrarCliente(); break;
            case 2: listarClientes(); break;
            case 3: atualizarCliente(); break;
            case 4: excluirCliente(); break;
            case 0: printf("Voltando ao menu principal...\n"); break;
            default: printf("Opcao invalida. Tente novamente.\n"); break;
        }
    }
}

void loopMenuReservas() {
    int opcao = -1;
    while (opcao != 0) {
        // O menu de reservas é um pouco diferente (Cadastrar, Listar, Cancelar)
        printf("\n--- Gerenciar Reservas ---\n");
        printf("1. Criar Nova Reserva\n");
        printf("2. Listar Todas as Reservas\n");
        printf("3. Cancelar Reserva\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("----------------------------\n");
        
        opcao = lerInteiro("Sua opcao: ");
        
        switch (opcao) {
            case 1: criarReserva(); break;
            case 2: listarReservas(); break;
            case 3: cancelarReserva(); break; // 'Excluir' aqui é 'Cancelar'
            case 0: printf("Voltando ao menu principal...\n"); break;
            default: printf("Opcao invalida. Tente novamente.\n"); break;
        }
    }
}


// --- FUNÇÃO PRINCIPAL ---

int main() {
    // Carrega os dados dos arquivos .txt para a memória
    carregarDados();

    int opcao = -1;
    
    // Loop de repetição principal
    while (opcao != 0) {
        exibirMenuPrincipal();
        opcao = lerInteiro("Sua opcao: ");

        switch (opcao) {
            case 1:
                loopMenuDogsitters();
                break;
            case 2:
                loopMenuClientes();
                break;
            case 3:
                loopMenuReservas();
                break;
            case 0:
                // Salva os dados da memória para os arquivos .txt
                salvarDados();
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
    }

    return 0;
}
