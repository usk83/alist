#ifndef ALIST_H
#define ALIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <ctype.h>

#define DEFAULT_LEVEL 3

struct alist {
  struct url_in *self;
  struct alist_children *children;
};

struct alist_children {
  struct alist *node;
  struct alist_children *next;
};

#endif
