#include "ui_topics.h"

void initTopicsUI(ST_TopicsUI *topics_ui){
  topics_ui->fixed = gtk_fixed_new();

  // TEXT 
  topics_ui->label = gtk_label_new("Conexão MQTT");
  gtk_widget_add_css_class(topics_ui_label, "topics_label_title");
  gtk_fixed_put(GTK_FIXED(topics_ui->fixed), topics_ui->label, 10, 30);

  // TOPIC (NAME)
  topics_ui->label = gtk_label_new("Tópico");
  gtk_widget_add_css_class(topics_ui->label, "topics_label_topic");
  gtk_fixed_put(GTK_FIXED(topics_ui->fixed), topics_ui->label, 30, 90);
  topics_ui->entry_topic = gtk_entry_new();
  gtk_widget_set_size_request(topics_ui->entry_topic, 300, -1);
  gtk_widget_add_css_class(topics_ui->entry_topic, "topics_entry_topic");
  gtk_fixed_put(GTK_FIXED(topics_ui->fixed), topics_ui->entry_topic, 30, 110);

  // QoS (DROPDOWN)
  topics_ui->label = gtk_label_new("QoS");
  gtk_widget_add_css_class(topics_ui->label, "topics_label_qos");
  gtk_fixed_put(GTK_FIXED(topics_ui->fixed), topics_ui->label, 360 ,90);
  char *strings[3] = {"0", "1", "2"};
  topics_ui->dropdown_qos = gtk_drop_down_new_from_strings(strings);
  gtk_drop_down_set_show_arrow(GTK_DROP_DOWN(topics_ui->dropdown_qos), TRUE);
  gtk_widget_set_size_request(topics_ui->dropdown_qos, 100, -1);
  gtk_widget_add_css_class(topics_ui->dropdown_qos, "topics_dropdown_qos");
  gtk_fixed_put(GTK_FIXED(topics_ui->fixed), topics_ui->dropdown_qos, 360, 110);

  // BUTTON (ADD)
  createButtonWithImageLabel(&topics_ui->button_add, TOPIC_ADD_PATH, "ADD");
  gtk_widget_add_css_class(topics_ui->button_add.button, "topics_button_add");
  gtk_widget_add_css_class(topics_ui->button_add.label, "topics_button_add_label");
  gtk_widget_set_size_request(topics_ui->button_add.button, 40, 35);
  gtk_fixed_put(GTK_FIXED(topics_ui->fixed), topics_ui->button_add.button, 480, 90);

  //topics_ui->box_topics = gtk_box_new(GTK)
}
