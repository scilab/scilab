// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2003-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function c1=bytecodewalk(code,query,job)
  //walk along bytecode looking for a particular opcode (query) and
  //applying a function (job) at this point
  c1=[];
  lc0=1;
  lc=lc0;
  long_walk=size(code,'*')
  while lc<=long_walk
    //mprintf("lc=%d,op=%d\n",lc,code(lc))
    if code(lc)==query then
      c1=[c1  code(lc0:lc-1)]
      [ctemp,lc]=job(lc)
      c1=[c1  ctemp]
      lc0=lc
    else
    select code(lc)
     case 0 then //deleted ops
      lc=lc+double(code(lc+1))
     case 1 then //stackp (obsolete)
      lc=lc+1+nsiz
     case 2 then //stackg
      lc=lc+nsiz+3
     case 3 then //string
      ni=double(code(lc+1))
      lc=lc+2+ni
     case 4 then//[]
      lc=lc+1 
     case 5 then //allops
      lc=lc+4
     case 6 then //num
      lc=lc+3
     case 7 then//for
      c1=[c1 code(lc0:lc-1)]
      ll=lc
      lc=lc+1
      long=double(code(lc))
      lc=lc+1
      cv=bytecodewalk(code(lc:lc+long-1))
      lc=lc+long
      long=double(code(lc))
      lc=lc+1
      id=code(lc:lc-1+nsiz)
      lc=lc+nsiz
      cc=bytecodewalk(code(lc:lc+long-1))
      c1=[c1 int32(7) int32(size(cv,'*')) cv int32(size(cc,'*')) id cc]
      lc=lc+long
      lc0=lc
    case 8 then //if
      if code(lc+1)==0 then //very old code
	error(msprintf(gettext("%s: obsolete op-code %d.\n"),"bytecodewalk",8))
      end
      c1=[c1 code(lc0:lc-1)]
      ll=lc
      long_tot=abs(double(code(lc+1)))
      nif=double(code(lc+2))
      ctemp=code(lc:lc+3)

      lc=lc+4
      for k=1:nif
	long_if_expr=double(code(lc)) 
	lc=lc+1
	c_if_expr=bytecodewalk(code(lc:lc+long_if_expr-1))
	lc=lc+long_if_expr
	long_if_expr=size(c_if_expr,'*')
	long_if_then=double(code(lc)) 
	lc=lc+1
	c_if_then=bytecodewalk(code(lc:lc+long_if_then-1))
	lc=lc+long_if_then
	long_if_then=size(c_if_then,'*')
	ctemp=[ctemp int32(long_if_expr) c_if_expr int32(long_if_then) ...
	       c_if_then]
      end
      long_else=double(code(lc)) 
      lc=lc+1
      c_else=bytecodewalk(code(lc:lc+long_else-1))
      lc=lc+long_else
      long_else=size(c_else,'*')
      ctemp =[ctemp int32(long_else) c_else]
      //padding ???
      if lc-ll<long_tot then ctemp=[ctemp code(lc:ll+long_tot-1)];lc=ll+long_tot;end
      lc0=lc
