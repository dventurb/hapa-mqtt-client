// For more information, see the documentation from json-c library:
// https://json-c.github.io/json-c/json-c-0.10/doc/html/index.html

#include "json_utils.h"
#include "ui_form.h"

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
  json_object_object_add(json_connections, stMQTTConnectionGetID(connection), new_json_connection);

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
  struct json_object *update_json_connection = json_object_object_get(json_connections, stMQTTConnectionGetID(connection));

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

  json_object_to_file_ext(SETTINGS_JSON_PATH, json, JSON_C_TO_STRING_PRETTY);
  json_object_put(json);
}

// I found a better way to load the data from JSON.
void loadJSONToForm(ST_FormUI *form_ui){
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
    g_list_store_append(form_ui->connection_store, connection);
    struct json_object *name, *port, *protocol, *host, *username, *password;
    stMQTTConnectionSetID(connection, strdup(key));
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
  json_object_object_del(json_connections, stMQTTConnectionGetID(connection));
  json_object_to_file_ext(SETTINGS_JSON_PATH, json, JSON_C_TO_STRING_PRETTY);
  json_object_put(json);
}
