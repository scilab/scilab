function files=ls(varargin)
// interface to ls
// Author Serge Steer, Copyright INRIA
  opts=[]
  if size(varargin)==0 then
    path='./'
  else
    path=varargin($);
    for k=1:size(varargin)-1,opts=[opts varargin(k)],end
  end
  path = pathconvert(path,%f, %t); 

  //redefining  disp to avoid message when no file are found
  prot=funcprot();funcprot(0);deff('disp(txt)',' ');funcprot(prot)
  
  if MSDOS then
    if size(opts,'*')<>0 then warning('Options ignored'),end
    // dir returns names without the dirname 
    files=unix_g('dir /B /OD ""'+path+'""');
    if files == "" then files=[],end
    if files<>[] then 
      files=files($:-1:1)
      // prepend with the path, if required: when listing a full directory, path is not prepended
      if part(path,length(path))<>'\' then
	if isdir(path) then //yes
	  with_dir=%f
	else                //no
	  k=strindex(path,'\');
	  if k==[] then 
	    with_dir=%f
	  else
	    path=part(path,1:k($))
	    with_dir=%t
	  end
	end
      else
	with_dir=%f
      end
      if with_dir then files = path+files,end
    end
  else
    path=stripblanks(strsubst(stripblanks(path),' ','\ '))
    if and(opts<>'-b')&and(opts<>'--escape') then opts=[opts '-b'],end
    tab=%f
    if and(opts<>'-1') then
      if and(opts<>'-C') then opts=[opts '-C'],end
      ll=lines();ll=max(ll(1),20);
       opts=[opts '-w '+string(ll), '-T 0']
       tab=%t
    end
    files=unix_g('ls '+strcat(opts,' ')+' '+path);
    if files== "" then 
      files=[],
    elseif tab then 
      files=part(files,1:max(length(files))),
    end
  end
endfunction
