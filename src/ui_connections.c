#include "ui_connections.h"

void initConnectionsUI(ST_ConnectionsUI *connections_ui, GtkWidget *stack){
  // LAYOUT
  connections_ui->fixed = gtk_fixed_new();
  
  // TEXT 
  connections_ui->label = gtk_label_new("Conexão MQTT");
  gtk_widget_add_css_class(connections_ui->label, "connections_label_title");
  gtk_fixed_put(GTK_FIXED(connections_ui->fixed), connections_ui->label, 10, 30);

  // NAME 
  connections_ui->label = gtk_label_new("Nome");
  gtk_widget_add_css_class(connections_ui->label, "connections_label_name");
  gtk_fixed_put(GTK_FIXED(connections_ui->fixed), connections_ui->label, 30, 90);
  connections_ui->entry_name = gtk_entry_new();
  gtk_widget_set_size_request(connections_ui->entry_name, 300, -1);
  gtk_widget_add_css_class(connections_ui->entry_name, "connections_entry_name");
  gtk_fixed_put(GTK_FIXED(connections_ui->fixed), connections_ui->entry_name, 30, 110);
 
  // VALIDATE CERTIFICATE (SWITCH)
  connections_ui->label = gtk_label_new("Validar certificado");
  gtk_widget_add_css_class(connections_ui->label, "connections_label_certificate");
  gtk_fixed_put(GTK_FIXED(connections_ui->fixed), connections_ui->label, 400, 120);
  connections_ui->switch_certificate = gtk_switch_new();
  gtk_fixed_put(GTK_FIXED(connections_ui->fixed), connections_ui->switch_certificate, 440, 90);

  // ENCRYPTION (SWITCH) 
  connections_ui->label = gtk_label_new("Encriptação (tls)");
  gtk_widget_add_css_class(connections_ui->label, "connections_label_encryption");
  gtk_fixed_put(GTK_FIXED(connections_ui->fixed), connections_ui->label, 600, 120);
  connections_ui->switch_encryption = gtk_switch_new();
  gtk_fixed_put(GTK_FIXED(connections_ui->fixed), connections_ui->switch_encryption, 635, 90);

  // PORT 
  connections_ui->label = gtk_label_new("Porta");
  gtk_widget_add_css_class(connections_ui->label, "connections_label_port");
  gtk_fixed_put(GTK_FIXED(connections_ui->fixed), connections_ui->label, 600, 190);
  connections_ui->buffer = gtk_entry_buffer_new("1883", 4);
  connections_ui->entry_port = gtk_entry_new_with_buffer(connections_ui->buffer);
  gtk_widget_set_size_request(connections_ui->entry_port, 120, -1);
  gtk_widget_add_css_class(connections_ui->entry_port, "connections_entry_port");
  gtk_fixed_put(GTK_FIXED(connections_ui->fixed), connections_ui->entry_port, 600, 210);

  // PROTOCOL
  connections_ui->label = gtk_label_new("Protocolo");
  gtk_widget_add_css_class(connections_ui->label, "connections_label_protocol");
  gtk_fixed_put(GTK_FIXED(connections_ui->fixed), connections_ui->label, 30, 190);
  const char *strings[3] = {"mqtt//:", "ws://", NULL};
  connections_ui->dropdown_protocol = gtk_drop_down_new_from_strings(strings);
  gtk_drop_down_set_show_arrow(GTK_DROP_DOWN(connections_ui->dropdown_protocol), TRUE);
  gtk_widget_set_size_request(connections_ui->dropdown_protocol, 100, -1);
  gtk_widget_add_css_class(connections_ui->dropdown_protocol, "connections_dropdown_protocol");
  gtk_fixed_put(GTK_FIXED(connections_ui->fixed), connections_ui->dropdown_protocol, 30, 210);

  // HOST
  connections_ui->label = gtk_label_new("Host");
  gtk_widget_add_css_class(connections_ui->label, "connections_label_host");
  gtk_fixed_put(GTK_FIXED(connections_ui->fixed), connections_ui->label, 160, 190);
  connections_ui->entry_host = gtk_entry_new();
  gtk_widget_set_size_request(connections_ui->entry_host, 410, -1);
  gtk_widget_add_css_class(connections_ui->entry_host, "connections_entry_host");
  gtk_fixed_put(GTK_FIXED(connections_ui->fixed), connections_ui->entry_host, 160, 210);

  // USERNAME 
  connections_ui->label = gtk_label_new("Username");
  gtk_widget_add_css_class(connections_ui->label, "connections_label_username");
  gtk_fixed_put(GTK_FIXED(connections_ui->fixed), connections_ui->label, 30, 310);
  connections_ui->entry_username = gtk_entry_new();
  gtk_widget_set_size_request(connections_ui->entry_username, 330, -1);
  gtk_widget_add_css_class(connections_ui->entry_username, "connections_entry_username");
  gtk_fixed_put(GTK_FIXED(connections_ui->fixed), connections_ui->entry_username, 30, 330);

  // PASSWORD
  connections_ui->label = gtk_label_new("Password");
  gtk_widget_add_css_class(connections_ui->label, "connections_label_password");
  gtk_fixed_put(GTK_FIXED(connections_ui->fixed), connections_ui->label, 390, 310);
  connections_ui->entry_password = gtk_entry_new();
  gtk_widget_set_size_request(connections_ui->entry_password, 310, -1);
  gtk_widget_add_css_class(connections_ui->entry_password, "connections_entry_password");
  gtk_fixed_put(GTK_FIXED(connections_ui->fixed), connections_ui->entry_password, 390, 330);

  // SIGNALS (FORM SECTION)
  g_signal_connect(connections_ui->entry_name, "changed", G_CALLBACK(entryNameChanged), connections_ui);
  g_signal_connect(connections_ui->switch_certificate, "notify::active", G_CALLBACK(switchCertificateChanged), connections_ui);
  g_signal_connect(connections_ui->switch_encryption, "notify::active", G_CALLBACK(switchEncryptionChanged), connections_ui);
  g_signal_connect(connections_ui->entry_port, "changed", G_CALLBACK(entryPortChanged), connections_ui);
  g_signal_connect(connections_ui->dropdown_protocol, "notify::selected", G_CALLBACK(dropdownProtocolChanged), connections_ui);
  g_signal_connect(connections_ui->entry_host, "changed", G_CALLBACK(entryHostChanged), connections_ui);
  g_signal_connect(connections_ui->entry_username, "changed", G_CALLBACK(entryUsernameChanged), connections_ui);
  g_signal_connect(connections_ui->entry_password, "changed", G_CALLBACK(entryPasswordChanged), connections_ui);

  // BUTTONS (CONNECTION SECTION)

  // CONNECT (BUTTON)
  createButtonWithImageLabel(&connections_ui->button_connect, CONNECTION_ACTIVATE_PATH, "CONECTAR");
  gtk_widget_add_css_class(connections_ui->button_connect.button, "connections_button_connect");
  gtk_widget_add_css_class(connections_ui->button_connect.label, "connections_button_connect_label");
  gtk_widget_set_size_request(connections_ui->button_connect.button, 75, 35);
  gtk_fixed_put(GTK_FIXED(connections_ui->fixed), connections_ui->button_connect.button, 570, 430);

  // SAVE (BUTTON)
  createButtonWithImageLabel(&connections_ui->button_save, CONNECTION_SAVE_PATH, "SALVAR");
  gtk_widget_add_css_class(connections_ui->button_save.button, "connections_button_save");
  gtk_widget_add_css_class(connections_ui->button_save.label, "connections_button_save_label");
  gtk_widget_set_size_request(connections_ui->button_save.button, 70, 35);
  gtk_fixed_put(GTK_FIXED(connections_ui->fixed), connections_ui->button_save.button, 420, 430);
  
  // DELETE (BUTTON)
  createButtonWithImageLabel(&connections_ui->button_delete, CONNECTION_DELETE_PATH, "REMOVER");
  gtk_widget_add_css_class(connections_ui->button_delete.button, "connections_button_delete");
  gtk_widget_add_css_class(connections_ui->button_delete.label, "connections_button_delete_label");
  gtk_widget_set_size_request(connections_ui->button_delete.button, 70, 35);
  gtk_fixed_put(GTK_FIXED(connections_ui->fixed), connections_ui->button_delete.button, 30, 430);

  // OPTIONS (BUTTON)
  createButtonWithImageLabel(&connections_ui->button_options, CONNECTION_OPTIONS_PATH, "OPÇÕES");
  gtk_widget_add_css_class(connections_ui->button_options.button, "connections_button_options");
  gtk_widget_add_css_class(connections_ui->button_options.label, "connections_button_options_label");
  gtk_widget_set_size_request(connections_ui->button_options.button, 70, 35);
  gtk_fixed_put(GTK_FIXED(connections_ui->fixed), connections_ui->button_options.button, 190, 430);

  // SIGNALS (BUTTONS)
  g_signal_connect(connections_ui->button_save.button, "clicked", G_CALLBACK(saveConnection), connections_ui);
  g_signal_connect(connections_ui->button_delete.button, "clicked", G_CALLBACK(deleteConnection), connections_ui);
  g_signal_connect(connections_ui->button_options.button, "clicked", G_CALLBACK(switchToTopics), stack);

  // TODO - NOTIFICATION

}

