#!/usr/bin/env bats

@test "libkeepalive_socket: socket options set" {
  run env LD_PRELOAD=./libkeepalive_socket.so TCP_SYNCNT=1 TCP_MAXSEG=1400 TCP_WINDOW_CLAMP=9999 strace -e trace=network nc 127.11.7.43 19931
  cat << EOF
--- output
$output
--- output
EOF

  [ "$status" -ne 0 ]

  SO_KEEPALIVE='setsockopt\([0-9]*, SOL_SOCKET, SO_KEEPALIVE, \[1\], 4\) = 0'
  TCP_KEEPIDLE='setsockopt\([0-9]*, SOL_TCP, TCP_KEEPIDLE, \[15\], 4\) = 0'
  TCP_KEEPCNT='setsockopt\([0-9]*, SOL_TCP, TCP_KEEPCNT, \[9\], 4\) = 0'
  TCP_KEEPINTVL='setsockopt\([0-9]*, SOL_TCP, TCP_KEEPINTVL, \[15\], 4\) = 0'
  TCP_USER_TIMEOUT='setsockopt\([0-9]*, SOL_TCP, TCP_USER_TIMEOUT, \[150000\], 4\) = 0'
  TCP_SYNCNT='setsockopt\([0-9]*, SOL_TCP, TCP_SYNCNT, \[1\], 4\) = 0'
  TCP_MAXSEG='setsockopt\([0-9]*, SOL_TCP, TCP_MAXSEG, \[1400\], 4\) = 0'
  TCP_WINDOW_CLAMP='setsockopt\([0-9]*, SOL_TCP, TCP_WINDOW_CLAMP, \[9999\], 4\) = 0'

  [[ $output =~ $SO_KEEPALIVE ]]
  [[ $output =~ $TCP_KEEPIDLE ]]
  [[ $output =~ $TCP_KEEPCNT ]]
  [[ $output =~ $TCP_KEEPINTVL ]]
  [[ $output =~ $TCP_USER_TIMEOUT ]]
  [[ $output =~ $TCP_SYNCNT ]]
  [[ $output =~ $TCP_MAXSEG ]]

  case "$(uname -s)" in
  Linux)
    [[ $output =~ $TCP_SYNCNT ]]
    [[ $output =~ $TCP_WINDOW_CLAMP ]]
    ;;
  *)
    ;;
  esac
}
