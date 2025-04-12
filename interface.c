#include <gtk/gtk.h>
#include <json-c/json_object.h>
#include <json-c/json_object_iterator.h>
#include <json-c/json_tokener.h>
#include <json-c/json_types.h>
#include <json-c/json_util.h>
#include <mosquitto.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>
#include "conexao_mqtt.h"

// STRUCT CONEXÃO MQTT
struct _STConexaoMQTT {
  GObject parent_instance;
  int id;
  char *nome;
  char *protocolo;
  char *host;
  char *username;
  char *password;
  int porta;
};

G_DEFINE_TYPE(STConexaoMQTT, st_conexao_mqtt, G_TYPE_OBJECT)

static void st_conexao_mqtt_finalize(GObject *gobject){
  STConexaoMQTT *conexao = ST_CONEXAO_MQTT(gobject);

  g_free(conexao->nome);
  g_free(conexao->protocolo);
  g_free(conexao->host);
  g_free(conexao->username);
  g_free(conexao->password);

  G_OBJECT_CLASS(st_conexao_mqtt_parent_class)->finalize(gobject);
}

static void st_conexao_mqtt_class_init(STConexaoMQTTClass *klass){
  GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
  gobject_class->finalize = st_conexao_mqtt_finalize;
}

static void st_conexao_mqtt_init(STConexaoMQTT *conexao){
  conexao->nome = g_strdup("nova conexão");
  conexao->protocolo = g_strdup("mqtt");
  conexao->host = NULL;
  conexao->username = NULL;
  conexao->password = NULL;
  conexao->porta = 1883;
}

// STRUCT BOTÕES (Imagem + Texto)
typedef struct {
    GtkWidget *button;
    GtkWidget *box;
    GtkWidget *label;
    GtkWidget *image;
} ST_BUTTON;

// STRUCT BOX SUPERIOR
typedef struct {
    GtkWidget *box;
    GtkWidget *button_menu;
    GtkWidget *search_box;
    GtkWidget *search_entry;
    GtkWidget *image;
    GtkWidget *titulo;
    ST_BUTTON button_switch;
} ST_TopBox;

// STRUCT MAIN (Página principal)
typedef struct {
    GtkWidget *fixed;
} ST_MainUI;

// STRUCT FORMULÁRIO (Página de adicionar nova conexão)
typedef struct {
    GtkWidget *fixed;
    GtkWidget *label;
    GtkWidget *image_add;
    GtkWidget *label_connections;
    GtkWidget *box_connections;
    GtkGesture *gesture;
    GtkWidget *entry_name;
    GtkWidget *entry_host;
    GtkWidget *entry_port;
    GtkEntryBuffer *buffer;
    GtkWidget *dropdown_protocolo;
    GtkWidget *entry_username;
    GtkWidget *entry_password;
    ST_BUTTON button_save;
    ST_BUTTON button_delete;
    ST_BUTTON button_connect;
    ST_BUTTON button_advance;
    GtkWidget *list_view;
    GtkListItemFactory *factory;
    GtkSingleSelection *selection_model;
    GListStore *conexao_store;
    GtkWidget *scrolled;
} ST_FormUI;


// STRUCT TODOS OS WIDGETS
typedef struct {
    GtkWidget *window;
    GtkWidget *main_box;
    GtkWidget *stack;
    ST_TopBox top_box;
    ST_FormUI form_ui;
    ST_MainUI main_ui;
} ST_AppWidgets;

// typedef struct ST_INFO {
// struct mosquitto *mosq;
// char *broker;
// int port;
//} ST_INFO;

// SIGNAL
static void activate(GtkApplication *app, gpointer user_data);
static void switchToMain(GtkButton *button, gpointer user_data);
static void switchToForm(GtkButton *button, gpointer user_data);
static void selectionChanged(GtkSelectionModel *selection_model, guint position, guint n_items, gpointer user_data);
static void entryNameChanged(GtkEntry *entry_name, gpointer user_data);
static void dropdownProtocoloChanged(GObject *object, GParamSpec *pspec, gpointer user_data);
static void entryHostChanged(GtkEntry *entry_host, gpointer user_data);
static void entryPortChanged(GtkEntry *entry_port, gpointer user_data);
static void entryUsernameChanged(GtkEntry *entry_username, gpointer user_data);
static void entryPasswordChanged(GtkEntry *entry_password, gpointer user_data);
static void setupListConnections(GtkListItemFactory *factory, GtkListItem *item, gpointer user_data);
static void bindListConnections(GtkListItemFactory *factory, GtkListItem *item, gpointer user_data);
static void addNewConnections(GtkGestureClick *gesture, int n_press, double x, double y, gpointer user_data);
static void saveConnection(GtkButton *button, gpointer user_data);

