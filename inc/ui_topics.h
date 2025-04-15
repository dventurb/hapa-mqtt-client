#ifndef UI_TOPICS_H
#define UI_TOPICS_H

#include <gtk/gtk.h>
#include "widgets.h"
#include "paths.h"
#include "topic.h"

typedef struct {
  GtkWidget *fixed;
  GtkWidget *label;
  GtkWidget *entry_topic;
  GtkWidget *dropdown_qos;
  GtkWidget *list_view;
  GtkWidget *scrolled;
  GtkListItemFactory *factory;
  GListStore *topics_store;
  GtkNoSelection *no_selection;
  ST_BUTTON button_add;
  ST_BUTTON button_back;
  ST_BUTTON button_certificate;
  ST_BUTTON button_delete;
}ST_TopicsUI;

void initTopicsUI(ST_TopicsUI *topics_ui);
void addTopic(GtkButton *button, gpointer user_data);
void setupFactory1(GtkListItemFactory *factory, GtkListItem *item, gpointer user_data);
void bindFactory1(GtkListItemFactory *factory, GtkListItem *item, gpointer user_data);

#endif
