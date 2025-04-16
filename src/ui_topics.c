#include "ui_topics.h"

void initTopicsUI(ST_TopicsUI *topics_ui, GtkWidget *stack){
  topics_ui->fixed = gtk_fixed_new();

  // TEXT 
  topics_ui->label = gtk_label_new("Conexão MQTT");
  gtk_widget_add_css_class(topics_ui->label, "topics_label_title");
  gtk_fixed_put(GTK_FIXED(topics_ui->fixed), topics_ui->label, 10, 30);

  // TOPIC (NAME)
  topics_ui->label = gtk_label_new("Tópico");
  gtk_widget_add_css_class(topics_ui->label, "topics_label_topic");
  gtk_fixed_put(GTK_FIXED(topics_ui->fixed), topics_ui->label, 30, 90);
  topics_ui->entry_topic = gtk_entry_new();
  gtk_widget_set_size_request(topics_ui->entry_topic, 400, -1);
  gtk_widget_add_css_class(topics_ui->entry_topic, "topics_entry_topic");
  gtk_fixed_put(GTK_FIXED(topics_ui->fixed), topics_ui->entry_topic, 30, 110);

  // QoS (DROPDOWN)
  topics_ui->label = gtk_label_new("QoS");
  gtk_widget_add_css_class(topics_ui->label, "topics_label_qos");
  gtk_fixed_put(GTK_FIXED(topics_ui->fixed), topics_ui->label, 460 ,90);
  const char *strings[4] = {"0", "1", "2", NULL};
  topics_ui->dropdown_qos = gtk_drop_down_new_from_strings(strings);
  gtk_drop_down_set_show_arrow(GTK_DROP_DOWN(topics_ui->dropdown_qos), TRUE);
  gtk_widget_set_size_request(topics_ui->dropdown_qos, 100, -1);
  gtk_widget_add_css_class(topics_ui->dropdown_qos, "topics_dropdown_qos");
  gtk_fixed_put(GTK_FIXED(topics_ui->fixed), topics_ui->dropdown_qos, 460, 110);

  // BUTTON (ADD)
  createButtonWithImageLabel(&topics_ui->button_add, TOPIC_ADD_PATH, "ADICIONAR");
  gtk_widget_add_css_class(topics_ui->button_add.button, "topics_button_add");
  gtk_widget_add_css_class(topics_ui->button_add.label, "topics_button_add_label");
  gtk_widget_set_size_request(topics_ui->button_add.button, 40, 35);
gtk_fixed_put(GTK_FIXED(topics_ui->fixed), topics_ui->button_add.button, 580, 110);
  g_signal_connect(topics_ui->button_add.button, "clicked", G_CALLBACK(addTopic), topics_ui);

  // LIST STORE (TOPICS)
  topics_ui->topics_store = g_list_store_new(ST_TYPE_MQTT_TOPIC);

  // FACTORY
  topics_ui->factory = gtk_signal_list_item_factory_new();
  g_signal_connect(topics_ui->factory, "setup", G_CALLBACK(setupFactory1), topics_ui);
  g_signal_connect(topics_ui->factory, "bind", G_CALLBACK(bindFactory1), NULL);

  topics_ui->no_selection = gtk_no_selection_new(G_LIST_MODEL(topics_ui->topics_store));

  // LIST VIEW
  topics_ui->list_view = gtk_list_view_new(GTK_SELECTION_MODEL(topics_ui->no_selection), topics_ui->factory);

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
  gtk_fixed_put(GTK_FIXED(topics_ui->fixed), topics_ui->scrolled, 30, 150);

  // BUTTON (BACK)
  createButtonWithImageLabel(&topics_ui->button_back, TOPIC_BACK_PATH, "VOLTAR");
  gtk_widget_add_css_class(topics_ui->button_back.button, "topics_button_back");
  gtk_widget_add_css_class(topics_ui->button_back.label, "topics_button_back_label");
  gtk_widget_set_size_request(topics_ui->button_back.button, 40, 35);
  gtk_fixed_put(GTK_FIXED(topics_ui->fixed), topics_ui->button_back.button, 570, 430);
  
  g_signal_connect(topics_ui->button_back.button, "clicked", G_CALLBACK(switchToConnection), stack);
}

void addTopic(GtkButton *button, gpointer user_data){
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
} 

void setupFactory1(GtkListItemFactory *factory, GtkListItem *item, gpointer user_data){
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

void bindFactory1(GtkListItemFactory *factory, GtkListItem *item, gpointer user_data){
  STMQTTTopic *topic = gtk_list_item_get_item(item);

  GtkWidget *label = g_object_get_data(G_OBJECT(item), "topic");
  gtk_label_set_max_width_chars(GTK_LABEL(label), 1);
  gtk_label_set_text(GTK_LABEL(label), stMQTTTopicGetName(topic));

  label = g_object_get_data(G_OBJECT(item), "qos");
  gtk_label_set_text(GTK_LABEL(label), stMQTTTopicGetQoS(topic));
}

void deleteTopic(GtkGestureClick *gesture, int n_press, double  x, double y, gpointer user_data){
  ST_TopicsUI *topics_ui = (ST_TopicsUI *)user_data;

  GtkWidget *image = gtk_event_controller_get_widget(GTK_EVENT_CONTROLLER(gesture));
  GtkListItem *item = g_object_get_data(G_OBJECT(image), "list_item");
  int position = gtk_list_item_get_position(GTK_LIST_ITEM(item));

  g_list_store_remove(topics_ui->topics_store, position);
}

void switchToConnection(GtkButton *button, gpointer user_data){
  GtkWidget *stack = (GtkWidget *)user_data;

  gtk_stack_set_visible_child_name(GTK_STACK(stack), "connections");
}