// FUNÇÕES
void createButtonWithImageLabel(ST_BUTTON *button, const char *pathToImage, const char *textButton);
void initTopBox(ST_TopBox *top_box);
void initMainUI(ST_MainUI *main_ui);
void initFormUI(ST_FormUI *form_ui);
void initAppWidgets(ST_AppWidgets *app_ui, GtkApplication *app);
void adicionarJSON(STConexaoMQTT *conexao);
void alterarJSON(STConexaoMQTT *conexao, int position);
void carregarJSON(ST_FormUI *form_ui);
STConexaoMQTT *st_conexao_mqtt_new(void);
// void encerrarMQTT(struct mosquitto *mosq);

// MAIN
int main(int argc, char *argv[]) {
    GtkApplication *app;
    ST_AppWidgets app_ui;
    app = gtk_application_new("com.iot.hapa", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), &app_ui);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    // encerrarMQTT(info->mosq);
    return status;
}

static void activate(GtkApplication *app, gpointer user_data) {
    ST_AppWidgets *app_ui = (ST_AppWidgets *)user_data;
    initAppWidgets(app_ui, app);

    g_signal_connect(app_ui->top_box.button_switch.button, "clicked", G_CALLBACK(switchToForm), app_ui);

    GtkCssProvider *provider = gtk_css_provider_new();
    GFile *css_file = g_file_new_for_path("style.css");
    gtk_css_provider_load_from_file(provider, css_file);
    gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    gtk_window_present(GTK_WINDOW(app_ui->window));
    g_object_unref(css_file);
}

static void switchToMain(GtkButton *button, gpointer user_data) {
    ST_AppWidgets *app_ui = (ST_AppWidgets *)user_data;

    gtk_stack_set_visible_child_name(GTK_STACK(app_ui->stack), "main");

    gtk_image_set_from_file(GTK_IMAGE(app_ui->top_box.button_switch.image),"connect.png");
    gtk_widget_set_size_request(app_ui->top_box.button_switch.image, 24, 24);
    gtk_widget_set_margin_end(app_ui->top_box.button_switch.button, 40);
    gtk_label_set_text(GTK_LABEL(app_ui->top_box.button_switch.label), "CONECTAR");

    g_signal_handlers_disconnect_by_func(app_ui->top_box.button_switch.button, switchToMain, app_ui);
    g_signal_connect(app_ui->top_box.button_switch.button, "clicked",G_CALLBACK(switchToForm), app_ui);
}

static void switchToForm(GtkButton *button, gpointer user_data) {
    ST_AppWidgets *app_ui = (ST_AppWidgets *)user_data;

    gtk_stack_set_visible_child_name(GTK_STACK(app_ui->stack), "form");

    gtk_image_set_from_file(GTK_IMAGE(app_ui->top_box.button_switch.image),"back.png");
    gtk_widget_set_size_request(app_ui->top_box.button_switch.image, 24, 24);
    gtk_widget_set_margin_end(app_ui->top_box.button_switch.button, 40);
    gtk_label_set_text(GTK_LABEL(app_ui->top_box.button_switch.label), "VOLTAR");

    g_signal_handlers_disconnect_by_func(app_ui->top_box.button_switch.button,switchToForm, app_ui);
    g_signal_connect(app_ui->top_box.button_switch.button, "clicked",G_CALLBACK(switchToMain), app_ui);
}

static void selectionChanged(GtkSelectionModel *selection_model, guint position, guint n_items, gpointer user_data){
  ST_FormUI *form_ui = (ST_FormUI *)user_data;
  position = gtk_single_selection_get_selected(form_ui->selection_model);
  STConexaoMQTT *conexao = g_list_model_get_item(G_LIST_MODEL(form_ui->conexao_store), position);
  
  // DEBUG:
 // g_print("Posicao: %u", position);
 // g_print("Nome: %s\n", conexao->nome);

  g_signal_handlers_block_by_func(form_ui->entry_name, entryNameChanged, form_ui);

  GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(form_ui->entry_name));
  gtk_entry_buffer_set_text(buffer, conexao->nome ? conexao->nome : "", -1);

  buffer = gtk_entry_get_buffer(GTK_ENTRY(form_ui->entry_host));
  gtk_entry_buffer_set_text(buffer, conexao->host ? conexao->host : "", -1);

  char *porta = g_strdup_printf("%d", conexao->porta);
  buffer = gtk_entry_get_buffer(GTK_ENTRY(form_ui->entry_port));
  gtk_entry_buffer_set_text(buffer, porta, -1);
  g_free(porta);
  
  if((strcmp(conexao->protocolo, "ws")) == 0){
    gtk_drop_down_set_selected(GTK_DROP_DOWN(form_ui->dropdown_protocolo), 1);
  }else{
    gtk_drop_down_set_selected(GTK_DROP_DOWN(form_ui->dropdown_protocolo), 0);
  }
  buffer = gtk_entry_get_buffer(GTK_ENTRY(form_ui->entry_username));
  gtk_entry_buffer_set_text(buffer, conexao->username ? conexao->username : "", -1);

 g_signal_handlers_unblock_by_func(form_ui->entry_name, entryNameChanged, form_ui);
}

