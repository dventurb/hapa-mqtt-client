#include "ui_home.h"

void initHomeUI(ST_HomeUI *home_ui){
  home_ui->fixed = gtk_fixed_new();
  gtk_widget_set_hexpand(home_ui->fixed, TRUE);
  gtk_widget_set_vexpand(home_ui->fixed, TRUE);
  gtk_widget_add_css_class(home_ui->fixed, "home_layout");
}
