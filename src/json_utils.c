// For more information, see the documentation from json-c library:
// https://json-c.github.io/json-c/json-c-0.10/doc/html/index.html

#include "json_utils.h"
#include "ui_connections.h"

void addConnectionToJSON(STMQTTConnection *connection){
  FILE *file = fopen(SETTINGS_JSON_PATH, "r");
  if(!file){
    return;
  }
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);

  char *data = malloc(size + 1);
  if(!data){
    fclose(file);
    return;
  }
  size_t read = fread(data, 1, size, file);
  if(read != size){
    fclose(file);
    free(data);
    return;
  }
  fclose(file);
  data[read] = '\0';

  struct json_object *json = json_tokener_parse(data);
  if(!json){
    free(data);
    return;
  }
  free(data);

  struct json_object *json_connections;
  if((json_object_object_get_ex(json, "ConnectionsSettings", &json_connections)) == 0){
    json_connections = json_object_new_object();
    json_object_object_add(json, "ConnectionsSettings", json_connections);
  }

  struct json_object *new_json_connection = json_object_new_object();
  const char *name = stMQTTConnectionGetName(connection);
  json_object_object_add(new_json_connection, "name", json_object_new_string(name));
  json_object_object_add(new_json_connection, "port", json_object_new_int(stMQTTConnectionGetPort(connection)));
  const char *protocol = stMQTTConnectionGetProtocol(connection); 
  json_object_object_add(new_json_connection, "protocol", json_object_new_string(protocol));
  const char *host = stMQTTConnectionGetHost(connection); 
  json_object_object_add(new_json_connection, "host", json_object_new_string(host));
  const char *username = stMQTTConnectionGetUsername(connection);  
  json_object_object_add(new_json_connection, "username", json_object_new_string(username));
  const char *password = stMQTTConnectionGetPassword(connection); 
  json_object_object_add(new_json_connection, "password", json_object_new_string(password));
  const char *client_id = stMQTTConnectionGetClientID(connection);
  json_object_object_add(new_json_connection, "client_id", json_object_new_string(client_id));
 int max_itens = g_list_model_get_n_items(G_LIST_MODEL(stMQTTConnectionGetTopics(connection)));
  struct json_object *json_array = json_object_new_array();
  for(int i = 0; i < max_itens; i++){
    STMQTTTopic *topic = g_list_model_get_item(G_LIST_MODEL(stMQTTConnectionGetTopics(connection)), i);
    struct json_object *json_topics = json_object_new_object();
    json_object_object_add(json_topics, "topic", json_object_new_string(stMQTTTopicGetName(topic)));
    json_object_object_add(json_topics, "qos", json_object_new_string(stMQTTTopicGetQoS(topic)));
    json_object_array_put_idx(json_array, i, json_topics);
  }
  json_object_object_add(new_json_connection, "subscriptions", json_array);
  json_object_object_add(json_connections, stMQTTConnectionGetConnectionID(connection), new_json_connection);

  json_object_to_file_ext(SETTINGS_JSON_PATH, json, JSON_C_TO_STRING_PRETTY);
  json_object_put(json);
}

void updateConnectionInJSON(STMQTTConnection *connection){
  FILE *file = fopen(SETTINGS_JSON_PATH, "r");
  if(!file){
    return;
  }
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);

  char *data = malloc(size + 1);
  if(!data){
    fclose(file);
    return;
  }
  size_t read = fread(data, 1, size, file);
  if(read != size){
    fclose(file);
    free(data);
    return;
  }
  fclose(file);
  data[read] = '\0';

  struct json_object *json = json_tokener_parse(data);
  if(!json){
    free(data);
    return;
  }
  free(data);

  struct json_object *json_connections = json_object_object_get(json, "ConnectionsSettings");
  struct json_object *update_json_connection = json_object_object_get(json_connections, stMQTTConnectionGetConnectionID(connection));

  const char *name = stMQTTConnectionGetName(connection);
  json_object_object_add(update_json_connection, "name", json_object_new_string(name));
  json_object_object_add(update_json_connection, "port", json_object_new_int(stMQTTConnectionGetPort(connection)));
  const char *protocol = stMQTTConnectionGetProtocol(connection);
  json_object_object_add(update_json_connection, "protocol", json_object_new_string(protocol));
  const char *host = stMQTTConnectionGetHost(connection);
  json_object_object_add(update_json_connection, "host", json_object_new_string(host));
  const char *username = stMQTTConnectionGetUsername(connection);
  json_object_object_add(update_json_connection, "username", json_object_new_string(username));
  const char *password = stMQTTConnectionGetPassword(connection);
  json_object_object_add(update_json_connection, "password", json_object_new_string(password));
  const char *client_id = stMQTTConnectionGetClientID(connection);
  json_object_object_add(update_json_connection, "client_id", json_object_new_string(client_id));
  
  json_object_to_file_ext(SETTINGS_JSON_PATH, json, JSON_C_TO_STRING_PRETTY);
  json_object_put(json);
}