static void entryNameChanged(GtkEntry *entry_name, gpointer user_data){
  ST_FormUI *form_ui = (ST_FormUI *)user_data;
  guint position = gtk_single_selection_get_selected(form_ui->selection_model);
  
  STConexaoMQTT *conexao = g_list_model_get_item(G_LIST_MODEL(form_ui->conexao_store), position);

  GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(form_ui->entry_name));
  const char *nome = gtk_entry_buffer_get_text(buffer);
  
  g_free(conexao->nome);
  conexao->nome = g_strdup(nome);

  g_list_model_items_changed(G_LIST_MODEL(form_ui->conexao_store), position, 1 , 1); 
  g_object_unref(conexao);
}

static void dropdownProtocoloChanged(GObject *object, GParamSpec *pspec, gpointer user_data){
  ST_FormUI *form_ui = (ST_FormUI *)user_data;
  guint position = gtk_single_selection_get_selected(form_ui->selection_model);
  
  STConexaoMQTT *conexao = g_list_model_get_item(G_LIST_MODEL(form_ui->conexao_store), position);
  
  guint item = gtk_drop_down_get_selected(GTK_DROP_DOWN(form_ui->dropdown_protocolo));
  
  g_free(conexao->protocolo);

  if(item == 0){
    conexao->protocolo = g_strdup("mqtt");
  }else if(item == 1){
    conexao->protocolo = g_strdup("ws");
  }
  g_list_model_items_changed(G_LIST_MODEL(form_ui->conexao_store), position, 1, 1);
  g_object_unref(conexao);
}

static void entryHostChanged(GtkEntry *entry_host, gpointer user_data){
  ST_FormUI *form_ui = (ST_FormUI *)user_data;
  guint position = gtk_single_selection_get_selected(form_ui->selection_model);

  STConexaoMQTT *conexao = g_list_model_get_item(G_LIST_MODEL(form_ui->conexao_store), position);
  
  GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(form_ui->entry_host));
  const char *host = gtk_entry_buffer_get_text(buffer);

  g_free(conexao->host);
  conexao->host = g_strdup(host);

  g_list_model_items_changed(G_LIST_MODEL(form_ui->conexao_store), position, 1 , 1);
  g_object_unref(conexao);
}

static void entryPortChanged(GtkEntry *entry_port, gpointer user_data){
  ST_FormUI *form_ui = (ST_FormUI *)user_data;
  guint position = gtk_single_selection_get_selected(form_ui->selection_model);

  STConexaoMQTT *conexao = g_list_model_get_item(G_LIST_MODEL(form_ui->conexao_store), position);

  GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(form_ui->entry_port));
  const char *port = gtk_entry_buffer_get_text(buffer);
  long int num = strtol(port, NULL, 10);

  conexao->porta = (int)num;
  
  g_list_model_items_changed(G_LIST_MODEL(form_ui->conexao_store), position, 1, 1);
  g_object_unref(conexao);
}
  
static void entryUsernameChanged(GtkEntry *entry_username, gpointer user_data){
  ST_FormUI *form_ui = (ST_FormUI *)user_data;
  guint position = gtk_single_selection_get_selected(form_ui->selection_model);

  STConexaoMQTT *conexao = g_list_model_get_item(G_LIST_MODEL(form_ui->conexao_store), position);

  GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(form_ui->entry_username));
  const char *username = gtk_entry_buffer_get_text(buffer);

  g_free(conexao->username);
  conexao->username = g_strdup(username);

  g_list_model_items_changed(G_LIST_MODEL(form_ui->conexao_store), position, 1, 1);
  g_object_unref(conexao);
}

static void entryPasswordChanged(GtkEntry *entry_password, gpointer user_data){
  ST_FormUI *form_ui = (ST_FormUI *)user_data;
  guint position = gtk_single_selection_get_selected(form_ui->selection_model);

  STConexaoMQTT *conexao = g_list_model_get_item(G_LIST_MODEL(form_ui->conexao_store), position);
  
  GtkEditable *editable = GTK_EDITABLE(form_ui->entry_password);
  const char *password = gtk_editable_get_text(editable);
  g_free(conexao->password);
  conexao->password = g_strdup(password);
  g_object_unref(conexao);
}

