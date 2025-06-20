#include "ui_app.h"

void createApplication(GtkApplication *app, gpointer user_data){
  ST_AppWidgets *app_ui = (ST_AppWidgets *)user_data; // Casting
  initAppWidgets(app_ui, app);

  // The application start in the homepage, so the button_switch will be first connect
  // to the switchToForm.
  g_signal_connect(app_ui->top_box.button_switch.button, "clicked", G_CALLBACK(switchToForm), app_ui);

  // Apply CSS styles to the application
  GtkCssProvider *provider = gtk_css_provider_new();
  GFile *css_file = g_file_new_for_path(STYLE_CSS_PATH);
  gtk_css_provider_load_from_file(provider, css_file);
  gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  gtk_window_present(GTK_WINDOW(app_ui->window));
  g_object_unref(css_file);
}

void initAppWidgets(ST_AppWidgets *app_ui, GtkApplication *app){
  
  // WINDOW
  app_ui->window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(app_ui->window), "HAPA MQTT Client");
  gtk_window_set_default_size(GTK_WINDOW(app_ui->window), 1000, 525);
  gtk_window_set_icon_name(GTK_WINDOW(app_ui->window), "hapa-app-icon.png");
  gtk_window_set_deletable(GTK_WINDOW(app_ui->window), TRUE);
  gtk_window_set_resizable(GTK_WINDOW(app_ui->window), FALSE);

  // MAIN BOX
  app_ui->main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_window_set_child(GTK_WINDOW(app_ui->window), app_ui->main_box);

  // TOP BOX
  initTopBox(&app_ui->top_box);
  gtk_box_append(GTK_BOX(app_ui->main_box), app_ui->top_box.box);

  // STACK - Switch between pages
  app_ui->stack = gtk_stack_new();
  gtk_stack_set_transition_type(GTK_STACK(app_ui->stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT);
  gtk_box_append(GTK_BOX(app_ui->main_box), app_ui->stack);

  // FORM PAGE 
  initFormUI(&app_ui->form_ui);
  gtk_stack_add_named(GTK_STACK(app_ui->stack), app_ui->form_ui.main_box, "formpage");
  
  // HOMEPAGE
  initHomeUI(&app_ui->home_ui);
  gtk_stack_add_named(GTK_STACK(app_ui->stack), app_ui->home_ui.main_box, "homepage");
  g_signal_connect(app_ui->home_ui.selection_connection, "selection-changed", G_CALLBACK(selectionConnection), &app_ui->home_ui);

  gtk_stack_set_visible_child_name(GTK_STACK(app_ui->stack), "homepage");
  
  selectionConnection(GTK_SELECTION_MODEL(app_ui->home_ui.selection_connection), 1, 1, &app_ui->home_ui);
  selectionTopic(GTK_SELECTION_MODEL(app_ui->home_ui.selection_topic), 1, 1, &app_ui->home_ui);
}

void switchToForm(GtkButton *button, gpointer user_data){
  ST_AppWidgets *app_ui = (ST_AppWidgets *)user_data;

  gtk_stack_set_visible_child_name(GTK_STACK(app_ui->stack), "formpage");
  
  gtk_image_set_from_file(GTK_IMAGE(app_ui->top_box.button_switch.image), NAV_BACK_PATH);
  gtk_widget_set_size_request(app_ui->top_box.button_switch.image, 24, 24);
  gtk_widget_set_margin_end(app_ui->top_box.button_switch.button, 40);
  gtk_label_set_text(GTK_LABEL(app_ui->top_box.button_switch.label), "VOLTAR");

  // Disconnecting the previous.
  g_signal_handlers_disconnect_by_func(app_ui->top_box.button_switch.button, switchToForm, app_ui);

  // Connecting the new signal, now we are in the formpage, so the button_switch will be 
  // connect to the switchToHome.
  g_signal_connect(app_ui->top_box.button_switch.button, "clicked", G_CALLBACK(switchToHome), app_ui);
}

