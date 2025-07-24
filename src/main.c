#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define TAM_MAX 100

void removerNovaLinha(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') str[len - 1] = '\0';
}

int main() {

    SetConsoleOutputCP(CP_UTF8); // Suporte a UTF-8 no terminal do Windows

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

    printf("=== Cadastro de Usuário ===\n");

    printf("Nome: ");
    fgets(nome, sizeof(nome), stdin);
    removerNovaLinha(nome);

    printf("Sobrenome: ");
    fgets(sobrenome, sizeof(sobrenome), stdin);
    removerNovaLinha(sobrenome);

    printf("CPF: ");
    fgets(cpf, sizeof(cpf), stdin);
    removerNovaLinha(cpf);

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
    int dispInt = atoi(disponibilidade);  // Conversão da string para int

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

    const char *caminho_db = "database/CODEGALLERY.db";
    Database db = conectaDB(caminho_db);

    if (db.status != 1) {
        fprintf(stderr, "Erro ao conectar ao banco.\n");
        return 1;
    }

    char *idUsuario = cadUser(db.db, nome, sobrenome, cpf);
    if (idUsuario) {
        printf("Usuário cadastrado com sucesso. ID: %s\n", idUsuario);
        free(idUsuario);
    } else {
        fprintf(stderr, "Erro ao cadastrar usuário.\n");
    }

    char *idLivro = addLivro(db.db, titulo, autor, ano, dispInt);
    if (idLivro) {
        printf("Livro cadastrado com sucesso. ID: %s\n", idLivro);
        free(idLivro);
    } else {
        fprintf(stderr, "Erro ao cadastrar livro.\n");
    }

    char *idEmp = regEmpLivro(db.db, data_emprestimo, data_devolucao, id_livro_fk, id_usuario_fk);
    if (idEmp) {
        printf("Livro emprestado com ID: %s\n", idEmp);
        free(idEmp);
    } else {
        fprintf(stderr, "Erro ao emprestar o livro.\n");
    }

    listUser(db.db);
    
    consultEmp(db.db);

    discDB(&db);
    return 0;
}