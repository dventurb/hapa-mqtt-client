#ifndef UI_WIDGETS_H
#define UI_WIDGETS_H

#include <gtk/gtk.h>
#include "topic.h"
#include "aux.h"

typedef struct {
 GtkWidget *button;
 GtkWidget *box;
 GtkWidget *image;
 GtkWidget *label;
}ST_BUTTON;

typedef struct _ST_HomeUI ST_HomeUI;

void createButtonWithImageLabel(ST_BUTTON *button, const char *pathToImage, const char *textButton);
void sendMessage(STMQTTTopic *topic, ST_HomeUI *home_ui, const char *payload);
void receiveMessage(ST_HomeUI *home_ui, const char *topic, const char *payload);

#endif 