void switchToHome(GtkButton *button, gpointer user_data){
  ST_AppWidgets *app_ui = (ST_AppWidgets *)user_data;
  
  app_ui->home_ui.connection_store = app_ui->form_ui.connections_ui.connection_store;
  
  app_ui->home_ui.selection_connection = gtk_single_selection_new(G_LIST_MODEL(app_ui->home_ui.connection_store));
  gtk_list_view_set_model(GTK_LIST_VIEW(app_ui->home_ui.list_view_connection), GTK_SELECTION_MODEL(app_ui->home_ui.selection_connection));
  int position = gtk_single_selection_get_selected(app_ui->home_ui.selection_connection);
  STMQTTConnection *connection = g_list_model_get_item(G_LIST_MODEL(app_ui->home_ui.connection_store), position);
  app_ui->home_ui.connection = connection;

  g_signal_connect(app_ui->home_ui.selection_connection, "selection-changed", G_CALLBACK(selectionConnection), &app_ui->home_ui);
 

  app_ui->home_ui.topics_store = stMQTTConnectionGetTopics(connection);
  app_ui->home_ui.selection_topic = gtk_single_selection_new(G_LIST_MODEL(app_ui->home_ui.topics_store));
  gtk_list_view_set_model(GTK_LIST_VIEW(app_ui->home_ui.list_view_topic), GTK_SELECTION_MODEL(app_ui->home_ui.selection_topic));
  position = gtk_single_selection_get_selected(app_ui->home_ui.selection_topic);
  STMQTTTopic *topic = g_list_model_get_item(G_LIST_MODEL(app_ui->home_ui.topics_store), 1);
  app_ui->home_ui.topic = topic;

  g_signal_connect(app_ui->home_ui.selection_topic, "selection-changed", G_CALLBACK(selectionTopic), &app_ui->home_ui);

  app_ui->home_ui.message_store = stMQTTTopicGetMessage(topic);
  GtkNoSelection *no_selection = gtk_no_selection_new(G_LIST_MODEL(app_ui->home_ui.message_store));
  gtk_list_view_set_model(GTK_LIST_VIEW(app_ui->home_ui.message_list_view), GTK_SELECTION_MODEL(no_selection));
  
  gtk_stack_set_visible_child_name(GTK_STACK(app_ui->stack), "homepage");
  gtk_image_set_from_file(GTK_IMAGE(app_ui->top_box.button_switch.image), NAV_CONNECTION_PATH);
  gtk_widget_set_size_request(app_ui->top_box.button_switch.image, 24, 24);
  gtk_widget_set_margin_end(app_ui->top_box.button_switch.button, 40);
  gtk_label_set_text(GTK_LABEL(app_ui->top_box.button_switch.label), "CONECTAR");

  g_signal_handlers_disconnect_by_func(app_ui->top_box.button_switch.button, switchToHome, app_ui);

  g_signal_connect(app_ui->top_box.button_switch.button, "clicked", G_CALLBACK(switchToForm), app_ui);
}

void selectionConnection(GtkSelectionModel *selection_model, int position, int n_items, gpointer user_data){
  ST_HomeUI *home_ui = (ST_HomeUI *)user_data;

  position = gtk_single_selection_get_selected(home_ui->selection_connection);
  STMQTTConnection *connection = g_list_model_get_item(G_LIST_MODEL(home_ui->connection_store), position);
  
  home_ui->connection = connection;
  home_ui->topics_store = stMQTTConnectionGetTopics(connection);
  
  home_ui->selection_topic = gtk_single_selection_new(G_LIST_MODEL(home_ui->topics_store));
  gtk_list_view_set_model(GTK_LIST_VIEW(home_ui->list_view_topic), GTK_SELECTION_MODEL(home_ui->selection_topic));
  
  position = gtk_single_selection_get_selected(home_ui->selection_topic);
  STMQTTTopic *topic = g_list_model_get_item(G_LIST_MODEL(home_ui->topics_store), position);
  
  if(home_ui->is_connected && home_ui->topic){
    mosquitto_unsubscribe(home_ui->mosq, NULL, stMQTTTopicGetName(home_ui->topic));
    mosquitto_subscribe(home_ui->mosq, NULL, stMQTTTopicGetName(topic), atoi(stMQTTTopicGetQoS(topic)));
  }

  home_ui->topic = topic;

  home_ui->message_store = stMQTTTopicGetMessage(topic);
  GtkNoSelection *no_selection = gtk_no_selection_new(G_LIST_MODEL(home_ui->message_store));
  gtk_list_view_set_model(GTK_LIST_VIEW(home_ui->message_list_view), GTK_SELECTION_MODEL(no_selection));

  g_signal_connect(home_ui->selection_topic, "selection-changed", G_CALLBACK(selectionTopic), home_ui);

  gtk_label_set_text(GTK_LABEL(home_ui->label_top_name), stMQTTConnectionGetName(connection));

  char *str = malloc(strlen(stMQTTConnectionGetHost(connection)) + strlen(stMQTTConnectionGetProtocol(connection)) + 4);
  sprintf(str, "%s//:%s", stMQTTConnectionGetProtocol(connection), stMQTTConnectionGetHost(connection));
  gtk_label_set_text(GTK_LABEL(home_ui->label_top_host), str);
  free(str);
}

void selectionTopic(GtkSelectionModel *selection_model, int position, int n_items, gpointer user_data){
  ST_HomeUI *home_ui = (ST_HomeUI *)user_data;

  position = gtk_single_selection_get_selected(home_ui->selection_topic);
  STMQTTTopic *topic = g_list_model_get_item(G_LIST_MODEL(home_ui->topics_store), position);
  
  if(home_ui->is_connected && home_ui->topic){
    mosquitto_unsubscribe(home_ui->mosq, NULL, stMQTTTopicGetName(home_ui->topic));
    mosquitto_subscribe(home_ui->mosq, NULL, stMQTTTopicGetName(topic), atoi(stMQTTTopicGetQoS(topic)));
  } 

  home_ui->message_store = stMQTTTopicGetMessage(topic);
  GtkNoSelection *no_selection = gtk_no_selection_new(G_LIST_MODEL(home_ui->message_store));
  gtk_list_view_set_model(GTK_LIST_VIEW(home_ui->message_list_view), GTK_SELECTION_MODEL(no_selection));

  home_ui->topic = topic;
}
