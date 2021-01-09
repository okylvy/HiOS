FROM ubuntu:20.04

RUN apt-get update && apt-get install -y make libc6-i386 nasm qemu-system vim gcc

COPY ./ /hios

WORKDIR /hios
CMD ["/bin/bash"]