void entryNameChanged(GtkEntry *entry_name, gpointer user_data){
  ST_ConnectionsUI *connections_ui = (ST_ConnectionsUI *)user_data;
  int position = gtk_single_selection_get_selected(connections_ui->selection_model);
  STMQTTConnection *connection = g_list_model_get_item(G_LIST_MODEL(connections_ui->connection_store), position);
  
  GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(connections_ui->entry_name));
  const char *name = gtk_entry_buffer_get_text(buffer);
  stMQTTConnectionSetName(connection, name);

  g_list_model_items_changed(G_LIST_MODEL(connections_ui->connection_store), position, 1, 1);
  g_object_unref(connection);
}

void switchCertificateChanged(GObject *switchCertValidation, GParamSpec *pspec, gpointer user_data){
  ST_ConnectionsUI *connections_ui = (ST_ConnectionsUI *)user_data;
  int position = gtk_single_selection_get_selected(connections_ui->selection_model);
  STMQTTConnection *connection = g_list_model_get_item(G_LIST_MODEL(connections_ui->connection_store), position);

  gboolean state = gtk_switch_get_active(GTK_SWITCH(connections_ui->switch_certificate));
  stMQTTConnectionSetCertValidation(connection, state);

  g_list_model_items_changed(G_LIST_MODEL(connections_ui->connection_store), position, 1, 1);
  g_object_unref(connection);
}

