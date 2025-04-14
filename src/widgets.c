#include "widgets.h"

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
