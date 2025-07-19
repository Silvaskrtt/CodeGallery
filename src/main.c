#include "database.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    // Conectar
    Database banco = conectar_banco("database/CODEGALLERY.db");
    if (banco.status != 1) {
        printf("Erro ao conectar! Código: %d\n", banco.status);
        return 1; // Falha ao conectar
    }

    printf("Banco conectado!\n");
    fflush(stdout);

    // Desconectar do DB
    desconectar_banco(&banco);

    // Pausa para o programa nao fechar
    system("pause");
    return 0;
}