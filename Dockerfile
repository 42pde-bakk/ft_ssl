# Debian 64 bit system:
FROM debian:11
WORKDIR /code

RUN apt-get update && apt-get upgrade -y
RUN apt-get install -y gcc g++ make wget git curl man vim cmake clang nano emacs python3-venv zsh python3-pip
