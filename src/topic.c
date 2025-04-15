#include "topic.h"

struct _STMQTTTopic {
  GObject parent_instance;
  char *name;
  char *path;
};

G_DEFINE_TYPE(STMQTTTopic, st_mqtt_topic, G_TYPE_OBJECT);

static void st_mqtt_topic_init(STMQTTTopic *topic){
  topic->name = NULL;
  topic->path = NULL;
}

static void st_mqtt_topic_finalize(GObject *object){
  STMQTTTopic *topic = ST_MQTT_Topic(object);
  free(topic->name);
  topic->name = NULL;
  free(topic->path);
  topic->path = NULL;
  G_OBJECT_CLASS(st_mqtt_topic_parent_class)->finalize(object);
}

static void st_mqtt_topic_class_init(STMQTTTopicClass *klass){
  GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
  gobject_class->finalize = st_mqtt_topic_finalize;
}

STMQTTTopic *st_mqtt_topic_new(void){
  return g_object_new(ST_TYPE_MQTT_TOPIC, NULL);
}

void st_mqtt_topic_free(STMQTTTopic *self){
  g_object_unref(self);
}

const char *st_mqtt_topic_get_name(STMQTTTopic *self){
  if(self->name){
    return self->name;
  }else {
    return "";
  }
}

const char *st_mqtt_topic_get_path(STMQTTTopic *self){
  if(self->path){
    return self->path;
  }else {
    return "";
  }
}

void st_mqtt_topic_set_name(STMQTTTopic *self, const char *name){
  if(self->name){
    free(self->name);
  }
  self->name = strdup(name);
}

void st_mqtt_topic_set_path(STMQTTTopic *self, const char *path){
  if(self->path){
    free(self->path);
  }
  self->path = strdup(path);
}

