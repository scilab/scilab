function dim=firstnonsingleton(in,opt)
// Copyright INRIA
// opt="str" then return "r" or "c" if possible
// opt="num" then return numerical values
rhs=argn(2)
if rhs==1 then
  opt="str"
end

if opt=="num" then
  dim=1
else
  dim="r"
end
sz=size(in)
for k=1:size(sz,"*")
  if sz(k)==0 then
    if opt=="num" then
      dim=1
    else
      dim="r"
    end
    break
  elseif sz(k)>1 then
    if k==1 then
      if opt=="num" then
	dim=1
      else
	dim="r"
      end
    elseif k==2 then
      if opt=="num" then
	dim=2
      else
	dim="c"
      end
    else
      dim=sz(k)
    end
    break
  end
end

endfunction
