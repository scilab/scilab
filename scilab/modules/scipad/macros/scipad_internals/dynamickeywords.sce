function dynamickeywords()
// wrapped as function to have all variables local
  function cset=lineform(keywordlist)
     keywordlist=sort(keywordlist)
     initial=gsort(unique(part(keywordlist,1)),"r","i")
     cset=[]
     for i=1:size(initial,1)
         cset(i)=strcat(keywordlist(part(keywordlist,1)==initial(i))," ")
     end
  endfunction


  function setscipadwords(wset,wtype)
    lp=lineform(wset);
    TCL_EvalStr("set chset(scilab."+wtype+") {}","scipad")
    for i=1:size(lp,1)
      initial=part(lp(i),1);
      TCL_EvalStr("append chset(scilab."+wtype+") """+..
               initial+"""","scipad")
      TCL_EvalStr("set words(scilab."+wtype+"."+initial+") """+..
               lp(i)+"""","scipad")
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
       if or(libstring=="") then
         warning(" suspect empty function name found in "+names(i)+" (BUG #2338)")
         libstring=libstring(libstring<>"")
       end
       libfun=[libfun;libstring(2:$)];
    end
  end

  setscipadwords(libfun,"libfun")

  if %scicos then
    //scicos basic functions: read the lib
    [l,s,b]=listvarinfile(SCI+"/modules/scicos/macros/scicos/lib");
    load(SCI+"/modules/scicos/macros/scicos/lib");
    n=string(eval(l)); scicosfun=(n(2:$));
    execstr("clear "+l);


    //scicos palettes: read each lib
    scicosblocks=[];
    if with_module('scicos') then
     subdirs=listfiles("SCI/modules/scicos/macros/scicos_blocks");
     for i=1:size(subdirs,"r")
       blocklib="SCI/modules/scicos/macros/scicos_blocks/"+subdirs(i)+"/lib";
       if fileinfo(blocklib)<>[] then
         [l,s,b]=listvarinfile(blocklib);
         load(blocklib);
         n=string(eval(l)); scicosblocks=[scicosblocks;(n(2:$))];
         execstr("clear "+l);
       end
     end
   end


    setscipadwords([scicosfun;scicosblocks],"scicos")
  end

  //TCL_EvalStr("tk_messageBox -message $words(scilab.predef.%)","scipad")

endfunction

dynamickeywords()

clear dynamickeywords
