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
void desconectar_banc(Database *db) {
    if (db->db) {
        sqlite3_close(db->db);
        db->db = NULL;
    }
    db->status = 0;
}