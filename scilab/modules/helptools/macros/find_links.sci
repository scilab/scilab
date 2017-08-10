// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// =============================================================================================
// find_links
//
// Private function !!!
// =============================================================================================

function flag = find_links(filein,fileout)

    // - return %t if a LINK were found
    // - Adapt the dtd link
    // - Adapt the date

    [lhs,rhs]=argn(0);

    flag=%f;

    if rhs<>2 then error(39), end

    sep=filesep();

    txt=mgetl(filein);

    //---------------------------------------------------------------------------------
    // Gestion de la DTD
    //---------------------------------------------------------------------------------

    dtd_line = grep(convstr(txt,"u"),"<!DOCTYPE MAN SYSTEM");

    if getos() == "Windows" then
        txt(dtd_line) = "<!DOCTYPE MAN SYSTEM ""file://"+SCI+"\modules\helptools\help.dtd"">";
    else
        txt(dtd_line) = "<!DOCTYPE MAN SYSTEM """+SCI+"/modules/helptools/help.dtd"">";
    end

    //---------------------------------------------------------------------------------
    // Gestion de la date (<DATE>$LastChangedDate: 2006-07-27 10:51:33 +0200 (jeu, 27 jui 2006) $</DATE>)
    //---------------------------------------------------------------------------------

    date_line =  grep(convstr(txt,"u"),"<DATE>");
    start_date = strindex(txt(date_line(1)),"$LastChangedDate");

    if start_date <> [] then
        start_date = start_date + 18;
        end_date = start_date + 10;
        txt(date_line) = "    <DATE>"+part(txt(date_line),start_date:end_date)+"</DATE>";
    end

    //---------------------------------------------------------------------------------
    // Gestion des liens
    //---------------------------------------------------------------------------------

    d=grep(txt,"<LINK>");

    if d==[] then mputl(txt,fileout); return; end

    for k=d
        tt=txt(k);
        l1=strindex(tt,"<LINK>");
        l2=strindex(tt,"</LINK>");
        nlink=size(l1,"*")
        for i=1:nlink
            name=part(tt,[l1(1)+6:l2(1)-1])
            path=get_absolute_file_path(filein);
            if length(name)<>0 then
                l=getlink(name,path,filein)
            else
                l="unknown";
            end
            // jpc 17 sept 2003
            l=strsubst(l,"//","/")
            tt=part(tt,[1:l1(1)-1])+"<A href="""+l+"""><VERB>"+name+"</VERB></A>"+part(tt,[l2(1)+7:length(tt)])
            l1=strindex(tt,"<LINK>")
            l2=strindex(tt,"</LINK>")
        end
        txt(k)=tt
    end

    mputl(txt,fileout);
    flag = %t;

endfunction


// =============================================================================================
// getlink
// =============================================================================================

function t=getlink(name,absolute_path,path)

    global %helps;
    global %helps_modules;
    if %helps_modules == [] then
        moduleslist = getmodules();
        for i = 1:size(moduleslist,"*")
            add_module_help_chapter(moduleslist(i));
        end
    end
    %HELPS=[%helps_modules;%helps];

    name=stripblanks(name)

    if getos() == "Windows" then
        sep="\";
    else
        sep="/";
    end

    man_found = [];

    //---------------------------------------------------------------------------------
    // On commmence par chercher dans le répertoire "name" ( cas le plus fréquent ).
    //---------------------------------------------------------------------------------

    if fileinfo(absolute_path+".list_htm") <> [] then

        whatis=mgetl(absolute_path+".list_htm");
        // 1er test ( avec le contenu de la balise title )
        f = grep(whatis,"- "+name+"==>");
        if f<>[] then
            for k1=f
                w = whatis(k1);
                w = strsubst(w,"- "+name+"==>","");
                man_found = absolute_path + w;
            end
        else
            // 2nd test ( avec le nom du fichier )
            f = grep(whatis,"==>"+name+".htm");
            if f<>[] then
                for k1=f
                    w = whatis(k1);
                    w = strsubst(w,"- "+name+"==>","");
                    man_found = absolute_path + name + ".htm";
                end
            end
        end

    end

    //---------------------------------------------------------------------------------
    // On recherche maintenant dans les répertoires désignés dans %helps
    //---------------------------------------------------------------------------------

    if man_found == [] then

        for k=1:size(%HELPS,1)

            current_help_path = %HELPS(k,1)+sep;

            if fileinfo(current_help_path+".list_htm") <> [] then

                whatis=mgetl(current_help_path+".list_htm");
                // 1er test ( avec le contenu de la balise title )
                f = grep(whatis,"- "+name+"==>");
                if f<>[] then
                    for k1=f
                        w = whatis(k1);
                        w = strsubst(w,"- "+name+"==>","");
                        man_found = current_help_path + w;
                    end
                else
                    // 2nd test ( avec le nom du fichier )
                    f = grep(whatis,"==>"+name+".htm");
                    if f<>[] then
                        for k1=f
                            w = whatis(k1);
                            w = strsubst(w,"- "+name+"==>","");
                            man_found = current_help_path + name + ".htm";
                        end
                    end
                end

                if man_found<>[] then break; end

            else

                if fileinfo(%HELPS(k,1)+sep+"whatis.htm") <> [] then
                    whatis = mgetl(%HELPS(k,1)+sep+"whatis.htm");
                    f      = grep(whatis,name);
                else
                    f      = [];
                end

                if f<>[] then
                    for k1=f
                        w=whatis(k1)
                        i=strindex(w,">"); j=strindex(w,"</A>")
                        if j<>[] then
                            lname=part(w,i(2)+1:j-1)
                            lnames=getwords(lname)
                            // transforms "toto titi tata" into ["toto" "titi" "tata"]
                            for ii=lnames
                                ok=%F
                                if ii==name then
                                    i=strindex(w,"HREF="""); j=strindex(w,""">")
                                    man_found=current_help_path+part(w,[i+6:j-1])
                                end
                                if man_found<>[] then break; end
                            end
                            if man_found<>[] then break; end
                        end
                    end
                end

                if man_found<>[] then break; end

            end // if fileinfo ....

        end // for k=1:size(%HELPS,1)

    end // if  man_found == []

    if man_found == [] then
        write(%io(2),"Bad LINK """+name+""" in file "+path);
        t=[];
        return;
    end

    t=getrelativefilename(absolute_path,man_found);
    t=strsubst(t,"\","/");

endfunction

// =============================================================================================
// getwords
// =============================================================================================

function vnames=getwords(names)
    v=strindex(names," ")
    if v==[] then
        vnames=[names]
    else
        vnames=[]; i=1
        for j=v
            vnames=[vnames,part(names,i:j-1)]
            i=j+1
        end
        vnames=[vnames,part(names,i:length(names))]
    end
endfunction
