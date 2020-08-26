#!/usr/bin/env bats

@test "libkeepalive_connect: socket options set" {
  run env LD_PRELOAD=./libkeepalive_listen.so timeout 3 strace -e trace=network nc -l 0
  cat << EOF
--- output
$output
--- output
EOF

  [ "$status" -ne 0 ]

  SO_KEEPALIVE='setsockopt\(4, SOL_SOCKET, SO_KEEPALIVE, \[1\], 4\) = 0'
  TCP_KEEPIDLE='setsockopt\(4, SOL_TCP, TCP_KEEPIDLE, \[15\], 4\) = 0'
  TCP_KEEPCNT='setsockopt\(4, SOL_TCP, TCP_KEEPCNT, \[9\], 4\) = 0'
  TCP_KEEPINTVL='setsockopt\(4, SOL_TCP, TCP_KEEPINTVL, \[15\], 4\) = 0'
  TCP_USER_TIMEOUT='setsockopt\(4, SOL_TCP, TCP_USER_TIMEOUT, \[135015\], 4\) = 0'

  [[ $output =~ $SO_KEEPALIVE ]]
  [[ $output =~ $TCP_KEEPIDLE ]]
  [[ $output =~ $TCP_KEEPCNT ]]
  [[ $output =~ $TCP_KEEPINTVL ]]
  [[ $output =~ $TCP_USER_TIMEOUT ]]
}
