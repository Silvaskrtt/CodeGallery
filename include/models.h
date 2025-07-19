#ifndef MODELS_H
#define MODELS_H

// STRUC para representa o Usuario
typedef struct {
    char ID_Usuario_PK[10];
    char Nome[100];
    char Sobrenome[100];
    char CPF[14];
} tbl_Usuarios;

// STRUC para representa o Telefone do Usuario
typedef struct {
    char ID_Telefone_PK[10];
    char NumTelefone[14];
    char ID_Usuario_FK[10];
} tbl_TelefoneUsuario;

// STRUC para representa o Livro
typedef struct {
    char ID_Livro[10];
    char Titulo[100];
    char Autor[100];
    char Ano[4];
    int disponibilidade;
} tbl_Livros;

// STRUC para representa o Emprestimo
typedef struct{
    char ID_Emprestimo_PK[10];
    char Data_Emprestimo[11];
    char Data_Devolucao[11];
    char ID_Livro_FK[11];
    char ID_Usuario_FK[11];
} tbl_Emprestimos;

#endif