// I found a better way to load the data from JSON.
void loadJSONToForm(ST_ConnectionsUI *connections_ui){
  FILE *file = fopen(SETTINGS_JSON_PATH, "r");
  if(!file){
    return;
  }
  
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);

  char *data = malloc(size + 1);
  if(!data){
    fclose(file);
    return;
  }
  size_t read = fread(data, 1, size, file);
  if(read != size){
    fclose(file);
    free(data);
    return;
  }
  fclose(file);
  data[read] = '\0';

  struct json_object *json = json_tokener_parse(data);
  if(!json){
    free(data);
    return;
  }
  free(data);
  struct json_object *json_connections = json_object_object_get(json, "ConnectionsSettings");
    
  json_object_object_foreach(json_connections, key, val){
    STMQTTConnection *connection = stMQTTConnectionNew();
    g_list_store_append(connections_ui->connection_store, connection);
    struct json_object *json_id, *name, *port, *protocol, *host, *username, *password, *client_id;
    stMQTTConnectionSetConnectionID(connection, strdup(key));
    json_id = json_object_object_get(json_connections, key);
    name = json_object_object_get(val, "name");
    if(name){
      stMQTTConnectionSetName(connection, json_object_get_string(name));
    }
    port = json_object_object_get(val, "port");
    if(port){
      stMQTTConnectionSetPort(connection, json_object_get_int(port));
    }
    protocol = json_object_object_get(val, "protocol");
    if(protocol){
      stMQTTConnectionSetProtocol(connection, json_object_get_string(protocol));
    }
    host = json_object_object_get(val, "host");
    if(host){
      stMQTTConnectionSetHost(connection, json_object_get_string(host));
    }
    username = json_object_object_get(val, "username");
    if(username){
      stMQTTConnectionSetUsername(connection, json_object_get_string(username));
    }
    password = json_object_object_get(val, "password");
    if(password){
      stMQTTConnectionSetPassword(connection, json_object_get_string(password));
   }
   client_id = json_object_object_get(val, "client_id");
    if(client_id){
      stMQTTConnectionSetClientID(connection, json_object_get_string(client_id));
    }
   struct json_object *json_array;
   json_object_object_get_ex(json_id, "subscriptions", &json_array);
   int array_length = json_object_array_length(json_array);
   for(int i = 0; i < array_length; i++){
      STMQTTTopic *topic = stMQTTTopicNew();
      struct json_object *index = json_object_array_get_idx(json_array, i);
      struct json_object *json_topic = json_object_object_get(index, "topic");
      struct json_object *json_qos = json_object_object_get(index, "qos");
      stMQTTTopicSetName(topic, json_object_get_string(json_topic));
      stMQTTTopicSetQoS(topic, json_object_get_string(json_qos));
      g_list_store_append(G_LIST_STORE(stMQTTConnectionGetTopics(connection)), topic);
      g_object_unref(topic);
    }
  }
  json_object_put(json);
}

void deleteConnectionInJSON(STMQTTConnection *connection){
  FILE *file = fopen(SETTINGS_JSON_PATH, "r");
  if(!file){
    return;
  }
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);

  char *data = malloc(size + 1);
  if(!data){
    fclose(file);
    return;
  }
  
  size_t read = fread(data, 1, size, file);
  if(read != size){
    fclose(file);
    free(data);
    return;
  }
  fclose(file);
  data[read] = '\0';

  struct json_object *json = json_tokener_parse(data);
  if(!json){
    free(data);
    return;
  }
  free(data);

  struct json_object *json_connections = json_object_object_get(json, "ConnectionsSettings");
  json_object_object_del(json_connections, stMQTTConnectionGetConnectionID(connection));
  json_object_to_file_ext(SETTINGS_JSON_PATH, json, JSON_C_TO_STRING_PRETTY);
  json_object_put(json);
}

void deleteTopicInJSON(STMQTTConnection *connection, int position){
  FILE *file = fopen(SETTINGS_JSON_PATH, "r");
  if(!file){
    return;
  }
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);

  char *data = malloc(size + 1);
  if(!data){
    fclose(file);
    return;
  }
  
  size_t read = fread(data, 1, size, file);
  if(read != size){
    fclose(file);
    free(data);
    return;
  }
  fclose(file);
  data[read] = '\0';

  struct json_object *json = json_tokener_parse(data);
  if(!json){
    free(data);
    return;
  }
  free(data);

  struct json_object *json_connections = json_object_object_get(json, "ConnectionsSettings");
  struct json_object *json_id = json_object_object_get(json_connections, stMQTTConnectionGetConnectionID(connection));
  struct json_object *json_array;
  json_object_object_get_ex(json_id, "subscriptions", &json_array);
  //struct json_object *index = json_object_array_get_idx(json_array, position);
  json_object_array_del_idx(json_array, position, 1);
  json_object_to_file_ext(SETTINGS_JSON_PATH, json, JSON_C_TO_STRING_PRETTY);
  json_object_put(json);
}
