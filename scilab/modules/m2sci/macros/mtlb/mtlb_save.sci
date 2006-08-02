function mtlb_save(mtlb_thefile,varargin)
//save variable under  matlab 4.x .mat binary format files
//see: www.mathworks.com/access/helpdesk/help/pdf_doc/matlab/matfile_format.pdf 

warning("This function is obsolete, use savematfile instead");

// Verify that all inputs are character strings
for k=1:size(varargin)
  if type(varargin(k))<>10 then
    error("All inputs must be character strings");
  end
end

[lhs,rhs]=argn(0)
mtlb_opts=[]
if rhs==1 then
  mtlb_names=who('get')
  mtlb_names(1:6)=[] // clear functions variables
  mtlb_names($-predef()+1:$)=[] // clear predefined variables
  funcprot(0)
  for k=size(mtlb_names,'*'):-1:1
    execstr('x='+mtlb_names(k))
    select type(x)
    case 1 then
    case 4 then
    case 5 then
    case 6 then
    case 10 then
    else
      mtlb_names(k)=[]
    end
  end
else
  for k=size(varargin):-1:1
    vk=varargin(k)
    if part(vk,1)=='-' then 
      mtlb_opts=[convstr(vk),mtlb_opts],
    else
      kk=k
      break
    end
  end
  mtlb_names=[]
  for k=1:kk
    mtlb_names=[mtlb_names, varargin(k)]
  end
end

k=strindex(mtlb_thefile,'.')
if k==[] then  //no extension given
  if find(mtlb_opts=='-ascii')==[] then
    mtlb_thefile=mtlb_thefile+'.mat'
  end
end


if mtlb_names==[] then return,end
if mtlb_opts==[] then //binary save
  [mtlb_fd,err]=mopen(mtlb_thefile,'wb',0)
  // check name conflicts
  for k=['mtlb_thefile','varargin','mtlb_names','mtlb_opts','mtlb_fd']
    if or(mtlb_names==k) then
      error('Name conflict: it is not possible to save variable with name '+k)
    end
  end
  // clear variable wich are no more used to avoid name conflicts
  clear('rhs','lhs','kk','k','err','mtlb_thefile');

  for mtlb_k=1:size(mtlb_names,'*')
    // perform changes on variables
    execstr('x='+mtlb_names(mtlb_k))
    it=0
    select type(x)
    case 1 then
      P=0
      T=0
      if norm(imag(x),1)<>0 then it=1,end
    case 4 then
      x=bool2s(x)
      P=5
      T=0
    case 5 then
      if norm(imag(x),1)<>0 then it1=1,else it1=0,end
      P=0
      T=2
      [x,v,mn]=spget(x);
      if it1==0 then
	x=[x real(v);[mn 0]]
      else
	x=[x real(v) imag(v);[mn 0 0]]
      end
    case 6 then
      x=bool2s(x)
      P=0
      T=2
      [x,v,mn]=spget(x);
      x=[x v;[mn 0]]
    case 8 then
      T=0
      select inttype(x)
	case 4 then P=2,
	case 14 then P=2,
	case 2 then P=3
	case 12 then P=4
	case 1 then P=5,
	case 11 then P=5,
      end
      x=double(x)
    case 10 then
      x1=part(x(:),1:max(length(x)))
      x=[]
      for l=1:size(x1,1)
	x=[x;ascii(x1(l))]
      end
      P=5
      T=1
    else
      error('Attempt to write an unsupported data type to an ASCII file')
    end
    [m,n]=size(x)
  
      
    M = 0 //little endian
    O = 0
    MOPT=[M O P T]
    
    [m,n]=size(x)
    head=[MOPT*[1000;100;10;1] m,n,it,length(mtlb_names(mtlb_k))+1]

    head=mput(head,'uil',mtlb_fd);
    mput([ascii(mtlb_names(mtlb_k)) 0],"c",mtlb_fd);
    select P
    case 0 then
      flag='dl'
    case 1 then
      flag='fl'
    case 2 then
      flag='il'
    case 3 then
      flag='sl'
    case 4 then
      flag='usl'
    case 5 then
      flag='uc'
    end
    if T==0 then
      if x<>[] then
	mput(real(x(:).'),flag,mtlb_fd);
	if it==1
	  mput(imag(x(:).'),flag,mtlb_fd);
	end
      end
    elseif T==1
      v=mput(x(:).',flag,mtlb_fd);
    elseif T==2 then  //sparse
      mput(x(:).',flag,mtlb_fd);
    end
  end
  mclose(mtlb_fd);
else //ascii save

  if convstr(mtlb_opts(1))<>'-ascii' then 
    error('Uknown or misplaced option '+mtlb_opts(1))
  end
    if size(mtlb_opts,'*')==3 then
    sep=str2code(-40)
  else
    sep=' '
  end
  if size(mtlb_opts,'*')==1 then //8 digits save
    mtlb_fmt='(1pe14.7'+sep+')'
  else
    mtlb_fmt='(1pe23.15'+sep+')'
  end

  mtlb_fd=file('open',mtlb_thefile,'unknown')
  // clear variable wich are no more used to avoid name conflicts
  for k=['mtlb_thefile','varargin','mtlb_names','mtlb_fmt','mtlb_fd']
    if or(mtlb_names==k) then
      error('Name conflict: it is not possible to save variable with name '+k)
    end
  end
  clear('rhs','lhs','kk','err','sep');
  for mtlb_k=1:size(mtlb_names,'*')
    // perform changes on variables
    execstr('x='+mtlb_names(mtlb_k))
    select type(x)
    case 1 then
      write(mtlb_fd,real(x),'('+string(size(x,2))+mtlb_fmt+')')
    case 4 then
      write(mtlb_fd,bool2s(x),'('+string(size(x,2))+mtlb_fmt+')')
    case 5 then
      [ij,x]=spget(real(x));x=[ij x];
      write(mtlb_fd,real(x),'(2f8.0,1x'+string(size(x,2))+mtlb_fmt+')')
    case 6 then
      [ij,x]=spget(bool2s(x));x=[ij x];
      write(mtlb_fd,real(x),'(2f8.0,1x'+string(size(x,2))+mtlb_fmt+')')
    case 10 then
      x=part(x(:),1:max(length(x)))
      x1=[]
      for l=1:size(x,1)
	x1=[x1;ascii(x(l))]
      end
      write(mtlb_fd,x1,'('+string(size(x1,2))+mtlb_fmt+')')
    end
  end
  file('close',mtlb_fd)
end


 
endfunction
