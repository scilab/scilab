function [lb,nwrk]=newlab(nwrk)
//Cette macro retourne lb  le numero d'etiquette suivant
//!
// Copyright INRIA
lb=nwrk(9)+1
  nwrk(9)=lb
endfunction
