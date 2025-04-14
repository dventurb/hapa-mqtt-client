#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include <gtk/gtk.h>
#include <json-c/json.h>
#include <stdio.h>
#include <stdlib.h>
#include "connection.h"
#include "paths.h"

typedef struct _ST_FormUI ST_FormUI;

void addConnectionToJSON(STMQTTConnection *connection);
void updateConnectionInJSON(STMQTTConnection *connection, int position);
void loadJSONToForm(ST_FormUI *form_ui);

#endif
