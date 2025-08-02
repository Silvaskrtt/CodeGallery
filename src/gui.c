#include <gtk/gtk.h>
#include <glib/gstdio.h>
#include <database.h>

// --------- Funções Usuários para conexão do GTK com DB ---------

// Callback para cadastrar usuário ao clicar no botão "Usuários"
static void on_cadastrar_usuario(GtkButton *button, gpointer user_data) {
    GtkBuilder *builder = GTK_BUILDER(user_data);

    // Obtém referências para os campos de entrada
    GtkEntry *nome_entry = GTK_ENTRY(gtk_builder_get_object(builder, "nome_entry"));
    GtkEntry *sobrenome_entry = GTK_ENTRY(gtk_builder_get_object(builder, "Sobrenome_entry"));
    GtkEntry *cpf_entry = GTK_ENTRY(gtk_builder_get_object(builder, "CPF_entry"));

    // Verifica se todos os campos foram encontrados
    if (!nome_entry || !sobrenome_entry || !cpf_entry) {
        g_warning("Não foi possível obter todos os campos de entrada");
        return;
    }

    // Lê os textos inseridos nos campos
    const char *nome = gtk_editable_get_text(GTK_EDITABLE(nome_entry));
    const char *sobrenome = gtk_editable_get_text(GTK_EDITABLE(sobrenome_entry));
    const char *cpf = gtk_editable_get_text(GTK_EDITABLE(cpf_entry));

    // Verifica se todos os campos estão preenchidos
    if (!nome || !sobrenome || !cpf) {
        g_warning("Todos os campos devem ser preenchidos");
        return;
    }

    // Conecta ao banco de dados
    Database db = conectaDB("database/BOOKSTACK.db");
    if (db.status != 1) {
        g_warning("Falha ao conectar ao banco de dados");
        return;
    }

    // Insere novo usuário
    char *idUsuario = cadUser(db.db, nome, sobrenome, cpf);
    if (idUsuario) {
        g_print("Usuário cadastrado com ID: %s\n", idUsuario);
        free(idUsuario);
        
        // Limpa os campos após o cadastro
        gtk_editable_set_text(GTK_EDITABLE(nome_entry), "");
        gtk_editable_set_text(GTK_EDITABLE(sobrenome_entry), "");
        gtk_editable_set_text(GTK_EDITABLE(cpf_entry), "");
    } else {
        g_warning("Falha ao cadastrar usuário");
    }

    // Fecha conexão com o banco
    discDB(&db);
}

// Abre a janela de gerenciamento de usuários
static void on_users_clicked(GtkButton *button, gpointer user_data) {
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;
    
    // Carrega a interface de usuários a partir do arquivo .ui
    if (!gtk_builder_add_from_file(builder, "src/users_window.ui", &error)) {
        g_warning("Erro ao carregar users_window.ui: %s", error ? error->message : "Erro desconhecido");
        if (error) g_clear_error(&error);
        g_object_unref(builder);
        return;
    }

    // Obtém referência para a janela
    GtkWindow *users_window = GTK_WINDOW(gtk_builder_get_object(builder, "users_window"));
    if (!users_window) {
        g_warning("Falha ao obter a janela de usuários do builder");
        g_object_unref(builder);
        return;
    }
    
    // Mantém o builder vivo enquanto a janela existir
    g_object_set_data_full(G_OBJECT(users_window), "builder", builder, (GDestroyNotify)g_object_unref);

    // Conecta evento do botão de cadastro de usuário
    GtkButton *cad_btn = GTK_BUTTON(gtk_builder_get_object(builder, "cadastrar_usuario_btn"));
    if (cad_btn) {
        g_signal_connect(cad_btn, "clicked", G_CALLBACK(on_cadastrar_usuario), builder);
    }

    // Associa a janela à aplicação
    gtk_window_set_application(users_window, GTK_APPLICATION(user_data));
    gtk_window_present(users_window);
}

// --------- Funções Livros para conexão do GTK com DB ---------

