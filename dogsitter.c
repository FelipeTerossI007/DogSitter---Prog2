#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dogsitter.h"

// --- VARIÁVEIS GLOBAIS PARA ARMAZENAMENTO DINÂMICO ---
Dogsitter *listaSitters = NULL;
int numSitters = 0;
int capSitters = 0;

Cliente *listaClientes = NULL;
int numClientes = 0;
int capClientes = 0;

Reserva *listaReservas = NULL;
int numReservas = 0;
int capReservas = 0;

// Nomes dos arquivos de dados
const char *FILE_SITTERS = "sitters.txt";
const char *FILE_CLIENTS = "clients.txt";
const char *FILE_RESERVATIONS = "reservations.txt";

// --- IMPLEMENTAÇÃO DAS FUNÇÕES DE UTILIDADE ---

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void lerString(const char *prompt, char *destino, int tamanho) {
    printf("%s", prompt);
    fgets(destino, tamanho, stdin);
    
    size_t len = strlen(destino);
    if (len > 0 && destino[len - 1] == '\n') {
        destino[len - 1] = '\0';
    } else {
        limparBufferEntrada();
    }
}

int lerInteiro(const char *prompt) {
    char buffer[50];
    int valor;
    char *endptr;
    while (1) {
        lerString(prompt, buffer, sizeof(buffer));
        valor = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\0') {
            printf("Entrada invalida. Digite um numero inteiro.\n");
        } else {
            return valor;
        }
    }
}

float lerFloat(const char *prompt) {
    char buffer[50];
    float valor;
    char *endptr;
    while (1) {
        lerString(prompt, buffer, sizeof(buffer));
        valor = strtof(buffer, &endptr);
        
        if (endptr == buffer || *endptr != '\0') {
            printf("Entrada invalida. Digite um numero (ex: 50.75).\n");
        } else {
            return valor;
        }
    }
}

// --- IMPLEMENTAÇÃO DOS MENUS (PROTÓTIPOS DE TELA) ---

void exibirMenuPrincipal() {
    printf("\n--- Sistema de Reserva de Dogsitters ---\n");
    printf("1. Gerenciar Dogsitters\n");
    printf("2. Gerenciar Clientes\n");
    printf("3. Gerenciar Reservas\n");
    printf("0. Salvar e Sair\n");
    printf("----------------------------------------\n");
}

void exibirSubMenu(const char *titulo) {
    printf("\n--- %s ---\n", titulo);
    printf("1. Cadastrar\n");
    printf("2. Listar\n");
    printf("3. Atualizar\n");
    printf("4. Excluir\n");
    printf("0. Voltar ao Menu Principal\n");
    printf("---------------------------------\n");
}


// --- IMPLEMENTAÇÃO - DOGSITTERS (CRUD) ---

void cadastrarDogsitter() {
    if (numSitters == capSitters) {
        capSitters = (capSitters == 0) ? 10 : capSitters * 2;
        listaSitters = (Dogsitter*) realloc(listaSitters, capSitters * sizeof(Dogsitter));
        if (listaSitters == NULL) {
            printf("Erro critico: Falha ao alocar memoria!\n");
            exit(1);
        }
    }

    Dogsitter *novo = &listaSitters[numSitters];
    novo->id = getProximoIDDogsitter();
    printf("Cadastrando novo Dogsitter (ID: %d)\n", novo->id);
    lerString("Nome: ", novo->nome, 100);
    lerString("Telefone: ", novo->telefone, 20);
    novo->taxaPorDia = lerFloat("Taxa por dia (R$): ");

    numSitters++;
    printf("Dogsitter cadastrado com sucesso!\n");
}

void listarDogsitters() {
    printf("\n--- Lista de Dogsitters Cadastrados (%d) ---\n", numSitters);
    if (numSitters == 0) {
        printf("Nenhum dogsitter cadastrado.\n");
        return;
    }
    for (int i = 0; i < numSitters; i++) {
        Dogsitter *s = &listaSitters[i];
        printf("ID: %d | Nome: %s | Telefone: %s | Taxa: R$ %.2f\n",
               s->id, s->nome, s->telefone, s->taxaPorDia);
    }
}

Dogsitter* buscarDogsitterPorID(int id) {
    for (int i = 0; i < numSitters; i++) {
        if (listaSitters[i].id == id) {
            return &listaSitters[i];
        }
    }
    return NULL;
}

