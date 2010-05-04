//CHECKSTYLE:OFF 

package org.scilab.modules.xpad;

import java.io.IOException;
import javax.swing.text.Element;
 
%% 

%public
%class LineTypeScanner
%final
%unicode
%char
%type int

%switch

%{
    private ScilabDocument doc;

    public LineTypeScanner(ScilabDocument doc) {
    	this.doc = doc;
    }
   
    public int getLineType(int p0, int p1) {
    	try {
	    yyreset(new ScilabDocumentReader(doc, p0, p1));
	    return yylex();
	} catch (IOException e) {  
	    return ScilabDocument.ScilabLeafElement.NOTHING;	
    	}
    }	    
%}

%eofval{
  return ScilabDocument.ScilabLeafElement.NOTHING;
%eofval}

/* main character classes */
white = [ \t]*
eol = \n

spec = [^a-zA-Z0-9%_#!$?0-9]?

fun = {white} "function" {spec}
endfun = {white} "endfunction" {spec}

%%

<YYINITIAL> {
  {fun}				 {
  				   return ScilabDocument.ScilabLeafElement.FUN;
 				 }

  {endfun}			 {
  				   return ScilabDocument.ScilabLeafElement.ENDFUN;
 				 }

  .				 |
  {eol}				 { 
				   return ScilabDocument.ScilabLeafElement.NOTHING;
				 }
}