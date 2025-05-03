#include "ui_topbox.h"

void initTopBox(ST_TopBox *top_box){
  
  // BOX
  top_box->box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_add_css_class(top_box->box, "topbox_box");

  // LOGOTIPO
  top_box->image = gtk_image_new_from_file(LOGOTIPO_PATH);
  gtk_widget_set_size_request(top_box->image, 48, 48);
  gtk_widget_set_margin_start(top_box->image, 12);
  gtk_box_append(GTK_BOX(top_box->box), top_box->image);

  // TEXT
  top_box->label = gtk_label_new("HAPA MQTT Client");
  gtk_widget_add_css_class(top_box->label, "topbox_label");
  gtk_widget_set_margin_start(top_box->label, 10);
  gtk_box_append(GTK_BOX(top_box->box), top_box->label);

  // SEARCH BOX
  top_box->search_box = gtk_center_box_new();
  gtk_widget_set_hexpand(top_box->search_box, TRUE);
  gtk_box_append(GTK_BOX(top_box->box), top_box->search_box);

  // SEARCH ENTRY
  top_box->search_entry = gtk_search_entry_new();
  gtk_widget_set_size_request(top_box->search_entry, 400, -1);
  gtk_widget_add_css_class(top_box->search_entry, "topbox_searchentry");
  gtk_center_box_set_center_widget(GTK_CENTER_BOX(top_box->search_box), top_box->search_entry);

  // BUTTON - The button that switch the stack page
  createButtonWithImageLabel(&top_box->button_switch, NAV_CONNECTION_PATH, "CONECTAR");
  gtk_widget_set_size_request(top_box->button_switch.image, 24, 24);
  gtk_widget_set_margin_end(top_box->button_switch.button, 40);
  gtk_widget_add_css_class(top_box->button_switch.label, "topbox_button_connect_label");
  gtk_widget_add_css_class(top_box->button_switch.button, "topbox_button_connect");
  gtk_box_append(GTK_BOX(top_box->box), top_box->button_switch.button);

  // MENU BUTTON - Still no features
  top_box->button_menu = gtk_menu_button_new();
  top_box->image = gtk_image_new_from_file(NAV_MENU_PATH);
  gtk_menu_button_set_child(GTK_MENU_BUTTON(top_box->button_menu), top_box->image);
  gtk_widget_set_margin_end(top_box->button_menu, 12);
  gtk_widget_add_css_class(top_box->button_menu, "topbox_button_menu");
  gtk_widget_set_size_request(top_box->button_menu, 50, 50);
  gtk_box_append(GTK_BOX(top_box->box), top_box->button_menu);
}