static void setupListConnections(GtkListItemFactory *factory, GtkListItem *item, gpointer user_data) {
    ST_FormUI *form_ui = (ST_FormUI *)user_data;
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_add_css_class(box, "list_item");

    GtkWidget *label = gtk_label_new(NULL);
    gtk_widget_add_css_class(label, "list_item_label1");
    gtk_list_item_set_child(item, box);
    g_object_set_data(G_OBJECT(item), "label1", label);
    gtk_label_set_max_width_chars(GTK_LABEL(label), 1);
    gtk_label_set_ellipsize(GTK_LABEL(label), 3);
    gtk_box_append(GTK_BOX(box), label);
   
    label = gtk_label_new(NULL);
    gtk_widget_add_css_class(label, "list_item_label2");
    g_object_set_data(G_OBJECT(item), "label2", label);
    gtk_label_set_max_width_chars(GTK_LABEL(label), 1);
    gtk_label_set_ellipsize(GTK_LABEL(label), 3);
    //gtk_label_set_text(GTK_LABEL(label), texto);
  
    gtk_box_append(GTK_BOX(box), label);

    gtk_list_item_set_child(item, box);
}

static void bindListConnections(GtkListItemFactory *factory, GtkListItem *item, gpointer user_data) {
    ST_FormUI *form_ui = (ST_FormUI *)user_data;
    STConexaoMQTT *conexao = gtk_list_item_get_item(item);

    GtkWidget *label = g_object_get_data(G_OBJECT(item), "label1");
    gtk_label_set_max_width_chars(GTK_LABEL(label), 1);
    gtk_label_set_text(GTK_LABEL(label), conexao->nome ? conexao->nome : "");
    
    label = g_object_get_data(G_OBJECT(item), "label2");
    gtk_label_set_max_width_chars(GTK_LABEL(label), 1);
    char *texto = g_strdup_printf("%s://%s", conexao->protocolo ? conexao->protocolo : "", conexao->host ? conexao->host : "");
    gtk_label_set_text(GTK_LABEL(label), texto);
}

static void addNewConnections(GtkGestureClick *gesture, int n_press, double x, double y, gpointer user_data) {
    ST_FormUI *form_ui = (ST_FormUI *)user_data;
    STConexaoMQTT *conexao = st_conexao_mqtt_new();
    guint position = g_list_model_get_n_items(G_LIST_MODEL(form_ui->conexao_store)) - 1;
    conexao->id = position + 1; 
    adicionarJSON(conexao);
    g_list_store_append(form_ui->conexao_store, conexao);
    gtk_single_selection_set_selected(form_ui->selection_model, position);
}

static void saveConnection(GtkButton *button, gpointer user_data){
  ST_FormUI *form_ui = (ST_FormUI *)user_data;
  guint position = gtk_single_selection_get_selected(form_ui->selection_model);
  STConexaoMQTT *conexao = g_list_model_get_item(G_LIST_MODEL(form_ui->conexao_store), position);
  alterarJSON(conexao, position);
}

