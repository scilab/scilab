#****************************************************************************
#                          Maple to Scilab interface
#                        Version 1.1.2 (7 July 2004)
#                       Copyright (C) 1997-2004 INRIA 
#
#                                Claude Gomez				    
#			email: Claude.Gomez@inria.fr
#	       Web site: http://www-rocq.inria.fr/scilab/gomez
#									    
#				 SCILAB project				    
#									    
#                              INRIA-Rocquencourt
# 			      Domaine de Voluceau			    
# 			             BP 105			    
# 			     78153 Le Chesnay Cedex			    
# 				     FRANCE				    
#****************************************************************************

maple2scilab:=proc(fname::{name,string},expr,parameters,code::{name,string})
local ffile,k,lpath,maplematrix,ofile,para,release,sfile,soptimized;
global _ifunc,_old_labelling,_old_quiet,_old_screenwidth,_scicall,
       optimized,machine_include;
_old_quiet:=interface(quiet); interface(quiet=true);
_old_labelling:=interface(labelling); interface(labelling=false);
_old_screenwidth:=interface(screenwidth); interface(screenwidth=80);
# check arguments
if type(expr,scalar) then 
  maplematrix:=convert([[expr]],matrix);
elif type(expr,matrix) then
  maplematrix:=expr;
else
  maplematrix:=convert(expr,matrix);
fi;
# check parameters
if type(parameters,name) then
  para:=[parameters];
elif type(parameters,list) then
  para:=parameters;
else
  `maple2scilab/error`(`"parameters" argument must be a name or a list`)
fi;
for k in para do
  if type(eval(k),name) then next;
  elif type(eval(k),vector) or type(eval(k),matrix) then
    if entries(eval(k))<>NULL then
      `maple2scilab/error`(`matrix or vector parameter "`||k||
                           `" must not have entries`);
    fi
   else
      `maple2scilab/error`(`a parameter must be a name, a vector or a matrix`);
   fi;
od;
if code<>'f' and code<>'c' and code<>'s' then
  `maple2scilab/error`(`code must be 'f' (Fortran), 'c' (C) or 's' (Scilab)`);
fi;
lpath:=``;
if nargs=5 then lpath:=cat(args[5],`/`); fi;
_ifunc:=linalg[indexfunc](maplematrix);
if (_ifunc<>NULL and _ifunc<>'sparse') then
  `maple2scilab/error`(``||`matrix with index function "`||_ifunc||
                       `" not implemented`);
fi;
# look for Maple release number
release:=interface(version):
if SearchText(`Release 4`,release)<> 0 or
   SearchText(`Release 5`,release)<> 0 then
  ERROR(`Maple 6 or above required`):
fi:
if code='f' then
  if assigned(optimized) then
    soptimized:=optimized;
  fi;
  optimized:=soptimized;
  ffile:=cat(``||lpath,fname,`.f`);
  writeto(ffile);
  `maple2scilab/make_fortran`(fname,maplematrix,para);
  writeto(terminal);
elif code='c' then
  if not assigned(machine_include) then
    ERROR(`For generating C code, you must define "machine_include" variable`);
  fi;
  if assigned(optimized) then
    soptimized:=optimized;
  fi;
  optimized:=soptimized;
  ffile:=cat(``||lpath,fname,`.c`);
  writeto(ffile);
  `maple2scilab/make_c`(fname,maplematrix,para);
  writeto(terminal);
elif code='s' then
  ffile:=cat(``||lpath,fname,`.sci`);
  `maple2scilab/scilab`(fname,expr,ffile,para);
  `maple2scilab/lprint`(` `);
  `maple2scilab/lprint`(`Scilab file created: `||ffile);
  `maple2scilab/lprint`(` `);
  `maple2scilab/lprint`(`Usage in Scilab: exec('`||ffile||`');`);
  `maple2scilab/lprint`(`                 `||_scicall);
  interface(quiet=_old_quiet);
  interface(labelling=_old_labelling);
  interface(screenwidth=_old_screenwidth);
  RETURN();
fi;
# generate Scilab code
sfile:=cat(``||lpath,fname,`.sci`);
writeto(sfile);
`maple2scilab/make_scilab_interface`(fname,maplematrix,para,code);
writeto(terminal);
`maple2scilab/lprint`(` `);
if code='f' then
  `maple2scilab/lprint`(`Fortran file created (to be compiled): `||ffile);
