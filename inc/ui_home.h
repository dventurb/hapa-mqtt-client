#ifndef UI_HOME_H
#define UI_HOME_H

#include <gtk/gtk.h>

typedef struct {
  GtkWidget *paned;
}ST_HomeUI;

void initHomeUI(ST_HomeUI *home_ui);

#endif
