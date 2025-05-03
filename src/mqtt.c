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
  if(msg->payload == NULL){
    return;
  } 
  STMessageData *message_data = stMessageDataNew(); 
  stMessageDataSetPayload(message_data, msg->payload);
  stMessageDataSetTopic(message_data, stMQTTTopicGetName(home_ui->topic));
  stMessageDataSetDirection(message_data, ST_MESSAGE_RECEIVED);
  home_ui->message_data = message_data;

  g_idle_add((GSourceFunc)addMsgToListView, home_ui);
}

void publishMQTT(struct mosquitto *mosq, STMQTTTopic *topic, ST_HomeUI *home_ui, const char *payload){
  int rc = mosquitto_publish(mosq, NULL, stMQTTTopicGetName(topic), strlen(payload), payload, atoi(stMQTTTopicGetQoS(topic)), false);

  if(rc != MOSQ_ERR_SUCCESS){
    // TODO: Create a function to send a warning "message fail"
    return;
  }else {
    STMessageData *message_data = stMessageDataNew();
    stMessageDataSetPayload(message_data, payload);
    stMessageDataSetTopic(message_data, stMQTTTopicGetName(topic));
    stMessageDataSetDirection(message_data, ST_MESSAGE_SENT);
    home_ui->message_data = message_data;
    
    g_idle_add((GSourceFunc)addMsgToListView, home_ui);
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
