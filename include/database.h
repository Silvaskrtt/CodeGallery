#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

// STRUCT para a conexao do DB
typedef struct {
    sqlite3 *db;
    int status;
} Database;

Database conectar_banco(const char *caminho_db);
void desconectar_banco(Database *db);
char* gerarIDPersonalizado(sqlite3 *db, const char *prefixo, const char *tabela, const char *colunaID, int totalDigitos);

#endif