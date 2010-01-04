#ifndef METAUTIL_H
#define METAUTIL_H

#define QUOTE(x)     #x
#define QUOTE_VAL(x) QUOTE(x)

#define MAIN_EXE_FILE QUOTE_VAL(APPLICATION_BASE_NAME)

#endif
