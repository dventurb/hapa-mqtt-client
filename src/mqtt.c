#include "mqtt.h"
#include "ui_home.h"

struct mosquitto *connectMQTT(ST_HomeUI *home_ui, STMQTTConnection *connection, STMQTTTopic *topic){
  mosquitto_lib_init();
  struct mosquitto *mosq = mosquitto_new(NULL, TRUE, home_ui);
  if(!mosq){
    return NULL;
  }

  if(stMQTTConnectionGetEncryption(connection) && stMQTTConnectionGetCertValidation(connection)){
    int tls_rst = mosquitto_tls_set(mosq, stMQTTConnectionGetServerCertificate(connection), NULL, stMQTTConnectionGetClientCertificate(connection), stMQTTConnectionGetClientKey(connection), NULL);
    if(tls_rst != MOSQ_ERR_SUCCESS){
      return NULL;
    }
  }else if(stMQTTConnectionGetEncryption(connection) && !stMQTTConnectionGetCertValidation(connection)){
    int tls_rst = mosquitto_tls_set(mosq, stMQTTConnectionGetServerCertificate(connection), NULL, NULL, NULL, NULL);
    if(tls_rst != MOSQ_ERR_SUCCESS){
      return NULL;
    }
  }
  if(stMQTTConnectionGetUsername(connection) && stMQTTConnectionGetPassword(connection)){
    mosquitto_username_pw_set(mosq, stMQTTConnectionGetUsername(connection), stMQTTConnectionGetPassword(connection));
  }
  
  int rc = mosquitto_connect(mosq, stMQTTConnectionGetHost(connection), stMQTTConnectionGetPort(connection), 60);
  if(rc != MOSQ_ERR_SUCCESS) {
    return NULL;
  }
  rc = mosquitto_subscribe(mosq, NULL, stMQTTTopicGetName(topic), atoi(stMQTTTopicGetQoS(topic)));
  if(rc != MOSQ_ERR_SUCCESS){
    return NULL;
  }
  mosquitto_message_callback_set(mosq, receiveMQTT);
  mosquitto_loop_start(mosq);

  return mosq;
}

void receiveMQTT(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *msg){
  ST_HomeUI *home_ui = (ST_HomeUI *)userdata;

  ST_MessageData *message_data = malloc(sizeof(ST_MessageData));
  message_data->home_ui = home_ui;
  message_data->payload = strndup(msg->payload, msg->payloadlen);
  message_data->topic = strdup(msg->topic);
  
  g_idle_add_full(G_PRIORITY_DEFAULT_IDLE, (GSourceFunc)updateMessageUI, message_data, destroyMessageData);
}

void publishMQTT(struct mosquitto *mosq, STMQTTTopic *topic, ST_HomeUI *home_ui, const char *payload){
  int rc = mosquitto_publish(mosq, NULL, stMQTTTopicGetName(topic), strlen(payload), payload, atoi(stMQTTTopicGetQoS(topic)), false);

  if(rc != MOSQ_ERR_SUCCESS){
    // TODO: Create a function to send a warning "message fail"
    return;
  }else {
    sendMessage(topic, home_ui, payload);
  }
}

void disconnectMQTT(struct mosquitto **mosq){
  if(*mosq){
    mosquitto_loop_stop(*mosq, true);
    mosquitto_disconnect(*mosq);
    mosquitto_destroy(*mosq);
    *mosq = NULL;
  }
  mosquitto_lib_cleanup();
}
