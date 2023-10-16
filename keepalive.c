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
#include "keepalive.h"

void keepalive_init(keepalive_t *opt) {
  opt->debug = 0;
  opt->tcp_keepidle = 15; /* TCP_KEEPIDLE: seconds */
  opt->tcp_keepcnt = 9;   /* TCP_KEEPCNT: 9 retries */
  opt->tcp_keepintvl =
      15; /* TCP_KEEPINTVL:  9 * 15 = 135 seconds to disconnect */

  opt->tcp_user_timeout =
      -1; /* TCP_USER_TIMEOUT: seconds: 0 to use system default, -1 to derive
             from tcp_keepidle, tcp_keepcnt and tcp_keepintvl */

  opt->tcp_syncnt = 0;       /* TCP_SYNCNT: system default */
  opt->tcp_defer_accept = 0; /* TCP_DEFER_ACCEPT: seconds */

  opt->tcp_maxseg = 0;       /* TCP_MAXSEG: system default */
  opt->tcp_window_clamp = 0; /* TCP_WINDOW_CLAMP: system default */
}

int keepalive(int sockfd, keepalive_t *opt) {
  int enable = 1;

  if (setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &enable, sizeof(enable)) <
      0) {
    if (opt->debug)
      (void)fprintf(stderr, "libkeepalive:setsockopt(SO_KEEPALIVE, 1): %s\n",
                    strerror(errno));
    return -1;
  }

#ifdef TCP_KEEPIDLE
  if (opt->tcp_keepidle > 0 &&
      setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPIDLE, &opt->tcp_keepidle,
                 sizeof(opt->tcp_keepidle)) < 0) {
    if (opt->debug)
      (void)fprintf(stderr, "libkeepalive:setsockopt(TCP_KEEPIDLE, %d): %s\n",
                    opt->tcp_keepidle, strerror(errno));
  }
#endif

#ifdef TCP_KEEPCNT
  if (opt->tcp_keepcnt > 0 &&
      setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPCNT, &opt->tcp_keepcnt,
                 sizeof(opt->tcp_keepcnt)) < 0) {
    if (opt->debug)
      (void)fprintf(stderr, "libkeepalive:setsockopt(TCP_KEEPCNT, %d): %s\n",
                    opt->tcp_keepcnt, strerror(errno));
  }
#endif

#ifdef TCP_KEEPINTVL
  if (opt->tcp_keepintvl > 0 &&
      setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPINTVL, &opt->tcp_keepintvl,
                 sizeof(opt->tcp_keepintvl)) < 0) {
    if (opt->debug)
      (void)fprintf(stderr, "libkeepalive:setsockopt(TCP_KEEPINTVL, %d): %s\n",
                    opt->tcp_keepintvl, strerror(errno));
  }
#endif

#ifdef TCP_USER_TIMEOUT
  if (opt->tcp_user_timeout > 0 &&
      setsockopt(sockfd, IPPROTO_TCP, TCP_USER_TIMEOUT, &opt->tcp_user_timeout,
                 sizeof(opt->tcp_user_timeout)) < 0) {
    if (opt->debug)
      (void)fprintf(stderr,
                    "libkeepalive:setsockopt(TCP_USER_TIMEOUT, %d): %s\n",
                    opt->tcp_user_timeout, strerror(errno));
  }
#endif

#ifdef TCP_SYNCNT
  if (opt->tcp_syncnt > 0 &&
      setsockopt(sockfd, IPPROTO_TCP, TCP_SYNCNT, &opt->tcp_syncnt,
                 sizeof(opt->tcp_syncnt)) < 0) {
    if (opt->debug)
      (void)fprintf(stderr, "libkeepalive:setsockopt(TCP_SYNCNT, %d): %s\n",
                    opt->tcp_syncnt, strerror(errno));
  }
#endif

#ifdef TCP_DEFER_ACCEPT
  if (opt->tcp_defer_accept > 0 &&
      setsockopt(sockfd, IPPROTO_TCP, TCP_DEFER_ACCEPT, &opt->tcp_defer_accept,
                 sizeof(opt->tcp_defer_accept)) < 0) {
    if (opt->debug)
      (void)fprintf(stderr,
                    "libkeepalive:setsockopt(TCP_DEFER_ACCEPT, %d): %s\n",
                    opt->tcp_defer_accept, strerror(errno));
  }
#endif

#ifdef TCP_MAXSEG
  if (opt->tcp_maxseg > 0 &&
      setsockopt(sockfd, IPPROTO_TCP, TCP_MAXSEG, &opt->tcp_maxseg,
                 sizeof(opt->tcp_maxseg)) < 0) {
    if (opt->debug)
      (void)fprintf(stderr, "libkeepalive:setsockopt(TCP_MAXSEG, %d): %s\n",
                    opt->tcp_maxseg, strerror(errno));
  }
#endif

#ifdef TCP_WINDOW_CLAMP
  if (opt->tcp_window_clamp > 0 &&
      setsockopt(sockfd, IPPROTO_TCP, TCP_WINDOW_CLAMP, &opt->tcp_window_clamp,
                 sizeof(opt->tcp_window_clamp)) < 0) {
    if (opt->debug)
      (void)fprintf(stderr,
                    "libkeepalive:setsockopt(TCP_WINDOW_CLAMP, %d): %s\n",
                    opt->tcp_window_clamp, strerror(errno));
  }
#endif

  return 0;
}
