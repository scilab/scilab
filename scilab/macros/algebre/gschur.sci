function [As,Es,Z,dim]= gschur(A,E,extern)

if argn(2)<=2 then
  warning('Obsolete function gschur. Please, replace gschur by schur')
  if argn(1)==2 then
    [As,Es]= schur(A,E)
  else
    [As,Es,Z,dim]= schur(A,E) 
  end
elseif type(extern)==10 then
  if convstr(extern)=='c'|convstr(extern)=='d' then
    warning('Obsolete function gschur. Please, replace gschur by schur')
    [As,Es,Z,dim]= schur(A,E,extern) 
  else // hard coded
    error('Obsolete function gschur, the old external cannot be used')
    //impossible to redefine
  end
else //coded by a scilab function
  //---- old------------------
  //flag=extern(x) 
  //x(1) ==1 ==> x(2:3)=[al,be]
  //x(1) ==2 ==> x(2:3)=[s,p]
  //---- new ------------------
  //R =[real(al),imag(al),be]
  warning('Obsolete function. Please, replace gschur by schur and adapt"+...
	  " the external')
  deff('t=%_rule(R)',['if R(2)==0 then'
		    '  t=extern([1,R(1),R(3)])==1'
		    'else		    '
		    '  c=(R(1)+%i*R(2))/R(3)'
		    '  t=extern([2,real(c+c''),real(c*c'')])==1'
                    'end;'])
  [As,Es,Z,dim]= schur(A,E,%_rule) 
end

