#ifndef UI_FORM_H
#define UI_FORM_H 

#include <gtk/gtk.h>
#include "connection.h"
#include "topic.h"
#include "json_utils.h"
#include "widgets.h"
#include "paths.h"
#include "aux.h"
#include "ui_topics.h"

typedef struct _ST_FormUI {
  GtkWidget *main_box;
  GtkWidget *fixed;
  GtkWidget *stack;
  GtkWidget *label;
  GtkWidget *image;
  GtkWidget *box_connections;
  GtkWidget *entry_name;
  GtkWidget *entry_host;
  GtkWidget *entry_port;
  GtkWidget *entry_username;
  GtkWidget *entry_password;
  GtkWidget *dropdown_protocol;
  GtkWidget *list_view;
  GtkWidget *scrolled;
  GtkEntryBuffer *buffer;
  GtkGesture *gesture;
  GtkListItemFactory *factory;
  GtkSingleSelection *selection_model;
  GListStore *connection_store;
  ST_BUTTON button_save;
  ST_BUTTON button_delete;
  ST_BUTTON button_connect;
  ST_BUTTON button_options;
  ST_TopicsUI topics_ui;
}ST_FormUI;

void initFormUI(ST_FormUI *form_ui);
void selectionChanged(GtkSelectionModel *selection_model, int position, int n_items, gpointer user_data);
void setupFactory(GtkListItemFactory *factory, GtkListItem *item, gpointer user_data);
void bindFactory(GtkListItemFactory *factory, GtkListItem *item, gpointer user_data);
void addNewConnections(GtkGestureClick *gesture, int n_press, double x, double y, gpointer user_data);
void entryNameChanged(GtkEntry *entry_name, gpointer user_data);
void entryPortChanged(GtkEntry *entry_port, gpointer user_data);
void dropdownProtocolChanged(GObject *object, GParamSpec *pspec, gpointer user_data);
void entryHostChanged(GtkEntry *entry_host, gpointer user_data);
void entryUsernameChanged(GtkEntry *entry_username, gpointer user_data);
void entryPasswordChanged(GtkEntry *entry_password, gpointer user_data);
void saveConnection(GtkButton *button, gpointer user_data);
void deleteConnection(GtkButton *button, gpointer user_data);
void switchToTopics(GtkButton *button, gpointer user_data);

#endif
