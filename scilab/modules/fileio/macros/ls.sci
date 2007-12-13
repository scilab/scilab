function files=ls(varargin)
// interface to ls
// Author Serge Steer, Copyright INRIA
// update Allan CORNET (Windows)

  opts=[];
  
  if size(varargin)==0 then
    path='./';
  else
    path=varargin(1);
    for k=2:size(varargin)
      opts=[opts varargin(k)];
    end
  end
    
  if MSDOS then
    if size(opts,'*')<>0 then warning('Options ignored'),end
    // dir returns names without the dirname 
    files=listfiles(path);
  else
    path = pathconvert(path,%f, %t); 
    //redefining  disp to avoid message when no file are found
    prot=funcprot();funcprot(0);deff('disp(txt)',' ');funcprot(prot)
    
    path=stripblanks(strsubst(stripblanks(path),' ','\ '));
    if and(opts<>'-b')&and(opts<>'--escape') then opts=[opts '-b'],end
    
    tab=%f;
    
    if and(opts<>'-1') then
      if and(opts<>'-C') then 
        opts=[opts '-C'];
      end
      ll   = lines();
      ll   = max(ll(1),20);
      opts = [opts '-w '+string(ll), '-T 0'];
      tab  = %t;
    end
    
    files=unix_g('ls '+strcat(opts,' ')+' '+path);
    
    if (files == "") | (files == []) then 
      files=[];
      return;
    elseif tab then 
      files=part(files,1:max(length(files)));
    end
  end
  
endfunction

