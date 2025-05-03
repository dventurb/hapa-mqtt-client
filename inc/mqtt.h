#ifndef MQTT_H
#define MQTT_H

#include <mosquitto.h>
#include <string.h>
#include "connection.h"
#include "topic.h"
#include "widgets.h"

typedef struct _ST_HomeUI ST_HomeUI;
typedef struct _STMessageData STMessageData;

struct mosquitto *connectMQTT(ST_HomeUI *home_ui, STMQTTConnection *connection, STMQTTTopic *topic);
void receiveMQTT(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *msg);
void publishMQTT(struct mosquitto *mosq, STMQTTTopic *topic, ST_HomeUI *home_ui, const char *payload);
void disconnectMQTT(struct mosquitto **mosq);

#endif