void atualizarDogsitter() {
    int id = lerInteiro("Digite o ID do dogsitter a atualizar: ");
    Dogsitter *s = buscarDogsitterPorID(id);

    if (s == NULL) {
        printf("Erro: Dogsitter com ID %d nao encontrado.\n", id);
        return;
    }

    printf("Atualizando dados de: %s (Deixe em branco para manter o atual)\n", s->nome);
    char buffer[100];

    lerString("Novo Nome: ", buffer, 100);
    if (strlen(buffer) > 0) strcpy(s->nome, buffer);

    lerString("Novo Telefone: ", buffer, 20);
    if (strlen(buffer) > 0) strcpy(s->telefone, buffer);

    // CORREÇÃO: Permite pular a taxa (deixar em branco) para manter a atual
    char bufferTaxa[50];
    printf("Nova Taxa por dia (R$) (Atual: %.2f): ", s->taxaPorDia);
    lerString("", bufferTaxa, 50); // Lê como string
    
    if (strlen(bufferTaxa) > 0) {
        char *endptr;
        float novaTaxa = strtof(bufferTaxa, &endptr);
        
        // Valida se a conversão foi bem-sucedida
        if (endptr == bufferTaxa || *endptr != '\0') {
            printf("Entrada invalida. A taxa nao foi alterada.\n");
        } else {
            s->taxaPorDia = novaTaxa;
        }
    }
    
    printf("Dados atualizados com sucesso!\n");
}

void excluirDogsitter() {
    int id = lerInteiro("Digite o ID do dogsitter a excluir: ");
    
    int indice = -1;
    for (int i = 0; i < numSitters; i++) {
        if (listaSitters[i].id == id) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("Erro: Dogsitter com ID %d nao encontrado.\n", id);
        return;
    }

    // Validação de integridade: não exclui se tiver reserva ativa
    for(int i=0; i < numReservas; i++) {
        if(listaReservas[i].idDogsitter == id && listaReservas[i].status == 'A') {
            printf("Erro: Dogsitter nao pode ser excluido pois possui reservas ativas.\n");
            return;
        }
    }

    // Otimização: move o último elemento para a posição do excluído
    if (indice != numSitters - 1) {
        listaSitters[indice] = listaSitters[numSitters - 1];
    }
    
    numSitters--;
    printf("Dogsitter excluido com sucesso!\n");
}


// --- IMPLEMENTAÇÃO - CLIENTES (CRUD) ---

void cadastrarCliente() {
    if (numClientes == capClientes) {
        capClientes = (capClientes == 0) ? 10 : capClientes * 2;
        listaClientes = (Cliente*) realloc(listaClientes, capClientes * sizeof(Cliente));
        if (listaClientes == NULL) {
            printf("Erro critico: Falha ao alocar memoria!\n");
            exit(1);
        }
    }

    Cliente *novo = &listaClientes[numClientes];
    novo->id = getProximoIDCliente();
    printf("Cadastrando novo Cliente (ID: %d)\n", novo->id);
    lerString("Nome do Cliente: ", novo->nome, 100);
    lerString("Telefone: ", novo->telefone, 20);
    lerString("Nome do Cachorro: ", novo->nomeCachorro, 100);

    numClientes++;
    printf("Cliente cadastrado com sucesso!\n");
}

void listarClientes() {
    printf("\n--- Lista de Clientes Cadastrados (%d) ---\n", numClientes);
    if (numClientes == 0) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }
    for (int i = 0; i < numClientes; i++) {
        Cliente *c = &listaClientes[i];
        printf("ID: %d | Nome: %s | Telefone: %s | Cachorro: %s\n",
               c->id, c->nome, c->telefone, c->nomeCachorro);
    }
}

Cliente* buscarClientePorID(int id) {
    for (int i = 0; i < numClientes; i++) {
        if (listaClientes[i].id == id) {
            return &listaClientes[i];
        }
    }
    return NULL;
}

void atualizarCliente() {
    int id = lerInteiro("Digite o ID do cliente a atualizar: ");
    Cliente *c = buscarClientePorID(id);

    if (c == NULL) {
        printf("Erro: Cliente com ID %d nao encontrado.\n", id);
        return;
    }

    printf("Atualizando dados de: %s (Deixe em branco para manter o atual)\n", c->nome);
    char buffer[100];

    lerString("Novo Nome: ", buffer, 100);
    if (strlen(buffer) > 0) strcpy(c->nome, buffer);

    lerString("Novo Telefone: ", buffer, 20);
    if (strlen(buffer) > 0) strcpy(c->telefone, buffer);

    lerString("Novo Nome do Cachorro: ", buffer, 100);
    if (strlen(buffer) > 0) strcpy(c->nomeCachorro, buffer);
    
    printf("Dados atualizados com sucesso!\n");
}

