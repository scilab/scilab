function f=fields(t)
  if or(type(t)==[16,17]) then
    f=getfield(1,t);f=matrix(f(2:$),-1,1)
  else
    f=[]
  end
endfunction