elif code='c' then
  `maple2scilab/lprint`(`C file created (to be compiled): `||ffile);
fi;
ofile:=cat(``||lpath,fname,`.o`);
`maple2scilab/lprint`(`Scilab file created: `||sfile);
`maple2scilab/lprint`(` `);
`maple2scilab/lprint`(`Usage in Scilab: link('`||ofile||`','`||fname||`');`);
`maple2scilab/lprint`(`                 exec('`||sfile||`');`);
`maple2scilab/lprint`(`                 `||_scicall);
interface(quiet=_old_quiet);
interface(labelling=_old_labelling);
interface(screenwidth=_old_screenwidth);
RETURN();
end:

`maple2scilab/lprint` := proc(str) 
  printf(`%s\n`,str);
end:

`maple2scilab/error`:=proc(message)
interface(quiet=_old_quiet);
interface(labelling=_old_labelling);
interface(screenwidth=_old_screenwidth);
ERROR(message);
end:

# This procedure generates the Fortran subroutine
#   corresponding to the maple matrix "maplematrix".
# If the matrix is not sparse, the Fortran subroutine is
#   fname(x1,x2,...,xn,fmat); it computes fmat(i,j) as a function of
#   the arguments x1,x2,...,xn.
# If the matrix is sparse, the Fortran subroutine is
#   fname(x1,x2,...,xn,ij,v,mn); it computes v(i) as a function of
#   the arguments x1,x2,...,xn. ij and mn are used for creating the
#   Scilab sparse matrix.
# Each argument can be a maple scalar, vector or matrix which should be
#   in the list "arguments".

`maple2scilab/make_fortran`:=proc(fname,maplematrix,arguments)
global _ifunc,optimized;
local ent,flist,fmat,ij,ind,k,listarg,ll,mn,nent,soptimized,v;
listarg:=[]:
ll:=NULL;
if arguments<>[] then
  for k in arguments do
    if type(eval(k),name) then
      listarg:=[op(listarg),k];
    elif type(eval(k),vector) then
      listarg:=[op(listarg),k[linalg[vectdim](eval(k))]];
    elif type(eval(k),matrix) then
      listarg:=[op(listarg),
                k[linalg[rowdim](eval(k)),linalg[coldim](eval(k))]];
    fi;
  od;
  if listarg<>[] then ll:=['declaref','doubleprecision',listarg]; fi;
fi;
if _ifunc=NULL then
  flist:=['subroutinem',fname,[op(arguments),'fmat'],
           [['commentf',``],
            ['commentf',`Function generated by Maple to Scilab interface`],
            ['commentf',``],
            ['declaref',`implicit doubleprecision`,[`(a-h,o-z)`]],
            ll,
            ['declaref','dimension',
              [fmat[linalg[rowdim](maplematrix),linalg[coldim](maplematrix)]]],
            ['matrixm','fmat',maplematrix]]];
elif _ifunc='sparse' then
  ind:=matrix([indices(maplematrix)]); 
  ent:=vector(map(op,[entries(maplematrix)]));
  nent:=linalg[vectdim](ent);
  flist:=['subroutinem',fname,[op(arguments),'ij','v','mn'],
           [['commentf',``],
            ['commentf',`Function generated by Maple to Scilab interface`],
            ['commentf',``],
            ['declaref',`implicit doubleprecision`,[`(a-h,o-z)`]],
            ll,
            ['declaref','dimension',
             [ij[nent,2],v[nent],mn[2]]],
            ['equalf',mn[1],linalg[rowdim](maplematrix)],
            ['equalf',mn[2],linalg[coldim](maplematrix)],
            ['matrixm','ij',ind],
            ['matrixm','v',ent]]];
fi;
soptimized:=false;
if assigned(optimized) then
  soptimized:=optimized;
fi;
init_genfor();
optimized:=soptimized;
genfor(flist);
end:

# This procedure generates the C function
#   corresponding to the maple matrix "maplematrix".
# If the matrix is not sparse, the C function is
#   void fname(x1,x2,...,xn,fmat); it computes fmat(i,j) as a function of
#   the arguments x1,x2,...,xn.
# If the matrix is sparse, the C function is
#   void fname(x1,x2,...,xn,ij,v,mn); it computes v(i) as a function of
#   the arguments x1,x2,...,xn. ij and mn are used for creating the
#   Scilab sparse matrix.
# Each argument can be a maple scalar, vector or matrix which should be
#   in the list "arguments".

