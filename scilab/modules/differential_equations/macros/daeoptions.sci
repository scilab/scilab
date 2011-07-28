// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Sabine GAUZERE
// ...
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [%DAEOPTIONS] = daeoptions(%DAEOPTIONS)

//%DAEOPTIONS=list(tstop,imode,band,maxstep,stepin,nonneg,isest)
// This function displays the command line 
// %DAEOPTIONS=[...] to be executed 
// for defining the variable %DAEOPTIONS
// This variables sets a number of optional parameters 
// for the lsod* fortran routines.

  //options = [1,0,0,%inf,0,2,500,12,5,0,-1,-1];
  options = list ([],0,[],[],[],0,0);
  //default = [string(options(1:10)),sci2exp(options(11:12))]
  default = ["[]","0","[]","[]","[]","0","0"];
  //default(find(default=='Inf'))="%inf"

  if argn(2)>0 then
    options=%DAEOPTIONS
  end

  //lab_=[string(options(1:10)),sci2exp(options(11:12))]
  lab_= ["[]","0","[]","[]","[]","0","0"];
  //lab_(find(lab_=="[]"))="[]"


chapeau=["Defining %DAEOPTIONS variable";
         "*****************************";
	 'Meaning of tstop:';
	 'tstop is a real scalar which gives the maximum time'; 
         'for which g is allowed to be evaluated';
	 '[] : if no limits imposed for time';
	 ' ';
	 'Meaning of imode:';
	 '0 : if only the user specified time point values';
	 '1 : if dae returns its intermediate computed values';
	 ' ';
	 'Meaning of band:';
	 ' ';
         'Meaning of maxstep:';
	 ' ';
         'Meaning of stepin:';
	 ' ';
         'Meaning of nonneg:';
         '0 : if the solution is known to be negative';
         '1 : if the solution is known to be non negative';
	 ' ';
         'Meaning of isest:';
         '0 : if g(t0,y0,ydot0)=0';
         '1 : if ydot0 is just an estimation';
	 ' '
	 'Default values are given in square brackets'
	 'If the function is called without argument, default values"+...
	 " are used"
	]

dims = list("vec",-1,"vec",1,"vec",-1,"vec",-1,"vec",-1,..
	    "vec",1,"vec",1);


labels = ["tstop (maximum time) ","imode (assumes imode = 0 or 1)",...
          "band ()",...
          "maxstep (max step size)","stepin (initial step size)",...
          "nonneg (assumes nonneg = 0 or 1)",...
          "isest (assumes isest = 0 or 1)"] +'    ['+default+']';

[ok,tstop,imode,band,maxstep,stepin,nonneg,isest] = getvalue(chapeau,labels,dims,lab_);
//ml = mlmu(1);
//mu = mlmu(2);

DAEOPTIONS = list(tstop,imode,band,maxstep,stepin,nonneg,isest);

if DAEOPTIONS<>list() then
  %DAEOPTIONS=DAEOPTIONS
else
  %DAEOPTIONS=options
end

endfunction
