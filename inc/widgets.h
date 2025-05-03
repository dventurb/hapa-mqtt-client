#ifndef UI_WIDGETS_H
#define UI_WIDGETS_H

#include <gtk/gtk.h>
#include "topic.h"
#include "aux.h"
#include "message.h"

typedef struct {
 GtkWidget *button;
 GtkWidget *box;
 GtkWidget *image;
 GtkWidget *label;
}ST_BUTTON;

typedef struct _ST_HomeUI ST_HomeUI;
typedef struct _STMessageData STMessageData;

void createButtonWithImageLabel(ST_BUTTON *button, const char *pathToImage, const char *textButton);
GtkWidget *buildMessageBox(STMessageData *message_data);
GtkWidget *sendMessage(const char *topic, const char *payload);
GtkWidget *receiveMessage(const char *topic, const char *payload);
void scrollToBottom(GtkScrolledWindow *scrolled);

#endif 
