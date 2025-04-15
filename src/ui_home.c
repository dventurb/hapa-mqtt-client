#include "ui_home.h"

void initHomeUI(ST_HomeUI *home_ui){
  home_ui->paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_widget_add_css_class(home_ui->paned, "home_layout");

 // home_ui->tree_list = gtk_tree_list_model_new()
}