void createButtonWithImageLabel(ST_BUTTON *button, const char *pathToImage, const char *textButton) {
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

void initTopBox(ST_TopBox *top_box) {
    top_box->box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_add_css_class(top_box->box, "topbox");

    top_box->image = gtk_image_new_from_file("hapa.png");
    gtk_widget_set_size_request(top_box->image, 48, 48);
    gtk_widget_set_margin_start(top_box->image, 12);
    gtk_box_append(GTK_BOX(top_box->box), top_box->image);

    top_box->titulo = gtk_label_new("HAPA IoT");
    gtk_widget_add_css_class(top_box->titulo, "app-title");
    gtk_widget_set_margin_start(top_box->titulo, 10);
    gtk_box_append(GTK_BOX(top_box->box), top_box->titulo);

    top_box->search_box = gtk_center_box_new();
    gtk_widget_set_hexpand(top_box->search_box, TRUE);
    gtk_box_append(GTK_BOX(top_box->box), top_box->search_box);

    top_box->search_entry = gtk_search_entry_new();
    gtk_widget_set_size_request(top_box->search_entry, 400, -1);
    gtk_widget_add_css_class(top_box->search_entry, "searchentry");
    gtk_center_box_set_center_widget(GTK_CENTER_BOX(top_box->search_box), top_box->search_entry);
    createButtonWithImageLabel(&top_box->button_switch, "connect.png", "CONECTAR");
    gtk_widget_set_size_request(top_box->button_switch.image, 24, 24);
    gtk_widget_set_margin_end(top_box->button_switch.button, 40);
    gtk_widget_add_css_class(top_box->button_switch.label, "button_connect_label");
    gtk_widget_add_css_class(top_box->button_switch.button, "button_connect");
    gtk_box_append(GTK_BOX(top_box->box), top_box->button_switch.button);

    top_box->button_menu = gtk_menu_button_new();
    top_box->image = gtk_image_new_from_file("menu.png");
    gtk_menu_button_set_child(GTK_MENU_BUTTON(top_box->button_menu), top_box->image);
    gtk_widget_set_margin_end(top_box->button_menu, 12);
    gtk_widget_add_css_class(top_box->button_menu, "button_menu");
    gtk_widget_set_size_request(top_box->button_menu, 50, 50);
    gtk_box_append(GTK_BOX(top_box->box), top_box->button_menu);
}

void initMainUI(ST_MainUI *main_ui) {
    main_ui->fixed = gtk_fixed_new();
    gtk_widget_set_hexpand(main_ui->fixed, TRUE);
    gtk_widget_set_vexpand(main_ui->fixed, TRUE);
    gtk_widget_add_css_class(main_ui->fixed, "layout");
}

void initFormUI(ST_FormUI *form_ui) {
    form_ui->fixed = gtk_fixed_new();
    gtk_widget_set_hexpand(form_ui->fixed, TRUE);
    gtk_widget_set_vexpand(form_ui->fixed, TRUE);
    gtk_widget_add_css_class(form_ui->fixed, "layout_form");

    form_ui->box_connections = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_size_request(form_ui->box_connections, 200, 500);
    gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->box_connections, 10, 20);

    form_ui->conexao_store = g_list_store_new(ST_TYPE_CONEXAO_MQTT); 
    form_ui->image_add = gtk_image_new_from_file("add_connect.png");
    gtk_widget_set_size_request(form_ui->image_add, 30, 30);
    gtk_widget_add_css_class(form_ui->image_add, "form_ui_image_add");
    gtk_box_append(GTK_BOX(form_ui->box_connections), form_ui->image_add);

    form_ui->gesture = gtk_gesture_click_new();
    gtk_widget_add_controller(form_ui->image_add, GTK_EVENT_CONTROLLER(form_ui->gesture));
    g_signal_connect(form_ui->gesture, "pressed", G_CALLBACK(addNewConnections), form_ui);

    form_ui->label_connections = gtk_label_new("Conexões");
    gtk_box_append(GTK_BOX(form_ui->box_connections), form_ui->label_connections);
    gtk_widget_add_css_class(form_ui->label_connections, "form_ui_label_connections");  

    form_ui->selection_model = gtk_single_selection_new(G_LIST_MODEL(form_ui->conexao_store));
    carregarJSON(form_ui);
    g_signal_connect(form_ui->selection_model, "selection-changed", G_CALLBACK(selectionChanged), form_ui);
    
    form_ui->factory = gtk_signal_list_item_factory_new();
    g_signal_connect(form_ui->factory, "setup", G_CALLBACK(setupListConnections), form_ui);
    g_signal_connect(form_ui->factory, "bind", G_CALLBACK(bindListConnections), form_ui);

    form_ui->list_view = gtk_list_view_new(GTK_SELECTION_MODEL(form_ui->selection_model), form_ui->factory);
    gtk_widget_add_css_class(form_ui->list_view, "form_ui_listview");

    form_ui->scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(form_ui->scrolled), form_ui->list_view);
    gtk_box_append(GTK_BOX(form_ui->box_connections), form_ui->scrolled);
    gtk_widget_set_hexpand(form_ui->scrolled, TRUE);
    gtk_widget_set_vexpand(form_ui->scrolled, TRUE);

    form_ui->label = gtk_label_new("Conexão MQTT");
    gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->label, 250, 30);
    gtk_widget_add_css_class(form_ui->label, "form_ui_label_title");

    form_ui->label = gtk_label_new("Nome");
    gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->label, 270, 90);
    gtk_widget_add_css_class(form_ui->label, "form_ui_label_name");
    form_ui->entry_name = gtk_entry_new();
    gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->entry_name, 270, 110);
    gtk_widget_set_size_request(form_ui->entry_name, 300, -1);
    gtk_widget_add_css_class(form_ui->entry_name, "form_ui_entry_name");
    g_signal_connect(form_ui->entry_name, "changed", G_CALLBACK(entryNameChanged), form_ui);

    form_ui->label = gtk_label_new("Protocolo");
    gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->label, 270, 190);
    gtk_widget_add_css_class(form_ui->label, "form_ui_label_protocolo");
    const char *list_dropdown[3] = {"mqtt://", "ws://", NULL};
    form_ui->dropdown_protocolo = gtk_drop_down_new_from_strings(list_dropdown);
    gtk_drop_down_set_show_arrow(GTK_DROP_DOWN(form_ui->dropdown_protocolo), TRUE);
    gtk_widget_set_size_request(form_ui->dropdown_protocolo, 100, -1);
    gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->dropdown_protocolo, 270, 210);
    gtk_widget_add_css_class(form_ui->dropdown_protocolo, "form_ui_dropdown_protocolo");
    g_signal_connect(form_ui->dropdown_protocolo, "notify::selected", G_CALLBACK(dropdownProtocoloChanged), form_ui);
    
    form_ui->label = gtk_label_new("Host");
    gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->label, 400, 190);
    gtk_widget_add_css_class(form_ui->label, "form_ui_label_host");
    form_ui->entry_host = gtk_entry_new();
    gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->entry_host, 400, 210);
    gtk_widget_set_size_request(form_ui->entry_host, 410, -1);
    gtk_widget_add_css_class(form_ui->entry_host, "form_ui_entry_host");
    g_signal_connect(form_ui->entry_host, "changed", G_CALLBACK(entryHostChanged), form_ui);

    form_ui->label = gtk_label_new("Porta");
    gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->label, 840, 190);
    gtk_widget_add_css_class(form_ui->label, "form_ui_label_port");
    form_ui->buffer = gtk_entry_buffer_new("1883", 4);
    form_ui->entry_port = gtk_entry_new_with_buffer(form_ui->buffer);
    gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->entry_port, 840, 210);
    gtk_widget_set_size_request(form_ui->entry_port, 120, -1);
    gtk_widget_add_css_class(form_ui->entry_port, "form_ui_entry_port");
    g_signal_connect(form_ui->entry_port, "changed", G_CALLBACK(entryPortChanged), form_ui);

    form_ui->label = gtk_label_new("Username");
    gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->label, 270, 310);
    gtk_widget_add_css_class(form_ui->label, "form_ui_label_username");
    form_ui->entry_username = gtk_entry_new();
    gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->entry_username, 270, 330);
    gtk_widget_set_size_request(form_ui->entry_username, 330, -1);
    gtk_widget_add_css_class(form_ui->entry_username, "form_ui_entry_username");
    g_signal_connect(form_ui->entry_username, "changed", G_CALLBACK(entryUsernameChanged), form_ui);
    
    form_ui->label = gtk_label_new("Password");
    gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->label, 630, 310);
    gtk_widget_add_css_class(form_ui->label, "form_ui_label_password");
    form_ui->entry_password = gtk_password_entry_new();
    gtk_password_entry_set_show_peek_icon(GTK_PASSWORD_ENTRY(form_ui->entry_password), TRUE);
    gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->entry_password, 630, 330);
    gtk_widget_set_size_request(form_ui->entry_password, 330, -1);
    gtk_widget_add_css_class(form_ui->entry_password, "form_ui_entry_password");
    g_signal_connect(form_ui->entry_password, "changed", G_CALLBACK(entryPasswordChanged), form_ui);
    
    createButtonWithImageLabel(&form_ui->button_connect, "on_connect.png", "CONECTAR");
    gtk_widget_add_css_class(form_ui->button_connect.button, "form_ui_button_connect_button");
    gtk_widget_add_css_class(form_ui->button_connect.label, "form_ui_button_connect_label");
    gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->button_connect.button, 810, 430);
    gtk_widget_set_size_request(form_ui->button_connect.button, 75, 35);

    createButtonWithImageLabel(&form_ui->button_save, "save_connect.png", "SALVAR");
    gtk_widget_add_css_class(form_ui->button_save.button, "form_ui_button_save_button");
    gtk_widget_add_css_class(form_ui->button_save.label, "form_ui_button_save_label");
    gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->button_save.button, 660, 430);
    gtk_widget_set_size_request(form_ui->button_save.button, 70, 35);
    g_signal_connect(form_ui->button_save.button, "clicked", G_CALLBACK(saveConnection), form_ui);

    createButtonWithImageLabel(&form_ui->button_delete, "delete.png", "REMOVER");
    gtk_widget_add_css_class(form_ui->button_delete.button, "form_ui_button_delete_button");
    gtk_widget_add_css_class(form_ui->button_delete.label, "form_ui_button_delete_label");
    gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->button_delete.button, 270, 430);
    gtk_widget_set_size_request(form_ui->button_delete.button, 70, 35);
    
    createButtonWithImageLabel(&form_ui->button_advance, "advance.png", "OPÇÕES");
    gtk_widget_add_css_class(form_ui->button_advance.button, "form_ui_button_advance_button");
    gtk_widget_add_css_class(form_ui->button_advance.label, "form_ui_button_advance_label");
    gtk_fixed_put(GTK_FIXED(form_ui->fixed), form_ui->button_advance.button, 430, 430);
    gtk_widget_set_size_request(form_ui->button_advance.button, 70, 35);
}

