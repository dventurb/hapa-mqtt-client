#include "aux.h"

const char *getID(void){
  uuid_t uuid;
  char *str = malloc(37);
  uuid_generate_random(uuid);
  uuid_unparse(uuid, str);
  return str;
}

int countDigits(int num){
  int counter = 0;
  while(num != 0){
    num = num / 10;
    counter++;
  }
  return counter;
}

const char *getDateTime(void){
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  int sec = tm.tm_sec;
  int min = tm.tm_min;
  int hour = tm.tm_hour;
  int day = tm.tm_mday;
  int month = tm.tm_mon + 1;
  int year = tm.tm_year + 1900;
  int lenght = countDigits(sec) + countDigits(min) + countDigits(hour) + countDigits(day)+ countDigits(month) + countDigits(year) + 1;
  char *date = malloc(lenght);
  sprintf(date, "%d-%d-%d %d:%d:%d", year, month, day, hour, min, sec);
  return date;
}

char *getSettingsPath(void){
  static char *settings_path = NULL;
  if (settings_path == NULL){ 
    const char *dir = g_get_user_data_dir();
    settings_path = g_build_filename(dir, "hapa-mqtt-client", "settings.json", NULL);
  }
  g_print("\n%s\n", settings_path);
  return settings_path;
}
