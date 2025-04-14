#include "ui_form.h"

// FORM UI 
void initFormUI(ST_FormUI *form_ui){

  // FIXED - I use fixed to attach the widgets, because the application 
  // has one fixed size. This ensures the position of the widgets.
  form_ui->fixed = gtk_fixed_new();
  gtk_widget_set_hexpand(form_ui->fixed, TRUE);
  gtk_widget_set_vexpand(form_ui->fixed, TRUE);
  gtk_widget_add_css_class(form_ui->fixed, "form_layout");
  
  // BOX CONNECTIONS (LEFT SIDE)
  form_ui->box_connections = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_widget_set_size_request(form_ui->box_connections, 200, 500);
  gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->box_connections, 10, 20);

  // LIST STORE - This is where the MQTT connections will be stored.
  form_ui->connection_store = g_list_store_new(ST_TYPE_MQTT_CONNECTION);
  
  // IMAGE (CONNECTION ADD)
  form_ui->image = gtk_image_new_from_file(CONNECTION_ADD_PATH); 
  gtk_widget_set_size_request(form_ui->image, 30, 30);
  gtk_widget_add_css_class(form_ui->image, "form_image_connections");
  gtk_box_append(GTK_BOX(form_ui->box_connections), form_ui->image);

  // GESTURE - This is to use that image to connect a signal, like a button.
  form_ui->gesture = gtk_gesture_click_new();
  gtk_widget_add_controller(form_ui->image, GTK_EVENT_CONTROLLER(form_ui->gesture));
  g_signal_connect(form_ui->gesture, "pressed", G_CALLBACK(addNewConnections), form_ui);

  // TEXT 
  form_ui->label = gtk_label_new("Conexões");
  gtk_widget_add_css_class(form_ui->label, "form_label_connections");
  gtk_box_append(GTK_BOX(form_ui->box_connections), form_ui->label);

  // SELECTION MODEL - Allows selecting a single connection from the list store.
  form_ui->selection_model = gtk_single_selection_new(G_LIST_MODEL(form_ui->connection_store));
  loadJSONToForm(form_ui);
  g_signal_connect(form_ui->selection_model, "selection-changed", G_CALLBACK(selectionChanged), form_ui);

  // FACTORY - Provides a visual representation of each connection in the list view.
  form_ui->factory = gtk_signal_list_item_factory_new();
  g_signal_connect(form_ui->factory, "setup", G_CALLBACK(setupFactory), form_ui);
  g_signal_connect(form_ui->factory, "bind", G_CALLBACK(bindFactory), form_ui);

  // LIST VIEW 
  form_ui->list_view = gtk_list_view_new(GTK_SELECTION_MODEL(form_ui->selection_model), form_ui->factory);
  gtk_widget_add_css_class(form_ui->list_view, "form_listview");

  // SCROLLED 
  form_ui->scrolled = gtk_scrolled_window_new();
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(form_ui->scrolled), form_ui->list_view);
  gtk_box_append(GTK_BOX(form_ui->box_connections), form_ui->scrolled);
  gtk_widget_set_hexpand(form_ui->scrolled, TRUE);
  gtk_widget_set_vexpand(form_ui->scrolled, TRUE);
  
  // TEXT 
  form_ui->label = gtk_label_new("Conexão MQTT");
  gtk_widget_add_css_class(form_ui->label, "form_label_title");
  gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->label, 250, 30);

  // FORM SECTION - Entry and labels for the MQTT connection.
  
  // NAME 
  form_ui->label = gtk_label_new("Nome");
  gtk_widget_add_css_class(form_ui->label, "form_label_name");
  gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->label, 270, 90);
  form_ui->entry_name = gtk_entry_new();
  gtk_widget_set_size_request(form_ui->entry_name, 300, -1);
  gtk_widget_add_css_class(form_ui->entry_name, "form_entry_name");
  gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->entry_name, 270, 110);
  
  // PORT 
  form_ui->label = gtk_label_new("Porta");
  gtk_widget_add_css_class(form_ui->label, "form_label_port");
  gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->label, 840, 190);
  form_ui->buffer = gtk_entry_buffer_new("1883", 4);
  form_ui->entry_port = gtk_entry_new_with_buffer(form_ui->buffer);
  gtk_widget_set_size_request(form_ui->entry_port, 120, -1);
  gtk_widget_add_css_class(form_ui->entry_port, "form_entry_port");
  gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->entry_port, 840, 210);

  // PROTOCOL
  form_ui->label = gtk_label_new("Protocolo");
  gtk_widget_add_css_class(form_ui->label, "form_label_protocol");
  gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->label, 270, 190);
  const char *strings[3] = {"mqtt//:", "ws://", NULL};
  form_ui->dropdown_protocol = gtk_drop_down_new_from_strings(strings);
  gtk_drop_down_set_show_arrow(GTK_DROP_DOWN(form_ui->dropdown_protocol), TRUE);
  gtk_widget_set_size_request(form_ui->dropdown_protocol, 100, -1);
  gtk_widget_add_css_class(form_ui->dropdown_protocol, "form_dropdown_protocol");
  gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->dropdown_protocol, 270, 210);

  // HOST
  form_ui->label = gtk_label_new("Host");
  gtk_widget_add_css_class(form_ui->label, "form_label_host");
  gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->label, 400, 190);
  form_ui->entry_host = gtk_entry_new();
  gtk_widget_set_size_request(form_ui->entry_host, 410, -1);
  gtk_widget_add_css_class(form_ui->entry_host, "form_entry_host");
  gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->entry_host, 400, 210);

  // USERNAME 
  form_ui->label = gtk_label_new("Username");
  gtk_widget_add_css_class(form_ui->label, "form_label_username");
  gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->label, 270, 310);
  form_ui->entry_username = gtk_entry_new();
  gtk_widget_set_size_request(form_ui->entry_username, 330, -1);
  gtk_widget_add_css_class(form_ui->entry_username, "form_entry_username");
  gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->entry_username, 270, 330);

  // PASSWORD
  form_ui->label = gtk_label_new("Password");
  gtk_widget_add_css_class(form_ui->label, "form_label_password");
  gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->label, 630, 310);
  form_ui->entry_password = gtk_entry_new();
  gtk_widget_set_size_request(form_ui->entry_password, 310, -1);
  gtk_widget_add_css_class(form_ui->entry_password, "form_entry_password");
  gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->entry_password, 630, 330);

  // SIGNALS (FORM SECTION)
  g_signal_connect(form_ui->entry_name, "changed", G_CALLBACK(entryNameChanged), form_ui);
  g_signal_connect(form_ui->entry_port, "changed", G_CALLBACK(entryPortChanged), form_ui);
  g_signal_connect(form_ui->dropdown_protocol, "notify::selected", G_CALLBACK(dropdownProtocolChanged), form_ui);
  g_signal_connect(form_ui->entry_host, "changed", G_CALLBACK(entryHostChanged), form_ui);
  g_signal_connect(form_ui->entry_username, "changed", G_CALLBACK(entryUsernameChanged), form_ui);
  g_signal_connect(form_ui->entry_password, "changed", G_CALLBACK(entryPasswordChanged), form_ui);

  // BUTTONS SECTION

  // CONNECT (BUTTON)
  createButtonWithImageLabel(&form_ui->button_connect, CONNECTION_ACTIVATE_PATH, "CONECTAR");
  gtk_widget_add_css_class(form_ui->button_connect.button, "form_button_connect");
  gtk_widget_add_css_class(form_ui->button_connect.label, "form_button_connect_label");
  gtk_widget_set_size_request(form_ui->button_connect.button, 75, 35);
  gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->button_connect.button, 810, 430);

  // SAVE (BUTTON)
  createButtonWithImageLabel(&form_ui->button_save, CONNECTION_SAVE_PATH, "SALVAR");
  gtk_widget_add_css_class(form_ui->button_save.button, "form_button_save");
  gtk_widget_add_css_class(form_ui->button_save.label, "form_button_save_label");
  gtk_widget_set_size_request(form_ui->button_save.button, 70, 35);
  gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->button_save.button, 660, 430);
  
  // DELETE (BUTTON)
  createButtonWithImageLabel(&form_ui->button_delete, CONNECTION_DELETE_PATH, "REMOVER");
  gtk_widget_add_css_class(form_ui->button_delete.button, "form_button_delete");
  gtk_widget_add_css_class(form_ui->button_delete.label, "form_button_delete_label");
  gtk_widget_set_size_request(form_ui->button_delete.button, 70, 35);
  gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->button_delete.button, 270, 430);

  // OPTIONS (BUTTON)
  createButtonWithImageLabel(&form_ui->button_options, CONNECTION_OPTIONS_PATH, "OPÇÕES");
  gtk_widget_add_css_class(form_ui->button_options.button, "form_button_options");
  gtk_widget_add_css_class(form_ui->button_options.label, "form_button_options_label");
  gtk_widget_set_size_request(form_ui->button_options.button, 70, 35);
  gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->button_options.button, 430, 430);

  // SIGNALS (BUTTONS)
  g_signal_connect(form_ui->button_save.button, "clicked", G_CALLBACK(saveConnection), form_ui);
}


