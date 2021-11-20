# SYNOPSIS

# client only 
LD\_PRELOAD=libkeepalive.so *COMMAND* *ARG* *...*

# server only 
LD\_PRELOAD=libkeepalive_listen.so *COMMAND* *ARG* *...*

# client and server
LD\_PRELOAD=libkeepalive.so:libkeepalive_listen.so *COMMAND* *ARG* *...*

# DESCRIPTION

libkeepalive: set TCP keepalives options

libkeepalive is a small library for setting various socket options
required for enabling TCP keepalives. See:

https://blog.cloudflare.com/when-tcp-sockets-refuse-to-die/

http://codearcana.com/posts/2015/08/28/tcp-keepalive-is-a-lie.html

`libkeepalive` works by intercepting calls to `connect(2)` using
`LD_PRELOAD`. Before `connect(2)`ing, `setsockopt(2)` is called using
the configured socket options.

`libkeepalive_listen` works by intercepting calls to `listen(2)`
using `LD_PRELOAD`. Socket options are set when the application calls
`listen(2)`. Socket options for `accept`(2)'ed fd's are inherited from
the listener socket.

libkeepalive requires the program to be dynamically linked and will
not work with statically linked programs or programs that directly
make syscalls.

libkeepalive is a small LD_PRELOAD library to enable TCP keepalives and
TCP_USER_TIMEOUT on any sockets opened by dynamically linked applications,
either outbound (connect(2), using libkeepalive.so) or inbound (listen(2),
using libkeepalive_listen.so).

The typical situation is that a long lasting connection is established
across some network boundary. The connection is idle and some intermediary
device drops the connection state. Enabling TCP keepalives will keep the
connection active and, if the connection is dropped by the intermediary,
force it to be closed.

Alternatively, the connection can be dropped by the intermediary while
there is still pending data. The TCP_USER_TIMEOUT setting will force
the connection to be closed.

# ENVIRONMENT VARIABLES

Setting options to 0 will use the system default.

## COMMON VARIABLES

`LIBKEEPALIVE_DEBUG`
: Write errors to stdout (default: disabled). Set to any value to enable.

    LIBKEEPALIVE_DEBUG=1

`TCP_KEEPIDLE`
: The number of seconds a connection is idle before TCP keepalives are sent
  (default: 15).

`TCP_KEEPCNT`
: If the peer does not respond, the number of keepalives sent before
  terminating the connection. Note: see `tcp`(7) for interaction of
  `TCP_KEEPCNT` with `TCP_USER_TIMEOUT` (default: 9).

`TCP_KEEPINTVL`
: Configures the interval in seconds a keepalive is retried if the peer
  is not responding (default: 15).

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
# ALTERNATIVES

* [libkeepalive](http://libkeepalive.sourceforge.net/)

  The original libkeepalive, the one included in your package system,
  that this library unfortunately (and unintentionally) name squatted.

# SEE ALSO

_socket_(7), _tcp_(7), _connect_(2), _listen_(2), _accept_(2), _setsockopt_(2)
