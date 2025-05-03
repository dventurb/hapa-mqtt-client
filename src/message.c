#include "message.h"

struct _STMessageData {
  GObject parent_instance;
  char *topic;
  char *payload;
  char *timestamp;
  STMessageDirection direction;
};

G_DEFINE_TYPE(STMessageData, st_message_data, G_TYPE_OBJECT);

static void st_message_data_init(STMessageData *message){
  message->topic = NULL;
  message->payload = NULL;
  message->timestamp = NULL;
}

static void st_message_data_finalize(GObject *object){
  STMessageData *message = ST_MessageData(object);
  free(message->topic);
  message->topic = NULL;
  free(message->payload);
  message->payload = NULL;
  free(message->timestamp);
  message->timestamp = NULL;
  G_OBJECT_CLASS(st_message_data_parent_class)->finalize(object);
}

static void st_message_data_class_init(STMessageDataClass *klass){
  GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
  gobject_class->finalize = st_message_data_finalize;
}

STMessageData *st_message_data_new(void){
  return g_object_new(ST_TYPE_MESSAGE_DATA, NULL);
}

void st_message_data_free(STMessageData *self){
  g_object_unref(self);
}

const char *st_message_data_get_topic(STMessageData *self){
  if(self->topic){
    return self->topic;
  }else {
    return "";
  }
}

const char *st_message_data_get_payload(STMessageData *self){
  if(self->payload){
    return self->payload;
  }else {
    return "";
  }
}

const char *st_message_data_get_timestamp(STMessageData *self){
  if(self->timestamp){
    return self->timestamp;
  }else {
    return "";
  }
}

STMessageDirection st_message_data_get_direction(STMessageData *self){
  return self->direction;
}

void st_message_data_set_topic(STMessageData *self, const char *topic){
  if(self->topic){
    free(self->topic);
  }
  self->topic = strdup(topic);
}

void st_message_data_set_payload(STMessageData *self, const char *payload){
  if(self->payload){
    free(self->payload);
  }
  self->payload = strdup(payload);
}

void st_message_data_set_timestamp(STMessageData *self, const char *timestamp){
  if(self->timestamp){
    free(self->timestamp);
  }
  self->timestamp = strdup(timestamp);
}

void st_message_data_set_direction(STMessageData *self, STMessageDirection direction){
  self->direction = direction;
}
