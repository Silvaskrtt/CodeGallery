#include <gtk/gtk.h>

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *button;

  // Criando uma nova Janela, setando o nome e tamanho da janela!
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "BookStack");
  gtk_window_set_default_size (GTK_WINDOW (window), 1320, 720);
  gtk_window_present (GTK_WINDOW (window));

  // Cotainer que irá agrupar os botoes do sistema
  grid = gtk_grid_new ();

  gtk_window_set_child (GTK_WINDOW (window), grid);

  button = gtk_button_new_with_label ("Usúarios");

  // configurando a posiçao do botao
  gtk_grid_attach (GTK_GRID (grid), button, 0,0,1,1);

  gtk_window_set_child (GTK_WINDOW (window), grid);

  button = gtk_button_new_with_label ("Livros");

  gtk_grid_attach (GTK_GRID (grid), button, 1,0,1,1);

  gtk_window_set_child (GTK_WINDOW (window), grid);

  button = gtk_button_new_with_label ("Empréstimos");

  gtk_grid_attach (GTK_GRID (grid), button, 2,0,1,1);
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
