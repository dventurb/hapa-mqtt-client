// Implementation of GObject sub-type for the MQTT Topics, each connection can have 
// multiple topics. 

#ifndef TOPIC_H
#define TOPIC_H

#include <glib-object.h>
#include <string.h>
#include <stdlib.h>

G_BEGIN_DECLS

#define ST_TYPE_MQTT_TOPIC (st_mqtt_topic_get_type())
G_DECLARE_FINAL_TYPE(STMQTTTopic, st_mqtt_topic, ST, MQTT_Topic, GObject);

// Convert to CamelCase
#define stMQTTTopicNew st_mqtt_topic_new
#define stMQTTTopicFree st_mqtt_topic_free
#define stMQTTTopicGetName st_mqtt_topic_get_name
#define stMQTTTopicGetQoS st_mqtt_topic_get_qos
#define stMQTTTopicSetName st_mqtt_topic_set_name
#define stMQTTTopicSetQoS st_mqtt_topic_set_qos

STMQTTTopic *st_mqtt_topic_new(void);
void st_mqtt_topic_free(STMQTTTopic *self);

const char *st_mqtt_topic_get_name(STMQTTTopic *self);
const char *st_mqtt_topic_get_qos(STMQTTTopic *self);

void st_mqtt_topic_set_name(STMQTTTopic *self, const char *name);
void st_mqtt_topic_set_qos(STMQTTTopic *self, const char *qos);

G_END_DECLS

#endif
