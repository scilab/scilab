function g=getgeom()
// call within a simulation function of a scicos block this function 
//returns a vector g containing [win x,y,w,h] of the block. win is either 
//the number of main scicos window or -1 if the block is not in the main
//window.



path=%cpr.corinv(curblock())

g=[-1;zeros(4,1)],
direc=path(1:$-1)
if direc==[]&or(curwin==winsid()) then
  g(1)=curwin
else
  dd=inactive_windows(1)
  for i=1:size(dd)
    if isequal(dd(i),direc) then 
       g(1)=inactive_windows(2)(i),
       if ~or(g(1)==winsid()) then g(1)=-1,end
       break,
    end
  end
end
if g(1)>-1 then
  Path=list('objs');
  for pk=direc
    Path=lstcat(Path,pk,'model','rpar','objs');
  end
  Path=lstcat(Path,path($))
  o=scs_m(Path)
  orig=o.graphics.orig;sz=o.graphics.sz
  g(2:5)=[orig(:);sz(:)]
end
endfunction