`maple2scilab/make_c`:=proc(fname,maplematrix,arguments)
global _ifunc,autodeclare,optimized,precision,machine_include;
local dcl,ent,flist,fmat,i,ij,ind,j,k,kk,listarg,m,mn,n,nent,
      soptimized,v;
listarg:=[]:
dcl:=[];
if arguments<>[] then
  for k in arguments do
    if type(eval(k),name) then 
      listarg:=[op(listarg),``||`*arg_`||k];
      dcl:=[op(dcl),['declareC','double',[``||k||`=*arg_`||k]]];
    elif type(eval(k),vector) then
      kk:=` `||k;
      listarg:=[op(listarg),kk[linalg[vectdim](eval(k))+1]];
    elif type(eval(k),matrix) then
      kk:=` `||k;
      listarg:=[op(listarg),kk[linalg[rowdim](eval(k))+1,
                                     linalg[coldim](eval(k))+1]];
    fi;
  od;
fi;
m:=linalg[rowdim](maplematrix); n:=linalg[coldim](maplematrix);
if _ifunc=NULL then
  flist:=[['commentC',` Function generated by Maple to Scilab interface `],[],
          ['includeC',` "`||machine_include||`" `],
          ['includeC',`<math.h>`],[],
          ['functionm','void',``||fname,
           [['double',[op(listarg),fmat[n+1,m+1]]]],
           [op(dcl),['matrixm','fmat',linalg[transpose](maplematrix)]]]];
elif _ifunc='sparse' then
  ind:=matrix([indices(maplematrix)]); 
  ent:=vector(map(op,[entries(maplematrix)]));
  nent:=linalg[vectdim](ent);
  flist:=[['commentC',` Function generated by Maple to Scilab interface `],[],
          ['includeC',` "`||machine_include||`" `],
          ['includeC',`<math.h>`],[],
          ['functionm','void',``||fname,
           [['double',[op(listarg)]],['int',[ij[3,nent+1]]],
            ['double',[`*v`]],['int',[`*mn`]]],
           [op(dcl),[equalC,mn[1],m], [equalC,mn[2],n],
            ['matrixm','ij',linalg[transpose](ind)],
            ['matrixm','v',ent]]]];
fi;
soptimized:=false;
if assigned(optimized) then
  soptimized:=optimized;
fi;
init_genC();
optimized:=soptimized;
autodeclare:='double';
precision:='double';
genC(flist);
end:

# This procedure generates the Scilab function "fname" which calls
#   the fortran or C subroutine "fname".

`maple2scilab/make_scilab_interface`:=proc(fname,maplematrix,arguments,code)
local a,chain,ff,first,k,kk,l1,l2,naargs,nent,rest,snd,ss:
global _scicall;
#
naargs:=nops(arguments);
ff:=`(`;
if arguments<>[] then
  ff:=cat(ff,arguments[1]);
  k:=2;
  while k<naargs+1 do
    ff:=cat(ff,`,`,arguments[k]); k:=k+1; 
  od;
fi;
ff:=cat(ff,`)`);
_scicall:=cat(`out=`,fname,ff);
ff:=cat(`function [out]=`,fname,ff);
l1:=0:
rest:=ff:
if length(ff)>70 then 
  while length(rest)>70 do
    first[l1]:=cat(substring(rest,1..70),`...`);
    l1:=l1+1;
    first[l1]:=substring(rest,71..length(rest));
    rest:=first[l1];
  od:
else
  first[0]:=ff;
fi:
#
if _ifunc=NULL then
  ss:=cat(`out=fort(`,`'`,fname,`',`);
elif _ifunc='sparse' then
  ss:=cat(`[ij,v,mn]=fort(`,`'`,fname,`',`);
fi;
k:=1;
if arguments<>[] then
  for kk in arguments do
    a:=kk;
    if type(eval(kk),matrix) and code='c' then a:=cat(a,`'`); fi;
    chain:=cat(a,`,`,k,`,`,`'d',`);
    ss:=cat(ss,chain);
    k:=k+1;
  od;
