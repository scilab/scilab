function loadmatfile(varargin)
// Loads variables in a Matlab binary or ASCII file into Scilab
// This function has been developped following the 'MAT-File Format' description:
// www.mathworks.com/access/helpdesk/help/pdf_doc/matlab/matfile_format.pdf 
// Copyright INRIA
// Authors: SS, VC

// Verify that all inputs are character strings
for k=1:size(varargin)
  if type(varargin(k))<>10 then
    error("All inputs must be character strings");
  end
end

[lhs,rhs]=argn(0)

fil=[]
bin=[]
varnames=[]

if rhs==1 then // Compatibility with old loadmatfile version
  bin=%T
  fil=varargin(1)
else // Try to find type binary or ASCII ?
  // Filename is the first parameter: loadmatfile(filename[,opts])
  // or the second parameter: loadmatfile(filetype,filename[,opts]) with filetype equal to -ascii or -mat
  // filetype can also be included in opts
  k=1
  while k<=lstsize(varargin)
    select varargin(k)
    case "-mat"
      bin=%T
      k=k+1
    case "-ascii"
      bin=%F
      k=k+1
    case "-regexp"
      warning("Option regexp not implemented: IGNORED")
      while k<=lstsize(varargin) & and(varargin(k)<>["-mat","-ascii"])
	k=k+1
      end
    else 
      if isempty(fil) then // Filename
	fil=varargin(k)
	if fileparts(fil,"extension")==".mat" & isempty(bin) then // extension .mat and bin not already fixed by options
	  bin=%T
	elseif isempty(bin) then
	  bin=%F
	end
      else // Variable names
	varnames=[varnames;varargin(k)]
      end
      k=k+1
    end
  end
  if isempty(bin) then
    bin=%T
  end
end

if fileparts(fil,"extension")=="" then
  fil=fil+".mat"
end

// --- BINARY FILE ---
if bin then 
  ReadmiMatrix=ReadmiMatrix; // Loads the matfile functions (ReadmiMatrix.sci) 

  // Try to read file as a level 5 binary file
  // File opening
  fd=open_matfile(fil)
  // Reads the file header
  ierr=execstr("[head,version,endian]=matfile_header(fd)","errcatch")
  
  if ierr<>0 then
    errmsg=lasterror()
    // This line has to be mofified according to error message in 'matfile_header' function
    if stripblanks(errmsg)=="Invalid level 5 binary MAT-file!" then 
      // Level 4 binary file ?
      level=4;
    else
      // Wrong file
      error(errmsg);
    end
  else
    // Level 5 binary file
    level=5;
  end
  
  // --- LEVEL 5 BINARY FILE (This part already existed in old version) ---
  if level==5 then
    //--set constants
    exec(LoadMatConstants,-1);
    
    //--loop on the stored variables
    Name='';Names=[];Matrices=list()
    while %t 
      ierr=execstr('[Matrix,Name]=ReadmiMatrix(fd)','errcatch') //read next variable
      if ierr<>0 then mclose(fd),disp(lasterror()),return,end 
      if meof(fd) then  break,end //EOF reached 
      
      // Old version compatibility | Name has been given
      if isempty(varnames) | or(Name==varnames) then
	Names=[Names,Name];Matrices($+1)=Matrix
      end
      
    end
    //--file closing
    mclose(fd)
    
    //return variables in the calling context
    execstr('['+strcat(Names,',')+']=resume(Matrices(:))')
  
  // --- LEVEL 4 BINARY FILE (Copy/Paste from mtlb_load.sci) ---
  elseif level==4 then
    l_flags=['dl','fl','ll','sl','uls','uc']
    b_flags=['db','fb','lb','sb','ubs','uc']
    deff('Error(msg)',['mclose(fd)' ;'error(msg)'])
    [fd,err]=mopen(fil,'rb',0)
    if err<>0 then error('File '+fil+' cannot be opened for reading'),end
    
    vars=list() //list to store loaded variables
    names=[]  // vector of variables names
    
    while %t 
      offset=mtell(fd)
      mopt=mget(1,'uil',fd)
      if meof(fd)<>0 then break,end
      if mopt>5000 then
	mseek(offset,fd)
	mopt=mget(1,'uib',fd)
	
	if mopt>5000 then
	  Error('Incorrect file')
	end
      end
      MOPT=[]
      for k=1:4
	r=mopt-10*int(mopt/10);
	mopt=int((mopt-r)/10);
	MOPT=[r MOPT];
      end
      select MOPT(1)
      case 0
	fl='uil'
	flag=l_flags(MOPT(3)+1)
      case 1
	fl='uib'
	flag=b_flags(MOPT(3)+1)
      case 2
	Error('VAX D-float not handled')
      case 3
	Error('VAX G-float not handled')
      case 4
	Error('Cray encoding not handled')
      else
	Error('Unknown binary number format')
      end
      t=mget(4,fl,fd);
      if meof(fd)<>0 then Error('Incorrect file'),end
      m=t(1);n=t(2);it=t(3),namelen=t(4)
      name=mget(namelen,"c",fd);
      if meof(fd)<>0 then Error('Incorrect file'),end
      name=ascii(name(1:namelen-1))
      
      // Old version compatibility | Name has been given
      if isempty(varnames) | or(name==varnames) then
	names=[names name]
      end
      
      if MOPT(4)==0 then  // regular matrix
	v=mget((it+1)*m*n,flag,fd);
	if meof(fd)<>0 then Error('Incorrect file'),end
	if it==0 then
	  mat=matrix(v,m,n);
	elseif it==1
	  mat=matrix(v(1:m*n),m,n)+%i*matrix(v(m*n+1:$),m,n)
	end
      elseif MOPT(4)==1 // vector of strings
	v=mget(m*n,flag,fd);
	if meof(fd)<>0 then Error('Incorrect file'),end
	mat=matrix(v(1:m*n),m,n);
	w=mat;
	mat=[];
	for k=1:m
	  mat=[mat;ascii(w(k,:))]
	end
      elseif MOPT(4)==2 //sparse matrix
	//sparse
	Nnz=m-1;
	it=n-3;if it<>0&it<>1 then Error('Unknown sparse type'),end
	ir=mget(Nnz,flag,fd);m=mget(1,"d",fd);
	jc=mget(Nnz,flag,fd);n=mget(1,"d",fd);
	v=mget(Nnz,flag,fd);junk=mget(1,"d",fd);
	if meof(fd)<>0 then Error('Incorrect file'),end
	if it==1 then
	  //complex
	  v=v+%i*mget(Nnz,flag,fd);
	end
	mat=sparse([ir;jc]',v,[m n]);
      end
      // Old version compatibility | Name has been given
      if isempty(varnames) | or(name==varnames) then
	vars($+1)=mat
      end
    end
    mclose(fd);
    //form execstr instruction to resume variables in the calling environment
    execstr('['+strcat(names,',')+']=resume(vars(:))')  
  else
    error("Unknown Matlab binary file format");
  end
   
// --- ASCII FILE (Copy/Paste from mtlb_load.sci) ---
else
  ke=strindex(fil,'.')
  if ke==[] then ke=length(fil),else ke=ke($)-1,end
  kp=strindex(fil,['/','\'])
  if kp==[] then kp=1,else kp=kp($)+1,end
  name=part(fil,kp:ke)
  mat=evstr(mgetl(fil))
  execstr(name+'= resume(mat)')
end
endfunction

