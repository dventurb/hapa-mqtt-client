#include "ui_home.h"

void initHomeUI(ST_HomeUI *home_ui){
  home_ui->paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_widget_add_css_class(home_ui->paned, "home_layout");

  home_ui->box_connections = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_paned_set_start_child(GTK_PANED(home_ui->paned), home_ui->box_connections);
  
}
