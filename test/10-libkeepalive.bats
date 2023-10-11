#!/usr/bin/env bats

@test "libkeepalive: socket options set" {
  run env LD_PRELOAD=./libkeepalive.so strace -e trace=network nc 127.11.7.43 19931
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

  [[ $output =~ $SO_KEEPALIVE ]]
  [[ $output =~ $TCP_KEEPIDLE ]]
  [[ $output =~ $TCP_KEEPCNT ]]
  [[ $output =~ $TCP_KEEPINTVL ]]
  [[ $output =~ $TCP_USER_TIMEOUT ]]
}
