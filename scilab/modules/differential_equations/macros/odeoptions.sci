function [%ODEOPTIONS]=odeoptions(%ODEOPTIONS)
// Copyright INRIA

  options=[1,0,0,%inf,0,2,500,12,5,0,-1,-1];
  default=[string(options(1:10)),sci2exp(options(11:12))]
  default(find(default=='Inf'))="%inf"
  if argn(2)>0 then
    options=%ODEOPTIONS
  end
  lab_=[string(options(1:10)),sci2exp(options(11:12))]
  lab_(find(lab_=='Inf'))="%inf"
//%ODEOPTIONS=[itask,tcrit,h0,hmax,hmin,jactyp,mxstep,..
//             maxordn,maxords,ixpr, ml,mu]
// This function displays the command line 
// %ODEOPTIONS=[...] to be executed 
// for defining the variable %ODEOPTIONS
// This variables sets a number of optional parameters 
// for the lsod* fortran routines.
load(SCI+'/macros/scicos/lib')
chapeau=["Defining %ODEOPTIONS variable";
         "*****************************";
	 'Meaning of itask and tcrit:';
	 '1 : normal computation at specified times';
	 '2 : computation at mesh points (given in first row of output of ode)';
	 '3 : one step at one internal mesh point and return';
	 '4 : normal computation without overshooting tcrit';
	 '5 : one step, without passing tcrit, and return';
	 ' ';
	 'Meaning of jactype:';
	 '0 : functional iterations (no jacobian used (''adams'' or ''stiff'' only))';
	 '1 : user-supplied full jacobian';
	 '2 : internally generated full jacobian';
	 '3 : internally generated diagonal jacobian (''adams'' or ''stiff'' only))';
	 '4 : user-supplied banded jacobian (see ml,mu)';
	 '5 : internally generated banded jacobian (see ml,mu)';
	 ' ';
	 'Meaning of ml,mu:';
	 'If jactype = 4 or 5 ml and mu are the lower and upper half-banwidths';
	 'of the banded jacobian: the band is the i,j''s with i-ml <= j <= ny-1';
	 'If jactype = 4 the jacobian function must return';
	 'a matrix J which is  ml+mu+1 x ny (where ny=dim of y in ydot=f(t,y));'
	 'such that column 1 of J is made of mu zeros followed by';
	 'df1/dy1, df2/dy1, df3/dy1,... (1+ml possibly non-zero entries)';
	 'column 2 is made of mu-1 zeros followed by df1/dx2, df2/dx2,etc';
	 ' '
	 'Default values are given in square brackets'
	 'If the function is called without argument, default values"+...
	 " are used"
	]

dims=list("vec",1,"vec",1,"vec",1,"vec",1,"vec",1,..
	  "vec",1,"vec",1,"vec",1,"vec",1,"vec",1,"vec",2);


labels=["itask (1,2,3,4,5) ","tcrit (assumes itask=4 or 5)",...
    "h0 (first step tried)",...
    "hmax (max step size)","hmin (min step size)",...
    "jactype (0,1,2,3,4,5)","mxstep (max number of steps allowed)",...
    "maxordn (maximum non-stiff order allowed, at most 12)",...
    "maxords(maximum stiff order allowed, at most 5) ",...
    "ixpr (print level 0 or 1)","[ml,mu]"] +'    ['+default+']';

[ok,itask,tcrit,h0,hmax,hmin,jactyp,mxstep,maxordn,maxords,ixpr,mlmu]=..
    tk_getvalue(chapeau,labels,dims,lab_);
ml=mlmu(1);mu=mlmu(2);
ODEOPTIONS=[itask,tcrit,h0,hmax,hmin,jactyp,mxstep,maxordn,maxords,ixpr, ...
	 ml,mu];
if ODEOPTIONS<>[] then
  %ODEOPTIONS=ODEOPTIONS
else
  %ODEOPTIONS=options
end
endfunction
