#include "mynetworks.h"

int parseurl(struct url_in *urlinfo)
{
  char *tmp_p;
  int i;
  bool ipv6flag = false;

  if (sscanf(urlinfo->url, "http://%s", urlinfo->host_path)) {
  }
  else if (sscanf(urlinfo->url, "https://%s", urlinfo->host_path)){
  }
  else {
    strcpy(urlinfo->host_path, urlinfo->url);
  }

  separate_host_path(urlinfo);

  // IPv6で指定されている
  if (urlinfo->host[0] == '[') {
    ipv6flag  =true;
    tmp_p = strchr(urlinfo->host, ']');
    // 閉じがない
    if (!tmp_p) {
      ERROR("Entered host `%s` is invalid.\n", urlinfo->host);
      return 1;
    }
    // 閉じがある
    else {
      *tmp_p = '\0'; // 閉じ括弧を消す
      // ipアドレスの指定の後に
      if (*(tmp_p+1) != '\0') {
        if (*(tmp_p+1) != ':') {
          ERROR("Entered port `%s` is invalid.\n", tmp_p+1);
          return 1;
        }
        else {
          if (separate_host_port(urlinfo, tmp_p+1)) {
            return 1;
          }
        }
      }

      // 先頭の[を削除
      for (i = 0; urlinfo->host[i]; i++) {
        urlinfo->host[i] = urlinfo->host[i+1];
      }
    }
  }
  // v6の指定ではない
  else {
    // :の有無確認
    tmp_p = strchr(urlinfo->host, ':');
    // portがある
    if (tmp_p) {
      if (separate_host_port(urlinfo, tmp_p) != 0) {
        return 1;
      }
      *tmp_p = '\0';
    }
  }

  // hostが空でないか
  if (*urlinfo->host == '\0') {
    ERROR("Please input host.\n");
    return -1;
  }

  // 正しいv6のアドレスであるか確認
  if (ipv6flag) {
    if (validate_v6addr(urlinfo) != 0) {
      return 1;
    }
  }

  // ポート番号が未指定の場合
  if (urlinfo->port == 0) {
    getport(urlinfo);
  }
  return 0;
}

int geturl(struct url_in *urlinfo)
{
  urlinfo->url[0] = '\n';
  while (urlinfo->url[0] == '\n') {
    printf("URL> ");
    fgets(urlinfo->url, sizeof(urlinfo->url), stdin);
  }

  // 改行があれば取り除く
  if (urlinfo->url[strlen(urlinfo->url)-1] == '\n') {
    urlinfo->url[strlen(urlinfo->url)-1] = '\0';
  }
  // ぴったりでなければエラー
  else if (getchar() != '\n') {
    ERROR("Entered URL is too long. Please enter less than 2048 characters.\n");
    return 1;
  }
  return 0;
}

void separate_host_path(struct url_in *urlinfo)
{
  char *tmp_p;

  tmp_p = strchr(urlinfo->host_path, '/');
  // pathの記述がある
  if (tmp_p) {
    strcpy(urlinfo->path, tmp_p);
    *tmp_p = '\0';

    strcpy(urlinfo->host, urlinfo->host_path);
  }
  // hostのみ
  else {
    strcpy(urlinfo->host, urlinfo->host_path);
    urlinfo->path[0] = '/';
    urlinfo->path[1] = '\0';
  }
}

int separate_host_port(struct url_in *urlinfo, char *tmp_p)
{
  int i;
  bool flag = false;

  // 数字であれば
  for (i = 1; isdigit(*(tmp_p+i)); i++) {
    // 入力されたport番号が大きすぎる(もしくは不正)
    if ((flag && *(tmp_p+i) > 53) || i > 5) {
      ERROR("Entered port `%s` is too big.\n", tmp_p+1);
      return 1;
    }
    urlinfo->port *= 10;
    urlinfo->port += *(tmp_p+i);
    urlinfo->port -= 48; // asciiコードと数字の差を調整
    // 5桁で65535を越える可能性がある
    if (urlinfo->port > 6553) {
      flag = true;
    }
  }
  if (tmp_p[i] != '\0') {
    // 終端でない → 不正なport番号
    ERROR("Entered port `%s` is invalid.\n", tmp_p+1);
    return 1;
  }
  return 0;
}

int validate_v6addr(struct url_in *urlinfo)
{
  char tmpc;
  int i;
  for (i = 0; urlinfo->host[i] != '\0'; i++) {
    tmpc = urlinfo->host[i];
    // 0~:、かa~fかA~F以外が含まれる場合は向こうはhostと判定
    if ((47 < tmpc && tmpc < 59) || (64 < tmpc && tmpc < 71) || (96 < tmpc && tmpc < 103)) {
      continue;
    }
    ERROR("Entered host `%s` is invalid.\n", urlinfo->host);
    return 1;
  }
  return 0;
}

void getport(struct url_in *urlinfo)
{
  struct servent *serv;
  // ポート番号を取得
  serv = getservbyname("http", "tcp");
  if (serv != NULL) {
    urlinfo->port = ntohs((unsigned short)serv->s_port);
  }
  // 取得出来なかった場合は80とする
  else {
    urlinfo->port = 80;
  }
}

void showurlinfo(struct url_in *urlinfo)
{
  printf("Connecting to\n\thost: %s\n\tport: %d\n\tpath: %s\n", urlinfo->host, urlinfo->port, urlinfo->path);
  return;
}

int create_socket(struct url_in *urlinfo)
{
  struct addrinfo *ai;
  int sockfd;

  // ipアドレスの取得
  struct addrinfo hints, *res;
  struct sockaddr_in *sin;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = PF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  getaddrinfo(urlinfo->host, "http", &hints, &res);
  // int errcode = getaddrinfo(urlinfo->host, "http", &hints, &res);
  if (!res) {
    ERROR("Cannot find IP address of `%s`.\n", urlinfo->host);
    return -1;
  }

  for (ai = res; ai; ai = ai->ai_next) {
    sin = (struct sockaddr_in *)ai->ai_addr;
    sin->sin_port = htons(urlinfo->port);

    // ソケット生成
    if ((sockfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol)) < 0) {
      perror("socket");
      return -1;
    }

    // サーバに接続
    if (connect(sockfd, ai->ai_addr, ai->ai_addrlen) == -1) {
      continue;
    }

    char *ipaddr = (char *)addr_print(ai->ai_family, ai->ai_addr);

    printf("connected to %s\n", ipaddr);
    strncpy(urlinfo->ip_addr, (char *)ipaddr, sizeof(urlinfo->ip_addr)-1);
    break;
  }
  // 領域の解放
  freeaddrinfo(res);

  return sockfd;
}

const char* addr_print(int family, struct sockaddr *addr)
{
  static char dstaddrstrbuf[INET6_ADDRSTRLEN];

  return family == PF_INET
    ? inet_ntop(PF_INET, &(((struct sockaddr_in*)addr)->sin_addr), dstaddrstrbuf, sizeof(dstaddrstrbuf))
    : inet_ntop(PF_INET6, &(((struct sockaddr_in6*)addr)->sin6_addr), dstaddrstrbuf, sizeof(dstaddrstrbuf));
}
