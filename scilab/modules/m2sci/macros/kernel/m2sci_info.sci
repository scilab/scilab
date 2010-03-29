// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function m2sci_info(txt,verb_mode)
// verb_mode=0: no information displayed
// verb_mode=1: information written as comment is resulting SCI-file
// verb_mode=2: information written as comment is resulting SCI-file and in logfile
// verb_mode=3: information written as comment is resulting SCI-file, in logfile and displayed in Scilab window
// verb_mode=-1: information written in logfile and displayed in Scilab window (should not be used by user) (TEST MODE)
// verb_mode=-2: information written in logfile and displayed in Scilab window (should not be used by user) and varslist displayed in Scilab command window (DEBUG MODE)



if verb_mode==0 then
  return
end

if verb_mode>0 then
  if exists("m2sci_to_insert_b")==0 then
    write(%io(2),gettext("loginfos: verb_mode 0 should not be used in this context, information ignored."));
    return
  end
  global("m2sci_to_insert_b")
  for k=1:size(txt,"*")
    m2sci_to_insert_b($+1)=Equal(..
	list(Variable("ans",Infer())),..
	Funcall("%comment",1,list(Cste(" "+txt(k))),list(Variable("",Infer()))))
    m2sci_to_insert_b($+1)=list("EOL");
  end
end

if verb_mode>1 | verb_mode==-1 then
  write(logfile,margin+txt); // margin is defined in mfile2sci()
end

if verb_mode>2 | verb_mode==-1 then
  mprintf("%s\n",strsubst(strsubst(margin+txt, "%i", "%%i"),"%n","%%n")); // margin is defined in mfile2sci()
end
endfunction
