#include "stdio.h"

#include "gtk/gtk.h"

static void activate(GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder;
    GtkWidget *window;
    GError *err = NULL;

    builder = gtk_builder_new();

    if (!gtk_builder_add_from_file(builder, "./ui/interface.xml", &err)) {
        g_printerr("%s", err->message);
        g_clear_error(&err);
        return;
    }

    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    if (!window) {
        g_printerr("Cannot get window");
        g_object_unref(builder);
        return;
    }

    gtk_window_set_application(GTK_WINDOW(window), app);
    gtk_window_present(GTK_WINDOW(window));
    g_object_unref(builder);
}

int main(int argc, char** argv) {
    printf("Hello world from c\n");
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
