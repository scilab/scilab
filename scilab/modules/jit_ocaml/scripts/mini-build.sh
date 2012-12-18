#!/bin/sh

rm -f scilab-bin scilab-cli-bin
make -C modules/jit_ocaml && \
make -C modules/jit_ocaml install && \
  make scilab-bin scilab-cli-bin install-binPROGRAMS

