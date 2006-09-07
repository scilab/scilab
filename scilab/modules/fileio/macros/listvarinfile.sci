// set of functions for getting information about variables saved in a Scilab 
//  data file
// Author: Serge Steer, 31 Jan 2001, reedited by Enrico Segre
function varargout=listvarinfile(fil)
  lhs=argn(1)
  u=mopen(fil,"rb")
  typenames=[
      "constant";
      "polynomial";
      ""
      "boolean";
      "sparse";
      "boolean sparse";
      "Matlab sparse";
      "integer"
      "graphic handles"; //not yet done
      "string";
      "function";
      " "
      "function";
      "library";
      "list"
      "tlist"
      "mlist"]


  nams=[]
  dims=list()
  typs=[];vols=[]
  
  if lhs==1 then
  //write the display header
    write(%io(2),part("Name",1:25)+ part("Type",1:15)+part("Size",1:16)+..
    	     part("Bytes",1:10))
    write(%io(2),"---------------------------------------------------------------")
  end
  
  while %t
    w=mget(6,"il",u)
    if size(w,"*")<6|meof(u)<>0 then break,end
    nam=code2var(w);

    [typ,dim,vol]=listnextvar(u)
    if typ<=17 then
      typn=typenames(typ)
    elseif typ==129 then
      typn="size implicit";
    end

    if lhs==1 then
      write(%io(2),part(nam,1:25)+part(typn,1:15)+..
    	   part(strcat(string(dim)," by "),1:16)+part(string(vol),1:10))
    end
    typs=[typs;typ]
    dims($+1)=dim
    nams=[nams;nam]
    vols=[vols;vol]
   end
  mclose(u)
  
  varargout=list(nams,typs,dims,vols)
  if lhs==1 then varargout=list([]); end
  
endfunction

/////////////////////////////////////////////////

