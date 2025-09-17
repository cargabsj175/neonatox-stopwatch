// src/main.c - Cronómetro NeonatoX
// GTK 4 + C | Compatible con GTK 4.0+ | Menú con botones, acerca de, marcas
#include <gtk/gtk.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LAPS 100

// Estructura de la app
typedef struct {
    GtkWidget *window;
    GtkWidget *label_time;
    GtkWidget *button_start;
    GtkWidget *button_stop;
    GtkWidget *button_lap;
    GtkWidget *button_reset;
    GtkWidget *listbox_laps;
    GtkApplication *app;  // Referencia a la app para usar en señales

    gboolean running;
    GTimer *timer;
    guint tick_source;

    // Almacenar marcas
    char laps[MAX_LAPS][32];
    int lap_count;
} AppData;

// Formatear tiempo: ms -> mm:ss.mmm
void format_time(guint64 ms, char *buffer, size_t size) {
    int minutes = (ms / 1000) / 60;
    int seconds = (ms / 1000) % 60;
    int millis = ms % 1000;
    g_snprintf(buffer, size, "%02d:%02d.%03d", minutes, seconds, millis);
}

// Actualizar la etiqueta del tiempo
gboolean update_display(gpointer user_data) {
    AppData *app = (AppData *)user_data;
    if (!app->running) return G_SOURCE_CONTINUE;

    guint64 elapsed = g_timer_elapsed(app->timer, NULL) * 1000;
    char buffer[64];
    format_time(elapsed, buffer, sizeof(buffer));
    gtk_label_set_text(GTK_LABEL(app->label_time), buffer);

    return G_SOURCE_CONTINUE;
}

// Botón: Iniciar
void on_start_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    AppData *app = (AppData *)user_data;
    if (!app->running) {
        app->running = TRUE;
        g_timer_continue(app->timer);
        app->tick_source = g_timeout_add(10, update_display, app);
        gtk_widget_set_sensitive(app->button_start, FALSE);
        gtk_widget_set_sensitive(app->button_stop, TRUE);
        gtk_widget_set_sensitive(app->button_lap, TRUE);
    }
}

// Botón: Detener
void on_stop_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    AppData *app = (AppData *)user_data;
    if (app->running) {
        app->running = FALSE;
        g_timer_stop(app->timer);
        if (app->tick_source > 0) {
            g_source_remove(app->tick_source);
            app->tick_source = 0;
        }
        gtk_widget_set_sensitive(app->button_start, TRUE);
        gtk_widget_set_sensitive(app->button_stop, FALSE);
        gtk_widget_set_sensitive(app->button_lap, FALSE);
    }
}

// Botón: Marca (Lap)
void on_lap_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    AppData *app = (AppData *)user_data;
    if (app->running && app->lap_count < MAX_LAPS) {
        guint64 elapsed = g_timer_elapsed(app->timer, NULL) * 1000;
        char buffer[64];
        format_time(elapsed, buffer, sizeof(buffer));

        // Guardar marca
        snprintf(app->laps[app->lap_count], 32, "Vuelta %d: %s", app->lap_count + 1, buffer);
        app->lap_count++;

        // Mostrar en listbox
        GtkWidget *row = gtk_list_box_row_new();
        GtkWidget *label = gtk_label_new(app->laps[app->lap_count - 1]);
        gtk_widget_set_halign(label, GTK_ALIGN_START);
        gtk_widget_set_margin_top(label, 4);
        gtk_widget_set_margin_bottom(label, 4);
        gtk_widget_set_margin_start(label, 8);
        gtk_widget_set_margin_end(label, 8);
        gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row), label);
        gtk_list_box_insert(GTK_LIST_BOX(app->listbox_laps), row, -1);
    }
}

// Botón: Reiniciar
void on_reset_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    AppData *app = (AppData *)user_data;
    app->running = FALSE;
    g_timer_reset(app->timer);
    g_timer_stop(app->timer);

    if (app->tick_source > 0) {
        g_source_remove(app->tick_source);
        app->tick_source = 0;
    }

    gtk_label_set_text(GTK_LABEL(app->label_time), "00:00.000");
    gtk_widget_set_sensitive(app->button_start, TRUE);
    gtk_widget_set_sensitive(app->button_stop, FALSE);
    gtk_widget_set_sensitive(app->button_lap, FALSE);

    // Limpiar lista de marcas
    gtk_list_box_remove_all(GTK_LIST_BOX(app->listbox_laps));
    app->lap_count = 0;
}

// Función compatible con GTK 4.0+: eliminar todas las filas
void gtk_list_box_remove_all(GtkListBox *box) {
    GtkListBoxRow *row;
    while ((row = gtk_list_box_get_row_at_index(box, 0)) != NULL) {
        gtk_list_box_remove(box, GTK_WIDGET(row));
    }
}