void selectionChanged(GtkSelectionModel *selection_model, int position, int n_items, gpointer user_data){
  ST_FormUI *form_ui = (ST_FormUI *)user_data;
  position = gtk_single_selection_get_selected(form_ui->selection_model);
  STMQTTConnection *connection = g_list_model_get_item(G_LIST_MODEL(form_ui->connection_store), position);

  g_signal_handlers_block_by_func(form_ui->entry_name, entryNameChanged, form_ui);

  GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(form_ui->entry_name));
  gtk_entry_buffer_set_text(buffer, stMQTTConnectionGetName(connection), -1);
 
  int length = snprintf(NULL, 0, "%d", stMQTTConnectionGetPort(connection));
  char *port = malloc(length + 1);
  sprintf(port, "%d", stMQTTConnectionGetPort(connection));
  buffer = gtk_entry_get_buffer(GTK_ENTRY(form_ui->entry_port));
  gtk_entry_buffer_set_text(buffer, port, -1);
  free(port);

  if((strcmp(stMQTTConnectionGetProtocol(connection), "ws")) == 0){
    gtk_drop_down_set_selected(GTK_DROP_DOWN(form_ui->dropdown_protocol), 1);
  }else {
    gtk_drop_down_set_selected(GTK_DROP_DOWN(form_ui->dropdown_protocol), 0);
  }

  buffer = gtk_entry_get_buffer(GTK_ENTRY(form_ui->entry_host));
  gtk_entry_buffer_set_text(buffer, stMQTTConnectionGetHost(connection) ,-1);

  buffer = gtk_entry_get_buffer(GTK_ENTRY(form_ui->entry_username));
  gtk_entry_buffer_set_text(buffer, stMQTTConnectionGetUsername(connection), -1);

  // FIX: Set password buffer 

  g_signal_handlers_unblock_by_func(form_ui->entry_name, entryNameChanged, form_ui);
}

