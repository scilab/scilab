function dynamickeywords()
// wrapped as function to have all variables local
  function cset=lineform(keywordlist)
     keywordlist=sort(keywordlist)
     initial=gsort(unique(part(keywordlist,1)),"r","i")
     cset=""
     for i=1:size(initial,1)
       cset(i)=strcat(keywordlist(part(keywordlist,1)==initial(i))," ")
     end
  endfunction


  function setscipadwords(wset,wtype)
    lp=lineform(wset);
    TCL_EvalStr("scipad eval {set chset(scilab."+wtype+") {}}")
    for i=1:size(lp,1)
      initial=part(lp(i),1);
      TCL_EvalStr("scipad eval {append chset(scilab."+wtype+") """+..
               initial+"""}")
      TCL_EvalStr("scipad eval {set words(scilab."+wtype+"."+initial+") """+..
               lp(i)+"""}")
    end
  endfunction

  //commands and primitives
  [primitives,commands]=what();
  setscipadwords(commands,"command")
  setscipadwords(primitives,"intfun")

  //predefined variables
  names=who("get"); p=names(($-predef())+1:$);
  setscipadwords(p,"predef")

  //library functions
  libfun=[]; libvar=[];
  for i=1:size(names,1)
    if type(eval(names(i)))==14 then
       libvar=[libvar;names(i)];
       libstring=string(eval(names(i)));
       libfun=[libfun;libstring(2:$)];
    end
  end
  
  setscipadwords(libfun,"libfun")

  //scicos basic functions: read the lib
  [l,s,b]=listvarinfile(SCI+"/macros/scicos/lib");
  load(SCI+"/macros/scicos/lib");
  n=string(eval(l)); scicosfun=(n(2:$));
  execstr("clear "+l);

  //scicos palettes: read each lib
  scicosblocks=[];
  subdirs=listfiles("SCI/macros/scicos_blocks");
  for i=1:size(subdirs,"r")
    blocklib="SCI/macros/scicos_blocks/"+subdirs(i)+"/lib";
    if fileinfo(blocklib)<>[] then
      [l,s,b]=listvarinfile(blocklib);
      load(blocklib);
      n=string(eval(l)); scicosblocks=[scicosblocks;(n(2:$))];
      execstr("clear "+l);
    end
  end

  setscipadwords([scicosfun;scicosblocks],"scicos")

  //TCL_EvalStr("scipad eval {tk_messageBox -message $words(scilab.predef.%)}")
  
endfunction

dynamickeywords()

clear dynamickeywords
