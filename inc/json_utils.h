#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include <gtk/gtk.h>
#include <json-c/json.h>
#include <stdio.h>
#include <stdlib.h>
#include "connection.h"
#include "paths.h"

// CIRCULAR DEPENDENCIE - This is to fix the circular dependencie of 
// ui_form.h -> json_uitls.h || json_uitls.h -> ui_form.h 
// Maybe I will just put the structs types in one file together.
typedef struct _ST_ConnectionsUI ST_ConnectionsUI;
typedef struct _ST_HomeUI ST_HomeUI;

void addConnectionToJSON(STMQTTConnection *connection);
void updateConnectionInJSON(STMQTTConnection *connection);
void loadJSONToForm(ST_ConnectionsUI *connections_ui);
void loadJSONToHome(ST_HomeUI *home_ui);
void deleteConnectionInJSON(STMQTTConnection *connection);
void deleteTopicInJSON(STMQTTConnection *connection, int position);

#endif