// SETUP - creates the visual structure of the item (initially). 
void setupFactory(GtkListItemFactory *factory, GtkListItem *item, gpointer user_data){
  //ST_FormUI *form_ui = (ST_FormUI *)user_data;
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_widget_add_css_class(box, "form_listitem");

  GtkWidget *label = gtk_label_new(NULL);
  gtk_widget_add_css_class(label, "form_listitem_name");
  //gtk_list_item_set_child(item, box);
  g_object_set_data(G_OBJECT(item), "name", label);
  gtk_label_set_max_width_chars(GTK_LABEL(label), 1);
  gtk_label_set_ellipsize(GTK_LABEL(label), 3); // Limit label
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
  int position = g_list_model_get_n_items(G_LIST_MODEL(form_ui->connection_store)) - 1;
  stMQTTConnectionSetID(connection, (position + 1));
  addConnectionToJSON(connection);
  g_list_store_append(form_ui->connection_store, connection);
  gtk_single_selection_set_selected(form_ui->selection_model, position);
}

void entryNameChanged(GtkEntry *entry_name, gpointer user_data){
  ST_FormUI *form_ui = (ST_FormUI *)user_data;
  int position = gtk_single_selection_get_selected(form_ui->selection_model);
  STMQTTConnection *connection = g_list_model_get_item(G_LIST_MODEL(form_ui->connection_store), position);
  
  GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(form_ui->entry_name));
  const char *name = gtk_entry_buffer_get_text(buffer);
  stMQTTConnectionSetName(connection, name);

  g_list_model_items_changed(G_LIST_MODEL(form_ui->connection_store), position, 1, 1);
  g_object_unref(connection);
}

