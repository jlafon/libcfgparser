#include <regex.h>
#include "uthash.h"

#define CFGPARSER_MAX_LINE 2048

struct cfgparser_section
{
    char name[CFGPARSER_MAX_LINE];
    UT_hash_handle hh;
};

struct cfgparser_value
{
    struct cfgparser_section * section;
    char   key[CFGPARSER_MAX_LINE];
    char value[CFGPARSER_MAX_LINE];
    UT_hash_handle hh;
};



typedef struct cfgparser_context
{
  struct cfgparser_section * section_list;
  struct cfgparser_value * value_list;
  regex_t comment_pattern;
  regex_t section_pattern;
  regex_t value_pattern;
  regex_t key_pattern;
} cfgparser_context_t;

/* Read in a cfg file */
int cfgparser_load_file( char * file, cfgparser_context_t * context );
int cfgparser_get_str( char * section, char * key, char * value, cfgparser_context_t * context ); 
