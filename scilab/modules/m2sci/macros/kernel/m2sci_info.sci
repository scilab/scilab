// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
