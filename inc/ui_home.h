#ifndef UI_HOME_H
#define UI_HOME_H

#include <gtk/gtk.h>

typedef struct {
  GtkWidget *paned; 
  GtkWidget *box_connections;
  GtkWidget *box_topics;
  GtkWidget *scrolled;
  GtkWidget *list_view;
  GtkWidget *tree_model;
  GtkWidget *label
}ST_HomeUI;

void initHomeUI(ST_HomeUI *home_ui);

#endif
