#include "ui_form.h"

// FORM UI 
void initFormUI(ST_FormUI *form_ui){

  form_ui->main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_widget_set_size_request(form_ui->main_box, 1000, 500);
  gtk_widget_set_vexpand(form_ui->main_box, FALSE);
  gtk_widget_set_hexpand(form_ui->main_box, FALSE);
  gtk_box_set_homogeneous(GTK_BOX(form_ui->main_box), FALSE);
  gtk_widget_add_css_class(form_ui->main_box, "form_layout");
  
  // BOX CONNECTIONS (LEFT SIDE)
  form_ui->box_connections = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_widget_set_size_request(form_ui->box_connections, 200, 500);
  gtk_widget_set_margin_start(form_ui->box_connections, 10);
  gtk_widget_set_vexpand(form_ui->box_connections, FALSE);
  gtk_widget_set_hexpand(form_ui->box_connections, FALSE);
  gtk_box_append(GTK_BOX(form_ui->main_box), form_ui->box_connections);
  
  // IMAGE (CONNECTION ADD)
  form_ui->image = gtk_image_new_from_file(CONNECTION_ADD_PATH); 
  gtk_widget_set_size_request(form_ui->image, 30, 30);
  gtk_widget_set_margin_top(form_ui->image, 10);
  gtk_widget_add_css_class(form_ui->image, "form_image_connections");
  gtk_box_append(GTK_BOX(form_ui->box_connections), form_ui->image);

  // GESTURE - This is to use that image to connect a signal, like a button.
  form_ui->gesture = gtk_gesture_click_new();
  gtk_widget_add_controller(form_ui->image, GTK_EVENT_CONTROLLER(form_ui->gesture));
  g_signal_connect(form_ui->gesture, "pressed", G_CALLBACK(addNewConnections), form_ui);

  // TEXT - "Conexões" 
  form_ui->label = gtk_label_new("Conexões");
  gtk_widget_add_css_class(form_ui->label, "form_label_connections");
  gtk_box_append(GTK_BOX(form_ui->box_connections), form_ui->label);

  // STACK - Two stack pages for connection simple data (CONNECTION SECTION) 
  // and other for connections topics and QOS (TOPICS SECTION).
  form_ui->stack = gtk_stack_new();
  gtk_stack_set_transition_type(GTK_STACK(form_ui->stack), GTK_STACK_TRANSITION_TYPE_SLIDE_UP);
 gtk_box_append(GTK_BOX(form_ui->main_box), form_ui->stack);

 // STACK - Connection Section  
  initConnectionsUI(&form_ui->connections_ui, form_ui->stack);
  gtk_stack_add_named(GTK_STACK(form_ui->stack), form_ui->connections_ui.fixed, "connections");
  
  // STACK - Topics Section
  initTopicsUI(&form_ui->topics_ui, form_ui->stack);
  gtk_stack_add_named(GTK_STACK(form_ui->stack), form_ui->topics_ui.fixed, "topics");
 
  // STACK - Start in the Connections Page.
  gtk_stack_set_visible_child_name(GTK_STACK(form_ui->stack), "connections");

  // FACTORY - Provides a visual representation of each connection in the list view.
  form_ui->factory = gtk_signal_list_item_factory_new();
  g_signal_connect(form_ui->factory, "setup", G_CALLBACK(setupFactory), form_ui);
  g_signal_connect(form_ui->factory, "bind", G_CALLBACK(bindFactory), form_ui);

  // LIST VIEW 
  form_ui->list_view = gtk_list_view_new(GTK_SELECTION_MODEL(form_ui->connections_ui.selection_model), form_ui->factory);
  gtk_widget_add_css_class(form_ui->list_view, "form_listview");

  // SCROLLED 
  form_ui->scrolled = gtk_scrolled_window_new();
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(form_ui->scrolled), form_ui->list_view);
  gtk_box_append(GTK_BOX(form_ui->box_connections), form_ui->scrolled);
  gtk_widget_set_hexpand(form_ui->scrolled, TRUE);
  gtk_widget_set_vexpand(form_ui->scrolled, TRUE);
}

// SETUP - creates the visual structure of the item (initially). 
void setupFactory(GtkListItemFactory *factory, GtkListItem *item, gpointer user_data){
  //ST_FormUI *form_ui = (ST_FormUI *)user_data;
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_widget_add_css_class(box, "form_listitem");

  GtkWidget *label = gtk_label_new(NULL);
  gtk_widget_add_css_class(label, "form_listitem_name");
  g_object_set_data(G_OBJECT(item), "name", label);
  gtk_label_set_max_width_chars(GTK_LABEL(label), 1);
  gtk_label_set_ellipsize(GTK_LABEL(label), 3); // Limit chars
  gtk_box_append(GTK_BOX(box), label);

  label = gtk_label_new(NULL);
  gtk_widget_add_css_class(label, "form_listitem_host");
  g_object_set_data(G_OBJECT(item), "host", label);
  gtk_label_set_max_width_chars(GTK_LABEL(label), 1);
  gtk_label_set_ellipsize(GTK_LABEL(label), 3);
  gtk_box_append(GTK_BOX(box), label);

  gtk_list_item_set_child(item, box);
}

// BIND - updates the item when it becomes visible on the screen.
void bindFactory(GtkListItemFactory *factory, GtkListItem *item, gpointer user_data){
  //ST_FormUI *form_ui = (ST_FormUI *)user_data;
  STMQTTConnection *connection = gtk_list_item_get_item(item);

  GtkWidget *label = g_object_get_data(G_OBJECT(item), "name");
  gtk_label_set_max_width_chars(GTK_LABEL(label), 1);
  gtk_label_set_text(GTK_LABEL(label), stMQTTConnectionGetName(connection));

  label = g_object_get_data(G_OBJECT(item), "host");
  gtk_label_set_max_width_chars(GTK_LABEL(label), 1);
  // Example: mqtt + "://" + 192.168.1.1 + '\0' 
  char *text = malloc(strlen(stMQTTConnectionGetProtocol(connection)) + strlen(stMQTTConnectionGetHost(connection)) + 4);
  sprintf(text, "%s://%s", stMQTTConnectionGetProtocol(connection), stMQTTConnectionGetHost(connection));
  gtk_label_set_text(GTK_LABEL(label), text);
}

void addNewConnections(GtkGestureClick *gesture, int n_press, double x, double y, gpointer user_data){
  ST_FormUI *form_ui = (ST_FormUI *)user_data;
  STMQTTConnection *connection = stMQTTConnectionNew();
  int position = g_list_model_get_n_items(G_LIST_MODEL(form_ui->connections_ui.connection_store)) - 1;
  stMQTTConnectionSetID(connection, getID());
  addConnectionToJSON(connection);
  g_list_store_append(form_ui->connections_ui.connection_store, connection);
  gtk_single_selection_set_selected(form_ui->connections_ui.selection_model, position);
}

