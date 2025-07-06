#include "gtk/gtk.h"

#include "shaders_metadata.h"
#include <glib-object.h>
#include <glib.h>
#include <gtk/gtkdropdown.h>
#include <locale.h>

static void activate(GtkApplication *app, gpointer user_data);
static void gtk_dropdown_selected(GtkDropDown*, GParamSpec* , gpointer);
static void set_css();

int main(int argc, char** argv) {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    shaders_metadata_list_t *list = shaders_metadata_list_load_from_json("shaders.json");
    GtkApplication *app;
    int status;

    for (int i = 0; i < list->size; i++) {
        g_print("%s\t%s\n", list->item[i].display_name, list->item[i].shader_name);
    }

    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect(app, "activate", G_CALLBACK(activate), list);
    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    shaders_metadata_list_free(list);

    return status;
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder;
    GtkWidget *window;
    GtkWidget *scenas_dropdown;
    GtkStringList *model;
    GError *err = NULL;
    shaders_metadata_list_t *list;

    builder = gtk_builder_new();

    if (gtk_builder_add_from_file(builder, "./ui/interface.xml", &err)
        && (window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window")))
        && (scenas_dropdown = GTK_WIDGET(gtk_builder_get_object(builder, "scenas_dropdown")))
        && (model = gtk_string_list_new(NULL))
        && (list = (shaders_metadata_list_t *)user_data)
    ) {

        gtk_window_set_application(GTK_WINDOW(window), app);
        gtk_window_present(GTK_WINDOW(window));

        for (int i = 0; i < list->size; i++) {
            gtk_string_list_append(model, list->item[i].display_name);
        }

        gtk_drop_down_set_model(GTK_DROP_DOWN(scenas_dropdown), G_LIST_MODEL(model));
        g_signal_connect(scenas_dropdown, "notify::selected-item", G_CALLBACK(gtk_dropdown_selected), list);
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
    shaders_metadata_list_t *list = (shaders_metadata_list_t *)user_data;
    guint idx = gtk_drop_down_get_selected(self);
    shaders_metadata_list_set_selected_idx(list, idx);
    g_print("selected: %s\n", list->item[list->selected_idx].shader_name);
}

static void set_css() {
    GdkDisplay *display = gdk_display_get_default();
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "./styles/style.css");
    gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    g_object_unref(provider);
}
