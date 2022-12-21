#!/usr/bin/env bash

set -e # Exit on error

echo "password" > password_file
openssl des-ecb -p -in openssl.html -out ciphertext.html -pass "pass:$(cat password_file)" -S C2F403B11671DC9A
# Run the openssl command in docker, since the openssl version (3.0.2) on the Dell's at 42 and my laptop fails on des-ecb and des-cbc
./ft_ssl des-ecb -d -i ciphertext.html -o decrypted.html -p "$(cat password_file)" -s C2F403B11671DC9A
# And run the ./ft_ssl command on our hostmachine
open *.html

#echo "Make sure they really tell you what is going on 'under the hood' when you use CBC mode, and how it is more secure than ECB" | openssl des-cbc -K "BABE101010FACADE" -iv "77696E6B66616365" | ./ft_ssl des-cbc -k "BABE101010FACADE" -v "77696E6B66616365" -d