function [typ,dim,vol]=listnextvar(u)
  typ=mget(1,"il",u) //The variable type
  select typ
  case 1 then //matrix of numbers
    w=mget(3,"il",u) //header of the variable 
    m=w(1);n=w(2);it=w(3);dim=[m,n]
    N=m*n*(it+1)
    mseek(8*N,u,"cur") //skip the values
    //A=mget(N,"dl",u)
    vol=4*4+8*N
  case 2 then //polynomial
    w=mget(3,"il",u) //header of the variable 
    m=w(1);n=w(2);it=w(3);dim=[m,n]
    mseek(4*4,u,"cur") //skeep the formal variable name
    w=mget(m*n+1,"il",u) //get the pointer table
    //I=mget(m*n+1,"il",u)
    N=(w(m*n+1)-1)*(it+1)
    mseek(8*N,u,"cur") //skip the values
    //A=mget(N,"dl",u)
    vol=(4+4+(m*n+1))*4+8*N
  case 4 then //boolean
    w=mget(2,"il",u) //header of the variable 
    m=w(1);n=w(2);dim=[m,n]
    N=m*n
    mseek(4*N,u,"cur") // skip the values
    //A=mget(N,"il",u)
    vol=(4+N)*4
  case 5 then //sparse
    w=mget(4,"il",u) //header of the variable 
    m=w(1);n=w(2);it=w(3);nel=w(4);dim=[m,n]
    mseek(4*(m+nel),u,"cur")// skip the index
    mseek(8*nel*(it+1),u,"cur") // skip the non zero values
    //I=mget(((m+nel),"il",u)
    //A=mget((nel*(it+1)),"dl",u)
    vol=(5+m+nel)*4+8*nel*(it+1)
  case 6 then //boolean sparse
    w=mget(4,"il",u) //header of the variable 
    m=w(1);n=w(2);it=w(3);nel=w(4);dim=[m,n]
    mseek(4*(m+nel),u,"cur")// skip the index
    //I=mget((m+nel),"il",u)
    vol=(5+m+nel)*4
  case 8 then //integer
    w=mget(3,"il",u) //header of the variable 
    m=w(1);n=w(2);it=w(3);dim=[m,n]
    select it
    case 4 then 
      mseek(m*n*4,u,"cur")
      //A=mget((m*n,"il",u)
      vol=4*4+m*n*4
    case 2 then 
      mseek(m*n*2,u,"cur")
      //A=mget((m*n,"sl",u)
      vol=4*4+m*n*2
    case 1 then 
      mseek(m*n*1,u,"cur")
      //A=mget((m*n,"c",u)
      vol=4*4+m*n
    case 14 then 
      mseek(m*n*4,u,"cur")
      //A=mget((m*n,"uil",u)
      vol=4*4+m*n*4
    case 12 then 
      mseek(m*n*2,u,"cur")
      //A=mget((m*n,"usl",u)
      vol=4*4+m*n*2
    case 11 then 
      mseek(m*n*1,u,"cur")
      //A=mget((m*n,"uc",u)
      vol=4*4+m*n
    end
  case 10 then //string
    w=mget(3,"il",u)
    m=w(1);n=w(2);it=w(3);dim=[m,n]
    w=mget(m*n+1,"il",u) //get the pointer table
    N=w($)-1 //total number of chars
    mseek(4*N,u,"cur") //skip the values, N characters coded in N integers
    vol=(4*m*n+1)*4+4*N
  case 11 then // function (not compiled)
    dim=[]
    nout=mget(1,"il",u)
    mseek(nout*6*4,u,"cur") //skip lhs names, nout names coded over 6 integers
    nin=mget(1,"il",u)
    mseek(nin*6*4,u,"cur") //skip rhs names, nin names coded over 6 integers
    N=mget(1,"il",u) //code length
    mseek(N*4,u,"cur") //skip code (N characters coded in N integers)
    vol=(3+(nin+nout)*6+N)*4
  case 13 then // compiled function 
    dim=[]
    nout=mget(1,"il",u)
    mseek(nout*6*4,u,"cur") //skip lhs names, nout names coded over 6 integers
    nin=mget(1,"il",u)
    mseek(nin*6*4,u,"cur") //skip rhs names, nin names coded over 6 integers
    N=mget(1,"il",u) //code length
    mseek(N*4,u,"cur") //skip pseudo-code N integers
    vol=(3+(nin+nout)*6+N)*4
  case 14 then //library
    np=mget(1,"il",u) //path length
    mseek(np*4,u,"cur") //skeep the path
    nclas=29
    nn=mget(1,"il",u) //number of names
    dim=nn
    mseek((nclas+1)*4,u,"cur") //skeep the class pointers
    mseek(nn*6*4,u,"cur") //skeep the names
    vol=(2+np+1+nclas+1+nn*6)*4
  case 15 then //list
    n=mget(1,"il",u);dim=n//number of fields
    mseek((n+1)*4,u,"cur") //skip pointers
    vol=(2+(n+1))*4
    for k=1:n
      [t,d,v]=listnextvar(u)
      vol=vol+v
    end
  case 16 then //tlist
    n=mget(1,"il",u);dim=n//number of fields
    mseek((n+1)*4,u,"cur") //skip pointers
    vol=(2+(n+1))*4
    for k=1:n
      [t,d,v]=listnextvar(u)
      vol=vol+v
    end
  case 17 then //mlist
    n=mget(1,"il",u);dim=n//number of fields
    mseek((n+1)*4,u,"cur") //skip pointers
    vol=(2+(n+1))*4
    for k=1:n
      [t,d,v]=listnextvar(u)
      vol=vol+v
    end
  case 129 then //size implicit index (same as polynomial)
    w=mget(3,"il",u) //header of the variable 
    m=w(1);n=w(2);it=w(3);dim=[m,n]
    mseek(4*4,u,"cur") //skeep the formal variable name
    w=mget(m*n+1,"il",u) //get the pointer table
    //I=mget(m*n+1,"il",u)
    N=(w(m*n+1)-1)*(it+1)
    mseek(8*N,u,"cur") //skip the values
    //A=mget(N,"dl",u)
    vol=(4+4+(m*n+1))*4+8*N
  else
    error("Unknown variable type, may be incorrect file type")
  end
endfunction

//////////////////////////////////////////////////////////

function v=code2var(id)
  nsiz=6
  i1=1
  for l=1:nsiz
    idl=id(l)
    for i=i1:i1+3
      k=int((idl+128)/256)
      if k<0 then k=k-1,end
      ch=idl-256*k
      if ch==40 then
        n=i-1
        v=code2str(str)
        return
      end
      idl=k
      str(i)=ch
    end
    i1=i1+4
    v=code2str(str)
  end
endfunction

