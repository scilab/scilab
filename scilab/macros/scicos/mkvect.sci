function out=mkvect(in)
// Computes pointer vector with the number of elements as input.
// Ex: [1 2 3] gives [1 2 4 7] as output
// Copyright INRIA
out=rtitr(poly(1,'z','c'),poly(1,'z'),in)+1
endfunction
