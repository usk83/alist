#ifndef MYOPT_H
#define MYOPT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define ARG_LEN 2049

struct set_opt {
  char option;
  char option_arg[ARG_LEN];
  struct set_opt *next;
};

static bool enable_i_option = false;

void free_setopt(struct set_opt *header);
void init_setopt(struct set_opt **setopt);
int checkopt(int argc, char *argv[], struct set_opt *setopt);
void show_help(char *command);
void set_option(struct set_opt *setopt, char option, char *arg);
bool is_enable_i_option();

#endif