void initAppWidgets(ST_AppWidgets *app_ui, GtkApplication *app) {

    // WINDOW
    app_ui->window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(app_ui->window), "HAPA IoT");
    gtk_window_set_default_size(GTK_WINDOW(app_ui->window), 1000, 600);
    gtk_window_set_icon_name(GTK_WINDOW(app_ui->window), "hapa");
    gtk_window_set_deletable(GTK_WINDOW(app_ui->window), TRUE);
    gtk_window_set_resizable(GTK_WINDOW(app_ui->window), FALSE);

    // BOX PRINCIPAL
    app_ui->main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_window_set_child(GTK_WINDOW(app_ui->window), app_ui->main_box);

    // BOX SUPERIOR
    initTopBox(&app_ui->top_box);
    gtk_box_append(GTK_BOX(app_ui->main_box), app_ui->top_box.box);

    // STACK (Alternar entre páginas)
    app_ui->stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(app_ui->stack), 3);
    gtk_box_append(GTK_BOX(app_ui->main_box), app_ui->stack);

    // MAIN (Página Principal)
    initMainUI(&app_ui->main_ui);
    // gtk_box_append(GTK_BOX(app_ui->main_box), app_ui->main_ui.fixed);
    gtk_stack_add_named(GTK_STACK(app_ui->stack), app_ui->main_ui.fixed, "main");

    // FORMULÁRIO (Página de adicionar nova conexão)
    initFormUI(&app_ui->form_ui);
    gtk_stack_add_named(GTK_STACK(app_ui->stack), app_ui->form_ui.fixed, "form");

    gtk_stack_set_visible_child_name(GTK_STACK(app_ui->stack), "main");
}

