#include "ui_topics.h"

static void setupFactory(GtkListItemFactory *factory, GtkListItem *item, gpointer user_data);
static void bindFactory(GtkListItemFactory *factory, GtkListItem *item, gpointer user_data);

void initTopicsUI(ST_TopicsUI *topics_ui, GtkWidget *stack){
  topics_ui->stack = gtk_stack_new();
  gtk_stack_set_transition_type(GTK_STACK(topics_ui->stack), GTK_STACK_TRANSITION_TYPE_SLIDE_UP);

  /* --------------------------- TOPICS FORM SECTION --------------------------- */

  topics_ui->fixed_form = gtk_fixed_new();  
  gtk_stack_add_named(GTK_STACK(topics_ui->stack), topics_ui->fixed_form, "topics");
  
  // TEXT 
  topics_ui->label = gtk_label_new("Conexão MQTT");
  gtk_widget_add_css_class(topics_ui->label, "topics_label_title");
  gtk_fixed_put(GTK_FIXED(topics_ui->fixed_form), topics_ui->label, 10, 30);

  // TOPIC (NAME)
  topics_ui->label = gtk_label_new("Tópico");
  gtk_widget_add_css_class(topics_ui->label, "topics_label_topic");
  gtk_fixed_put(GTK_FIXED(topics_ui->fixed_form), topics_ui->label, 30, 90);
  topics_ui->entry_topic = gtk_entry_new();
  gtk_widget_set_size_request(topics_ui->entry_topic, 400, -1);
  gtk_widget_add_css_class(topics_ui->entry_topic, "topics_entry_topic");
  gtk_fixed_put(GTK_FIXED(topics_ui->fixed_form), topics_ui->entry_topic, 30, 110);

  // QoS (DROPDOWN)
  topics_ui->label = gtk_label_new("QoS");
  gtk_widget_add_css_class(topics_ui->label, "topics_label_qos");
  gtk_fixed_put(GTK_FIXED(topics_ui->fixed_form), topics_ui->label, 460 ,90);
  const char *strings[4] = {"0", "1", "2", NULL};
  topics_ui->dropdown_qos = gtk_drop_down_new_from_strings(strings);
  gtk_drop_down_set_show_arrow(GTK_DROP_DOWN(topics_ui->dropdown_qos), TRUE);
  gtk_widget_set_size_request(topics_ui->dropdown_qos, 100, -1);
  gtk_widget_add_css_class(topics_ui->dropdown_qos, "topics_dropdown_qos");
  gtk_fixed_put(GTK_FIXED(topics_ui->fixed_form), topics_ui->dropdown_qos, 460, 110);

  // BUTTON (ADD)
  createButtonWithImageLabel(&topics_ui->button_add, TOPIC_ADD_PATH, "ADICIONAR");
  gtk_widget_add_css_class(topics_ui->button_add.button, "topics_button_add");
  gtk_widget_add_css_class(topics_ui->button_add.label, "topics_button_add_label");
  gtk_widget_set_size_request(topics_ui->button_add.button, 40, 35);
gtk_fixed_put(GTK_FIXED(topics_ui->fixed_form), topics_ui->button_add.button, 580, 110);
  g_signal_connect(topics_ui->button_add.button, "clicked", G_CALLBACK(addNewTopic), topics_ui);

  // FACTORY
  topics_ui->factory = gtk_signal_list_item_factory_new();
  g_signal_connect(topics_ui->factory, "setup", G_CALLBACK(setupFactory), topics_ui);
  g_signal_connect(topics_ui->factory, "bind", G_CALLBACK(bindFactory), NULL);
 
  // LIST STORE && SELECTION MODEL (NO SELECTION)
  topics_ui->topics_store = g_list_store_new(ST_TYPE_MQTT_TOPIC);
  topics_ui->no_selection = gtk_no_selection_new(G_LIST_MODEL(topics_ui->topics_store));
  
  // SELECTION MODEL - Topics Section
  topics_ui->list_view = gtk_list_view_new(GTK_SELECTION_MODEL(topics_ui->no_selection), topics_ui->factory);
  gtk_list_view_set_model(GTK_LIST_VIEW(topics_ui->list_view), GTK_SELECTION_MODEL(topics_ui->no_selection));

  // SCROLLED 
  topics_ui->scrolled = gtk_scrolled_window_new();
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(topics_ui->scrolled), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
  topics_ui->box_topics = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

  // BOX TOPICS - One box on top with labels and the List View with items.
  topics_ui->box_top = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_widget_set_size_request(topics_ui->box_top, 660, 45);
  topics_ui->label = gtk_label_new("Tópico");
  gtk_widget_set_margin_start(topics_ui->label, 40);
  gtk_widget_set_hexpand(topics_ui->label, TRUE);
  gtk_widget_add_css_class(topics_ui->label, "topics_list_title");
  gtk_box_append(GTK_BOX(topics_ui->box_top), topics_ui->label);
  topics_ui->label = gtk_label_new("QoS");
  gtk_widget_set_margin_end(topics_ui->label, 15);
  gtk_widget_add_css_class(topics_ui->label, "topics_list_qos");
  gtk_box_append(GTK_BOX(topics_ui->box_top), topics_ui->label);
  gtk_widget_add_css_class(topics_ui->box_top, "topics_list_boxtop");
  gtk_box_append(GTK_BOX(topics_ui->box_topics), topics_ui->box_top);
  gtk_widget_add_css_class(topics_ui->box_topics, "topics_list_box");
  gtk_box_append(GTK_BOX(topics_ui->box_topics), topics_ui->list_view);
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(topics_ui->scrolled), topics_ui->box_topics);
  gtk_widget_set_size_request(topics_ui->scrolled, 660, 260);
  gtk_fixed_put(GTK_FIXED(topics_ui->fixed_form), topics_ui->scrolled, 30, 150);
  
  // ID Connection 
  topics_ui->label = gtk_label_new("MQTT Cliente ID");
  gtk_widget_add_css_class(topics_ui->label, "topics_label_clientID");
  gtk_fixed_put(GTK_FIXED(topics_ui->fixed_form), topics_ui->label, 30, 415);
  topics_ui->entry_id = gtk_entry_new();
  gtk_widget_add_css_class(topics_ui->entry_id, "topics_entry_clientID");
  gtk_widget_set_size_request(topics_ui->entry_id, 310, -1);
  gtk_fixed_put(GTK_FIXED(topics_ui->fixed_form), topics_ui->entry_id, 30, 430);

  // BUTTON (BACK)
  createButtonWithImageLabel(&topics_ui->button_back, TOPIC_BACK_PATH, "VOLTAR");
  gtk_widget_add_css_class(topics_ui->button_back.button, "topics_button_back");
  gtk_widget_add_css_class(topics_ui->button_back.label, "topics_button_back_label");
  gtk_widget_set_size_request(topics_ui->button_back.button, 40, 35);
  gtk_fixed_put(GTK_FIXED(topics_ui->fixed_form), topics_ui->button_back.button, 570, 430);
 
  // BUTTON (CERTIFICATES)
  createButtonWithImageLabel(&topics_ui->button_certificates, TOPIC_CERTIFICATES_PATH, "CERTIFICADOS");
  gtk_widget_add_css_class(topics_ui->button_certificates.button, "topics_button_certificates");
  gtk_widget_add_css_class(topics_ui->button_certificates.label, "topics_button_certificates_label");
  gtk_widget_set_size_request(topics_ui->button_certificates.button, 40, 35);
  gtk_fixed_put(GTK_FIXED(topics_ui->fixed_form), topics_ui->button_certificates.button, 370, 430);
 
  // SIGNALS
  g_signal_connect(topics_ui->entry_id, "changed", G_CALLBACK(entryIDChanged), topics_ui);
  g_signal_connect(topics_ui->button_back.button, "clicked", G_CALLBACK(switchToConnection), stack);
  g_signal_connect(topics_ui->button_certificates.button, "clicked", G_CALLBACK(goToCertificates), topics_ui->stack);

  
 /* --------------------------- CERTIFICATES SECTION --------------------------- */

  topics_ui->fixed_certificates = gtk_fixed_new();
  gtk_stack_add_named(GTK_STACK(topics_ui->stack), topics_ui->fixed_certificates, "certificates");
  
  // TEXT 
  topics_ui->label = gtk_label_new("Conexão MQTT");
  gtk_widget_add_css_class(topics_ui->label, "topics_label_title");
  gtk_fixed_put(GTK_FIXED(topics_ui->fixed_certificates), topics_ui->label, 10, 30);
 
  createButtonWithImageLabel(&topics_ui->button_server_certificate, TOPIC_CERTIFICATES_PATH, "CERTIFICADO DO SERVIDOR (CA)");
  gtk_widget_add_css_class(topics_ui->button_server_certificate.button, "topics_button_server_certificate");
  gtk_widget_add_css_class(topics_ui->button_server_certificate.label, "topics_button_server_certificate_label");
  gtk_widget_set_size_request(topics_ui->button_server_certificate.button, 60, 40);
  gtk_fixed_put(GTK_FIXED(topics_ui->fixed_certificates), topics_ui->button_server_certificate.button, 30, 90);

  createButtonWithImageLabel(&topics_ui->button_client_certificate, TOPIC_CERTIFICATES_PATH, "CERTIFICADO DO CLIENTE");
  gtk_widget_add_css_class(topics_ui->button_client_certificate.button, "topics_button_client_certificate");
  gtk_widget_add_css_class(topics_ui->button_client_certificate.label, "topics_button_client_certificate_label");
  gtk_widget_set_size_request(topics_ui->button_client_certificate.button, 50, 40);
  gtk_fixed_put(GTK_FIXED(topics_ui->fixed_certificates), topics_ui->button_client_certificate.button, 30, 150);

  createButtonWithImageLabel(&topics_ui->button_client_key, TOPIC_CERTIFICATES_PATH, "CHAVE DO CLIENTE");
  gtk_widget_add_css_class(topics_ui->button_client_key.button, "topics_button_client_key");
  gtk_widget_add_css_class(topics_ui->button_client_key.label, "topics_button_client_key_label");
  gtk_widget_set_size_request(topics_ui->button_client_key.button, 40, 40);
  gtk_fixed_put(GTK_FIXED(topics_ui->fixed_certificates), topics_ui->button_client_key.button, 30, 210);
  
  createButtonWithImageLabel(&topics_ui->button_certificates_back, TOPIC_BACK_PATH, "VOLTAR");
  gtk_widget_add_css_class(topics_ui->button_certificates_back.button, "topics_button_certificates_back");
  gtk_widget_add_css_class(topics_ui->button_certificates_back.label, "topics_button_certificates_back_label");
  gtk_widget_set_size_request(topics_ui->button_certificates_back.button, 40, 40);
  gtk_fixed_put(GTK_FIXED(topics_ui->fixed_certificates), topics_ui->button_certificates_back.button, 570, 430);

  // SIGNALS 
  g_signal_connect(topics_ui->button_certificates_back.button, "clicked", G_CALLBACK(goToTopics), topics_ui->stack);


  // Set topics form visible
  gtk_stack_set_visible_child_name(GTK_STACK(topics_ui->stack), "topics");
}

