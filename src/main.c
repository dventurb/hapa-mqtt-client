#include <gtk/gtk.h>
#include "ui_app.h"

int main(int argc, char **argv){
  ST_AppWidgets app_ui;
  GtkApplication *app = gtk_application_new("com.iot.hapa", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(createApplication), &app_ui);
  int status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  return status;
}
