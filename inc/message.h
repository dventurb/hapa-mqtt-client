#ifndef MESSAGE_H
#define MESSAGE_H

typedef enum {
  ST_MESSAGE_RECEIVED,
  ST_MESSAGE_SENT
}STMessageDirection;

#include <glib-object.h>
#include <string.h>
#include <stdlib.h>

G_BEGIN_DECLS

#define ST_TYPE_MESSAGE_DATA (st_message_data_get_type())
G_DECLARE_FINAL_TYPE(STMessageData, st_message_data, ST, MessageData, GObject);

// Convert to CamelCase
#define stMessageDataNew st_message_data_new
#define stMessageDataFree st_message_data_free
#define stMessageDataGetTopic st_message_data_get_topic
#define stMessageDataGetPayload st_message_data_get_payload
#define stMessageDataGetTimestamp st_message_data_get_timestamp
#define stMessageDataGetDirection st_message_data_get_direction
#define stMessageDataSetTopic st_message_data_set_topic
#define stMessageDataSetPayload st_message_data_set_payload
#define stMessageDataSetTimestamp st_message_data_set_timestamp
#define stMessageDataSetDirection st_message_data_set_direction

STMessageData *st_message_data_new(void);
void st_message_data_free(STMessageData *self);

const char *st_message_data_get_topic(STMessageData *self);
const char *st_message_data_get_payload(STMessageData *self);
const char *st_message_data_get_timestamp(STMessageData *self);
STMessageDirection st_message_data_get_direction(STMessageData *self);

void st_message_data_set_topic(STMessageData *self, const char *topic);
void st_message_data_set_payload(STMessageData *self, const char *payload);
void st_message_data_set_timestamp(STMessageData *self, const char *timestamp);
void st_message_data_set_direction(STMessageData *self, STMessageDirection direction);


G_END_DECLS

#endif
