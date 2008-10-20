function [y]=lgfft(x)
// Copyright INRIA
[o,i]=argn(0);
   if i < 1 then error(58); end;
   if type(x) <> 1 then error(53,1); end;
//
   z=testvec(x,'l');
   nz=maxi(size(z));
   pnz=int(log(nz)/log(2));
   rz=nz-2**pnz;
   if rz == 0 then y=nz; else y=2**(pnz+1); end;
endfunction
