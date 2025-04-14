#ifndef UI_APP_H
#define UI_APP_H

#include <gtk/gtk.h>
#include "ui_topbox.h"
#include "ui_home.h"
#include "ui_form.h"
#include "paths.h"

typedef struct {
  GtkWidget *window;
  GtkWidget *main_box;
  GtkWidget *stack;
  ST_TopBox top_box;
  ST_HomeUI home_ui;
  ST_FormUI form_ui;
}ST_AppWidgets;

void createApplication(GtkApplication *app, gpointer user_data);
void initAppWidgets(ST_AppWidgets *app_ui, GtkApplication *app);
void switchToForm(GtkButton *button, gpointer user_data);
void switchToHome(GtkButton *button, gpointer user_data);

#endif 
