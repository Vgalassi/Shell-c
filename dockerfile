FROM ubuntu:22.04

RUN apt-get update -y

RUN apt-get install -y gcc

RUN mkdir /Shell-c

WORKDIR /Shell-c

VOLUME [ "/Shell-c" ]
