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

  //are we in scilab4 or scilab5?
  if listfiles(SCI+"/modules/scipad/")<>[] then scilab5=%t; else scilab5=%f; end

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

  if exists("scicos") then  // was once %scicos==%t -- which test is stabler?
    if scilab5 then
      scicosdir=SCI+"/modules/scicos/macros/";
    else 
      scicosdir=SCI+"/macros/";
    end
    //scicos basic functions: read the lib
    [l,s,b]=listvarinfile(scicosdir+"scicos/lib");
    load(scicosdir+"scicos/lib");
    n=string(eval(l)); scicosfun=(n(2:$));
    execstr("clear "+l);


    //scicos palettes: read each lib
    scicosblocks=[];
// if %scicos makes the next test (scilab5 only) redundant, not so?
//    if with_module('scicos') then
     subdirs=listfiles(scicosdir+"/scicos_blocks");
     for i=1:size(subdirs,"r")
       blocklib=scicosdir+"/scicos_blocks/"+subdirs(i)+"/lib";
       if fileinfo(blocklib)<>[] then
         [l,s,b]=listvarinfile(blocklib);
         load(blocklib);
         n=string(eval(l)); scicosblocks=[scicosblocks;(n(2:$))];
         execstr("clear "+l);
       end
     end
//   end


    setscipadwords([scicosfun;scicosblocks],"scicos")
  end

  //TCL_EvalStr("tk_messageBox -message $words(scilab.predef.%)","scipad")

endfunction

dynamickeywords()

clear dynamickeywords
