#include "ui_home.h"

static void setupFactoryConnection(GtkListItemFactory *factory, GtkListItem *item, gpointer user_data);
static void bindFactoryConnection(GtkListItemFactory *factory, GtkListItem *item, gpointer user_data); 
static void setupFactoryTopic(GtkListItemFactory *factory, GtkListItem *item, gpointer user_data);
static void bindFactoryTopic(GtkListItemFactory *factory, GtkListItem *item, gpointer user_data);
   
void initHomeUI(ST_HomeUI *home_ui){
  
  // MAIN BOX
  home_ui->main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_widget_set_size_request(home_ui->main_box, 1000, 525);
  gtk_widget_add_css_class(home_ui->main_box, "home_layout");
  
  // BOX CONNECTIONS 
  home_ui->box_connections = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_widget_set_size_request(home_ui->box_connections, 250, 500);
  gtk_widget_set_margin_start(home_ui->box_connections, 10);
  gtk_box_append(GTK_BOX(home_ui->main_box), home_ui->box_connections);
  gtk_widget_add_css_class(home_ui->box_connections, "home_box_connections");
  
  home_ui->label = gtk_label_new("Conexões");
  gtk_widget_add_css_class(home_ui->label, "home_label_connections");
  gtk_widget_set_margin_top(home_ui->label, 10);
  gtk_box_append(GTK_BOX(home_ui->box_connections), home_ui->label);
  
  home_ui->connection_store = g_list_store_new(ST_TYPE_MQTT_CONNECTION);

  // SELECTION MODEL - For connections. 
  home_ui->selection_connection = gtk_single_selection_new(G_LIST_MODEL(home_ui->connection_store));
 
  // FACTORY - Connections
  home_ui->factory_connection = gtk_signal_list_item_factory_new();
  g_signal_connect(home_ui->factory_connection, "setup", G_CALLBACK(setupFactoryConnection), home_ui);
  g_signal_connect(home_ui->factory_connection, "bind", G_CALLBACK(bindFactoryConnection), home_ui);

  // LIST VIEW - Connections
  home_ui->list_view_connection = gtk_list_view_new(GTK_SELECTION_MODEL(home_ui->selection_connection), home_ui->factory_connection);
  gtk_widget_add_css_class(home_ui->list_view_connection, "home_listview_connection");

  // SCROLLED - Connections 
  home_ui->scrolled_connection = gtk_scrolled_window_new();
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(home_ui->scrolled_connection), home_ui->list_view_connection);
  gtk_box_append(GTK_BOX(home_ui->box_connections), home_ui->scrolled_connection);
  gtk_widget_set_vexpand(home_ui->scrolled_connection, TRUE);
  
  loadJSONToHome(home_ui);
  home_ui->topics_store = g_list_store_new(ST_TYPE_MQTT_TOPIC);

  // BOX Right - Selected the topic and pub/sub payloads.
  GtkWidget *box_right = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_box_append(GTK_BOX(home_ui->main_box), box_right);

  GtkWidget *box_right_top = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_widget_set_size_request(box_right_top, -1, 50);
  gtk_widget_set_hexpand(box_right_top, TRUE);
  gtk_widget_add_css_class(box_right_top, "home_box_right_top");
  gtk_box_append(GTK_BOX(box_right), box_right_top);

  home_ui->label_top = gtk_label_new("");
  gtk_widget_add_css_class(home_ui->label_top, "home_box_right_top_label");
  gtk_box_append(GTK_BOX(box_right_top), home_ui->label_top);
  gtk_widget_set_margin_start(home_ui->label_top, 10);

  GtkWidget *box_right_bottom = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_box_append(GTK_BOX(box_right), box_right_bottom);
  
  // TOPICS 
  home_ui->box_topics = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_widget_set_size_request(home_ui->box_topics, 200, 400);
  gtk_box_append(GTK_BOX(box_right_bottom), home_ui->box_topics);
  
  home_ui->factory_topic = gtk_signal_list_item_factory_new();
  g_signal_connect(home_ui->factory_topic, "setup", G_CALLBACK(setupFactoryTopic), home_ui);
  g_signal_connect(home_ui->factory_topic, "bind", G_CALLBACK(bindFactoryTopic), home_ui);

  home_ui->selection_topic = gtk_single_selection_new(G_LIST_MODEL(home_ui->topics_store));
  g_signal_connect(home_ui->factory_topic, "setup", G_CALLBACK(setupFactoryTopic), home_ui);
  g_signal_connect(home_ui->factory_topic, "bind", G_CALLBACK(bindFactoryTopic), home_ui);

  home_ui->list_view_topic = gtk_list_view_new(GTK_SELECTION_MODEL(home_ui->selection_topic), home_ui->factory_topic);

  home_ui->scrolled_topic = gtk_scrolled_window_new();
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(home_ui->scrolled_topic), home_ui->list_view_topic);
  gtk_widget_set_vexpand(home_ui->scrolled_topic, TRUE);
  gtk_box_append(GTK_BOX(home_ui->box_topics), home_ui->scrolled_topic);

  GtkWidget *box_payload = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_box_append(GTK_BOX(box_right_bottom), box_payload);

  home_ui->text_view = gtk_text_view_new();
  gtk_text_view_set_editable(GTK_TEXT_VIEW(home_ui->text_view), FALSE);
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(home_ui->text_view), GTK_WRAP_WORD_CHAR);
  gtk_widget_add_css_class(home_ui->text_view, "home_textview");

  home_ui->scrolled_payload = gtk_scrolled_window_new();
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(home_ui->scrolled_payload), home_ui->text_view);
  gtk_widget_set_hexpand(home_ui->scrolled_payload, TRUE);
  gtk_widget_set_vexpand(home_ui->scrolled_payload, TRUE);
  gtk_widget_add_css_class(home_ui->scrolled_payload, "home_scrolled_payload");
  gtk_box_append(GTK_BOX(box_payload), home_ui->scrolled_payload);

  home_ui->entry_payload = gtk_text_view_new();
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(home_ui->entry_payload));
  gtk_text_buffer_set_text(buffer, "Hello", -1);
  gtk_widget_add_css_class(home_ui->entry_payload, "home_entry_payload");
  home_ui->scrolled_payload = gtk_scrolled_window_new();
  gtk_widget_add_css_class(home_ui->scrolled_payload, "home_scrolled_entry");
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(home_ui->scrolled_payload), home_ui->entry_payload);
  gtk_box_append(GTK_BOX(box_payload), home_ui->scrolled_payload);

  home_ui->image = gtk_image_new_from_file(SEND_PAYLOAD_PATH);
  gtk_widget_set_size_request(home_ui->image, 35, 35);
  gtk_widget_set_halign(home_ui->image, GTK_ALIGN_END);
  gtk_widget_set_margin_bottom(home_ui->image, 5);
  gtk_widget_add_css_class(home_ui->image, "home_image_send");
  gtk_box_append(GTK_BOX(box_payload), home_ui->image);
  home_ui->gesture_send = gtk_gesture_click_new();
  gtk_widget_add_controller(home_ui->image, GTK_EVENT_CONTROLLER(home_ui->gesture_send));
  g_signal_connect(home_ui->gesture_send, "pressed", G_CALLBACK(sendPayload), home_ui);
}