fi;
ss:=cat(ss,`'out',`);
if _ifunc=NULL then
  ss:=cat(ss,`[`,linalg[rowdim](maplematrix),`,`,
             linalg[coldim](maplematrix),`],`,naargs+1,`,'d')`);
elif _ifunc='sparse' then
  nent:=nops([entries(maplematrix)]);
  ss:=cat(ss,`[`,nent,`,`,2,`],`,naargs+1,`,'i',`,
             `[`,nent,`,1],`,naargs+2,`,'d',`,
             `[2,1],`,naargs+3,`,'i')`);
fi;
l2:=0;
rest:=ss;
if length(ss)>70 then 
  while length(rest)>70 do
    snd[l2]:=cat(substring(rest,1..70),`...`);
    l2:=l2+1;
    snd[l2]:=substring(rest,71..length(rest));
    rest:=snd[l2];
  od;
else
  snd[0]:=ss;
fi;
`maple2scilab/lprint`(first[0]);
`maple2scilab/lprint`(`// Function generated by Maple to Scilab interface`);
for k from 1 to l1 do `maple2scilab/lprint`(first[k]); od;
for k from 0 to l2 do `maple2scilab/lprint`(snd[k]); od;
if _ifunc='sparse' then
  `maple2scilab/lprint`(`out=sparse(ij,v,mn)`);
fi;
`maple2scilab/lprint`(endfunction);
end:

# This procedure generates directly the Scilab function
#   corresponding to the maple expression "e".
# The Scilab function is fname(x1,x2,...,xn).
# Each argument can be a maple scalar, vector or matrix which should be
#   in the list "arguments".

`maple2scilab/scilab` := proc(fname,e,file,arguments)
  local bres;
  bres := traperror(`scilab/scilab1`(fname,e,file,arguments));
  writeto(terminal);
  if bres = lasterror then print(`maple2scilab/scilab`,lasterror) fi; 
end:

`scilab/scilab1` := proc(fname,e,file,arguments)
  global _ifunc,_scicall,_scilab_buffer,_scilab_level;
  local ent,ind,k,naargs,str;
  writeto(file);
  _scilab_level := 0;
  if type(e,'procedure') and type(e,'name') then 
    ERROR(`unable to translate a procedure to Scilab`,e);
  else
    naargs:=nops(arguments);
    str:=`(`;
    if arguments<>[] then
      str:=cat(str,arguments[1]);
      k:=2;
      while k<naargs+1 do
        str:=cat(str,`,`,arguments[k]); k:=k+1; 
      od;
    fi;
    str:=cat(str,`)`);
    _scicall:=cat(`out=`,fname,str);
    str:=cat(`function [out]=`,fname,str);
    `scilab/lprint`(str);
    str := `// Function generated by Maple to Scilab interface`;
    `scilab/lprint`(str);
    if _ifunc=NULL then
       _scilab_buffer := `out=`;
      `scilab/expression`(e);
      `scilab/lprint`(_scilab_buffer);
    elif _ifunc='sparse' then
      ind:=matrix([indices(e)]); 
      ent:=vector(map(op,[entries(e)]));
      _scilab_buffer := `ij=`;
      `scilab/expression`(ind);
      `scilab/lprint`(_scilab_buffer);
      _scilab_buffer := `v=`;
      `scilab/expression`(ent);
      `scilab/lprint`(_scilab_buffer);
      str := cat(`out=sparse(ij,v,[`,linalg[rowdim](e),`,`,
                 linalg[coldim](e),`])`);
      `scilab/lprint`(str);
    fi;
    `scilab/lprint`("endfunction");
  fi;
end:

`scilab/space` := proc(n) 
  if n = 0 then `` else map(cat,` `$n) fi 
end:

`scilab/lprint` := proc(str)
  global _scilab_level;
  local stro;
  stro := cat(`scilab/space`(_scilab_level),str);
  `maple2scilab/lprint`(stro)
end:

