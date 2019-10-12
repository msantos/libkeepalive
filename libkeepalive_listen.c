/* Copyright (c) 2019, Michael Santos <michael.santos@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include <arpa/inet.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>

void _init(void);
int (*sys_listen)(int sockfd, int backlog);
#pragma GCC diagnostic ignored "-Wpedantic"
int listen(int sockfd, int backlog);
#pragma GCC diagnostic warning "-Wpedantic"
static int keepalive(int sockfd);

char *debug;
int tcp_keepidle = 15;
int tcp_keepcnt = 9;
int tcp_keepintvl = 15;

int tcp_user_timeout =
    -1; /* TCP_USER_TIMEOUT: seconds: 0 to disable, -1 to derive from
                      tcp_keepidle, tcp_keepcnt and tcp_keepintvl */

int tcp_defer_accept = 0; /* TCP_DEFER_ACCEPT: seconds */

void _init(void) {
  const char *err;
  char *env_tcp_keepidle;
  char *env_tcp_keepcnt;
  char *env_tcp_keepintvl;
  char *env_tcp_user_timeout;
  char *env_tcp_defer_accept;

  debug = getenv("LIBKEEPALIVE_DEBUG");
  env_tcp_keepidle = getenv("TCP_KEEPIDLE");
  env_tcp_keepcnt = getenv("TCP_KEEPCNT");
  env_tcp_keepintvl = getenv("TCP_KEEPINTVL");
  env_tcp_user_timeout = getenv("TCP_USER_TIMEOUT");
  env_tcp_defer_accept = getenv("TCP_DEFER_ACCEPT");

  if (env_tcp_keepidle)
    tcp_keepidle = atoi(env_tcp_keepidle);

  if (env_tcp_keepcnt)
    tcp_keepidle = atoi(env_tcp_keepcnt);

  if (env_tcp_keepintvl)
    tcp_keepintvl = atoi(env_tcp_keepintvl);

  if (env_tcp_user_timeout)
    tcp_user_timeout = atoi(env_tcp_user_timeout);

  if (env_tcp_defer_accept)
    tcp_defer_accept = atoi(env_tcp_defer_accept);

  /* TCP_KEEPIDLE + TCP_KEEPINTVL * TCP_KEEPCNT */
  if (tcp_user_timeout < 0)
    tcp_user_timeout = tcp_keepidle + tcp_keepintvl * tcp_keepcnt * 1000;

#pragma GCC diagnostic ignored "-Wpedantic"
  sys_listen = dlsym(RTLD_NEXT, "listen");
#pragma GCC diagnostic warning "-Wpedantic"
  err = dlerror();

  if (err != NULL)
    (void)fprintf(stderr, "libkeepalive:dlsym (listen):%s\n", err);
}

static int keepalive(int sockfd) {
  int enable = 1;

  if (setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &enable, sizeof(enable)) <
      0) {
    if (debug)
      (void)fprintf(stderr, "libkeepalive:setsockopt(SO_KEEPALIVE)=1:%s\n",
                    strerror(errno));
    return -1;
  }

#ifdef TCP_KEEPIDLE
  if (tcp_keepidle > 0 && setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPIDLE,
                                     &tcp_keepidle, sizeof(tcp_keepidle)) < 0) {
    if (debug)
      (void)fprintf(stderr, "libkeepalive:setsockopt(TCP_KEEPIDLE):%s\n",
                    strerror(errno));
  }
#endif

#ifdef TCP_KEEPCNT
  if (tcp_keepcnt > 0 && setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPCNT,
                                    &tcp_keepcnt, sizeof(tcp_keepcnt)) < 0) {
    if (debug)
      (void)fprintf(stderr, "libkeepalive:setsockopt(TCP_KEEPCNT):%s\n",
                    strerror(errno));
  }
#endif

#ifdef TCP_KEEPINTVL
  if (tcp_keepintvl > 0 &&
      setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPINTVL, &tcp_keepintvl,
                 sizeof(tcp_keepintvl)) < 0) {
    if (debug)
      (void)fprintf(stderr, "libkeepalive:setsockopt(TCP_KEEPINTVL):%s\n",
                    strerror(errno));
  }
#endif

#ifdef TCP_USER_TIMEOUT
  if (tcp_user_timeout > 0 &&
      setsockopt(sockfd, IPPROTO_TCP, TCP_USER_TIMEOUT, &tcp_user_timeout,
                 sizeof(tcp_user_timeout)) < 0) {
    if (debug)
      (void)fprintf(stderr, "libkeepalive:setsockopt(TCP_USER_TIMEOUT):%s\n",
                    strerror(errno));
  }
#endif

#ifdef TCP_DEFER_ACCEPT
  if (tcp_defer_accept > 0 &&
      setsockopt(sockfd, IPPROTO_TCP, TCP_DEFER_ACCEPT, &tcp_defer_accept,
                 sizeof(tcp_defer_accept)) < 0) {
    if (debug)
      (void)fprintf(stderr, "libkeepalive:setsockopt(TCP_DEFER_ACCEPT):%s\n",
                    strerror(errno));
  }
#endif

  return 0;
}

int listen(int sockfd, int backlog) {
  int oerrno = errno;

  (void)keepalive(sockfd);
  errno = oerrno;
  return sys_listen(sockfd, backlog);
}
