function [path, fname, extension,version]=fileparts(name,sel)
  path='';fname='';extension='';version='';
  name=stripblanks(name)
  if stripblanks(name)=='' then return,end

  name=pathconvert(name,%f,%f)

  if MSDOS then seps=['\',':'],else seps='/',end

  ind=strindex(name,seps)
  if ind==[] then
    fname=name;
  else
    if part(name,ind($))==':' then
      path=part(name,1:ind($))+seps(1)
    else
      path=part(name,1:ind($))
    end
    fname=part(name,ind($)+1:length(name))
  end

  ind=strindex(fname,'.');
  if ind<>[] then
    extension=part(fname,ind($):length(fname));
    fname=part(fname,1:ind($)-1)
  end
  
  if argn(2)==2 & argn(1)==1 then
    execstr('path='+sel)
  end
    
endfunction
