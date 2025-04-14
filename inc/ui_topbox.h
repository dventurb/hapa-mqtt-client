#ifndef UI_TOPBOX_H
#define UI_TOPBOX_H

#include <gtk/gtk.h>
#include "widgets.h"
#include "paths.h"

typedef struct {
  GtkWidget *box;
  GtkWidget *button_menu;
  GtkWidget *search_box;
  GtkWidget *search_entry;
  GtkWidget *image;
  GtkWidget* label;
  ST_BUTTON button_switch;
}ST_TopBox;

void initTopBox(ST_TopBox *top_box);

#endif
