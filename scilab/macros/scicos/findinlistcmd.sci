function [path]=findinlistcmd(L,v,oper,path)
// Copyright INRIA
//recherche si un element de valeur v existe dans la liste L
  global paths
  if and(type(L)<>(15:17)) then error('First argument should be a list'),end
  firstlevel=argn(2)<4
  if firstlevel then paths=list(),path=[];end
  for k=1:size(L)
    l=L(k)
    if or(type(l)==(15:17)) then
      findinlist(l,v,[path,k])
    else
      if oper=='=' then
	if isequal(l,v) then
	  paths($+1)=[path k]
	end
      elseif oper=='>' then
	if l > v then 
	  paths($+1)=[path k]
	end
      elseif oper=='<' then
	if l < v then 
	  paths($+1)=[path k]
	end
      else
      end
    end
  end
  if firstlevel then path=paths,clearglobal paths,end
endfunction
