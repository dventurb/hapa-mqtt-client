#ifndef AUX_H
#define AUX_H

#include <uuid/uuid.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <glib.h>

const char *getID(void);
int countDigits(int num);
const char *getDateTime(void);
char *getSettingsPath(void);

#endif
