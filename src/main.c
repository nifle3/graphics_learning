#include "gtk/gtk.h"
#include <gdk/gdk.h>
#include <glib-object.h>
#include <glib.h>
#include <gmodule.h>
#include <gtk/gtkcssprovider.h>
#include <gtk/gtkdropdown.h>
#include <gtk/gtkshortcut.h>

#include "shader_info.h"

static void activate(GtkApplication *app, gpointer user_data);
static void gtk_dropdown_selected(GtkDropDown*, GParamSpec* , gpointer);
static void set_css();

int main(int argc, char** argv) {
    GtkApplication *app;
    shaders_array_t *shaders;
    int status;
    optional_shaders_array_t optional_shaders_array = create_shaders_array_from_json_file("shaders.json");

    if (is_shaders_array_t_ok(&optional_shaders_array)) {
        shaders = optional_shaders_array.t;

        for (int i = 0; i < shaders->size; i++) {
            g_print("shaders: name: %s, file_name %s\n", shaders->shader_info[i].display_name, shaders->shader_info[i].shader_file);
        }

        app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
        g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
        status = g_application_run(G_APPLICATION(app), argc, argv);
    } else {
        status = -1;
        g_printerr("Invalid json file");
    }

    if (shaders) {
        free_shaders_array(shaders);
    }
    if (app) {
        g_object_unref(app);
    }

    return status;
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder;
    GtkWidget *window;
    GtkWidget *scenas_dropdown;
    GError *err = NULL;

    builder = gtk_builder_new();

    if (gtk_builder_add_from_file(builder, "./ui/interface.xml", &err)
        && (window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window")))
        && (scenas_dropdown = GTK_WIDGET(gtk_builder_get_object(builder, "scenas_dropdown")))
    ) {

        gtk_window_set_application(GTK_WINDOW(window), app);
        gtk_window_present(GTK_WINDOW(window));

        g_signal_connect(scenas_dropdown, "notify::selected-item", G_CALLBACK(gtk_dropdown_selected), window);
        g_object_notify(G_OBJECT(scenas_dropdown), "selected-item");

        set_css();
    }

    if (err) {
        g_printerr("%s", err->message);
        g_clear_error(&err);
    }

    if (builder) {
        g_object_unref(builder);
    }

    if (!window) {
        g_printerr("Cannot get window");
    }

    if (!scenas_dropdown) {
        g_printerr("Canno get scenas_dropdown");
    }
}

static void gtk_dropdown_selected(GtkDropDown* self, GParamSpec* pspec, gpointer user_data) {
    g_print("hello from selected\n");
}

static void set_css() {
    GdkDisplay *display = gdk_display_get_default();
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "./styles/style.css");
    gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    g_object_unref(provider);
}
