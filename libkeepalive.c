/* Copyright (c) 2019-2023, Michael Santos <michael.santos@gmail.com>
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

#include "keepalive.h"

void _init(void);
static int (*sys_connect)(int sockfd, const struct sockaddr *addr,
                          socklen_t addrlen);
#pragma GCC diagnostic ignored "-Wpedantic"
int __attribute__((visibility("default")))
connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
#pragma GCC diagnostic warning "-Wpedantic"

static keepalive_t opt = {0};

void _init(void) {
  const char *err;

  char *env_debug;
  char *env_tcp_keepidle;
  char *env_tcp_keepcnt;
  char *env_tcp_keepintvl;
  char *env_tcp_user_timeout;
  char *env_tcp_syncnt;

  env_debug = getenv("LIBKEEPALIVE_DEBUG");
  env_tcp_keepidle = getenv("TCP_KEEPIDLE");
  env_tcp_keepcnt = getenv("TCP_KEEPCNT");
  env_tcp_keepintvl = getenv("TCP_KEEPINTVL");
  env_tcp_user_timeout = getenv("TCP_USER_TIMEOUT");
  env_tcp_syncnt = getenv("TCP_SYNCNT");

  keepalive_init(&opt);

  if (env_debug)
    opt.debug = 1;

  if (env_tcp_keepidle)
    opt.tcp_keepidle = atoi(env_tcp_keepidle);

  if (env_tcp_keepcnt)
    opt.tcp_keepidle = atoi(env_tcp_keepcnt);

  if (env_tcp_keepintvl)
    opt.tcp_keepintvl = atoi(env_tcp_keepintvl);

  if (env_tcp_user_timeout)
    opt.tcp_user_timeout = atoi(env_tcp_user_timeout);

  if (env_tcp_syncnt)
    opt.tcp_syncnt = atoi(env_tcp_syncnt);

  /* TCP_KEEPIDLE + TCP_KEEPINTVL * TCP_KEEPCNT */
  if (opt.tcp_user_timeout < 0)
    opt.tcp_user_timeout =
        opt.tcp_keepidle + opt.tcp_keepintvl * opt.tcp_keepcnt * 1000;

#pragma GCC diagnostic ignored "-Wpedantic"
  sys_connect = dlsym(RTLD_NEXT, "connect");
#pragma GCC diagnostic warning "-Wpedantic"
  err = dlerror();

  if (err != NULL)
    (void)fprintf(stderr, "libkeepalive:dlsym (connect): %s\n", err);
}

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
  int oerrno = errno;

  (void)keepalive(sockfd, &opt);
  errno = oerrno;
  return sys_connect(sockfd, addr, addrlen);
}
