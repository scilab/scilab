function g=getgeom()
// call within a simulation function of a scicos block this function 
//returns a vector g containing [win x,y,w,h] of the block. win is either 
//the number of main scicos window or -1 if the block is not in the main
//window.

[%ljunk,%mac]=where()
slevel=prod(size(find(%mac=='scicos')))
path=%cpr(4)(curblock())

if exists('windows')<>1|slevel<>1|size(path,'*')<>1 then 
  g=[-1;zeros(4,1)],
  return,
end
orig=scs_m(path)(2)(1);sz=scs_m(path)(2)(2)
g=[windows(1,2);orig(:);sz(:)]