void switchEncryptionChanged(GObject *switchEncryption, GParamSpec *pspec, gpointer user_data){
  ST_ConnectionsUI *connections_ui = (ST_ConnectionsUI *)user_data;
  int position = gtk_single_selection_get_selected(connections_ui->selection_model);
  STMQTTConnection *connection = g_list_model_get_item(G_LIST_MODEL(connections_ui->connection_store), position);

  gboolean state = gtk_switch_get_active(GTK_SWITCH(connections_ui->switch_encryption));
  stMQTTConnectionSetEncryption(connection, state);

  g_list_model_items_changed(G_LIST_MODEL(connections_ui->connection_store), position, 1, 1);
  g_object_unref(connection);
}

void entryPortChanged(GtkEntry *entry_port, gpointer user_data){
  ST_ConnectionsUI *connections_ui = (ST_ConnectionsUI *)user_data;
  int position = gtk_single_selection_get_selected(connections_ui->selection_model);
  STMQTTConnection *connection = g_list_model_get_item(G_LIST_MODEL(connections_ui->connection_store), position);

  GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(connections_ui->entry_port));
  const char *port = gtk_entry_buffer_get_text(buffer);
  long int number = strtol(port, NULL, 10);
  stMQTTConnectionSetPort(connection, (int)number);

  g_list_model_items_changed(G_LIST_MODEL(connections_ui->connection_store), position, 1, 1);
  g_object_unref(connection);
}

void dropdownProtocolChanged(GObject *object, GParamSpec *pspec, gpointer user_data){
  ST_ConnectionsUI *connections_ui = (ST_ConnectionsUI *)user_data;
  int position = gtk_single_selection_get_selected(connections_ui->selection_model);
  STMQTTConnection *connection = g_list_model_get_item(G_LIST_MODEL(connections_ui->connection_store), position);

  int item = gtk_drop_down_get_selected(GTK_DROP_DOWN(connections_ui->dropdown_protocol));
  if(item == 0){
    stMQTTConnectionSetProtocol(connection, "mqtt");
  }else if (item == 1){
    stMQTTConnectionSetProtocol(connection, "ws");
  }

  g_list_model_items_changed(G_LIST_MODEL(connections_ui->connection_store), position, 1, 1);
  g_object_unref(connection);
}

void entryHostChanged(GtkEntry *entry_host, gpointer user_data){
  ST_ConnectionsUI *connections_ui = (ST_ConnectionsUI *)user_data;
  int position = gtk_single_selection_get_selected(connections_ui->selection_model);
  STMQTTConnection *connection = g_list_model_get_item(G_LIST_MODEL(connections_ui->connection_store), position);

  GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(connections_ui->entry_host));
  const char *host = gtk_entry_buffer_get_text(buffer);
  stMQTTConnectionSetHost(connection, host);

  g_list_model_items_changed(G_LIST_MODEL(connections_ui->connection_store), position, 1, 1);
  g_object_unref(connection);
}