// Callback para cadastrar livro
static void on_cadastrar_livros(GtkButton *button, gpointer user_data) {
    GtkBuilder *builder = GTK_BUILDER(user_data);

    // Obtém campos de entrada
    GtkEntry *titulo_entry = GTK_ENTRY(gtk_builder_get_object(builder, "titulo_entry"));
    GtkEntry *autor_entry = GTK_ENTRY(gtk_builder_get_object(builder, "autor_entry"));
    GtkEntry *ano_entry = GTK_ENTRY(gtk_builder_get_object(builder, "ano_entry"));
    GtkEntry *disponibilidade_entry = GTK_ENTRY(gtk_builder_get_object(builder, "disponibilidade_entry"));

    // Verifica se todos os campos foram encontrados
    if (!titulo_entry || !autor_entry || !ano_entry || !disponibilidade_entry) {
        g_warning("Não foi possível obter todos os campos de entrada");
        return;
    }

    // Obtém valores digitados
    const char *titulo = gtk_editable_get_text(GTK_EDITABLE(titulo_entry));
    const char *autor = gtk_editable_get_text(GTK_EDITABLE(autor_entry));
    const char *ano = gtk_editable_get_text(GTK_EDITABLE(ano_entry));
    const char *disp_text = gtk_editable_get_text(GTK_EDITABLE(disponibilidade_entry));

    // Verifica se os campos não estão vazios
    if (!titulo || !autor || !ano || !disp_text) {
        g_warning("Todos os campos devem ser preenchidos");
        return;
    }

    // Conecta ao banco
    Database db = conectaDB("database/BOOKSTACK.db");
    if (db.status != 1) {
        g_warning("Falha ao conectar ao banco de dados");
        return;
    }

    // Converte disponibilidade para inteiro
    int disp_int = atoi(disp_text);

    // Insere novo livro
    char *idLivro = addLivro(db.db, titulo, autor, ano, disp_int);

    if (idLivro) {
        g_print("Livro cadastrado com ID: %s\n", idLivro);
        free(idLivro);

        // Limpa campos
        gtk_editable_set_text(GTK_EDITABLE(titulo_entry), "");
        gtk_editable_set_text(GTK_EDITABLE(autor_entry), "");
        gtk_editable_set_text(GTK_EDITABLE(ano_entry), "");
        gtk_editable_set_text(GTK_EDITABLE(disponibilidade_entry), "");
    } else {
        g_warning("Falha ao cadastrar livro");
    }

    discDB(&db);
}

// Abre a janela de gerenciamento de livros
static void on_livros_clicked(GtkButton *button, gpointer user_data) {
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;
    
    // Carrega interface de livros
    if (!gtk_builder_add_from_file(builder, "src/book_window.ui", &error)) {
        g_warning("Erro ao carregar book_window.ui: %s", error ? error->message : "Erro desconhecido");
        if (error) g_clear_error(&error);
        g_object_unref(builder);
        return;
    }

    GtkWindow *book_window = GTK_WINDOW(gtk_builder_get_object(builder, "book_window"));
    if (!book_window) {
        g_warning("Falha ao obter a janela de livros do builder");
        g_object_unref(builder);
        return;
    }
    
    // Mantém builder vivo enquanto a janela existir
    g_object_set_data_full(G_OBJECT(book_window), "builder", builder, (GDestroyNotify)g_object_unref);

    // Conecta botão de cadastro de livros
    GtkButton *cad_btn = GTK_BUTTON(gtk_builder_get_object(builder, "cadastrar_livro_btn"));
    if (cad_btn) {
        g_signal_connect(cad_btn, "clicked", G_CALLBACK(on_cadastrar_livros), builder);
    }

    gtk_window_set_application(book_window, GTK_APPLICATION(user_data));
    gtk_window_present(book_window);
}

// --------- Funções Empréstimo para conexão do GTK com DB ---------

// Callback para cadastrar empréstimo
static void on_cadastrar_emprestimo(GtkButton *button, gpointer user_data) {
    GtkBuilder *builder = GTK_BUILDER(user_data);

    // Obtém campos de entrada
    GtkEntry *dtEmp_entry = GTK_ENTRY(gtk_builder_get_object(builder, "dtEmp_entry"));
    GtkEntry *dtDev_entry = GTK_ENTRY(gtk_builder_get_object(builder, "dtDev_entry"));
    GtkEntry *idlivro_entry = GTK_ENTRY(gtk_builder_get_object(builder, "idlivro_entry"));
    GtkEntry *iduser_entry = GTK_ENTRY(gtk_builder_get_object(builder, "iduser_entry"));

    // Verifica se todos foram encontrados
    if (!dtEmp_entry || !dtDev_entry || !idlivro_entry || !iduser_entry) {
        g_warning("Não foi possível obter todos os campos de entrada");
        return;
    }

    // Obtém valores
    const char *dtEmp = gtk_editable_get_text(GTK_EDITABLE(dtEmp_entry));
    const char *dtDev = gtk_editable_get_text(GTK_EDITABLE(dtDev_entry));
    const char *idlivro = gtk_editable_get_text(GTK_EDITABLE(idlivro_entry));
    const char *iduser = gtk_editable_get_text(GTK_EDITABLE(iduser_entry));

    // Verifica se estão preenchidos
    if (!dtEmp || !dtDev || !idlivro || !iduser) {
        g_warning("Todos os campos devem ser preenchidos");
        return;
    }

    // Conecta ao banco
    Database db = conectaDB("database/BOOKSTACK.db");
    if (db.status != 1) {
        g_warning("Falha ao conectar ao banco de dados");
        return;
    }

    // Registra empréstimo
    char *idEmp = regEmpLivro(db.db, dtEmp, dtDev, idlivro, iduser);
    if (idEmp) {
        g_print("Empréstimo cadastrado com ID: %s\n", idEmp);
        free(idEmp);
        
        // Limpa campos
        gtk_editable_set_text(GTK_EDITABLE(dtEmp_entry), "");
        gtk_editable_set_text(GTK_EDITABLE(dtDev_entry), "");
        gtk_editable_set_text(GTK_EDITABLE(idlivro_entry), "");
        gtk_editable_set_text(GTK_EDITABLE(iduser_entry), "");
    } else {
        g_warning("Falha ao cadastrar usuário");
    }

    discDB(&db);
}

// Abre a janela de empréstimos
static void on_emp_clicked(GtkButton *button, gpointer user_data) {
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;
    
    // Carrega interface de empréstimos
    if (!gtk_builder_add_from_file(builder, "src/emp_window.ui", &error)) {
        g_warning("Erro ao carregar emp_window.ui: %s", error ? error->message : "Erro desconhecido");
        if (error) g_clear_error(&error);
        g_object_unref(builder);
        return;
    }

    GtkWindow *emp_window = GTK_WINDOW(gtk_builder_get_object(builder, "emp_window"));
    if (!emp_window) {
        g_warning("Falha ao obter a janela de empréstimo do builder");
        g_object_unref(builder);
        return;
    }
    
    // Mantém builder vivo enquanto a janela existir
    g_object_set_data_full(G_OBJECT(emp_window), "builder", builder, (GDestroyNotify)g_object_unref);

    // Conecta botão de cadastro de empréstimo
    GtkButton *cad_btn = GTK_BUTTON(gtk_builder_get_object(builder, "cadastrar_emprestimo_btn"));
    if (cad_btn) {
        g_signal_connect(cad_btn, "clicked", G_CALLBACK(on_cadastrar_emprestimo), builder);
    }

    gtk_window_set_application(emp_window, GTK_APPLICATION(user_data));
    gtk_window_present(emp_window);
}

// Função principal de inicialização da aplicação
static void activate(GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;

    // Carrega interface principal
    if (!gtk_builder_add_from_file(builder, "src/main_window.ui", &error)) {
        g_error("Erro ao carregar main_window.ui: %s", error ? error->message : "Erro desconhecido");
        if (error) g_clear_error(&error);
        g_object_unref(builder);
        return;
    }

    // Obtém janela principal
    GtkWindow *window = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
    if (!window) {
        g_error("Falha ao carregar a janela principal.");
        g_object_unref(builder);
        return;
    }

    // Carrega CSS
    GtkCssProvider *css = gtk_css_provider_new();
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(css),
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );
    gtk_css_provider_load_from_path(css, "assets/style.css");

    // Associa janela à aplicação
    gtk_window_set_application(window, app);

    // Obtém e conecta botões principais
    GtkButton *livros_btn = GTK_BUTTON(gtk_builder_get_object(builder, "livros"));
    if (livros_btn) {
        g_signal_connect(livros_btn, "clicked", G_CALLBACK(on_livros_clicked), app);
    } else {
        g_warning("Botão 'livros' não encontrado na interface");
    }

    GtkButton *usuarios_btn = GTK_BUTTON(gtk_builder_get_object(builder, "usuarios"));
    if (usuarios_btn) {
        g_signal_connect(usuarios_btn, "clicked", G_CALLBACK(on_users_clicked), app);
    } else {
        g_warning("Botão 'usuarios' não encontrado na interface");
    }

    GtkButton *emprestimo_btn = GTK_BUTTON(gtk_builder_get_object(builder, "emprestimo"));
    if (emprestimo_btn) {
        g_signal_connect(emprestimo_btn, "clicked", G_CALLBACK(on_emp_clicked), app);
    } else {
        g_warning("Botão 'emprestimo' não encontrado na interface");
    }

    // Exibe janela principal
    gtk_widget_set_visible(GTK_WIDGET(window), TRUE);
    g_object_unref(builder);
}   

// Função main
int main(int argc, char **argv) {
    #ifdef GTK_SRCDIR
        g_chdir(GTK_SRCDIR); // Ajusta diretório de trabalho se compilado com GTK_SRCDIR
    #endif

    // Inicializa GTK
    if (!gtk_init_check()) {
        g_printerr("Falha ao inicializar GTK\n");
        return 1;
    }

    // Cria aplicação GTK
    GtkApplication *app = gtk_application_new("org.gtk.bookstack", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    // Executa aplicação
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    
    g_object_unref(app);
    return status;
}
