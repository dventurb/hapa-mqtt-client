// This header defines the implementation of a new GObject type, 
// for correct usage with widgets like GListStore. 
// For more information about GObject, see the documentation:
// https://docs.gtk.org/gobject/tutorial.html

#ifndef CONNECTION_H
#define CONNECTION_H

#include <glib-object.h>
#include <gio/gio.h>
#include <string.h>
#include <stdlib.h>
#include "topic.h"

G_BEGIN_DECLS

#define ST_TYPE_MQTT_CONNECTION (st_mqtt_connection_get_type())
G_DECLARE_FINAL_TYPE(STMQTTConnection, st_mqtt_connection, ST, MQTT_Connection, GObject);

// I use CamelCase in the functions, so this is to convert a naming convention.
#define stMQTTConnectionNew st_mqtt_connection_new
#define stMQTTConnectionFree st_mqtt_connection_free
#define stMQTTConnectionGetName st_mqtt_connection_get_name
#define stMQTTConnectionGetConnectionID st_mqtt_connection_get_connectionID
#define stMQTTConnectionGetPort st_mqtt_connection_get_port
#define stMQTTConnectionGetProtocol st_mqtt_connection_get_protocol
#define stMQTTConnectionGetHost st_mqtt_connection_get_host
#define stMQTTConnectionGetUsername st_mqtt_connection_get_username
#define stMQTTConnectionGetPassword st_mqtt_connection_get_password
#define stMQTTConnectionGetTopics st_mqtt_connection_get_topics
#define stMQTTConnectionGetClientID st_mqtt_connection_get_clientID
#define stMQTTConnectionSetName st_mqtt_connection_set_name
#define stMQTTConnectionSetConnectionID st_mqtt_connection_set_connectionID
#define stMQTTConnectionSetPort st_mqtt_connection_set_port
#define stMQTTConnectionSetProtocol st_mqtt_connection_set_protocol
#define stMQTTConnectionSetHost st_mqtt_connection_set_host
#define stMQTTConnectionSetUsername st_mqtt_connection_set_username
#define stMQTTConnectionSetPassword st_mqtt_connection_set_password
#define stMQTTConnectionSetTopics st_mqtt_connection_set_topics
#define stMQTTConnectionSetClientID st_mqtt_connection_set_clientID

STMQTTConnection *st_mqtt_connection_new(void);
void st_mqtt_connection_free(STMQTTConnection *self);

const char *st_mqtt_connection_get_name(STMQTTConnection *self);
const char *st_mqtt_connection_get_connectionID(STMQTTConnection *self);
int st_mqtt_connection_get_port(STMQTTConnection *self);
const char *st_mqtt_connection_get_protocol(STMQTTConnection *self);
const char *st_mqtt_connection_get_host(STMQTTConnection *self);
const char *st_mqtt_connection_get_username(STMQTTConnection *self);
const char *st_mqtt_connection_get_password(STMQTTConnection *self);
GListStore *st_mqtt_connection_get_topics(STMQTTConnection *self);
const char *st_mqtt_connection_get_clientID(STMQTTConnection *self);

void st_mqtt_connection_set_name(STMQTTConnection *self, const char *name);
void st_mqtt_connection_set_connectionID(STMQTTConnection *self, const char *connection_id);
void st_mqtt_connection_set_port(STMQTTConnection *self, const int port);
void st_mqtt_connection_set_protocol(STMQTTConnection *self, const char *protocol);
void st_mqtt_connection_set_host(STMQTTConnection *self, const char *host);
void st_mqtt_connection_set_username(STMQTTConnection *self, const char *username);
void st_mqtt_connection_set_password(STMQTTConnection *self, const char *password);
void st_mqtt_connection_set_topics(STMQTTConnection *self, GListStore *store);
void st_mqtt_connection_set_clientID(STMQTTConnection *self, const char *client_id);

G_END_DECLS

#endif