void addNewTopic(GtkButton *button, gpointer user_data){
  ST_TopicsUI *topics_ui = (ST_TopicsUI *)user_data;
  STMQTTTopic *topic = stMQTTTopicNew();
  GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(topics_ui->entry_topic));
  const char *name = gtk_entry_buffer_get_text(buffer);
  stMQTTTopicSetName(topic, name);

  int item = gtk_drop_down_get_selected(GTK_DROP_DOWN(topics_ui->dropdown_qos));
  if(item == 0){
    stMQTTTopicSetQoS(topic, "0");
  }else if(item == 1){
    stMQTTTopicSetQoS(topic, "1");
  }else if(item == 2){
    stMQTTTopicSetQoS(topic, "2");
  }
  g_list_store_append(topics_ui->topics_store, topic);
  addConnectionToJSON(topics_ui->connection);
} 

void entryIDChanged(GtkEntry *entry_id, gpointer user_data){
  ST_TopicsUI *topics_ui = (ST_TopicsUI *)user_data;
  GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(topics_ui->entry_id));
  const char *id = gtk_entry_buffer_get_text(buffer);
  stMQTTConnectionSetClientID(topics_ui->connection, id);
  updateConnectionInJSON(topics_ui->connection);
}

static void setupFactory(GtkListItemFactory *factory, GtkListItem *item, gpointer user_data){
  ST_TopicsUI *topics_ui = (ST_TopicsUI *)user_data;

  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_widget_add_css_class(box, "topics_listitem");
  
  GtkWidget *image = gtk_image_new_from_file(TOPIC_DELETE_PATH);
  gtk_widget_set_size_request(image, 30, 25);
  gtk_widget_set_margin_start(image, 10);
  gtk_widget_set_margin_top(image, 5);
  gtk_widget_set_margin_bottom(image, 5);
  gtk_widget_add_css_class(image, "topics_listitem_image");
  gtk_box_append(GTK_BOX(box), image);
  g_object_set_data(G_OBJECT(image), "list_item", item);
  GtkGesture *gesture = gtk_gesture_click_new();
  gtk_widget_add_controller(image, GTK_EVENT_CONTROLLER(gesture));
  g_signal_connect(gesture, "pressed", G_CALLBACK(deleteTopic), topics_ui);


  GtkWidget *label = gtk_label_new(NULL);
  gtk_widget_add_css_class(label, "topics_listitem_topic");
  g_object_set_data(G_OBJECT(item), "topic", label);
  gtk_widget_set_hexpand(label, TRUE);
  gtk_label_set_max_width_chars(GTK_LABEL(label), 100);
  gtk_label_set_ellipsize(GTK_LABEL(label), 3); // Limit chars 
  gtk_box_append(GTK_BOX(box), label);

  label = gtk_label_new(NULL);
  gtk_widget_add_css_class(label, "topics_listitem_qos");
  gtk_widget_set_margin_end(label, 15);
  g_object_set_data(G_OBJECT(item), "qos", label);
  gtk_box_append(GTK_BOX(box), label);
  
  gtk_list_item_set_child(item, box);
}

