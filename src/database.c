#include "database.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

// Conectar ao DB
Database conectaDB(const char *caminho_db) {
    SetConsoleOutputCP(CP_UTF8); // Suporte a UTF-8 no terminal do Windows

    Database db = {NULL, 0};

    if (sqlite3_open(caminho_db, &db.db) != SQLITE_OK) {
        fprintf(stderr, "Erro ao abrir o banco: %s\n", sqlite3_errmsg(db.db));
        db.status = -1;
    } else {
        db.status = 1;
    }

    return db;
}

// Desconectar do DB
void discDB(Database *db) {
    if (db->db) {
        sqlite3_close(db->db);
        db->db = NULL;
    }
    db->status = 0;
}

// Função para gerar ID's personalizados
char* genIdPers(sqlite3 *db, const char *prefixo, const char *tabela, const char *colunaID, int totalDigitos) {
    if (!db || !prefixo || !tabela || !colunaID || totalDigitos <= 0) return NULL;

    char sql[256];
    snprintf(sql, sizeof(sql),
        "SELECT MAX(CAST(SUBSTR(%s, %d) AS INTEGER)) FROM %s WHERE %s LIKE '%s%%';",
        colunaID,
        (int)(strlen(prefixo) + 1),
        tabela,
        colunaID,
        prefixo);

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao preparar consulta: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    int proximoNumero = 1;
    if (sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_type(stmt, 0) != SQLITE_NULL) {
        proximoNumero = sqlite3_column_int(stmt, 0) + 1;
    }
    sqlite3_finalize(stmt);

    int tamanhoTotal = strlen(prefixo) + totalDigitos + 1;
    char *novoID = malloc(tamanhoTotal);
    if (!novoID) return NULL;

    snprintf(novoID, tamanhoTotal, "%s%0*d", prefixo, totalDigitos, proximoNumero);
    return novoID;
}

// --------- Funções da tbl_Usuarios ---------

char* cadUser(sqlite3 *db, const char *nome, const char *sobrenome, const char *cpf) {
    if (!db || !nome || !sobrenome || strlen(nome) == 0 || strlen(sobrenome) == 0) {
        fprintf(stderr, "Parâmetros inválidos para cadastrar usuário\n");
        return NULL;
    }

    char *novoID = genIdPers(db, "USR", "tbl_Usuarios", "ID_Usuario_PK", 3);
    if (!novoID) {
        fprintf(stderr, "Erro ao gerar novo ID para usuário\n");
        return NULL;
    }

    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO tbl_Usuarios(ID_Usuario_PK, Nome, Sobrenome, CPF) VALUES(?, ?, ?, ?);";

    int resultado = SQLITE_ERROR;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, novoID, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, nome, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, sobrenome, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, cpf, -1, SQLITE_TRANSIENT);

        resultado = sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        if (resultado != SQLITE_DONE) {
            fprintf(stderr, "Erro ao inserir usuário: %s\n", sqlite3_errmsg(db));
            free(novoID);
            return NULL;
        }
    } else {
        fprintf(stderr, "Erro ao preparar statement: %s\n", sqlite3_errmsg(db));
        free(novoID);
        return NULL;
    }

    printf("Usuário cadastrado com ID: %s\n", novoID);
    return novoID;
}

void listUser(sqlite3 *db) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT ID_Usuario_PK, Nome, Sobrenome, CPF FROM tbl_Usuarios;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Erro ao preparar a consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("\n=== Lista de Usuários ===\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *id = (const char *)sqlite3_column_text(stmt, 0);
        const char *nome = (const char *)sqlite3_column_text(stmt, 1);
        const char *sobrenome = (const char *)sqlite3_column_text(stmt, 2);
        const char *cpf = (const char *)sqlite3_column_text(stmt, 3);
        printf("ID: %s | Nome: %s %s | CPF: %s\n", id, nome, sobrenome, cpf);
    }

    sqlite3_finalize(stmt);
}

// --------- Funções da tbl_Livros ---------

char* addLivro(sqlite3 *db, const char *titulo, const char *autor, const char *ano, int disponibilidade) {
    if (!db || !titulo ||!autor || !ano || strlen(titulo) == 0 || strlen(autor) == 0 || strlen(ano) == 0) {
        fprintf(stderr, "Parâmetros inválidos para cadastrar usuário\n");
        return NULL;
    }

    char *novoID = genIdPers(db,"LIV", "tbl_Livros", "ID_Livro_PK", 3);
    if (!novoID) {
        fprintf(stderr, "Erro ao gerar novo ID para usuário\n");
        return NULL;
    }

    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO tbl_Livros(ID_Livro_PK, Titulo, Autor, Ano, Disponibilidade) VALUES(?, ?, ?, ?, ?);";

    int resultado = SQLITE_ERROR;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, novoID, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, titulo, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, autor, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, ano, -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 5, disponibilidade);

        resultado = sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        if (resultado != SQLITE_DONE) {
            fprintf(stderr, "Erro ao inserir o livro: %s\n", sqlite3_errmsg(db));
            free(novoID);
            return NULL;
        }
    } else {
        fprintf(stderr, "Erro ao preparar o statement: %s\n", sqlite3_errmsg(db));
        free(novoID);
        return NULL;
    }

    printf("Livro cadastrado com ID: %s\n", novoID);
    return novoID;
}

