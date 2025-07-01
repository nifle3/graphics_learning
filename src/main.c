#include "stdio.h"

#include "gtk/gtk.h"
#include <gdk/gdk.h>
#include <glib-object.h>
#include <gtk/gtkcssprovider.h>

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

    GdkDisplay *display = gdk_display_get_default();
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "./styles/style.css");
    gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    g_object_unref(provider);
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
