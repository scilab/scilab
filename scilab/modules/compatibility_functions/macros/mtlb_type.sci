function []=mtlb_type(filename)
// Copyright INRIA
// Emulation function for type() Matlab function
// V.C.
[x,ierr]=fileinfo(filename)
if ierr==0 then // File exists
  disp(mgetl(mopen(filename,"r")))
else
  if type(evstr(filename))==13 then // Compiled function
    disp(fun2string(evstr(filename)))
  elseif strindex(filename,".")==[] then // File without extension
    Mfilename=filename+".m"
    [x,ierr]=fileinfo(Mfilename)
    if ierr==0 then // filename.m exists
      disp(mgetl(mopen(Mfilename,"r")))
    else
      SCIfilename=filename+".sci"
      [x,ierr]=fileinfo(SCIfilename)
      if ierr==0 then // filename.sci exists
	disp(mgetl(mopen(SCIfilename,"r")))
      else
	disp("Could not open neither "+filename+" nor "+Mfilename+" nor "+SCIfilename); 
      end
    end
  end
end
endfunction
