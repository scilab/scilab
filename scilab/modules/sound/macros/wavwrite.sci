//----------------------------------------------------------
// INRIA 2005
// CORNET Allan
//----------------------------------------------------------
function []=wavwrite(y,Fs,nbits,wavefile)
  lhs=argn(1);
  rhs=argn(2);
  
  Fs_pref = 22050;
  nbits_pref = 16;
   
  if rhs<2|rhs>4 then
    error('Incorrect number of input arguments.');
  elseif rhs<3 then
     wavefile = Fs;
     Fs = Fs_pref;
     nbits = nbits_pref;
  elseif rhs<4 then
     wavefile = nbits;
     nbits = nbits_pref;
  end
  
  if (nbits<>16) then
    error('only 16 bits for the encoding.');
  end
  
  savewave(wavefile,y,Fs);
  
endfunction
//----------------------------------------------------------
