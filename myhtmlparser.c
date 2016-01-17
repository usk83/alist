#include "myhtmlparser.h"

void send_html_request(struct url_in *urlinfo, int sockfd)
{
  char send_buf[BUF_LEN];

  // HTTPリクエストを送信
  sprintf(send_buf, "GET %s HTTP/1.0\r\n", urlinfo->path);
  write(sockfd, send_buf, strlen(send_buf));

  sprintf(send_buf, "Host: %s:%d\r\n", urlinfo->host, urlinfo->port);
  write(sockfd, send_buf, strlen(send_buf));

  sprintf(send_buf, "\r\n");
  write(sockfd, send_buf, strlen(send_buf));
}

char get_from_buffer(int sockfd, char *buf, int size)
{
  static int index = 0;
  if (buf[index] == '\0') {
    if (read(sockfd, buf, size-1) <= 0) {
      return '\0'; // すべて読み込み終了
    }
    index = 0;
  }
  return buf[index++];
}

int find_key(char *key, int sockfd, char *buf, int size)
{
  int i;
  char keyC[KEY_LEN];
  char *key_p = key;
  char *keyC_p = keyC;
  char gfb;

  for(i = 0; *(key + i) != '\0' && i < KEY_LEN; i++) {
    keyC[i] = toupper(*(key + i));
  }
  keyC[i] = '\0';

  while (1) {
    if (!(gfb = get_from_buffer(sockfd, buf, size))) {
      return 1; // すべて読み込み終了
    }
    if (gfb == *key_p || gfb == *keyC_p) {
      key_p += 1;
      keyC_p += 1;
      if (*key_p == '\0') {
        return 0;
      }
    }
    else {
      key_p = key;
      keyC_p = keyC;
    }
  }
  return 0;
}

int get_title(struct alist *parent, int sockfd, char *buf, int size)
{
  char gfb;
  char *pt = parent->self->title;
  bool start_flag = false;
  while (1) {
    if (!(gfb = get_from_buffer(sockfd, buf, size))) {
      return 1; // すべて読み込み終了
    }

    if (!start_flag) {
      if (gfb == '>') {
        start_flag = true;
      }
      continue;
    }

    if (gfb != '<') {
      if (pt != &(parent->self->title[sizeof(parent->self->title)-1])) {
        *pt = gfb;
        pt += 1;
      }
      else {
        *(pt-4) = '.';
        *(pt-3) = '.';
        *(pt-2) = '.';
        return 0;
      }
    }
    else {
      if (!(gfb = get_from_buffer(sockfd, buf, size))) {
        return 1; // すべて読み込み終了
      }

      if (gfb == '/') {
        *pt = '\0';
        return 0;
      }
      else {
        *pt = '<';
        pt += 1;
      }
    }
  }
}

int get_anchor(struct alist *parent, int sockfd, char *buf, int size)
{
  char pare = get_from_buffer(sockfd, buf, size);
  char gfb = get_from_buffer(sockfd, buf, size);
  if (!pare || !gfb) {
    return 1;
  }
  if (gfb != '.' && gfb != '/' && gfb != '#' && gfb == 'h') {
    printf("└\"");
    while (gfb != pare && gfb != ' ' && gfb != '<') {
      printf("%c", gfb);
      gfb = get_from_buffer(sockfd, buf, size);
      if (!gfb) {
        return 1;
      }
    }
    puts("\"");
  }
  return 0;
}

int parse_html(struct alist *parent, int sockfd)
{
  char buf[BUF_LEN];
  int size = (int)sizeof(buf);

  if (find_key("<title", sockfd, buf, size) == 0) {
    if (get_title(parent, sockfd, buf, size) != 0) {
      return 1;
    }
    printf("`%s`\n", parent->self->title);
  }
  while (find_key("<a ", sockfd, buf, size) == 0) {
    if (find_key("href=", sockfd, buf, size) == 0) {
      if (get_anchor(parent, sockfd, buf, size) != 0) {
        return 1;
      }
    }
    else {
      return 1;
    }
  }
  return 0;
}
