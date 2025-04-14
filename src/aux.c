#include "aux.h"

const char *getID(void){
  uuid_t uuid;
  char *str = malloc(37);
  uuid_generate_random(uuid);
  uuid_unparse(uuid, str);
  return str;
}
