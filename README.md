libcfgparser 0.0.1
===============
libcfgparser is a very simple and crude cfg file parser for C programs.  It can
read (only) cfg files and return values from Python style cfg files. 

Dependencies
------------
libc

Compile and install
-------------------
```
./configure
sudo make install
```

Developer API Information
-------------------------
The basic program flow when using libcfgparser is the following:

1. Load a cfg file
2. Get string values from the file

```C
#include <libcfgparser.h>

/* Create a context */
cfgparser_context context;

/* Load a cfg file into memory */
cfgparser_load_file("foo.cfg",&context)

/* Read a value from the cfg file where the entry could be:
 * [section-one]
 * key: value
 * or
 * key=value
 * white space is ignored
 * */
char value[CFGPARSER_MAX_LINE];
cfgparser_get_str("section-one","key",value, &context);