void entryPortChanged(GtkEntry *entry_port, gpointer user_data){
  ST_FormUI *form_ui = (ST_FormUI *)user_data;
  int position = gtk_single_selection_get_selected(form_ui->selection_model);
  STMQTTConnection *connection = g_list_model_get_item(G_LIST_MODEL(form_ui->connection_store), position);

  GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(form_ui->entry_port));
  const char *port = gtk_entry_buffer_get_text(buffer);
  long int number = strtol(port, NULL, 10);
  stMQTTConnectionSetPort(connection, (int)number);

  g_list_model_items_changed(G_LIST_MODEL(form_ui->connection_store), position, 1, 1);
  g_object_unref(connection);
}

void dropdownProtocolChanged(GObject *object, GParamSpec *pspec, gpointer user_data){
  ST_FormUI *form_ui = (ST_FormUI *)user_data;
  int position = gtk_single_selection_get_selected(form_ui->selection_model);
  STMQTTConnection *connection = g_list_model_get_item(G_LIST_MODEL(form_ui->connection_store), position);

  int item = gtk_drop_down_get_selected(GTK_DROP_DOWN(form_ui->dropdown_protocol));

  if(item == 0){
    stMQTTConnectionSetProtocol(connection, "mqtt");
  }else if (item == 1){
    stMQTTConnectionSetProtocol(connection, "ws");
  }

  g_list_model_items_changed(G_LIST_MODEL(form_ui->connection_store), position, 1, 1);
  g_object_unref(connection);
}

void entryHostChanged(GtkEntry *entry_host, gpointer user_data){
  ST_FormUI *form_ui = (ST_FormUI *)user_data;
  int position = gtk_single_selection_get_selected(form_ui->selection_model);
  STMQTTConnection *connection = g_list_model_get_item(G_LIST_MODEL(form_ui->connection_store), position);

  GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(form_ui->entry_host));
  const char *host = gtk_entry_buffer_get_text(buffer);
  stMQTTConnectionSetHost(connection, host);

  g_list_model_items_changed(G_LIST_MODEL(form_ui->connection_store), position, 1, 1);
  g_object_unref(connection);
}

void entryUsernameChanged(GtkEntry *entry_username, gpointer user_data){
  ST_FormUI *form_ui = (ST_FormUI *)user_data;
  int position = gtk_single_selection_get_selected(form_ui->selection_model);
  STMQTTConnection *connection = g_list_model_get_item(G_LIST_MODEL(form_ui->connection_store), position);

  GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(form_ui->entry_host));
  const char *username = gtk_entry_buffer_get_text(buffer);
  stMQTTConnectionSetUsername(connection, username);

  g_list_model_items_changed(G_LIST_MODEL(form_ui->connection_store), position, 1, 1);
  g_object_unref(connection);
}

// FIX: Password does not updating
void entryPasswordChanged(GtkEntry *entry_password, gpointer user_data){
  ST_FormUI *form_ui = (ST_FormUI *)user_data;
  int position = gtk_single_selection_get_selected(form_ui->selection_model);
  STMQTTConnection *connection = g_list_model_get_item(G_LIST_MODEL(form_ui->connection_store), position);

  GtkEditable *editable = GTK_EDITABLE(form_ui->entry_password);
  const char *password = gtk_editable_get_text(editable);
  stMQTTConnectionSetPassword(connection, password);

  g_object_unref(connection);
}

void saveConnection(GtkButton *button, gpointer user_data){
  ST_FormUI *form_ui = (ST_FormUI *)user_data;
  int position = gtk_single_selection_get_selected(form_ui->selection_model);
  STMQTTConnection *connection = g_list_model_get_item(G_LIST_MODEL(form_ui->connection_store), position);
  updateConnectionInJSON(connection, position);
}