//      if size(ctemp,'*')<>long_tot then pause,end
      long_tot=size(ctemp,'*')
      ctemp(2)=-int32(long_tot)
      c1=[c1  ctemp]
    
    case 9 then //while
      if code(lc+1)==0 then //very old code
	error(msprintf(gettext("%s: obsolete op-code %d.\n"),"bytecodewalk",9))
      end
      c1=[c1 code(lc0:lc-1)]
      ll=lc
      long_tot=abs(double(code(lc+1)))
      lc=lc+4
      ctemp=int32([9 0 1 0])
      long_while_expr=double(code(lc)) 
      lc=lc+1
      c_while_expr=bytecodewalk(code(lc:lc+long_while_expr-1))
      lc=lc+long_while_expr
      long_while_expr=size(c_while_expr,'*')
      long_while_then=double(code(lc));llthen=lc
      lc=lc+1
      c_while_then=bytecodewalk(code(lc:lc+long_while_then-1));
      lc=lc+long_while_then;
      long_while_then=size(c_while_then,'*');
      ctemp=[ctemp int32(long_while_expr) c_while_expr int32(long_while_then) c_while_then];
      long_else=double(code(lc)) 
      lc=lc+1
      c_else=bytecodewalk(code(lc:lc+long_else-1))
      lc=lc+long_else
      long_else=size(c_else,'*')
      ctemp =[ctemp int32(long_else) c_else]
      if lc-ll<long_tot then ctemp=[ctemp code(lc:ll+long_tot-1)];lc=ll+long_tot;end
      lc0=lc
      long_tot=size(ctemp,'*')
      ctemp(2)=-int32(long_tot)
      c1=[c1  ctemp]
     case 10 then//select case
      c1=[c1 code(lc0:lc-1)]
      ll=lc
      long_tot=abs(double(code(lc+1)))
      ncase=double(code(lc+2))
      ctemp=code(lc:lc+2)
      lc=lc+3

      long_case_expr=double(code(lc)) 
      lc=lc+1
      c_case_expr=bytecodewalk(code(lc:lc+long_case_expr-1))
      lc=lc+long_case_expr
      long_case_expr=size(c_case_expr,'*')
      ctemp=[ctemp int32(long_case_expr) c_case_expr]
      for k=1:ncase
	long_case_expr=double(code(lc));
	lc=lc+1;
	c_case_expr=bytecodewalk(code(lc:lc+long_case_expr-1))
	lc=lc+long_case_expr;
	long_case_expr=size(c_case_expr,'*');
	long_case_then=double(code(lc)) 
	lc=lc+1
	c_case_then=bytecodewalk(code(lc:lc+long_case_then-1))
	lc=lc+long_case_then
	long_case_then=size(c_case_then,'*')
	ctemp=[ctemp int32(long_case_expr) c_case_expr int32(long_case_then) ...
	       c_case_then]
      end
      long_else=double(code(lc)) 
      lc=lc+1
      c_else=bytecodewalk(code(lc:lc+long_else-1))
      lc=lc+long_else
      long_else=size(c_else,'*')
      ctemp =[ctemp  int32(long_else) c_else]
      //padding ???
      if lc-ll<long_tot then ctemp=[ctemp code(lc:ll+long_tot-1)];lc=ll+long_tot;end

      lc0=lc
      //if size(ctemp,'*')<>long_tot then pause,end
      long_tot=size(ctemp,'*')
      ctemp(2)=int32(long_tot)
      c1=[c1  ctemp]
    case 11 then //try catch
      long_try=double(code(lc+1))
      long_catch=double(code(lc+2))
      lc=lc+3
      c_try=bytecodewalk(code(lc:lc+long_try-1))
      lc=lc+long_try
      long_try=size(c_try,'*')
  
      c_catch=bytecodewalk(code(lc:lc+long_catch-1))
      lc=lc+long_catch
      long_catch=size(c_catch,'*')
      c1=[c1 int32([11 long_try long_catch]) c_try c_catch]
      lc0=lc
    case 12 then //pause
      lc=lc+1
    case 13 then //break
      lc=lc+1
    case 14 then //abort
      lc=lc+1
    case 15 then //eol
      lc=lc+1
    case 16 then //line num
      lc=lc+2
    case 17 then //return
      lc=lc+1
    case 18 then //nmark named variable
      lc=lc+1+nsiz
    case 19 then  //Mkindx
      lc=lc+3
    case 20 then  
      lc=lc+1
    case 21 then //begrhs
      lc=lc+1
    case 22 then //printmode
      lc=lc+2
    case 23 then //name2var
      lc=lc+1+nsiz
    case 24 then //
      lc=lc+1
    case 25 then //profile
      lc=lc+3
    case 26 then //vector of string
      n=double(code(lc+1)*code(lc+2))
      nc=double(code(lc+4+n))-1
      lc=lc+5+n+nc
    case 27 then //varfunptr
      lc=lc+3+nsiz
    case 28 then //
      lc=lc+1
    case 29 then //assignment
      nlhs=double(code(lc+1))
      lc=lc+3+nlhs*(nsiz+1)
    case 30 then // skip logical evaluation
      lc=lc+3
    case 31 then //comment
      lc=lc+2+double(code(lc+1))
    case 99 then
      lc=lc+1
    else//Matfns
      lc=lc+4
    end
    end
  end
  c1=[c1 code(lc0:$)]
endfunction