void excluirCliente() {
    int id = lerInteiro("Digite o ID do cliente a excluir: ");
    
    int indice = -1;
    for (int i = 0; i < numClientes; i++) {
        if (listaClientes[i].id == id) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("Erro: Cliente com ID %d nao encontrado.\n", id);
        return;
    }

    // Validação de integridade
    for(int i=0; i < numReservas; i++) {
        if(listaReservas[i].idCliente == id && listaReservas[i].status == 'A') {
            printf("Erro: Cliente nao pode ser excluido pois possui reservas ativas.\n");
            return;
        }
    }

    if (indice != numClientes - 1) {
        listaClientes[indice] = listaClientes[numClientes - 1];
    }
    
    numClientes--;
    printf("Cliente excluido com sucesso!\n");
}


// --- IMPLEMENTAÇÃO - RESERVAS (CRUD) ---

void criarReserva() {
    if (numSitters == 0 || numClientes == 0) {
        printf("Erro: E preciso ter ao menos um Dogsitter e um Cliente cadastrado.\n");
        return;
    }
    
    if (numReservas == capReservas) {
        capReservas = (capReservas == 0) ? 20 : capReservas * 2;
        listaReservas = (Reserva*) realloc(listaReservas, capReservas * sizeof(Reserva));
        if (listaReservas == NULL) {
            printf("Erro critico: Falha ao alocar memoria!\n");
            exit(1);
        }
    }

    Reserva *nova = &listaReservas[numReservas];
    nova->id = getProximoIDReserva();
    
    printf("Criando Nova Reserva (ID: %d)\n", nova->id);
    
    listarClientes();
    Cliente *cliente = NULL;
    while(cliente == NULL) {
        int idCliente = lerInteiro("Digite o ID do Cliente: ");
        cliente = buscarClientePorID(idCliente);
        if(cliente == NULL) printf("ID invalido. Tente novamente.\n");
    }
    nova->idCliente = cliente->id;

    listarDogsitters();
    Dogsitter *sitter = NULL;
    while(sitter == NULL) {
        int idSitter = lerInteiro("Digite o ID do Dogsitter: ");
        sitter = buscarDogsitterPorID(idSitter);
        if(sitter == NULL) printf("ID invalido. Tente novamente.\n");
    }
    nova->idDogsitter = sitter->id;
    
    lerString("Data de Inicio (DD/MM/AAAA): ", nova->dataInicio, 11);
    
    // CORREÇÃO: A quantidade de dias é a fonte do cálculo, removendo a dataFim
    int dias = lerInteiro("Quantidade de dias da reserva: ");
    nova->custoTotal = sitter->taxaPorDia * dias;
    nova->status = 'A'; // Ativa

    numReservas++;
    printf("Reserva criada com sucesso! Custo total: R$ %.2f\n", nova->custoTotal);
}

void listarReservas() {
    printf("\n--- Lista de Reservas (%d) ---\n", numReservas);
    if (numReservas == 0) {
        printf("Nenhuma reserva cadastrada.\n");
        return;
    }
    
    for (int i = 0; i < numReservas; i++) {
        Reserva *r = &listaReservas[i];
        Cliente *c = buscarClientePorID(r->idCliente);
        Dogsitter *s = buscarDogsitterPorID(r->idDogsitter);
        
        // Programação defensiva (caso cliente/sitter tenha sido excluído)
        const char *nomeCliente = (c != NULL) ? c->nome : "???";
        const char *nomeSitter = (s != NULL) ? s->nome : "???";
        const char *status = (r->status == 'A') ? "Ativa" : "Cancelada";

        printf("ID: %d | Status: %s\n", r->id, status);
        printf("  Cliente: %s (ID: %d)\n", nomeCliente, r->idCliente);
        printf("  Dogsitter: %s (ID: %d)\n", nomeSitter, r->idDogsitter);
        printf("  Data Inicio: %s\n", r->dataInicio); // CORREÇÃO: Exibe apenas a data de início
        printf("  Custo: R$ %.2f\n", r->custoTotal);
        printf("--------------------------------------\n");
    }
}

