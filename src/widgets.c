#include "widgets.h"
#include "ui_home.h"

// Create a custom button with a image and text 
void createButtonWithImageLabel(ST_BUTTON *button, const char *pathToImage, const char *textButton){
  button->button = gtk_button_new();
  button->box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
  gtk_button_set_child(GTK_BUTTON(button->button), button->box);
  button->image = gtk_image_new_from_file(pathToImage);
  gtk_widget_set_size_request(button->image, 24, 24);
  gtk_widget_set_margin_start(button->box, 5);
  gtk_widget_set_margin_end(button->box, 5);
  button->label = gtk_label_new(textButton);
  gtk_box_append(GTK_BOX(button->box), button->label);
  gtk_box_append(GTK_BOX(button->box), button->image);
}

GtkWidget *buildMessageBox(STMessageData *message_data){
  if(stMessageDataGetDirection(message_data) == ST_MESSAGE_SENT){
    return sendMessage(stMessageDataGetTopic(message_data), stMessageDataGetPayload(message_data));
  }else if(stMessageDataGetDirection(message_data) == ST_MESSAGE_RECEIVED){
    return receiveMessage(stMessageDataGetTopic(message_data), stMessageDataGetPayload(message_data));
  }
  return receiveMessage(stMessageDataGetTopic(message_data), stMessageDataGetPayload(message_data));
}

GtkWidget *sendMessage(const char *topic, const char *payload){
  GtkWidget *box_in = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
  GtkWidget *box_out = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  GtkWidget *label = gtk_label_new(NULL);

  char *header = malloc(strlen(topic) + 7);
  sprintf(header, "Tópico: %s", topic);
  gtk_label_set_text(GTK_LABEL(label), header);
  gtk_label_set_xalign(GTK_LABEL(label), 0.0);
  gtk_label_set_selectable(GTK_LABEL(label), true);
  gtk_widget_add_css_class(label, "message_topic_sent");
  gtk_box_append(GTK_BOX(box_in), label);

  label = gtk_label_new(payload);
  gtk_label_set_xalign(GTK_LABEL(label), 0.0);
  gtk_label_set_wrap(GTK_LABEL(label), true);   // \n
  gtk_label_set_wrap_mode(GTK_LABEL(label), PANGO_WRAP_CHAR);
  gtk_label_set_max_width_chars(GTK_LABEL(label), 50);
  gtk_widget_set_hexpand(label, false);
  gtk_label_set_selectable(GTK_LABEL(label), true);
  gtk_widget_add_css_class(label, "message_payload_sent");
  gtk_box_append(GTK_BOX(box_in), label);

  gtk_widget_add_css_class(box_in, "message_sent");
  gtk_box_append(GTK_BOX(box_out), box_in);
  
  
  char *date = strdup(getDateTime());
  label = gtk_label_new(date);
  gtk_label_set_xalign(GTK_LABEL(label), 1.0);
  gtk_label_set_selectable(GTK_LABEL(label), true);
  gtk_widget_add_css_class(label, "message_date");
  gtk_box_append(GTK_BOX(box_out), label);
  
  gtk_widget_set_halign(box_out, GTK_ALIGN_END);
  gtk_widget_set_margin_start(box_out, 20);
  gtk_widget_set_margin_end(box_out, 20);
  gtk_widget_set_valign(box_out, GTK_ALIGN_END);
 
  free(header);
  free(date);

  return box_out;
 }

GtkWidget *receiveMessage(const char *topic, const char *payload){
  GtkWidget *box_in = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
  GtkWidget *box_out = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  GtkWidget *label = gtk_label_new(NULL);

  char *header = malloc(strlen(topic) + strlen("Tópico: ") + 1);
  snprintf(header, strlen(topic) + strlen("Tópico: ") + 1, "Tópico: %s", topic);
  gtk_label_set_text(GTK_LABEL(label), header);
  gtk_label_set_xalign(GTK_LABEL(label), 0.0);
  gtk_label_set_selectable(GTK_LABEL(label), true);
  gtk_widget_add_css_class(label, "message_topic_received");
  gtk_box_append(GTK_BOX(box_in), label);

  label = gtk_label_new(payload);
  gtk_label_set_xalign(GTK_LABEL(label), 0.0);
  gtk_label_set_wrap(GTK_LABEL(label), true);   // \n
  gtk_label_set_wrap_mode(GTK_LABEL(label), PANGO_WRAP_CHAR);
  gtk_label_set_max_width_chars(GTK_LABEL(label), 50);
  gtk_widget_set_hexpand(label, false);
  gtk_widget_set_vexpand(label, true);
  gtk_label_set_selectable(GTK_LABEL(label), true);
  gtk_widget_add_css_class(label, "message_payload_received");
  gtk_box_append(GTK_BOX(box_in), label);

  gtk_widget_add_css_class(box_in, "message_received");
  gtk_box_append(GTK_BOX(box_out), box_in);

  char *date = strdup(getDateTime());
  label = gtk_label_new(date);
  gtk_label_set_xalign(GTK_LABEL(label), 1.0);
  gtk_label_set_selectable(GTK_LABEL(label), true);
  gtk_widget_add_css_class(label, "message_date");
  gtk_box_append(GTK_BOX(box_out), label);

  gtk_widget_set_halign(box_out, GTK_ALIGN_START);
  gtk_widget_set_margin_start(box_out, 20);
  gtk_widget_set_margin_end(box_out, 20);
  gtk_widget_set_valign(box_out, GTK_ALIGN_START);

  free(header);
  free(date);

  return box_out;
}

void scrollToBottom(GtkScrolledWindow *scrolled){
  GtkAdjustment *adjustment = gtk_scrolled_window_get_vadjustment(scrolled);
  double max_value = gtk_adjustment_get_upper(adjustment) - gtk_adjustment_get_page_size(adjustment);

  gtk_adjustment_set_value(adjustment, max_value);
}
