function ok = xmlfiletohtml(path,xsl,directory_language,default_language)

    // =========================================================================================
    //
    // Authors : Jean-Philippe CHANCELIER, Pierre MARECHAL
    // Copyright INRIA/Enpc
    //
    // given a path on a Scilab help xml file (assumed to respect
    // SCI/helptools/help.dtd) this function generates the corresponding htm
    // file using the wanted xsl file
    //
    // Private function !!!
    // =========================================================================================

    // path    :     XML file
    // xsl     :     abolute path of the xsl path

    generate_cmd="sabcmd";

    [lhs,rhs] = argn(0);

    global LANGUAGE %helps

    xmlfile = path;
    needToBeCopied = %F;

    if rhs == 4 then

        if fileinfo(".list_"+default_language) <> [] then
            default_language_xml_files = mgetl(".list_"+default_language);
        end

        if find( default_language_xml_files == basename(path) ) <> [] then
            xmlfile = "../"+default_language+"/"+path;
            needToBeCopied = %T;
        end

    elseif rhs == 1 then

        select LANGUAGE

        case "eng" then
            xsl = pathconvert(SCI+"/modules/helptools/help_eng.xsl",%f,%f);
        case "fr" then
            xsl = pathconvert(SCI+"/modules/helptools/help_fr.xsl",%f,%f);
        end
    end

    //proceed if xml file is newest than htm file

    if newest(xmlfile,strsubst(path,".xml",".htm"))==1 then

        if needToBeCopied then
            tmp_file = mgetl(xmlfile);
            mputl(tmp_file,path);
        end

        mprintf("\tProcessing file %s.xml\n",basename(path));

        // build .xml2 file where LINK tags references are solved
        find_links(path,path+"2");

        in=path+"2"
        out=strsubst(path,".xml",".htm")

        // form the html generator command line instruction

        if  MSDOS then
            // sabcmd does not like c:/.. path replace it by file://c:/..
            xsl="file://"+xsl;
            generate_cmd=""""+WSCI+"\modules\helptools\bin\sablotron\sabcmd"+""""
            if basename(in)+".xml2"<>in then
                in="file://"+in;
            end
            if basename(out)+".xml2"<>out then
                out="file://"+out;
            end
            instr=generate_cmd+" "+xsl+" "+in+" "+out
            RM="del /s "
        else
            if generate_cmd=="xsltproc" then
                instr=generate_cmd+" -o "+out+" "+xsl+" "+in
            else
                instr=generate_cmd+" "+xsl+" "+in+" "+out
            end
            RM="rm -f "
        end

        //run html generator
        if execstr("unix_s("+sci2exp(instr)+")","errcatch")<>0 then
            write(%io(2),"Warning "+path+" does not follow the dtd","(a)");
            ok = %F;
        else
            ok = %T;
        end

        unix_s(RM+path+"2");

        if needToBeCopied then
            unix_s(RM+path);
        end
    else
        ok = %T;
    end

endfunction
