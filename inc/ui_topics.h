#ifndef UI_TOPICS_H
#define UI_TOPICS_H

#include <gtk/gtk.h>
#include "widgets.h"
#include "paths.h"
#include "topic.h"
#include "connection.h"
#include "json_utils.h"

typedef struct {
  GtkWidget *stack;
  GtkWidget *fixed_form;
  GtkWidget *fixed_certificates;
  GtkWidget *label;
  GtkWidget *entry_topic;
  GtkWidget *entry_id;
  GtkWidget *dropdown_qos;
  GtkWidget *box_topics;
  GtkWidget *box_top;
  GtkWidget *list_view;
  GtkWidget *scrolled;
  GtkListItemFactory *factory;
  GListStore *topics_store;
  GtkNoSelection *no_selection;
  GtkWidget *popover;
  ST_BUTTON button_add;
  ST_BUTTON button_back;
  ST_BUTTON button_certificates;
  ST_BUTTON button_delete;
  ST_BUTTON button_server_certificate;
  ST_BUTTON button_client_certificate;
  ST_BUTTON button_client_key;
  ST_BUTTON button_certificates_back;
  STMQTTConnection *connection;
}ST_TopicsUI;

void initTopicsUI(ST_TopicsUI *topics_ui, GtkWidget *stack);
void addNewTopic(GtkButton *button, gpointer user_data);
void entryIDChanged(GtkEntry *entry_id, gpointer user_data);
void deleteTopic(GtkGestureClick *gesture, int n_press, double  x, double y, gpointer user_data);
void switchToConnection(GtkButton *button, gpointer user_data);
void goToCertificates(GtkButton *button, gpointer user_data);
void goToTopics(GtkButton *button, gpointer user_data);

#endif
