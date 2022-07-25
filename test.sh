#!/usr/bin/env bash

RED=$'\e[1;31m'
GREEN=$'\e[1;32m'
MAGENTA=$'\e[1;35m'
CYN=$'\e[1;36m'
END=$'\e[0m'

function test_md5 {
  md5sum "$@" >/tmp/md5sum.log 2>/tmp/md5sum_error.log
  og_outcome=$?
  ./ft_ssl md5 "$@" >/tmp/ft_ssl.log 2>/tmp/ft_ssl_error.log
  ft_outcome=$?

  if [[ $og_outcome -ne $ft_outcome ]]; then
    echo "[${RED}KO${END}] Test '$@'"
    exit 1
  fi
  diff /tmp/md5sum.log /tmp/ft_ssl.log
  stdout_diff=$?
  diff /tmp/md5sum_error.log /tmp/ft_ssl_error.log
  stderr_diff=$?
  if [[ $stdout_diff -ne 0 || $stderr_diff -ne 0 ]]; then
    echo "[${RED}KO${END}] Test '$@'"
    exit 1
  fi
  echo "[${GREEN}OK${END}] Test '$@'"
}

if [[ ! -f "ft_ssl" ]];then
  make
fi

echo "The quick brown fox jumped over the lazy dog's back" > /tmp/fox.txt
touch /tmp/empty.txt
test_md5 /tmp/fox.txt
test_md5 /tmp/empty.txt
