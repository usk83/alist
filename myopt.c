#include "myopt.h"

void free_setopt(struct set_opt *setopt)
{
  struct set_opt *header = setopt;
  struct set_opt *swap = NULL;

  while(header != NULL){
    swap = header->next;
    free(header);
    header = swap;
  }
}

void init_setopt(struct set_opt **setopt)
{
  *setopt = (struct set_opt*)malloc(sizeof(struct set_opt));
  (*setopt)->option = '\0';
  (*setopt)->option_arg[0] = '\0';
  (*setopt)->next = NULL;
  return;
}

int checkopt(int argc, char *argv[], struct set_opt *setopt)
{
  int ch;
  bool hflag = false;

  while((ch = getopt(argc, argv, "hil:u:")) != -1) {
    switch (ch) {
      case 'h':
        hflag = true;
        break;
      case 'i':
        enable_i_option = true;
        break;
      case 'l':
        set_option(setopt, 'l', optarg);
        break;
      case 'u':
        set_option(setopt, 'u', optarg);
        break;
      default:
        // fprintf(stderr, "illegal option -- %c skipped\n", (char)optopt);
        break;
    }
  }

  if (hflag) {
    return 1;
  }
  else {
    return 0;
  }
}

void show_help(char *command)
{
  printf("usege: %s [-hi] [-l level] [-u url]\n\
\t-h: help\n\
\t-i: show detailed info\n\
\t-l: recursion depth (Future work)\n\
\t-u: set the target URL\n", command);
}

void set_option(struct set_opt *setopt, char option, char *arg)
{
  while (setopt->next != NULL) {
    setopt = setopt->next;
  }
  if (setopt->option != '\0') {
    init_setopt(&(setopt->next));
    setopt = setopt->next;
  }
  setopt->option = option;
  strncpy(setopt->option_arg, arg, sizeof(setopt->option_arg)-1);
}

bool is_enable_i_option()
{
  return enable_i_option;
}