`scilab/write` := proc(e) local d, l, maxl, stro;
  global _scilab_buffer,_scilab_level;
  maxl := 72;
  l := length(e);
  stro := cat(`scilab/space`(_scilab_level),_scilab_buffer);
  d := length(stro) + l - (maxl - 2);
  if d <= 0 then _scilab_buffer := cat(_scilab_buffer,e);
  else
    if l<>d and substring(e,l-d)=`.` then
      # problem if we cut at a point
      _scilab_buffer := cat(stro,substring(e,1..l-d-1),`..`);
      `maple2scilab/lprint`(_scilab_buffer);
      _scilab_buffer := substring(e,l-d..l);
    else
      _scilab_buffer := cat(stro,substring(e,1..l-d),`..`);
      `maple2scilab/lprint`(_scilab_buffer);
      _scilab_buffer := substring(e,l-d+1..l);
    fi
  fi
end:

`scilab/expression` := proc(e) local a;
  if type(e,'array') and type(e,name) then `scilab/array`(e)
  elif type(e,'array') then a:=e; `scilab/array`(a)
  elif type(e,'algebraic') then `scilab/alg`(e);
  elif type(e,'list') then `scilab/list`(e);
  else
    ERROR(`unable to translate to Scilab`,e);
  fi;
end:

`scilab/array` := proc(a) local i,j,ins,d,i1,i2,j1,j2;
  global _scilab_buffer;
  ins := op(2,op(a));
  d := nops([ins]);
  if (d = 1) then
    `scilab/write`(`[`); `scilab/lprint`(_scilab_buffer);
      for i from op(1,ins) to op(2,ins) do
        _scilab_buffer:=``;
        `scilab/alg`(a[i]); 
        `scilab/lprint`(``||_scilab_buffer||`;`);
      od;
    _scilab_buffer:=``;
    `scilab/lprint`(`]`);
  elif (d = 2) then
    `scilab/write`(`[[`); `scilab/lprint`(_scilab_buffer);
     j1 := op(1,op(2,[ins]));
     j2 := op(2,op(2,[ins]));
     i1 := op(1,op(1,[ins]));
     i2 := op(2,op(1,[ins]));
     for j from j1 to j2 do
       for i from i1 to i2 do
         _scilab_buffer:=``;
         `scilab/alg`(a[i,j]); 
         `scilab/lprint`(``||_scilab_buffer||`;`);
       od;
       if (j <> j2) then 
         _scilab_buffer:=``;
         `scilab/lprint`(`],[`) fi;
     od;
    _scilab_buffer:=``;
    `scilab/lprint`(`]]`);
  else
    ERROR(`unable to translate to Scilab`);
  fi    
end:

`scilab/alg` := proc(e) local k,p,s;
  if type(e,'float') then `scilab/write`(`scilab/float`(evalf(e)))
  elif type(e,'integer') then `scilab/write`(`scilab/integer`(e))
  elif (e = Pi) then `scilab/write`(`%pi`)
  elif (e = E) then `scilab/write`(`%e`)
  elif (e = I) then `scilab/write`(`%i`)

  elif type(e,`*`) then

	if op(1,e) + 1 = 0 then
		`scilab/write`(`-`);
		p := nops(e) = 2 and `scilab/precedence`(op(2,e)) <= 50;
		if p then `scilab/write`(`(`) fi;
		`scilab/alg`(subsop(1=1,e));
		if p then `scilab/write`(`)`) fi;
		RETURN()
	elif type(op(1,e),'fraction') and
		(op(1,op(1,e)) = 1 or op(1,op(1,e)) = -1) then
		if op(1,e) < 0 then `scilab/write`(`-`) fi;
		p := nops(e) = 2 and `scilab/precedence`(op(2,e)) <= 50;
		if p then `scilab/write`(`(`) fi;
		`scilab/alg`(subsop(1=1,e));
		if p then `scilab/write`(`)`) fi;
		`scilab/write`(`/`||(op(2,op(1,e))));
		RETURN()
	fi;

	p := `scilab/precedence`(e,1);
	if p then `scilab/write`(`(`) fi;
	`scilab/alg`(op(1,e));
	if p then `scilab/write`(`)`) fi;
	for k from 2 to nops(e) do
		if type(op(k,e),`^`) and type(op(2,op(k,e)),'numeric')
			and sign(op(2,op(k,e))) = -1 
		then s := op(1,op(k,e))^(-op(2,op(k,e))); 
                  `scilab/write`(`/`)
		else s := op(k,e); `scilab/write`(`*`)
		fi;
		p := `scilab/precedence`(s) < `scilab/precedence`(e);
		if p then `scilab/write`(`(`) fi;
		`scilab/alg`(s);
		if p then `scilab/write`(`)`) fi;
	od

  elif type(e,`^`) then

	if type(op(2,e),'rational') and op(2,e) < 0 then
		p := `scilab/precedence`(op(1,e));
		`scilab/write`(`1/`);
		if p < 70 then `scilab/write`(`(`) fi;
		`scilab/alg`(op(1,e)^(-op(2,e)));
		if p < 70 then `scilab/write`(`)`) fi;
	elif type(op(1,e),'indexed') and type(op(2,e),'integer') then
		`scilab/alg`(op(1,e));
		`scilab/write`(cat(`**`,op(2,e)));
	elif type(op(1,e),'name') and type(op(2,e),'integer') then
		`scilab/write`(cat(convert(op(1,e),string),`**`,op(2,e)) )
	elif type(op(2,e),'fraction') and op(2,op(2,e))=2 then
		s := op(1,e)^op(1,op(2,e));
		`scilab/alg`('sqrt'(s))
	else	p := `scilab/precedence`(e,1);
		if p then `scilab/write`(`(`) fi;
		`scilab/alg`(op(1,e));
		if p then `scilab/write`(`)`) fi;
		`scilab/write`(`**`);
		p := `scilab/precedence`(e,2);
		if p then `scilab/write`(`(`) fi;
		`scilab/alg`(op(2,e));
		if p then `scilab/write`(`)`) fi;
	fi

  elif type(e,`+`) then
	p := `scilab/precedence`(e,1);
	if p then `scilab/write`(`(`) fi;
	`scilab/alg`(op(1,e));
	if p then `scilab/write`(`)`) fi;
	for k from 2 to nops(e) do
		if not (type(op(k,e),'numeric') and op(k,e) < 0 or
			type(op(k,e),`*`) and type(op(1,op(k,e)),'numeric')
			and sign(op(1,op(k,e))) = -1)
		then `scilab/write`(`+`) fi;
		p := `scilab/precedence`(e,k);
		if p then `scilab/write`(`(`) fi;
		`scilab/alg`(op(k,e));
		if p then `scilab/write`(`)`) fi;
	od

  elif type(e,'function') then
  `scilab/function`(op(0,e),op(e))

  elif type(e,'indexed') then
    if (op(0,e) = `&args`) then
      `scilab/write`(_larg[op(1,op(1,e))])
    elif (op(0,e) = `&local`) then
      `scilab/write`(_lcal[op(1,op(1,e))])
    else
	`scilab/alg`(op(0,e));
	`scilab/write`(`(`);
        if type(op(1,e),'function') and (op(0,op(1,e)) = `&expseq`) then
          s := op(1,e);
        else s:= e; fi;
	`scilab/alg`(op(1,s));
	for k from 2 to nops(s) do
		`scilab/write`(`,`);
		`scilab/alg`(op(k,s))
	od;
	`scilab/write`(`)`)
    fi

  elif type(e,'name') then `scilab/write`(e)

  elif type(e,'fraction') then
	`scilab/alg`(evalf(op(1,e)));
	`scilab/write`('`/`');
	`scilab/alg`(evalf(op(2,e)));

  else ERROR(`unable to translate to Scilab`,e);
fi

end:

`scilab/function` := proc(f) local k;
  if (nargs > 1) and type(args[2],'function') and 
     (op(0,args[2]) = `&expseq`) then
    `scilab/function`(args[1],op(args[2]))
  elif f = `&ev` then `scilab/ev`(args[2])
  elif f = 'evalm' then `scilab/evalm`(args[2..nargs])
  elif f = 'add' then
	`scilab/write`(`(`);
       if nargs > 1 then
	  `scilab/alg`(args[2]);
	  for k from 3 to nargs do
		`scilab/write`(`+`);
		`scilab/alg`(args[k])
	  od;
        fi;
	`scilab/write`(`)`);
  elif f = 'multiply' then
	`scilab/write`(`(`);
       if nargs > 1 then
	  `scilab/alg`(args[2]);
	  for k from 3 to nargs do
		`scilab/write`(`*`);
		`scilab/alg`(args[k])
	  od;
        fi;
	`scilab/write`(`)`);
  elif f = 'inverse' then
	`scilab/write`(`(1/`);
       if nargs < 2 then ERROR(`inverse must have one argument`) fi;
	`scilab/alg`(args[2]);
	`scilab/write`(`)`);
  else	`scilab/alg`(
	  `scilab/function_name`(f));
	`scilab/write`(`(`);
        if nargs > 1 then
	  if type(args[2],'integer')
	  then `scilab/write`(`scilab/float`(evalf(args[2])))
	  else `scilab/alg`(args[2])
	  fi;
	  for k from 3 to nargs do
		`scilab/write`(`,`);
		if type(args[k],'integer')
		then `scilab/write`(`scilab/float`(evalf(args[k])))
		else `scilab/alg`(args[k])
		fi
	  od;
        fi;
	`scilab/write`(`)`);
        if (f = 'transpose') or (f = linalg['transpose']) then
          `scilab/write`(`'`) fi
  fi
