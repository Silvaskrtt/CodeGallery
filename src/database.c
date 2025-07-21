#include "database.h"
#include <stdio.h>

// Conectar ao DB
Database conectar_banco(const char *caminho_db) {
    Database db = {NULL, 0};

    if (sqlite3_open(caminho_db, &db.db) != SQLITE_OK) {
        fprintf(stderr, "Erro ao abrir o banco: %s\n", sqlite3_errmsg(db.db));
        db.status = -1; // Codigo de erro
    } else {
        db.status = 1; // Sucesso
    }

    return db;
}

// Desconectar do DB
void desconectar_banco(Database *db) {
    if (db->db) {
        sqlite3_close(db->db);
        db->db = NULL;
    }
    db->status = 0;
}

char* gerarIDPersonalizado(sqlite3 *db, const char *prefixo, const char *tabela, const char *colunaID, int totalDigitos) {
    if (!db || !prefixo || !tabela || !colunaID) return NULL;

    char sql[256];
    snprintf(sql, sizeof(sql),
            "SELECT MAX(SUBSTR(%s, %d)) FROM %s WHERE %s LIKE '%%%';",
            colunaID,
            (int)(strlen(prefixo) + 1),
            tabela,
            colunaID,
            prefixo);
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare16_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao preparar consulta: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

     int proximo_numero = 1;
    if (sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_text(stmt, 0)) {
        const unsigned char *ultimo_id = sqlite3_column_text(stmt, 0);
        proximo_numero = atoi((const char*)ultimo_id) + 1;
    }
    sqlite3_finalize(stmt);

    char *novo_id = malloc(strlen(prefixo) + 5); // Ex: USR + 9999 + \0
    if (!novo_id) return NULL;

    sprintf(novo_id, "%s%03d", prefixo, proximo_numero); // Formato com 3 dígitos, ex: USR007
    return novo_id;
}

