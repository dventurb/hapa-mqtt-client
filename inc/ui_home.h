#ifndef UI_HOME_H
#define UI_HOME_H

#include <gtk/gtk.h>
#include "connection.h"
#include "topic.h"
#include "json_utils.h"

typedef struct _ST_HomeUI{
  GtkWidget *main_box;
  GtkWidget *box_connections;
  GtkWidget *box_topics;
  GtkSingleSelection *selection_connection;
  GtkSingleSelection *selection_topic;
  GtkWidget *scrolled_connection;
  GtkWidget *scrolled_topic;
  GtkWidget *list_view_connection;
  GtkWidget *list_view_topic;
  GtkListItemFactory *factory_connection;
  GtkListItemFactory *factory_topic;
  GtkWidget *label;
  GtkWidget *label_top;
  GListStore *connection_store;
  GListStore *topics_store;
  GtkWidget *scrolled_payload;
  GtkWidget *text_view;
  GtkWidget *entry_payload;
  GtkWidget *image;
  GtkGesture *gesture_send;
  GtkGesture *gesture_start;
}ST_HomeUI;

void initHomeUI(ST_HomeUI *home_ui);
void sendPayload(GtkGestureClick *gesture, int n_press, double x, double y, gpointer user_data);

#endif
