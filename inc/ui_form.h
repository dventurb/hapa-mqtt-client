#ifndef UI_FORM_H
#define UI_FORM_H 

#include <gtk/gtk.h>
#include "topic.h"
#include "json_utils.h"
#include "widgets.h"
#include "paths.h"
#include "aux.h"
#include "ui_connections.h"
#include "ui_topics.h"

typedef struct _ST_FormUI {
  GtkWidget *main_box;
  GtkWidget *fixed;
  GtkWidget *stack;
  GtkWidget *label;
  GtkWidget *image;
  GtkWidget *box_connections;
  GtkWidget *list_view;
  GtkWidget *scrolled;
  GtkEntryBuffer *buffer;
  GtkGesture *gesture;
  GtkListItemFactory *factory;
  ST_ConnectionsUI connections_ui;
  ST_TopicsUI topics_ui;
}ST_FormUI;

typedef struct _STMQTTConnection _STMQTTConnection;

void initFormUI(ST_FormUI *form_ui);
void selectionChanged(GtkSelectionModel *selection_model, int position, int n_items, gpointer user_data);
void addNewConnections(GtkGestureClick *gesture, int n_press, double x, double y, gpointer user_data);

#endif
