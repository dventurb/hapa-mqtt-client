#ifndef UI_TOPICS_H
#define UI_TOPICS_H

#include <gtk/gtk.h>
#include "widgets.h"
#include "paths.h"

typedef struct {
  GtkWidget *fixed;
  GtkWidget *label
  GtkWidget *entry_topic;
  GtkWidget *dropdown_qos;
  GtkWidget *entry_id;
  GtkWidget *box_topics;
  GtkWidget *list_view;
  GtkWidget *scrolled;
  GtkListItemFactory *factory;
  GListStore *topics_store;
  ST_BUTTON *button_add;
  ST_BUTTON *button_back;
  ST_BUTTON *button_certificate;
  ST_BUTTON *button_delete;
}ST_TopicsUI;

void initTopicsUI(ST_TopicsUI *topics_ui);

#endif