void entryUsernameChanged(GtkEntry *entry_username, gpointer user_data){
  ST_ConnectionsUI *connections_ui = (ST_ConnectionsUI *)user_data;
  int position = gtk_single_selection_get_selected(connections_ui->selection_model);
  STMQTTConnection *connection = g_list_model_get_item(G_LIST_MODEL(connections_ui->connection_store), position);

  GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(connections_ui->entry_host));
  const char *username = gtk_entry_buffer_get_text(buffer);
  stMQTTConnectionSetUsername(connection, username);

  g_list_model_items_changed(G_LIST_MODEL(connections_ui->connection_store), position, 1, 1);
  g_object_unref(connection);
}

// FIX: Password does not updating
void entryPasswordChanged(GtkEntry *entry_password, gpointer user_data){
  ST_ConnectionsUI *connections_ui = (ST_ConnectionsUI *)user_data;
  int position = gtk_single_selection_get_selected(connections_ui->selection_model);
  STMQTTConnection *connection = g_list_model_get_item(G_LIST_MODEL(connections_ui->connection_store), position);

  GtkEditable *editable = GTK_EDITABLE(connections_ui->entry_password);
  const char *password = gtk_editable_get_text(editable);
  stMQTTConnectionSetPassword(connection, password);

  g_object_unref(connection);
}

void saveConnection(GtkButton *button, gpointer user_data){
  ST_ConnectionsUI *connections_ui = (ST_ConnectionsUI *)user_data;
  int position = gtk_single_selection_get_selected(connections_ui->selection_model);
  STMQTTConnection *connection = g_list_model_get_item(G_LIST_MODEL(connections_ui->connection_store), position);
  updateConnectionInJSON(connection);
}

void deleteConnection(GtkButton *button, gpointer user_data){
  ST_ConnectionsUI *connections_ui = (ST_ConnectionsUI *)user_data;
  int n_items = g_list_model_get_n_items(G_LIST_MODEL(connections_ui->connection_store));
  int position = gtk_single_selection_get_selected(connections_ui->selection_model);
  if(n_items > 1){
    STMQTTConnection *connection = g_list_model_get_item(G_LIST_MODEL(connections_ui->connection_store), position);
    if(position > 0){
      selectedItemChanged(position - 1, connections_ui);
    }else if(position == 0){
      selectedItemChanged(position + 1, connections_ui);
    }
    g_list_store_remove(connections_ui->connection_store, position);
    deleteConnectionInJSON(connection);
    stMQTTConnectionFree(connection);
 }
}

void switchToTopics(GtkButton *button, gpointer user_data){
  GtkWidget *stack = (GtkWidget *)user_data;

  gtk_stack_set_visible_child_name(GTK_STACK(stack), "topics");
}

void selectedItemChanged(int position, ST_ConnectionsUI *connections_ui){
  STMQTTConnection *connection = g_list_model_get_item(G_LIST_MODEL(connections_ui->connection_store), position);

  // Update Connection Form
  GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(connections_ui->entry_name));
  gtk_entry_buffer_set_text(buffer, stMQTTConnectionGetName(connection), -1);

  gtk_switch_set_active(GTK_SWITCH(connections_ui->switch_certificate), stMQTTConnectionGetCertValidation(connection));
  gtk_switch_set_active(GTK_SWITCH(connections_ui->switch_encryption), stMQTTConnectionGetEncryption(connection));

  int length = snprintf(NULL, 0, "%d", stMQTTConnectionGetPort(connection));
  char *port = malloc(length + 1);
  sprintf(port, "%d", stMQTTConnectionGetPort(connection));
  buffer = gtk_entry_get_buffer(GTK_ENTRY(connections_ui->entry_port));
  gtk_entry_buffer_set_text(buffer, port, -1);
  free(port);

  if((strcmp(stMQTTConnectionGetProtocol(connection), "ws")) == 0){
    gtk_drop_down_set_selected(GTK_DROP_DOWN(connections_ui->dropdown_protocol), 1);
  }else {
    gtk_drop_down_set_selected(GTK_DROP_DOWN(connections_ui->dropdown_protocol), 0);
  }

  buffer = gtk_entry_get_buffer(GTK_ENTRY(connections_ui->entry_host));
  gtk_entry_buffer_set_text(buffer, stMQTTConnectionGetHost(connection) ,-1);

  buffer = gtk_entry_get_buffer(GTK_ENTRY(connections_ui->entry_username));
  gtk_entry_buffer_set_text(buffer, stMQTTConnectionGetUsername(connection), -1);

  // FIX: Set password buffer
}
