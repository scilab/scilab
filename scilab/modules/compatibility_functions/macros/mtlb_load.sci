// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function mtlb_load(thefile,opt)
//loads matlab 4.x  binary (.mat) or ascii files
//
warning(msprintf(gettext("This function is obsolete, use %s instead."),"loadmatfile"));

l_flags=['dl','fl','ll','sl','uls','uc']
b_flags=['db','fb','lb','sb','ubs','uc']
deff('Error(msg)',['mclose(fd)' ;'error(msg)'])
[lhs,rhs]=argn(0)
// look for file type
if rhs==2 then
  if convstr(opt)=='-ascii' then
    bin=%f
  else
    error(msprintf(gettext("%s: Unknown option."),opt));
  end
else
  k=strindex(thefile,'.')
  if k==[] then  //no extension given
    thefile=thefile+'.mat'
    bin=%t
  else
    if part(thefile,k($):length(thefile))=='.mat' then 
      bin=%t,
    else
      bin=%f
    end
  end
end

if bin then
  [fd,err]=mopen(thefile,'rb',0)
  if err<>0 then error(msprintf(gettext("File %s cannot be opened for reading."),thefile)),end

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
	Error(gettext("Incorrect file."))
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
      Error(gettext("VAX D-float not handled."))
    case 3
      Error(gettext("VAX G-float not handled."))
    case 4
      Error(gettext("Cray encoding not handled."))
    else
      Error(gettext("Unknown binary number format."))
    end
    t=mget(4,fl,fd);
    if meof(fd)<>0 then Error(gettext("Incorrect file.")),end
    m=t(1);n=t(2);it=t(3),namelen=t(4)
    name=mget(namelen,"c",fd);
    if meof(fd)<>0 then Error(gettext("Incorrect file.")),end
    name=ascii(name(1:namelen-1))
    names=[names name]
    
    
    if MOPT(4)==0 then  // regular matrix
      v=mget((it+1)*m*n,flag,fd);
      if meof(fd)<>0 then Error(gettext("Incorrect file.")),end
      if it==0 then
	mat=matrix(v,m,n);
      elseif it==1
	mat=matrix(v(1:m*n),m,n)+%i*matrix(v(m*n+1:$),m,n)
      end
    elseif MOPT(4)==1 // vector of strings
      v=mget(m*n,flag,fd);
      if meof(fd)<>0 then Error(gettext("Incorrect file.")),end
      mat=matrix(v(1:m*n),m,n);
      w=mat;
      mat=[];
      for k=1:m
	mat=[mat;ascii(w(k,:))]
      end
    elseif MOPT(4)==2 //sparse matrix
      //sparse
      Nnz=m-1;
      it=n-3;if it<>0&it<>1 then Error(gettext("Unknown sparse type.")),end
      ir=mget(Nnz,flag,fd);m=mget(1,"d",fd);
      jc=mget(Nnz,flag,fd);n=mget(1,"d",fd);
      v=mget(Nnz,flag,fd);junk=mget(1,"d",fd);
      if meof(fd)<>0 then Error(gettext("Incorrect file.")),end
      if it==1 then
	//complex
	v=v+%i*mget(Nnz,flag,fd);
      end
      mat=sparse([ir;jc]',v,[m n]);
    end
    vars($+1)=mat
  end
  mclose(fd);
  //form execstr instruction to resume variables in the calling environment
  execstr('['+strcat(names,',')+']=resume(vars(:))')
else
  ke=strindex(thefile,'.')
  if ke==[] then ke=length(thefile),else ke=ke($)-1,end
  kp=strindex(thefile,['/','\'])
  if kp==[] then kp=1,else kp=kp($)+1,end
  name=part(thefile,kp:ke)
  mat=evstr(mgetl(thefile))
  execstr(name+'= resume(mat)')
end
endfunction
