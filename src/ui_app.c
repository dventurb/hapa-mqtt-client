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
  gtk_window_set_title(GTK_WINDOW(app_ui->window), "HAPA IoT");
  gtk_window_set_default_size(GTK_WINDOW(app_ui->window), 1000, 600);
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

  // HOMEPAGE
  initHomeUI(&app_ui->home_ui);
  gtk_stack_add_named(GTK_STACK(app_ui->stack), app_ui->home_ui.fixed, "homepage");

  // FORM PAGE 
  initFormUI(&app_ui->form_ui);
  gtk_stack_add_named(GTK_STACK(app_ui->stack), app_ui->form_ui.fixed, "formpage");
  
  // Set the homepage as the currently visible stack page.  
  gtk_stack_set_visible_child_name(GTK_STACK(app_ui->stack), "homepage");
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

  gtk_stack_set_visible_child_name(GTK_STACK(app_ui->stack), "homepage");

  gtk_image_set_from_file(GTK_IMAGE(app_ui->top_box.button_switch.image), NAV_CONNECTION_PATH);
  gtk_widget_set_size_request(app_ui->top_box.button_switch.image, 24, 24);
  gtk_widget_set_margin_end(app_ui->top_box.button_switch.button, 40);
  gtk_label_set_text(GTK_LABEL(app_ui->top_box.button_switch.label), "CONNECTAR");

  g_signal_handlers_disconnect_by_func(app_ui->top_box.button_switch.button, switchToHome, app_ui);

  g_signal_connect(app_ui->top_box.button_switch.button, "clicked", G_CALLBACK(switchToForm), app_ui);
}