// --------- Funções da tbl_Emprestimo ---------

char* regEmpLivro(sqlite3 *db, const char *data_emprestimo, const char *data_devolucao, const char *id_livro_fk, const char *id_usuario_fk) {
    if (!db || !data_emprestimo ||!data_devolucao || !id_livro_fk || !id_usuario_fk || strlen(data_emprestimo) == 0 || strlen(data_devolucao) == 0 || strlen(id_livro_fk) == 0 || strlen(id_usuario_fk) == 0) {
        fprintf(stderr, "Parâmetros inválidos para registrar o empréstimo do livro com ID: %s\n", id_livro_fk);
        return NULL;
    }

    char *novoID = genIdPers(db,"EMP", "tbl_Emprestimos", "ID_Emprestimo_PK", 3);
    if (!novoID) {
        fprintf(stderr, "Erro ao gerar novo ID para o empréstimo\n");
        return NULL;
    }

    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO tbl_Emprestimos(ID_Emprestimo_PK, Data_Emprestimo, Data_Devolucao, ID_Livro_FK, ID_Usuario_FK) VALUES(?, ?, ?, ?, ?);";

    int resultado = SQLITE_ERROR;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, novoID, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, data_emprestimo, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, data_devolucao, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, id_livro_fk, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 5, id_usuario_fk, -1, SQLITE_TRANSIENT);

        resultado = sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        if (resultado != SQLITE_DONE) {
            fprintf(stderr, "Erro ao inserir o livro: %s\n", sqlite3_errmsg(db));
            free(novoID);
            return NULL;
        }
    } else {
        fprintf(stderr, "Erro ao preparar o statement: %s\n", sqlite3_errmsg(db));
        free(novoID);
        return NULL;
    }

    printf("Empréstimo registrado com ID: %s\n", novoID);
    return novoID;
}

void consultEmp(sqlite3 *db) {
    const char *sql =
        "SELECT "
        "    tbl_Emprestimos.ID_Emprestimo_PK, "
        "    tbl_Emprestimos.ID_Livro_FK, "
        "    tbl_Emprestimos.ID_Usuario_FK, "
        "    tbl_Livros.Titulo, "
        "    tbl_Usuarios.Nome, "
        "    tbl_Usuarios.Sobrenome "
        "FROM tbl_Emprestimos "
        "INNER JOIN tbl_Livros "
        "    ON tbl_Emprestimos.ID_Livro_FK = tbl_Livros.ID_Livro_PK "
        "INNER JOIN tbl_Usuarios "
        "    ON tbl_Emprestimos.ID_Usuario_FK = tbl_Usuarios.ID_Usuario_PK;";

    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao preparar a consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("---------------------------------------------------------------------------------------------\n");
    printf("| %-12s | %-8s | %-10s | %-30s | %-12s | %-12s |\n",
           "ID Emprestimo", "ID Livro", "ID Usuário", "Título", "Nome", "Sobrenome");
    printf("---------------------------------------------------------------------------------------------\n");

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char *id_emprestimo_pk = sqlite3_column_text(stmt, 0);
        const unsigned char *id_livro_fk      = sqlite3_column_text(stmt, 1);
        const unsigned char *id_usuario_fk    = sqlite3_column_text(stmt, 2);
        const unsigned char *titulo           = sqlite3_column_text(stmt, 3);
        const unsigned char *nome             = sqlite3_column_text(stmt, 4);
        const unsigned char *sobrenome        = sqlite3_column_text(stmt, 5);

        printf("| %-12s | %-8s | %-10s | %-30s | %-12s | %-12s |\n",
               id_emprestimo_pk ? (const char*)id_emprestimo_pk : "",
               id_livro_fk ? (const char*)id_livro_fk : "",
               id_usuario_fk ? (const char*)id_usuario_fk : "",
               titulo ? (const char*)titulo : "",
               nome ? (const char*)nome : "",
               sobrenome ? (const char*)sobrenome : "");
    }

    printf("---------------------------------------------------------------------------------------------\n");

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Erro ao percorrer resultados: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}