// Acción: Mostrar "Acerca de"
void activate_about(GtkButton *button, gpointer user_data) {
    (void)button;
    AppData *app = (AppData *)user_data;

    const gchar *authors[] = { "Equipo NeonatoX", NULL };

    gtk_show_about_dialog(
        GTK_WINDOW(app->window),
        "program-name", "Cronómetro NeonatoX",
        "version", "1.0.0",
        "copyright", "© 2025 NeonatoX",
        "comments", "Un cronómetro simple con marcas de tiempo.",
        "website", "https://neonatox.example.com",
        "website-label", "Sitio web de NeonatoX",
        "authors", authors,
        "logo-icon-name", "neonatox-stopwatch",
        NULL
    );
}

// Inicializar interfaz y menú
static void activate(GtkApplication *app, gpointer user_data) {
    (void)user_data;

    AppData *data = g_new0(AppData, 1);
    data->running = FALSE;
    data->timer = g_timer_new();
    data->tick_source = 0;
    data->lap_count = 0;
    data->app = app;
    data->window = gtk_application_window_new(app);

    // === HEADER BAR CON MENÚ ===
    GtkHeaderBar *header_bar = GTK_HEADER_BAR(gtk_header_bar_new());
    gtk_window_set_titlebar(GTK_WINDOW(data->window), GTK_WIDGET(header_bar));
    gtk_window_set_title(GTK_WINDOW(data->window), "Cronómetro");

    // Botón de menú
    GtkMenuButton *menu_button = GTK_MENU_BUTTON(gtk_menu_button_new());
    GtkPopover *popover = GTK_POPOVER(gtk_popover_new());

    // Contenido del menú: GtkBox con botones
    GtkWidget *menu_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_margin_start(menu_box, 10);
    gtk_widget_set_margin_end(menu_box, 10);
    gtk_widget_set_margin_top(menu_box, 10);
    gtk_widget_set_margin_bottom(menu_box, 10);

    // Botón "Acerca de"
    GtkWidget *btn_about = gtk_button_new_with_label("Acerca de");
    g_signal_connect(btn_about, "clicked", G_CALLBACK(activate_about), data);
    gtk_box_append(GTK_BOX(menu_box), btn_about);

    // Botón "Salir"
    GtkWidget *btn_quit = gtk_button_new_with_label("Salir");
    g_signal_connect_swapped(btn_quit, "clicked", G_CALLBACK(g_application_quit), app);
    gtk_box_append(GTK_BOX(menu_box), btn_quit);

    // Configurar popover
    gtk_popover_set_child(popover, menu_box);
    gtk_menu_button_set_popover(menu_button, GTK_WIDGET(popover));
    gtk_header_bar_pack_end(header_bar, GTK_WIDGET(menu_button));

    // Contenedor principal
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_start(vbox, 10);
    gtk_widget_set_margin_end(vbox, 10);
    gtk_widget_set_margin_top(vbox, 10);
    gtk_widget_set_margin_bottom(vbox, 10);
    gtk_window_set_child(GTK_WINDOW(data->window), vbox);

    // Etiqueta de tiempo
    data->label_time = gtk_label_new("00:00.000");
    gtk_label_set_width_chars(GTK_LABEL(data->label_time), 15);
    gtk_widget_add_css_class(data->label_time, "title-1");
    gtk_box_append(GTK_BOX(vbox), data->label_time);

    // Botones
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    data->button_start = gtk_button_new_with_label("Iniciar");
    data->button_stop = gtk_button_new_with_label("Detener");
    data->button_lap = gtk_button_new_with_label("Marca");
    data->button_reset = gtk_button_new_with_label("Reiniciar");

    gtk_widget_set_sensitive(data->button_stop, FALSE);
    gtk_widget_set_sensitive(data->button_lap, FALSE);

    gtk_box_append(GTK_BOX(hbox), data->button_start);
    gtk_box_append(GTK_BOX(hbox), data->button_stop);
    gtk_box_append(GTK_BOX(hbox), data->button_lap);
    gtk_box_append(GTK_BOX(hbox), data->button_reset);

    gtk_box_append(GTK_BOX(vbox), hbox);

    // Lista de marcas con scroll
    data->listbox_laps = gtk_list_box_new();
    gtk_widget_set_vexpand(data->listbox_laps, TRUE);

    GtkWidget *scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), data->listbox_laps);
    gtk_box_append(GTK_BOX(vbox), scrolled);

    // Conectar señales
    g_signal_connect(data->button_start, "clicked", G_CALLBACK(on_start_clicked), data);
    g_signal_connect(data->button_stop, "clicked", G_CALLBACK(on_stop_clicked), data);
    g_signal_connect(data->button_lap, "clicked", G_CALLBACK(on_lap_clicked), data);
    g_signal_connect(data->button_reset, "clicked", G_CALLBACK(on_reset_clicked), data);

    // Mostrar ventana
    gtk_window_present(GTK_WINDOW(data->window));
}

// Punto de entrada
int main(int argc, char *argv[]) {
    GtkApplication *app = gtk_application_new("com.neonatox.stopwatch", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
