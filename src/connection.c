#include "connection.h"

struct _STMQTTConnection {
  GObject parent_instance; 
  int id;
  int port;
  char *name;
  char *protocol;
  char *host;
  char *username;
  char *password;
};

G_DEFINE_TYPE(STMQTTConnection, st_mqtt_connection, G_TYPE_OBJECT);

static void st_mqtt_connection_init(STMQTTConnection *connection){
  connection->id = 0;
  connection->port = 1883;
  connection->name = strdup("nova conexão");
  connection->protocol = strdup("mqtt");
  connection->host = NULL;
  connection->username = NULL;
  connection->password = NULL;
}

static void st_mqtt_connection_finalize(GObject *object){
  STMQTTConnection *connection = ST_MQTT_Connection(object);
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
  G_OBJECT_CLASS(st_mqtt_connection_parent_class)->finalize(object);
}

static void st_mqtt_connection_class_init(STMQTTConnectionClass *klass){
  GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
  gobject_class->finalize = st_mqtt_connection_finalize;
}

STMQTTConnection *st_mqtt_connection_new(void){
  return g_object_new(ST_TYPE_MQTT_CONNECTION, NULL);
}

const char *st_mqtt_connection_get_name(STMQTTConnection *self){
  if(self->name){
    return self->name;
  }else{
    return "";
  }
}

int st_mqtt_connection_get_id(STMQTTConnection *self){
    return self->id;
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

void st_mqtt_connection_set_name(STMQTTConnection *self, const char *name){
  if(self->name){
    free(self->name);
  }
  self->name = strdup(name);
}

void st_mqtt_connection_set_id(STMQTTConnection *self, int id){
 self->id = id;
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

