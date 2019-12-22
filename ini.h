#ifndef _INI_H_
#define _INI_H_

#define INI_FILE "/etc/servertool/check.sh"
#define INI_DIR "/etc/servertool/"

string get_key_value(const char *title,const char *key);
int set_key_value(const char *title,const char *key,const char *value);

#endif