void cancelarReserva() {
    int id = lerInteiro("Digite o ID da reserva a CANCELAR: ");
    Reserva *r = buscarReservaPorID(id);

    if (r == NULL) {
        printf("Erro: Reserva com ID %d nao encontrada.\n", id);
        return;
    }

    if (r->status == 'C') {
        printf("Esta reserva ja esta cancelada.\n");
        return;
    }

    r->status = 'C';
    printf("Reserva ID %d cancelada com sucesso.\n", id);
}

Reserva* buscarReservaPorID(int id) {
     for (int i = 0; i < numReservas; i++) {
        if (listaReservas[i].id == id) {
            return &listaReservas[i];
        }
    }
    return NULL;
}

// --- IMPLEMENTAÇÃO - GERAÇÃO DE ID (MÉTODO OTIMIZADO) ---

// Encontra o maior ID existente e retorna +1
int getProximoIDDogsitter() {
    int maxID = 0;
    for(int i = 0; i < numSitters; i++) {
        if(listaSitters[i].id > maxID) {
            maxID = listaSitters[i].id;
        }
    }
    return maxID + 1;
}

int getProximoIDCliente() {
    int maxID = 0;
    for(int i = 0; i < numClientes; i++) {
        if(listaClientes[i].id > maxID) {
            maxID = listaClientes[i].id;
        }
    }
    return maxID + 1;
}

int getProximoIDReserva() {
    int maxID = 0;
    for(int i = 0; i < numReservas; i++) {
        if(listaReservas[i].id > maxID) {
            maxID = listaReservas[i].id;
        }
    }
    return maxID + 1;
}


// --- IMPLEMENTAÇÃO - PERSISTÊNCIA EM DISCO (.txt) ---

// Função auxiliar para remover o '\n' lido pelo fgets
void removerNewline(char *str) {
    str[strcspn(str, "\n")] = 0;
}

// Funções específicas para Dogsitters
void carregarDogsittersTXT() {
    FILE *f = fopen(FILE_SITTERS, "r");
    if (f == NULL) return; 

    capSitters = 10;
    numSitters = 0;
    listaSitters = (Dogsitter*) malloc(capSitters * sizeof(Dogsitter));
    if(listaSitters == NULL) { printf("Falha ao alocar memoria inicial\n"); exit(1); }

    char buffer[256];
    
    while (fgets(buffer, sizeof(buffer), f) != NULL) {
        if (numSitters == capSitters) {
            capSitters *= 2;
            listaSitters = (Dogsitter*) realloc(listaSitters, capSitters * sizeof(Dogsitter));
            if(listaSitters == NULL) { printf("Falha ao realocar memoria\n"); exit(1); }
        }

        Dogsitter *s = &listaSitters[numSitters];
        s->id = atoi(buffer); 
        fgets(s->nome, 100, f); 
        removerNewline(s->nome);
        fgets(s->telefone, 20, f); 
        removerNewline(s->telefone);
        fgets(buffer, sizeof(buffer), f); 
        s->taxaPorDia = atof(buffer);
        numSitters++;
    }
    fclose(f);
    printf("Carregados %d dogsitters de %s\n", numSitters, FILE_SITTERS);
}

void salvarDogsittersTXT() {
    FILE *f = fopen(FILE_SITTERS, "w");
    if (f == NULL) {
        printf("Erro: Nao foi possivel abrir %s para escrita.\n", FILE_SITTERS);
        return;
    }

    for (int i = 0; i < numSitters; i++) {
        Dogsitter *s = &listaSitters[i];
        fprintf(f, "%d\n", s->id);
        fprintf(f, "%s\n", s->nome);
        fprintf(f, "%s\n", s->telefone);
        fprintf(f, "%.2f\n", s->taxaPorDia);
    }
    fclose(f);
    printf("Salvos %d dogsitters em %s\n", numSitters, FILE_SITTERS);
}

// Funções específicas para Clientes
void carregarClientesTXT() {
    FILE *f = fopen(FILE_CLIENTS, "r");
    if (f == NULL) return;

    capClientes = 10;
    numClientes = 0;
    listaClientes = (Cliente*) malloc(capClientes * sizeof(Cliente));
    if(listaClientes == NULL) { printf("Falha ao alocar memoria inicial\n"); exit(1); }

    char buffer[256];
    
    while (fgets(buffer, sizeof(buffer), f) != NULL) {
        if (numClientes == capClientes) {
            capClientes *= 2;
            listaClientes = (Cliente*) realloc(listaClientes, capClientes * sizeof(Cliente));
            if(listaClientes == NULL) { printf("Falha ao realocar memoria\n"); exit(1); }
        }
        
        Cliente *c = &listaClientes[numClientes];
        c->id = atoi(buffer);
        fgets(c->nome, 100, f);
        removerNewline(c->nome);
        fgets(c->telefone, 20, f);
        removerNewline(c->telefone);
        fgets(c->nomeCachorro, 100, f);
        removerNewline(c->nomeCachorro);
        numClientes++;
    }
    fclose(f);
    printf("Carregados %d clientes de %s\n", numClientes, FILE_CLIENTS);
}

