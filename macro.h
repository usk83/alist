#ifndef MACRO_H
#define MACRO_H

#ifndef DBGMODE
  #define DBGMODE 0
#endif

#define DBG(...) do{if(DBGMODE){(fprintf(stderr, "\033[33m=== %s %u @%s(): ",__FILE__,__LINE__,__func__), fprintf(stderr, __VA_ARGS__)), fprintf(stderr, "\033[39m\n");}}while(0)

#define STDDBG(...) do{if(DBGMODE){(printf("\033[33m=== %s %u @%s(): ",__FILE__,__LINE__,__func__), printf(__VA_ARGS__)), printf("\033[39m\n");}}while(0)

#define ERROR(...) (fprintf(stderr, "\033[31m"), fprintf(stderr, __VA_ARGS__), fprintf(stderr, "\033[39m\n"))

#endif
