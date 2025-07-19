PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE tbl_Usuarios(
    ID_Usuario_PK VARCHAR(10) NOT NULL PRIMARY KEY, 
    Nome VARCHAR(100),
    Sobrenome VARCHAR(100), 
    CPF VARCHAR(14)
    );

CREATE TABLE tbl_TelefoneUsuario(
    ID_Telefone_PK VARCHAR(10) NOT NULL PRIMARY KEY,
    NumTelefone VARCHAR(14),
    ID_Usuario_FK VARCHAR(10),
    FOREIGN KEY(ID_Usuario_FK) REFERENCES tbl_Usuarios(ID_Usuario_PK)
);

CREATE TABLE tbl_Livros(
    ID_Livro_PK VARCHAR(10) NOT NULL PRIMARY KEY,
    Titulo VARCHAR(100),
    Autor VARCHAR(100),
    Ano VARCHAR(4)
);

CREATE TABLE tbl_Emprestimos(
    ID_Emprestimo_PK VARCHAR(10) NOT NULL PRIMARY KEY,
    Data_Emprestimo DATE,
    Data_Devolucao DATE,
    ID_Livro_FK VARCHAR(10),
    ID_Usuario_FK VARCHAR(10),
    FOREIGN KEY(ID_Livro_FK) REFERENCES tbl_Livros(ID_Livro_PK),
    FOREIGN KEY(ID_Usuario_FK) REFERENCES tbl_Usuarios(ID_Usuario_PK)
);
COMMIT;
