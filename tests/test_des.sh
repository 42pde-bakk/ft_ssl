#!/usr/bin/env bash

set -e # Exit on error

# Is BASE64 working?
echo "repeat after me 'encoding is not encryption'" | base64
echo "repeat after me 'encoding is not encryption'" | ./ft_ssl base64
diff <(echo -n abc | openssl base64) <(echo -n abc | ./ft_ssl base64)
echo "All your base are belong to us. You have no chance make your time." | ./ft_ssl base64 | ./ft_ssl base64 -d


# Do they have a flag in their base?
echo "best game in the franchise" | ./ft_ssl base64 -e -o "halo1"
./ft_ssl base64 -d -i "halo1"

# DES-eptively easy!

export DES_KEY="C0FFEE69DEADBEEF"

echo "foo bar" | ./ft_ssl des-ecb -k "$DES_KEY" | openssl des-ecb -d -K "$DES_KEY" -provider=legacy
echo "foo bar" | openssl des-ecb -K "$DES_KEY" -provider=legacy | ./ft_ssl des-ecb -d -k "$DES_KEY"

echo "smol" | ./ft_ssl des-ecb -k "$DES_KEY" | openssl des-ecb -d -K "$DES_KEY" -provider=legacy
echo "smol" | openssl des-ecb -K "$DES_KEY" -provider=legacy | ./ft_ssl des-ecb -d -k "$DES_KEY"

echo "testing the key now" | openssl des-ecb -K "ABCD" -provider=legacy | ./ft_ssl des-ecb -d -k "ABCD"
echo "More key tests" | openssl des-ecb -K "FFFFFFFFFFFFFFFF" -provider=legacy | ./ft_ssl des-ecb -d -k "FFFFFFFFFFFFFFFF"
echo "what kind of lock takes no key?" | openssl des-ecb -K "00000000" -provider=legacy | ./ft_ssl des-ecb -d -k "00000000"


# DES simple flags!
## No, you can think of your own testcases here...


# YOU SHALL NOT PASS(word)!
echo "$(curl 'https://www.peereboom.us/assl/assl/html/openssl.html') (https://www.peereboom.us/assl/assl/html/openssl.html'))" > original.html
echo "password" > password_file # very secure
openssl des-ecb -p -in original.html -out ciphertext.html -pass "pass::$(cat password_file)" -provider=legacy
# ./ft_ssl des-ecb -d -i ciphertext.html -o decrypted.html -p "$(cat password_file)" -s (Copy the salt used by OpenSSL here!)


# Are they on the Cipher BlockChain? (Fuck Blockchain btw)

echo "Make sure they really tell you what is going on 'under the hood' when you use CBC mode, and how it is more secure than ECB" \
  | openssl des-cbc -K "BABE101010FACADE" -iv "77696E6B66616365" -provider=legacy | \
  ./ft_ssl des-cbc -k "BABE101010FACADE" -v "77696E6B66616365" -d

echo "bomb" | openssl des-ecb -p -pass "pass::password" -S D09F26526B82A403 -a
# *** WARNING : deprecated key derivation used.
# Using -iter or -pbkdf2 would be better.
# salt=D09F26526B82A403
# key=C6D957E0078885B0
# U2FsdGVkX1/QnyZSa4KkA9HIhmy6Gke4
