function xmlfiletohtml(path)
//given a path on a Scilab help xml file  (assumed to respect 
//SCI/man/man-rev.dtd ) this function generates the corresponding htm
//file using the /man/<LANGUAGE>/html-rev.xsl xsl file
  xsl= 'html-rev.xsl' //the xsl file name
  global LANGUAGE 
  path=pathconvert(path,%f,%t) // convert path to host convention
  
  //proceed if xml file is newest than htm file
  if newest(path,strsubst(path,".xml",".htm"))==1 then
    mprintf('  Processing file %s.xml\n',basename(path));

    // build .xml2 file where LINK tags references are solved
    find_links(path,path+"2")
    in=path+"2"
    out=strsubst(path,'.xml','.htm')
    
    // form the sabcmd command line instruction
    if  MSDOS then 
      // sabcmd does not like c:/.. path replace it by file://c:/..
      xsl='file://'+pathconvert(SCI+'/man/'+LANGUAGE)+xsl;
      sabcmd=WSCI+'\Win95-util\sablotron\sabcmd '+xsl
      RM='del /s '
    else
      xsl=pathconvert(SCI+'/man/'+LANGUAGE)+xsl;
      sabcmd='sabcmd '+xsl
      RM='rm -f '
    end

    //run sabcmd
    if execstr('unix_s('+sci2exp(sabcmd+' '+in+' '+out)+')','errcatch')<>0 then 
      write(%io(2),'     Warning '+path+' does not follow dtd','(a)')
    end
    unix_s(RM+path+"2")
  end
endfunction

