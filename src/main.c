#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <windows.h>

#define TAM_MAX 100     // Define um tamanho máximo para as strings

// Função para remover o '\n' do final das strings lidas com fgets
void removerNovaLinha(char *str) {
    size_t len = strlen(str);                 // Obtém o tamanho da string
    if (len > 0 && str[len - 1] == '\n')      // Verifica se o último caractere é '\n'
        str[len - 1] = '\0';                  // Substitui por '\0' para encerrar a string corretamente
}

int main() {

    // Define que o console usará UTF-8 (para suportar acentos no Windows)
    SetConsoleOutputCP(CP_UTF8);

    // Declara variáveis para armazenar dados de usuário, livro e empréstimo
    char nome[TAM_MAX];
    char sobrenome[TAM_MAX];
    char cpf[TAM_MAX];

    char titulo[TAM_MAX];
    char autor[TAM_MAX];
    char ano[TAM_MAX];
    char disponibilidade[TAM_MAX];

    char data_emprestimo[TAM_MAX];
    char data_devolucao[TAM_MAX];
    char id_livro_fk[TAM_MAX];
    char id_usuario_fk[TAM_MAX];

    // ===== Cadastro de Usuário =====
    printf("=== Cadastro de Usuário ===\n");

    printf("Nome: ");
    fgets(nome, sizeof(nome), stdin);   // Lê o nome digitado
    removerNovaLinha(nome);             // Remove o '\n'

    printf("Sobrenome: ");
    fgets(sobrenome, sizeof(sobrenome), stdin);  
    removerNovaLinha(sobrenome);

    printf("CPF: ");
    fgets(cpf, sizeof(cpf), stdin);
    removerNovaLinha(cpf);

    // ===== Cadastro de Livro =====
    printf("=== Cadastro de Livro ===\n");

    printf("Título: ");
    fgets(titulo, sizeof(titulo), stdin);
    removerNovaLinha(titulo);

    printf("Autor: ");
    fgets(autor, sizeof(autor), stdin);
    removerNovaLinha(autor);

    printf("Ano: ");
    fgets(ano, sizeof(ano), stdin);
    removerNovaLinha(ano);

    printf("Disponibilidade (0 ou 1): ");
    fgets(disponibilidade, sizeof(disponibilidade), stdin);
    removerNovaLinha(disponibilidade);
    int dispInt = atoi(disponibilidade);  // Converte string para inteiro (0 ou 1)

    // ===== Cadastro de Empréstimo =====
    printf("=== Cadastro de Emprestimo ===\n");

    printf("Data Emprestimo: ");
    fgets(data_emprestimo, sizeof(data_emprestimo), stdin);
    removerNovaLinha(data_emprestimo);

    printf("Data da devolução Prevista: ");
    fgets(data_devolucao, sizeof(data_devolucao), stdin);
    removerNovaLinha(data_devolucao);

    printf("ID do Livro a ser Emprestado: ");
    fgets(id_livro_fk, sizeof(id_livro_fk), stdin);
    removerNovaLinha(id_livro_fk);

    printf("Matrícula do Usuario (ID): ");
    fgets(id_usuario_fk, sizeof(id_usuario_fk), stdin);
    removerNovaLinha(id_usuario_fk);

    // ===== Conexão com o banco de dados =====
    const char *caminho_db = "database/BOOKSTACK.db";  // Caminho para o banco SQLite
    Database db = conectaDB(caminho_db);               // Abre a conexão

    if (db.status != 1) {  // Se a conexão falhou
        fprintf(stderr, "Erro ao conectar ao banco.\n");
        return 1;          // Encerra o programa
    }

    // ===== Inserir Usuário =====
    char *idUsuario = cadUser(db.db, nome, sobrenome, cpf); // Cadastra no banco
    if (idUsuario) {
        printf("Usuário cadastrado com sucesso. ID: %s\n", idUsuario);
        free(idUsuario);  // Libera memória alocada pela função cadUser
    } else {
        fprintf(stderr, "Erro ao cadastrar usuário.\n");
    }

    // ===== Inserir Livro =====
    char *idLivro = addLivro(db.db, titulo, autor, ano, dispInt); // Cadastra no banco
    if (idLivro) {
        printf("Livro cadastrado com sucesso. ID: %s\n", idLivro);
        free(idLivro);
    } else {
        fprintf(stderr, "Erro ao cadastrar livro.\n");
    }

    // ===== Registrar Empréstimo =====
    char *idEmp = regEmpLivro(db.db, data_emprestimo, data_devolucao, id_livro_fk, id_usuario_fk);
    if (idEmp) {
        printf("Livro emprestado com ID: %s\n", idEmp);
        free(idEmp);
    } else {
        fprintf(stderr, "Erro ao emprestar o livro.\n");
    }

    // ===== Listar Usuários =====
    listUser(db.db); // Mostra todos os usuários cadastrados

    // ===== Consultar Empréstimos =====
    consultEmp(db.db); // Mostra todos os empréstimos com join no banco

    // ===== Fechar conexão com o banco =====
    discDB(&db);

    return 0; // Fim do programa
}