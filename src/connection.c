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
  gboolean certificate;
  gboolean encryption;
  GListStore *topics;
};

G_DEFINE_TYPE(STMQTTConnection, st_mqtt_connection, G_TYPE_OBJECT);

static void st_mqtt_connection_init(STMQTTConnection *connection){
  connection->connection_id = NULL;
  connection->client_id = NULL;
  connection->port = 1883;
  connection->name = strdup("nova conexão");
  connection->protocol = strdup("mqtt");
  connection->host = NULL;
  connection->username = NULL;
  connection->password = NULL;
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
  free(connection->topics);
  connection->topics = NULL;
  G_OBJECT_CLASS(st_mqtt_connection_parent_class)->finalize(object);
}

static void st_mqtt_connection_class_init(STMQTTConnectionClass *klass){
  GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
  gobject_class->dispose = st_mqtt_connection_dispose;
  gobject_class->finalize = st_mqtt_connection_finalize;
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
