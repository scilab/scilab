function path=scs_full_path(P)
// Copyright INRIA
// Author S. Steer. 
// Given a path in the superclock hierachy, this function returns a path
// in the scs_m data structure
// P is a vector of numbers. All but the last entries are Superblocks index 
// path is a list such as scs_m(path) is the required object
  
  path=list('objs');
  for l=P(1:$-1),path($+1)=l;path($+1)='model';path($+1)= 'rpar';path($+1)='objs';end
  path($+1)=P($);
endfunction
