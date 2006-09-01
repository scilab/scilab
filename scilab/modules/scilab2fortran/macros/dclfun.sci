function [nwrk]=dclfun(nwrk,nam,typ)
//permet de declarer le type fortran du resultat d'une fonction
//!
// Copyright INRIA
select typ
case '0' then
  if find(nam==nwrk(14))==[] then  nwrk(14)=[nwrk(14);nam],end
case '1' then
  if find(nam==nwrk(15))==[] then  nwrk(15)=[nwrk(15);nam],end
end
endfunction
