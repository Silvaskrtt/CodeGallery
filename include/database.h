#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

// STRUCT para a conexao do DB
typedef struct {
    sqlite3 *db;
    int status;
} Database;

// --------- Funções para conexão do DB ---------

Database conectaDB(const char *caminho_db);
void discDB(Database *db);

// Função para gerar ID's personalizados

char* genIdPers(sqlite3 *db, const char *prefixo, const char *tabela, const char *colunaID, int totalDigitos);

// --------- Funções da tbl_Usuarios ---------

char* cadUser(sqlite3 *db, const char *nome, const char *sobrenome, const char *cpf);

void listUser(sqlite3 *db);

// --------- Funções da tbl_Livros ---------

char* addLivro(sqlite3 *db, const char *titulo, const char *autor, const char *ano, int disponibilidade);

void listLivro(sqlite3 *db);

// --------- Funções da tbl_Emprestimos ---------

char* regEmpLivro(sqlite3 *db, const char *data_emprestimo, const char *data_devolucao, const char *id_livro_fk, const char *id_usuario_fk);

void consultEmp(sqlite3 *db);

#endif