end:

`scilab/evalm` := proc()
  ERROR(`unable to translate evalm to Scilab`);
end:

`scilab/list` := proc(e) local k,s;
  `scilab/write`(`list(`);
  if nops(e) > 0 then
    if type(op(1,e),'function') and (op(0,op(1,e)) = `&expseq`) then
      s := op(1,e);
    else s:= e; fi;
    `scilab/expression`(op(1,s));
    for k from 2 to nops(s) do
		`scilab/write`(`,`);
		`scilab/expression`(op(k,s))
    od;
  fi;
  `scilab/write`(`)`)
end:

`scilab/precedence` := proc(e,k)
option `Copyright 1989 by the University of Waterloo`;

if nargs = 1 then
	if type(e,'name') then 99
	elif type(e,'integer') and e > 0 then 99
	elif type(e,`*`) then 70
	elif type(e,`+`) then 50
	elif type(e,`^`) then 80
	elif type(e,'float') and e > 0 then 99
	elif type(e,'function') or type(e,'indexed') then 99
	elif type(e,'rational') then 70
	elif type(e,'integer') then 60
	elif type(e,'float') then 60
	elif type(e,`=`) or type(e,`<`) or type(e,`<=`) or type(e,`<>`) then 40
	elif type(e,`not`) then 30
	elif type(e,`and`) then 20
	elif type(e,`or`) then 10
	else 0
	fi
elif type(e,`*`) and k = 1 and type(op(1,e),'numeric') then false
elif type(e,`^`) and k = 1 and type(op(1,e),`^`) then true
else `scilab/precedence`(e) > `scilab/precedence`(op(k,e))
fi

end:

`scilab/float` := proc(f) local mantissa,exponent,letter,quotient,prefix;
	if f = 0
	then mantissa := 0; exponent := 0
	else mantissa := op(1,f); exponent := op(2,f) fi;
	if exponent = 0 and mantissa < 1000000 then
                  RETURN( cat(mantissa,`.0`) ) 
	fi;
        letter := 'D';
	if mantissa < 0 then
		prefix := `-0.`;
		mantissa := -mantissa
	else prefix := `0.`
	fi;
	while irem(mantissa,10,'quotient') = 0 do
		mantissa := quotient;
		exponent := exponent+1
	od;
	exponent := exponent+length(mantissa);
	cat(prefix,mantissa,letter,exponent)
end:

`scilab/integer` := proc(e)
  if length(e) > Digits then
    `m_fortran/float`(evalf(e))
  else ``||e
  fi
end:

`scilab/bool` := proc(c)
  if member(whattype(c),{`=`,`<`,`>`,`<=`,`>=`,`<>`}) then
    `scilab/alg`(op(1,c));
    `scilab/write`(whattype(c));
    `scilab/alg`(op(2,c));
  elif c = true then
    `scilab/write`(`1=1`)
  elif c = false then
    `scilab/write`(`1=0`)
  else ERROR(`unable to translate to Scilab`,c)
  fi
end:

`scilab/function_name` := proc(f) f end:

`scilab/function_name`('arctanh') := 'atanh':
`scilab/function_name`('arctan') := 'atan':
`scilab/function_name`('arcsin') := 'asin':
`scilab/function_name`('arcsinh') := 'asinh':
`scilab/function_name`('arccos') := 'acos':
`scilab/function_name`('arccosh') := 'acosh':
`scilab/function_name`('cot') := 'cotg':
`scilab/function_name`('ln') := 'log':
`scilab/function_name`('transpose') := ``:
