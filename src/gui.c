#include <gtk/gtk.h>
#include <glib/gstdio.h>

// Função callback para o botão "Livros"
static void on_livros_clicked(GtkButton *button, gpointer user_data) {
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;
    
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

    gtk_window_set_application(book_window, GTK_APPLICATION(user_data));
    gtk_window_present(book_window);

    g_object_unref(builder);
}

// Callback da aplicação
static void activate(GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;

    // Carrega interface do arquivo XML
    if (!gtk_builder_add_from_file(builder, "src/main_window.ui", &error)) {
        g_error("Erro ao carregar main_window.ui: %s", error ? error->message : "Erro desconhecido");
        if (error) g_clear_error(&error);
        g_object_unref(builder);
        return;
    }

    // Obtém a janela principal
    GtkWindow *window = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
    if (!window) {
        g_error("Falha ao carregar a janela principal.");
        g_object_unref(builder);
        return;
    }

    // Carrega o CSS
    GtkCssProvider *css = gtk_css_provider_new();

    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(css),
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );

    // Associa a janela à aplicação
    gtk_window_set_application(window, app);
    gtk_css_provider_load_from_path(css, "assets/style.css");
    if (error) {
      g_warning("Erro ao carregar style.css: %s", error->message);
      g_clear_error(&error);
    }


    // Exibe a janela
    gtk_widget_set_visible(GTK_WIDGET(window), TRUE);

    // Acesso a botões com verificações
    GtkButton *livros_btn = GTK_BUTTON(gtk_builder_get_object(builder, "livros"));
    if (livros_btn) {
        g_signal_connect(livros_btn, "clicked", G_CALLBACK(on_livros_clicked), app);
    } else {
        g_warning("Botão 'livros' não encontrado na interface");
    }

    // Verifique também os outros botões se for usá-los
    GtkButton *usuarios_btn = GTK_BUTTON(gtk_builder_get_object(builder, "usuarios"));
    GtkButton *emprestimo_btn = GTK_BUTTON(gtk_builder_get_object(builder, "emprestimo"));

    g_object_unref(builder);
}

int main(int argc, char **argv) {
    #ifdef GTK_SRCDIR
        g_chdir(GTK_SRCDIR);
    #endif

    GtkApplication *app = gtk_application_new("org.gtk.bookstack", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}