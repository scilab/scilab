function const=Cste(value)
// Copyright INRIA
// Create a new constant tlist

rhs=argn(2)

if rhs~=1 then
  error("Cste: wrong number of rhs arguments !")
end

dims=list()

if value=="" then
  dims=list(0,0)
else
  sz=size(value)
  for k=1:size(sz,"*")
    dims($+1)=sz(k)
    if type(value)==String then
      if k==2 then
	dims($)=sum(length(value))
      end
    end
  end
end

if type(value)==String then
  prop=Real
  value=value
else
  if type(value)==1 then
    if isreal(value) then
      prop=Real
    else
      prop=Complex
    end
  else
    prop=Real
  end
end

const=tlist(["cste","value","infer"],value,Infer(dims,Type(type(value),prop)))
endfunction