static void setupFactoryConnection(GtkListItemFactory *factory, GtkListItem *item, gpointer user_data){
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_widget_add_css_class(box, "home_listitem_connection");
  
  GtkWidget *label = gtk_label_new(NULL);
  gtk_widget_add_css_class(label, "home_listitem_connection_label");
  g_object_set_data(G_OBJECT(item), "name", label);
  gtk_box_append(GTK_BOX(box), label);

  gtk_list_item_set_child(item, box);
}

static void bindFactoryConnection(GtkListItemFactory *factory, GtkListItem *item, gpointer user_data){
  STMQTTConnection *connection = gtk_list_item_get_item(item);

  GtkWidget *label_name = g_object_get_data(G_OBJECT(item), "name");

 gtk_label_set_text(GTK_LABEL(label_name), stMQTTConnectionGetName(connection));

}

static void setupFactoryTopic(GtkListItemFactory *factory, GtkListItem *item, gpointer user_data){
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_widget_add_css_class(box, "home_listitem_topic");

  GtkWidget *label = gtk_label_new(NULL);
  gtk_widget_add_css_class(label, "home_listitem_topic_label");
  g_object_set_data(G_OBJECT(item), "name_topic", label);
  gtk_widget_set_margin_start(label, 5);
  gtk_box_append(GTK_BOX(box), label);

  label = gtk_label_new(NULL);
  gtk_widget_add_css_class(label, "home_listitem_topic_qos");
  g_object_set_data(G_OBJECT(item), "qos_topic", label);
  gtk_widget_set_margin_end(label, 1);
  gtk_box_append(GTK_BOX(box), label);

  gtk_list_item_set_child(item, box);
}

static void bindFactoryTopic(GtkListItemFactory *factory, GtkListItem *item, gpointer user_data){
  STMQTTTopic *topic = gtk_list_item_get_item(item);

  GtkWidget *label = g_object_get_data(G_OBJECT(item), "name_topic");
  gtk_label_set_text(GTK_LABEL(label), stMQTTTopicGetName(topic));
  
  label = g_object_get_data(G_OBJECT(item), "qos_topic");
  gtk_label_set_text(GTK_LABEL(label), stMQTTTopicGetQoS(topic));
}

void sendPayload(GtkGestureClick *gesture, int n_press, double x, double y, gpointer user_data){
  ST_HomeUI *home_ui = (ST_HomeUI *)user_data;

  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(home_ui->entry_payload));
  GtkTextIter start, end;
  gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
  
  char *payload = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
  
  free(payload);
}
