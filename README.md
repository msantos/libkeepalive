libkeepalive - library for setting TCP keepalives options

# SYNOPSIS

# client only 
LD\_PRELOAD=libkeepalive.so *COMMAND* *ARG* *...*

# server only 
LD\_PRELOAD=libkeepalive_listen.so *COMMAND* *ARG* *...*

# client and server
LD\_PRELOAD=libkeepalive.so:libkeepalive_listen.so *COMMAND* *ARG* *...*

# DESCRIPTION

libkeepalive is a small library for setting various socket options
required for enabling TCP keepalives. See:

https://blog.cloudflare.com/when-tcp-sockets-refuse-to-die/

`libkeepalive` works by intercepting calls to `connect(2)` using
`LD_PRELOAD`. Before `connect(2)`ing, `setsockopt(2)` is called using
the configured socket options.

`libkeepalive_listen` works by intercepting calls to `listen(2)`
using `LD_PRELOAD`. Socket options are set when the application calls
`listen(2)`. Socket options for `accept`(2)'ed fd's are inherited from
the listener socket.

libkeepalive requires the program to be dynamically linked. libkeepalive
will not work with statically linked programs or programs that directly
make syscalls.

# ENVIRONMENT VARIABLES

Setting these options to 0 will use the system default.

## COMMON VARIABLES

`LIBKEEPALIVE_DEBUG`
: Write errors to stdout (default: disabled).

`TCP_KEEPIDLE`
: The number of seconds a connection is idle before TCP keepalives are sent
  (default: 15).

`TCP_KEEPCNT`
: If the peer does not respond, the number of keepalives sent before
  terminating the connection. Note: see `tcp`(7) for interaction of
  `TCP_KEEPCNT` with `TCP_USER_TIMEOUT` (default: 9).

`TCP_KEEPINTVL`
: Interval in seconds a keepalive is retried if the peer is not
  responding (default: 15).

`TCP_USER_TIMEOUT`
: Number in milliseconds before an inactive connection in ESTABLISHED
  state is terminated.

  When `TCP_USER_TIMEOUT` is enabled (default), the TCP keepalive count is
  ignored. The connection will be terminated when the `TCP_USER_TIMEOUT`
  is reached.

  The default `TCP_USER_TIMEOUT` is calculated using:

      tcp_keepidle + tcp_keepintvl * tcp_keepcnt * 1000;

  Possible values:

      0: use system default
      -1: derive from other settings (default)
      >0: set to this value

## libkeepalive

`TCP_SYNCNT`
: Number of SYN packets sent on `connect(2)` (default: 0 (system default))

## libkeepalive_listen

`TCP_DEFER_ACCEPT`
: Avoid waking up the server process until data is sent by the
  client. Number of seconds to wait (default: 0 (disabled))

# EXAMPLES

## netcat

```
## Use strace to verify setsockopt(2) is called

# run in a shell
LD_PRELOAD=libkeepalive_listen.so strace -e trace=network nc -k -l 9090

# in another shell
LD_PRELOAD=libkeepalive.so strace -e trace=network nc 127.0.0.1 9090
```

# SEE ALSO

_socket_(7), _tcp_(7), _connect_(2), _listen_(2), _accept_(2), _setsockopt_(2)
