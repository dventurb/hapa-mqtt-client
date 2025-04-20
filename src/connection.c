
// I'm still learning GObject and that properties things, so maybe some things are wrong.
// For more information and examples, see the following references:
// https://discourse.gnome.org/t/how-to-force-a-refresh-of-widget-values-in-a-columnview/11335/7 
// https://docs.gtk.org/gobject/class_method.Object.install_properties.html

#include "connection.h"

struct _STMQTTConnection {
  GObject parent_instance; 
  char *connection_id;
  char *client_id;
  int port;
  char *name;
  char *protocol;
  char *host;
  char *username;
  char *password;
  char *server_certificate;
  char *client_certificate;
  char *client_key;
  gboolean cert_validation;
  gboolean encryption;
  GListStore *topics;
};

// Each value corresponds to a property of the object.
enum {
  PROP_F0, // By default 
  PROP_NAME,
  PROP_HOST,
  N_PROPS  // Total properties
};

static GParamSpec *obj_properties[N_PROPS] = { NULL };

G_DEFINE_TYPE(STMQTTConnection, st_mqtt_connection, G_TYPE_OBJECT);

// To modify a property.
static void st_mqtt_connection_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec){
  STMQTTConnection *self = ST_MQTT_Connection(object);
  switch (prop_id) {
    case PROP_NAME:
      stMQTTConnectionSetName(self, g_value_dup_string(value));
      break;
    case PROP_HOST:
      stMQTTConnectionSetHost(self, g_value_dup_string(value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
  }
}

// To retrieve a value of the property.
static void st_mqtt_connection_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec){
  STMQTTConnection *self = ST_MQTT_Connection(object);
  switch (prop_id) {
    case PROP_NAME:
      g_value_set_string(value, stMQTTConnectionGetName(self));
      break;
    case PROP_HOST:
      // Example: mqtt + "://" + 192.168.1.1 + '\0';
      char *text = malloc(strlen(stMQTTConnectionGetProtocol(self)) + strlen(stMQTTConnectionGetHost(self)) + 4);
      sprintf(text, "%s://%s", stMQTTConnectionGetProtocol(self), stMQTTConnectionGetHost(self));
      g_value_set_string(value, text);
      free(text);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
  }
}

static void st_mqtt_connection_init(STMQTTConnection *connection){
  connection->connection_id = NULL;
  connection->client_id = NULL;
  connection->port = 1883;
  connection->name = strdup("nova conexão");
  connection->protocol = strdup("mqtt");
  connection->host = NULL;
  connection->username = NULL;
  connection->password = NULL;
  connection->cert_validation = FALSE;
  connection->encryption = FALSE;
  connection->server_certificate = NULL;
  connection->client_certificate = NULL;
  connection->client_key = NULL;
  connection->topics = g_list_store_new(ST_TYPE_MQTT_TOPIC);
}

static void st_mqtt_connection_dispose(GObject *object){
  STMQTTConnection *self = ST_MQTT_Connection(object);
  if(self->topics){
    g_clear_object(&self->topics);
  }
  G_OBJECT_CLASS(st_mqtt_connection_parent_class)->dispose(object);
}

static void st_mqtt_connection_finalize(GObject *object){
  STMQTTConnection *connection = ST_MQTT_Connection(object);
  free(connection->connection_id);
  connection->connection_id = NULL;
  free(connection->name);
  connection->name = NULL;
  free(connection->protocol);
  connection->protocol = NULL;
  free(connection->host);
  connection->host = NULL;
  free(connection->username);
  connection->username = NULL;
  free(connection->password);
  connection->password = NULL;
  free(connection->server_certificate);
  connection->server_certificate = NULL;
  free(connection->client_certificate);
  connection->client_certificate = NULL;
  free(connection->client_key);
  connection->client_key = NULL;
  free(connection->topics);
  connection->topics = NULL;
  G_OBJECT_CLASS(st_mqtt_connection_parent_class)->finalize(object);
}

static void st_mqtt_connection_class_init(STMQTTConnectionClass *klass){
  GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
  gobject_class->dispose = st_mqtt_connection_dispose;
  gobject_class->finalize = st_mqtt_connection_finalize;
  gobject_class->set_property = st_mqtt_connection_set_property;
  gobject_class->get_property = st_mqtt_connection_get_property;
  
  // Define each property with GParamSpec functions.
  // name used in g_object_set/get, name-readable, description, default value and if the property is R+W. 
  obj_properties[PROP_NAME] = g_param_spec_string("name", "Name", "Connection name", NULL, G_PARAM_READWRITE);
  obj_properties[PROP_HOST] = g_param_spec_string("host", "Host", "Broker address", NULL, G_PARAM_READWRITE);

  g_object_class_install_properties(gobject_class, N_PROPS, obj_properties);
}

STMQTTConnection *st_mqtt_connection_new(void){
  return g_object_new(ST_TYPE_MQTT_CONNECTION, NULL);
}

void st_mqtt_connection_free(STMQTTConnection *self){
  g_object_unref(self);
}

const char *st_mqtt_connection_get_name(STMQTTConnection *self){
  if(self->name){
    return self->name;
  }else{
    return "";
  }
}

const char *st_mqtt_connection_get_connectionID(STMQTTConnection *self){
  if(self->connection_id){ 
    return self->connection_id;
  }else {
    return "";
  }
}

int st_mqtt_connection_get_port(STMQTTConnection *self){ 
  return self->port;
}

const char *st_mqtt_connection_get_protocol(STMQTTConnection *self){
  if(self->protocol){ 
    return self->protocol;
  }else{
    return "";
  }
}

const char *st_mqtt_connection_get_host(STMQTTConnection *self){
  if(self->host){ 
    return self->host;
  }else {
    return "";
  }
}

const char *st_mqtt_connection_get_username(STMQTTConnection *self){
  if(self->username){
    return self->username;
  }else {
    return "";
  }
}

const char *st_mqtt_connection_get_password(STMQTTConnection *self){
  if(self->password){ 
    return self->password;
  }else {
    return "";
  }
}

gboolean st_mqtt_connection_get_certValidation(STMQTTConnection *self){
  return self->cert_validation;
}

gboolean st_mqtt_connection_get_encryption(STMQTTConnection *self){
  return self->encryption;
}

GListStore *st_mqtt_connection_get_topics(STMQTTConnection *self){
    return self->topics;
}

const char *st_mqtt_connection_get_clientID(STMQTTConnection *self){
  if(self->client_id){ 
    return self->client_id;
  }else {
    return "";
  }
}

const char *st_mqtt_connection_get_server_certificate(STMQTTConnection *self){
  if(self->server_certificate){
    return self->server_certificate;
  }else {
    return "";
  }
}

const char *st_mqtt_connection_get_client_certificate(STMQTTConnection *self){
  if(self->client_certificate){
    return self->client_certificate;
  }else {
    return "";
  }
}

const char *st_mqtt_connection_get_client_key(STMQTTConnection *self){
  if(self->client_key){
    return self->client_key;
  }else {
    return "";
  }
}

void st_mqtt_connection_set_name(STMQTTConnection *self, const char *name){
  if(self->name){
    free(self->name);
  }
  self->name = strdup(name);
}

void st_mqtt_connection_set_connectionID(STMQTTConnection *self, const char *connection_id){
 if(self->connection_id){
   free(self->connection_id);
 }
  self->connection_id = strdup(connection_id);
}

void st_mqtt_connection_set_port(STMQTTConnection *self, int port){
  self->port = port;
}

void st_mqtt_connection_set_protocol(STMQTTConnection *self, const char *protocol){
  if(self->protocol){
    free(self->protocol);
  }
  self->protocol = strdup(protocol);
}

void st_mqtt_connection_set_host(STMQTTConnection *self, const char *host){
  if(self->host){
    free(self->host);
  }
  self->host = strdup(host);
}

void st_mqtt_connection_set_username(STMQTTConnection *self, const char *username){
  if(self->username){
    free(self->username);
  }
  self->username = strdup(username);
}

void st_mqtt_connection_set_password(STMQTTConnection *self, const char *password){
  if(self->password){
    free(self->password);
  }
  self->password = strdup(password);
}

void st_mqtt_connection_set_certValidation(STMQTTConnection *self, gboolean cert_validation){
  self->cert_validation = cert_validation;
}

void st_mqtt_connection_set_encryption(STMQTTConnection *self, gboolean encryption){
  self->encryption = encryption;
}

void st_mqtt_connection_set_topics(STMQTTConnection *self, GListStore *store){
  free(self->topics);
  self->topics = store;
}

void st_mqtt_connection_set_clientID(STMQTTConnection *self, const char *client_id){
 if(self->client_id){
   free(self->client_id);
 }
  self->client_id = strdup(client_id);
}

void st_mqtt_connection_set_server_certificate(STMQTTConnection *self, const char *server_certificate){
  if(self->server_certificate){
    free(self->server_certificate);
  }
  self->server_certificate = strdup(server_certificate);
}

void st_mqtt_connection_set_client_certificate(STMQTTConnection *self, const char *client_certificate){
  if(self->client_certificate){
    free(self->client_certificate);
  }
  self->client_certificate = strdup(client_certificate);
}

void st_mqtt_connection_set_client_key(STMQTTConnection *self, const char *client_key){
  if(self->client_key){
    free(self->client_key);
  }
  self->client_key = strdup(client_key);
}
