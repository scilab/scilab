function s=samwr(sizam,numsamp,X)
//
//This  function gives  a  s matrix  sizsam  x numsamp.   It
//contains  numsamp  random samples  (the  columns) each  of
//sizam  (<size(X,'*'))  extractions,  without  replacement,
//from the  vector X.
//
//author: carlos klimann
//
//date: 2003-09-11
  if argn(2)<>3 then 
    error('samwr requires  three input parameters.'), 
  end
  if X==[]|sizam==0|numsamp==0 then s=[]; return;end
  sizx=size(X,'*')
  if sizam>sizx then
    error('Value of first parameter must be less or equal than lenght of last parameter')
  end
  
  perm=grand(numsamp,'prm',(1:sizx)')
  s=matrix(X(perm),-1,numsamp)
  s=s(1:sizam,:)
endfunction
