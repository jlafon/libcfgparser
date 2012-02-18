/*    file: cfgparser.c
    Author: Jharrod LaFon
   Purpose: A very simple config file parser (read only).
*/
#include <stdio.h>
#include <regex.h>
#include <string.h>
#include "libcfgparser.h"
#include "uthash.h"

/* Regex patterns for matching cfg file parts */
#define CFGPARSER_COMMENT_PATTERN "[#;].*"
#define CFGPARSER_SECTION_PATTERN "\\[[a-zA-Z0-9\\-][a-zA-Z0-9\\-]*\\]"
#define CFGPARSER_VALUE_PATTERN   "[ \t]*\\([a-zA-Z0-9,\\-]*\\)[ \t]*[=:][ \t]*\\([a-zA-Z0-9,-\\.@]*\\)"

/* In case I messed up the POSIX regex */
void
_print_regex_error(regex_t* reg, int status)
{
    char error[CFGPARSER_MAX_LINE];
    regerror(status, reg, error, sizeof(error));
    fprintf(stderr, "Failed to compile regex: %s\n", error);
}

/* Compile the patterns for matching:
 * a section header,
 * a key[:=] value pattern,
 * a key[:=] pattern
 */
int
_compile_patterns(cfgparser_context_t* context)
{
    int status;

    if((status = regcomp(&context->comment_pattern, CFGPARSER_COMMENT_PATTERN, 0)) != 0) {
        _print_regex_error(&context->comment_pattern, status);
        goto error;
    }

    if((status = regcomp(&context->section_pattern, CFGPARSER_SECTION_PATTERN, 0)) != 0) {
        _print_regex_error(&context->section_pattern, status);
        goto error;
    }

    if((status = regcomp(&context->value_pattern, CFGPARSER_VALUE_PATTERN, 0))     != 0) {
        _print_regex_error(&context->value_pattern, status);
        goto error;
    }

    return 0;
error:
    return -1;
}

/* Try and match a key,value pair from a line of the config file.
   Not string safe. */
int
_match_value(char* line, char* key, char* value, cfgparser_context_t* context)
{
    regmatch_t match[3];
    int len;

    if(!regexec(&context->value_pattern, line, 3, match, 0)) {
        len = match[1].rm_eo - match[1].rm_so;
        /* TODO: Check string length and complain */
        memcpy(key, line + match[1].rm_so, len);
        key[len] = 0;
        len = match[2].rm_eo - match[2].rm_so;
        memcpy(value, line + match[2].rm_so, len);
        value[len] = 0;
        //        printf("%s -> %s\n",key,value);
        return 1;
    }

    return 0;
}

/* Try and match a comment */
int
_match_comment(char* line, cfgparser_context_t* context)
{
    regmatch_t match;
    int len = strlen(line);

    if(len == 0 || len >= CFGPARSER_MAX_LINE)
    { return 0; }

    if(!regexec(&context->comment_pattern, line, 1, &match, 0)) {
        return 1;
    }

    return 0;
}

/* Try and match a section header */
int
_match_section(char* line, char* section, cfgparser_context_t* context)
{
    regmatch_t match;
    int len = strlen(line);

    if(len == 0 || len >= CFGPARSER_MAX_LINE)
    { return 0; }

    if(!regexec(&context->section_pattern, line, 1, &match, 0)) {
        memcpy(section, line + match.rm_so + 1, match.rm_eo - match.rm_so - 2);
        section[match.rm_eo - match.rm_so - 2] = 0;
        return 1;
    }

    return 0;
}

/* Add a section to the context */
void
_add_section(char* section, cfgparser_context_t* context)
{
    struct cfgparser_section* head = context->section_list, *new_section;
    new_section = malloc(sizeof(*new_section));
    strcpy(new_section->name, section);
    HASH_ADD_STR(head, name, new_section);
    context->section_list = head;
}

/* Add a key,value to the context under section */
void
_add_value(char* section, char* key, char* value, cfgparser_context_t* context)
{
    struct cfgparser_value* head = context->value_list, *new_value;
    struct cfgparser_section* section_ptr = NULL;
    new_value = malloc(sizeof(*new_value));
    strcpy(new_value->key, section);
    strcat(new_value->key, ".");
    strcat(new_value->key, key);
    strcpy(new_value->value, value);
    /* Make sure the section exists */
    HASH_FIND_STR(context->section_list, section, section_ptr);

    if(!section_ptr) {
        fprintf(stderr, "Unexpected error.\n");
        return;
    }
    else
    { new_value->section = section_ptr; }

    HASH_ADD_STR(head, key, new_value);
    context->value_list = head;
    return;
}

/* Search for a value of key in the given section */
int cfgparser_get_str(char* section, char* key, char* value, cfgparser_context_t* context)
{
    struct cfgparser_value* val = NULL;
    char search_key[CFGPARSER_MAX_LINE];
    sprintf(search_key, "%s.%s", section, key);
    HASH_FIND_STR(context->value_list, search_key, val);

    if(val) {
        int len = strlen(val->value);

        if(len > CFGPARSER_MAX_LINE)
        { goto false; }

        strcpy(value, val->value);
        return 1;
    }

false:
    return 0;
}

/* Read in a config file and parse its values */
int
cfgparser_load_file(char* file, cfgparser_context_t* context)
{
    FILE* config_file = NULL;
    char line[CFGPARSER_MAX_LINE];
    char section[CFGPARSER_MAX_LINE];
    char key[CFGPARSER_MAX_LINE];
    char value[CFGPARSER_MAX_LINE];

    if(_compile_patterns(context) != 0)
    { return -1; }

    context->section_list = NULL;
    context->value_list = NULL;

    if(NULL == (config_file = fopen(file, "r"))) {
        fprintf(stderr, "Unable to open file: '%s'\n", file);
    }

    while(NULL != fgets(line, CFGPARSER_MAX_LINE, config_file)) {
        if(_match_comment(line, context))
        { continue; }
        else if(_match_section(line, section, context)) {
            _add_section(section, context);
        }
        else if(_match_value(line, key, value, context)) {
            _add_value(section, key, value, context);
        }
    }

    fclose(config_file);
    return 0;
}
