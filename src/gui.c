#include <gtk/gtk.h>
#include <glib/gstdio.h>

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  // Instancia GtkBuilder e carrega o UI
  GtkBuilder *builder = gtk_builder_new ();
  GError *error = NULL;

  gtk_builder_add_from_file(builder, "src/builder.ui", &error);
  if (error) {
    g_error("Erro ao carregar builder.ui: %s", error->message);
  }

  GObject *window = gtk_builder_get_object (builder, "window");
  if (!GTK_IS_WINDOW(window)) {
    g_error("Falha ao carregar a janela principal.");
  }
  gtk_window_set_application (GTK_WINDOW (window), app);

  GObject *button = gtk_builder_get_object (builder, "usuarios");
  if (!GTK_IS_BUTTON(button)) {
    g_warning("Botão 'usuarios' não encontrado.");
  }

  gtk_widget_set_visible (GTK_WIDGET (window), TRUE);

  g_object_unref (builder);
}

int
main (int    argc,
      char **argv)
{
  #ifdef GTK_SRCDIR
    g_chdir (GTK_SRCDIR);
  #endif

  GtkApplication *app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  
  int status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}