static void bindFactory(GtkListItemFactory *factory, GtkListItem *item, gpointer user_data){
  STMQTTTopic *topic = gtk_list_item_get_item(item);

  GtkWidget *label = g_object_get_data(G_OBJECT(item), "topic");
  gtk_label_set_max_width_chars(GTK_LABEL(label), 100);
  gtk_label_set_text(GTK_LABEL(label), stMQTTTopicGetName(topic));

  label = g_object_get_data(G_OBJECT(item), "qos");
  gtk_label_set_text(GTK_LABEL(label), stMQTTTopicGetQoS(topic));
}

void deleteTopic(GtkGestureClick *gesture, int n_press, double  x, double y, gpointer user_data){
  ST_TopicsUI *topics_ui = (ST_TopicsUI *)user_data;

  GtkWidget *image = gtk_event_controller_get_widget(GTK_EVENT_CONTROLLER(gesture));
  GtkListItem *item = g_object_get_data(G_OBJECT(image), "list_item");
  int position = gtk_list_item_get_position(GTK_LIST_ITEM(item));
  deleteTopicInJSON(topics_ui->connection, position);
  g_list_store_remove(topics_ui->topics_store, position);
}

void switchToConnection(GtkButton *button, gpointer user_data){
  GtkWidget *stack = (GtkWidget *)user_data;

  gtk_stack_set_visible_child_name(GTK_STACK(stack), "connections");
}

void goToCertificates(GtkButton *button, gpointer user_data){
  GtkWidget *stack = (GtkWidget *)user_data;

  gtk_stack_set_visible_child_name(GTK_STACK(stack), "certificates");
}

void goToTopics(GtkButton *button, gpointer user_data){
  GtkWidget *stack = (GtkWidget *)user_data;
  
  gtk_stack_set_visible_child_name(GTK_STACK(stack), "topics");
}
