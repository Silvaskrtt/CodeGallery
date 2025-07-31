#include <gtk/gtk.h>
#include <glib/gstdio.h>

// Callback da aplicação
static void
activate(GtkApplication *app, gpointer user_data)
{
  GtkBuilder *builder = gtk_builder_new();
  GError *error = NULL;

  // Carrega interface do arquivo XML
  gtk_builder_add_from_file(builder, "src/builder.ui", &error);
  if (error) {
    g_error("Erro ao carregar builder.ui: %s", error->message);
  }

  // Obtém a janela principal
  GObject *window = gtk_builder_get_object(builder, "window");
  if (!GTK_IS_WINDOW(window)) {
    g_error("Falha ao carregar a janela principal.");
  }

  // Carrega o CSS
  GtkCssProvider *css = gtk_css_provider_new();
  gtk_css_provider_load_from_path(css, "assets/style.css");
  if (error) {
    g_warning("Erro ao carregar style.css: %s", error->message);
    g_clear_error(&error);
  }

  gtk_style_context_add_provider_for_display(
    gdk_display_get_default(),
    GTK_STYLE_PROVIDER(css),
    GTK_STYLE_PROVIDER_PRIORITY_USER
  );

  // Associa a janela à aplicação
  gtk_window_set_application(GTK_WINDOW(window), app);

  // Exibe a janela
  gtk_widget_set_visible(GTK_WIDGET(window), TRUE);

  // Acesso a botões
  GObject *usuarios_btn   = gtk_builder_get_object(builder, "usuarios");
  GObject *livros_btn     = gtk_builder_get_object(builder, "livros");
  GObject *emprestimo_btn = gtk_builder_get_object(builder, "emprestimo");

  // Aqui vou conectar os sinais aos botões
  // Exemplo:
  // g_signal_connect(usuarios_btn, "clicked", G_CALLBACK(funcao_callback), NULL);

  g_object_unref(builder);
}

int main(int argc, char **argv)
{
  #ifdef GTK_SRCDIR
    g_chdir(GTK_SRCDIR); // builddir
  #endif

  GtkApplication *app = gtk_application_new("org.gtk.bookstack", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

  int status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}