STConexaoMQTT *st_conexao_mqtt_new(void){
  return g_object_new(ST_TYPE_CONEXAO_MQTT, NULL);
}

void adicionarJSON(STConexaoMQTT *conexao){
  FILE *ficheiro = fopen("settings.json", "r");
  if(!ficheiro){
    perror("Erro ao abrir ficheiro JSON:");
    return;
  }
  
  fseek(ficheiro, 0, SEEK_END);
  long tamanho = ftell(ficheiro);
  rewind(ficheiro);

  char *conteudo = malloc(tamanho + 1);
  if(!conteudo){
    fclose(ficheiro);
    return;
  }
  
  size_t read = fread(conteudo, 1, tamanho, ficheiro);
  if(read != tamanho){
    perror("Erro ao ler ficheiro JSON:");
    free(conteudo);
    fclose(ficheiro);
    return;
  }
  
  fclose(ficheiro);
  conteudo[read] = '\0';

  struct json_object *json = json_tokener_parse(conteudo);
  if(!json){
    free(conteudo);
    perror("Erro ao fazer parse da string");
    return;
  }
  free(conteudo);

  struct json_object *conexoes;
  if((json_object_object_get_ex(json, "ConnectionsSettings", &conexoes)) == 0){
    conexoes = json_object_new_object();
    json_object_object_add(json, "CoonnectionsSettings", conexoes);
  }

  struct json_object *nova_conexao = json_object_new_object();
  const char *nome = conexao->nome ? conexao->nome : "";
  json_object_object_add(nova_conexao, "nome", json_object_new_string(nome));
  json_object_object_add(nova_conexao, "porta", json_object_new_int(conexao->porta));
  const char *protocolo = conexao->protocolo ? conexao->protocolo : "";
  json_object_object_add(nova_conexao, "protocolo", json_object_new_string(protocolo));
  const char *host = conexao->host ? conexao->host : "";
  json_object_object_add(nova_conexao, "host", json_object_new_string(host));
  const char *username = conexao->username ? conexao->username : "";
  json_object_object_add(nova_conexao, "username", json_object_new_string(username));
  const char *password = conexao->password ? conexao->password : "";
  json_object_object_add(nova_conexao, "password", json_object_new_string(password));
  char buffer[10];
  sprintf(buffer, "ID: %d", conexao->id);
  json_object_object_add(conexoes, buffer, nova_conexao);
  
  json_object_to_file_ext("settings.json", json, JSON_C_TO_STRING_PRETTY);
  json_object_put(json);
}

