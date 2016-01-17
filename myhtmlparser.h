#ifndef MYHTMLPARSER_H
#define MYHTMLPARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <ctype.h>

#include "mynetworks.h"
#include "alist.h"

#define BUF_LEN 256
#define KEY_LEN 16

void send_html_request(struct url_in *urlinfo, int sockfd);
char get_from_buffer(int sockfd, char *buf, int size);
int find_key(char *key, int sockfd, char *buf, int size);
int get_title(struct alist *parent, int sockfd, char *buf, int size);
int get_anchor(struct alist *parent, int sockfd, char *buf, int size);
int parse_html(struct alist *parent, int sockfd);

#endif
