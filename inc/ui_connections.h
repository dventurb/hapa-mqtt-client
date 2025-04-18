#ifndef UI_CONNECTIONS_H
#define UI_CONNECTIONS_H

#include <gtk/gtk.h>
#include "widgets.h"
#include "connection.h"
#include "paths.h"
#include "json_utils.h"

typedef struct _ST_ConnectionsUI {
  GListStore *connection_store;
  GtkSingleSelection *selection_model;
  GtkWidget *fixed;
  GtkWidget *label;
  GtkWidget *switch_certificate;
  GtkWidget *switch_encryption;
  GtkWidget *entry_name;
  GtkWidget *entry_host;
  GtkWidget *entry_port;
  GtkWidget *entry_username;
  GtkWidget *entry_password;
  GtkWidget *dropdown_protocol;
  GtkEntryBuffer *buffer;
  ST_BUTTON button_save;
  ST_BUTTON button_delete;
  ST_BUTTON button_connect;
  ST_BUTTON button_options;
}ST_ConnectionsUI;

void initConnectionsUI(ST_ConnectionsUI *connections_ui, GtkWidget *stack);
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
