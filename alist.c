#include "alist.h"
#include "macro.h"
#include "myopt.h"
#include "mynetworks.h"
#include "myhtmlparser.h"

int main(int argc, char *argv[])
{
  int level = DEFAULT_LEVEL; // default設定
  struct set_opt *setopt = NULL;
  struct url_in urlinfo;
  struct alist root;

  init_setopt(&setopt);
  memset(&urlinfo, 0, sizeof(urlinfo));

  if (checkopt(argc, argv, setopt)) {
    show_help(argv[0]);
    exit(EXIT_SUCCESS);
  }

  // 引数付きオプションがある場合の操作
  while (setopt->option != '\0') {
    switch (setopt->option) {
      case 'l':
        level = atoi(setopt->option_arg);
        break;
      case 'u':
        strncpy(urlinfo.url, setopt->option_arg, sizeof(urlinfo.url)-1);
        break;
      default:
        break;
    }
    if (setopt->next == NULL) {
      break;
    }
    setopt = setopt->next;
  }
  free_setopt(setopt);

  if (urlinfo.url[0] == '\0') {
    if (geturl(&urlinfo) != 0) {
      exit(EXIT_FAILURE);
    }
  }

  if (parseurl(&urlinfo) != 0) {
    exit(EXIT_FAILURE);
  }

  if (is_enable_i_option()) {
    showurlinfo(&urlinfo);
  }

  int sockfd = create_socket(&urlinfo);

  if (sockfd > 0) {
    send_html_request(&urlinfo, sockfd);

    root.self = &urlinfo;
    parse_html(&root, sockfd);

    close(sockfd); // ファイルディスクリプタのスローズ
  }

  return EXIT_SUCCESS;
}
