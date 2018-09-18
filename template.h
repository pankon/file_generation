
#include "file_generator.h"

char *filename = "simple_mmap";
char *prefix = "SimpleMmap";

struct header header = {
    "\n * SimpleMmap",
    "Nathan Pankowsky",
    "Usage:\n"
    " *\n"
    " * nu, just use it"
    "",
    "",
};

struct line lines[] = {
    {NULL, ' '},
};

struct signature signatures[] = {
    { 
        "GetMeACharArray",
        "char *filename",
        "char *",
        "Get a char array, mmapped"
    },
    { 
        "PleaseDisposeOfThisCharArray",
        "char *mmapped_data",
        "char *",
        "Get a rid of a mmapped char array"
    },
    {NULL, NULL, NULL, NULL}
};

