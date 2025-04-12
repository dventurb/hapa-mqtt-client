#pragma once 

#include <glib-object.h>

G_BEGIN_DECLS

#define ST_TYPE_CONEXAO_MQTT (st_conexao_mqtt_get_type())
G_DECLARE_FINAL_TYPE (STConexaoMQTT, st_conexao_mqtt, ST, CONEXAO_MQTT, GObject)

STConexaoMQTT *conexao_mqtt_new ();

G_END_DECLS