void alterarJSON(STConexaoMQTT *conexao, int position){
  FILE *ficheiro = fopen("settings.json", "r");
  if(!ficheiro){
    perror("Erro ao abrir JSON:");
    return;
  }

  fseek(ficheiro, 0, SEEK_END);
  long tamanho = ftell(ficheiro);
  rewind(ficheiro);

  char *conteudo = malloc(tamanho + 1);
  if(!conteudo){
    fclose(ficheiro);
    return;
  }

  size_t read = fread(conteudo, 1, tamanho, ficheiro);
  if(read != tamanho){
    printf("Erro ao ler ficheiro JSON\n");
    fclose(ficheiro);
    free(conteudo);
    return;
  }

  fclose(ficheiro);
  conteudo[read] = '\0';
  struct json_object *json = json_tokener_parse(conteudo);
  if(!json){
    printf("Erro ao fazer parse da string");
    free(conteudo); 
    return;
  }
  free(conteudo);
  
  struct json_object *connections_settings = json_object_object_get(json, "ConnectionsSettings");
  char buffer[10];
  sprintf(buffer, "ID: %d", position);
  struct json_object *id = json_object_object_get(connections_settings, buffer);
  
  const char *nome = conexao->nome ? conexao->nome : "";
  json_object_object_add(id, "nome", json_object_new_string(nome));
  json_object_object_add(id, "porta", json_object_new_int(conexao->porta));
  const char *protocolo = conexao->protocolo ? conexao->protocolo : "";
  json_object_object_add(id, "protocolo", json_object_new_string(protocolo));
  const char *host = conexao->host ? conexao->host : "";
  json_object_object_add(id, "host", json_object_new_string(host));
  const char *username = conexao->username ? conexao->username : "";
  json_object_object_add(id, "username", json_object_new_string(username));
  const char *password = conexao->password ? conexao->password : "";
  json_object_object_add(id, "password", json_object_new_string(password));
  
  json_object_to_file_ext("settings.json", json, JSON_C_TO_STRING_PRETTY);
  json_object_put(json);
}

void carregarJSON(ST_FormUI *form_ui){
  FILE *ficheiro = fopen("settings.json", "r");
  if(!ficheiro){
    perror("Erro ao abrir ficheiro JSON:");
    return;
  }
  int bufferLength = 64, contador = 0;
  char buffer[bufferLength];
  while(fgets(buffer, bufferLength, ficheiro)){
    if(strstr(buffer, "ID:")){
      contador++;
    }
  }

  fseek(ficheiro, 0, SEEK_END);
  long tamanho = ftell(ficheiro);
  rewind(ficheiro);

  char *conteudo = malloc(tamanho + 1);
  if(!conteudo){
    printf("Erro ao alocar\n");
    fclose(ficheiro);
    return;
  }

  size_t read = fread(conteudo, 1, tamanho, ficheiro);
  conteudo[read] = '\0';
  if(read != tamanho){
    printf("Erro ao ler ficheiro.\n");
    fclose(ficheiro);
    free(conteudo);
  }
  fclose(ficheiro);
  
  struct json_object *json = json_tokener_parse(conteudo);
  if(!json){
    printf("Erro ao fazer parse da string\n");
    free(conteudo);
    return;
  }
  free(conteudo);
  struct json_object *connections_settings = json_object_object_get(json, "ConnectionsSettings");

  for(int i = 0; i < contador; i++){
    STConexaoMQTT *conexao = st_conexao_mqtt_new();
    g_list_store_append(form_ui->conexao_store, conexao);
    struct json_object *nome, *porta, *protocolo, *host, *username, *password;
    sprintf(buffer, "ID: %d", i);
    struct json_object *id = json_object_object_get(connections_settings, buffer);
    nome = json_object_object_get(id, "nome");
    if(nome){
      conexao->nome = strdup(json_object_get_string(nome));
    }
    porta = json_object_object_get(id, "porta");
    if(porta){
      conexao->porta = json_object_get_int(porta);
    }
    protocolo = json_object_object_get(id, "protocolo");
    if(protocolo){
      conexao->protocolo = strdup(json_object_get_string(protocolo));
    }
    host = json_object_object_get(id, "host");
    if(host){
      conexao->host = strdup(json_object_get_string(host));
    }
    username = json_object_object_get(id, "username");
    if(username){
      conexao->username = strdup(json_object_get_string(username));
    }
    password = json_object_object_get(id, "password");
    if(password){
      conexao->password = strdup(json_object_get_string(password));
    }
  }
  json_object_put(json);
}

// static void encerrarMQTT(struct mosquitto *mosq) {
// if (mosq) {
// mosquitto_loop_stop(mosq, true);
// mosquitto_disconnect(mosq);
// mosquitto_destroy(mosq);
// mosquitto_lib_cleanup();
//}
//}
