function [As,Es,Z,dim]= gschur(A,E,extern)

  if argn(2)<=2 then
    warning('Obsolete function gschur. Please, replace gschur by schur')
    if argn(1)==2 then
      [As,Es]= schur(A,E)
    else
      [As,Es,Q,Z]= schur(A,E);Q=Q'
    end
  elseif type(extern)==10 then
    if convstr(extern)=='c'|convstr(extern)=='d' then
      warning('Obsolete function gschur. Please, replace gschur by schur')
      if argn(1)==4 then
	[As,Es,Z,dim]= schur(A,E,extern) 
      elseif argn(1)==2 then
	[As,Es,Z,dim]= schur(A,E,extern) 
	Es=dim;As=Z;
      end
    else // hard coded
      error('Obsolete function gschur, the old external cannot be used. See help')
      
      //impossible to redefine
    end
  else //coded by a scilab function
       //---- old------------------
       //flag=extern(x) 
       //x(1) ==1 ==> x(2:3)=[al,be]
       //x(1) ==2 ==> x(2:3)=[s,p]
       warning('Obsolete function. Please, replace gschur by schur and adapt"+...
	       " the external')
       deff('t=%_rule(Alpha,Beta)',['if imag(Alpha)==0 then'
		    '  t=extern([1,real(Alpha),Beta])==1'
		    'else		    '
		    '  c=Alpha/Beta'
		    '  t=extern([2,real(c+c''),real(c*c'')])==1'
                    'end;'])
       [As,Es,Z,dim]= schur(A,E,%_rule) 
  end
endfunction

