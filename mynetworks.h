#ifndef MYNETWORKS_H
#define MYNETWORKS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <ctype.h>

#include "myopt.h"
#include "macro.h"

#define URL_LEN 2049
#define INFO_LEN 1025

struct url_in {
  char url[URL_LEN];
  char host_path[URL_LEN];
  char host[URL_LEN];
  char path[URL_LEN];
  unsigned short port;
  char ip_addr[INFO_LEN];
  char title[INFO_LEN];
};

int parseurl(struct url_in *urlinfo);
int geturl(struct url_in *urlinfo);
void separate_host_path(struct url_in *urlinfo);
int separate_host_port(struct url_in *urlinfo, char *tmp_p);
int validate_v6addr(struct url_in *urlinfo);
void getport(struct url_in *urlinfo);
void showurlinfo(struct url_in *urlinfo);
int create_socket(struct url_in *urlinfo);
const char* addr_print(int family, struct sockaddr *addr);

#endif
