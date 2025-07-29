# Compilador e flags
CC = gcc
CFLAGS = -Iinclude -Wall -std=c11 `pkg-config --cflags gtk4`
LDFLAGS = -lsqlite3 `pkg-config --libs gtk4`

# Arquivos fonte
SRC = src/database.c src/main.c src/models.c src/gui.c

# Diretórios
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
EXEC = $(BUILD_DIR)/CodeGallery.exe

# Geração dos arquivos .o dentro de build/obj/
OBJ = $(patsubst src/%.c, $(OBJ_DIR)/%.o, $(SRC))

# Regra padrão
all: $(EXEC)

# Regra para gerar o executável
$(EXEC): $(OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CC) -o $@ $^ $(LDFLAGS)

# Regra para compilar arquivos .c para .o no diretório obj/
$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Limpar arquivos
clean:
	rm -rf $(BUILD_DIR)