void salvarClientesTXT() {
    FILE *f = fopen(FILE_CLIENTS, "w");
    if (f == NULL) {
        printf("Erro: Nao foi possivel abrir %s para escrita.\n", FILE_CLIENTS);
        return;
    }

    for (int i = 0; i < numClientes; i++) {
        Cliente *c = &listaClientes[i];
        fprintf(f, "%d\n", c->id);
        fprintf(f, "%s\n", c->nome);
        fprintf(f, "%s\n", c->telefone);
        fprintf(f, "%s\n", c->nomeCachorro);
    }
    fclose(f);
    printf("Salvos %d clientes em %s\n", numClientes, FILE_CLIENTS);
}

// Funções específicas para Reservas
void carregarReservasTXT() {
    FILE *f = fopen(FILE_RESERVATIONS, "r");
    if (f == NULL) return;

    capReservas = 10;
    numReservas = 0;
    listaReservas = (Reserva*) malloc(capReservas * sizeof(Reserva));
    if(listaReservas == NULL) { printf("Falha ao alocar memoria inicial\n"); exit(1); }
    
    char buffer[256];
    
    while (fgets(buffer, sizeof(buffer), f) != NULL) {
        if (numReservas == capReservas) {
            capReservas *= 2;
            listaReservas = (Reserva*) realloc(listaReservas, capReservas * sizeof(Reserva));
            if(listaReservas == NULL) { printf("Falha ao realocar memoria\n"); exit(1); }
        }
        
        Reserva *r = &listaReservas[numReservas];
        r->id = atoi(buffer); // Linha 1: ID
        
        fgets(buffer, sizeof(buffer), f); // Linha 2: ID Cliente
        r->idCliente = atoi(buffer);

        fgets(buffer, sizeof(buffer), f); // Linha 3: ID Dogsitter
        r->idDogsitter = atoi(buffer);
        
        fgets(r->dataInicio, 11, f); // Linha 4: Data Inicio
        removerNewline(r->dataInicio);

        // CORREÇÃO: A dataFim foi removida
        
        fgets(buffer, sizeof(buffer), f); // Linha 5: Custo
        r->custoTotal = atof(buffer);

        fgets(buffer, sizeof(buffer), f); // Linha 6: Status
        r->status = buffer[0];

        numReservas++;
    }
    fclose(f);
    printf("Carregadas %d reservas de %s\n", numReservas, FILE_RESERVATIONS);
}

void salvarReservasTXT() {
    FILE *f = fopen(FILE_RESERVATIONS, "w");
    if (f == NULL) {
        printf("Erro: Nao foi possivel abrir %s para escrita.\n", FILE_RESERVATIONS);
        return;
    }

    for (int i = 0; i < numReservas; i++) {
        Reserva *r = &listaReservas[i];
        fprintf(f, "%d\n", r->id);
        fprintf(f, "%d\n", r->idCliente);
        fprintf(f, "%d\n", r->idDogsitter);
        fprintf(f, "%s\n", r->dataInicio);
        // CORREÇÃO: A dataFim foi removida
        fprintf(f, "%.2f\n", r->custoTotal);
        fprintf(f, "%c\n", r->status);
    }
    fclose(f);
    printf("Salvas %d reservas em %s\n", numReservas, FILE_RESERVATIONS);
}

// Funções globais de Carregar/Salvar
void carregarDados() {
    printf("Carregando dados do disco (arquivos .txt)...\n");
    carregarDogsittersTXT();
    carregarClientesTXT();
    carregarReservasTXT();
}

void salvarDados() {
    printf("Salvando dados no disco (arquivos .txt)...\n");
    salvarDogsittersTXT();
    salvarClientesTXT();
    salvarReservasTXT();

    // Libera toda a memória alocada dinamicamente
    free(listaSitters);
    free(listaClientes);
    free(listaReservas);
    printf("Memoria liberada. Encerrando.\n");
}
