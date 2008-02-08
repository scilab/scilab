function files=ls(varargin)
// interface to ls
// Author Serge Steer, Copyright INRIA
// update Allan CORNET (Windows)
// Update Sylvestre Ledru - Linux version is now based on listfiles
  opts=[];

 
  if size(varargin)==0 then
    path='./';
  else
    path=varargin(1);
    for k=2:size(varargin)
      opts=[opts varargin(k)];
    end
  end
    
  
  if size(opts,'*')<>0 then warning(msprintf(gettext("%s: Options ignored"),"ls")),end
  // dir returns names without the dirname 
  files=listfiles(path);
  
endfunction

