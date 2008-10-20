function [Inn,X,Gbar]=colinout(G)
// Copyright INRIA
[Innt,Xt,Gbart]=rowinout(G');
Inn=Innt';X=Xt';Gbar=Gbart';
endfunction
