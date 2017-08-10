//CHECKSTYLE:OFF 

package org.scilab.modules.scinotes;

import java.io.IOException;
import javax.swing.text.Element;
import javax.swing.text.BadLocationException;

@javax.annotation.Generated("JFlex")
@SuppressWarnings("fallthrough")
%% 

%public
%class IndentScanner
%final
%unicode
%char
%type int

%{
    private ScilabDocument doc;
    private Element elem;

    public IndentScanner(ScilabDocument doc) {
    	this.doc = doc;
	this.elem = doc.getDefaultRootElement();
    }
   
    public int getIndentNumber(int lineNumber, char type) {
        if (lineNumber < 0) {
	    return 0;
	}
        try {
	     Element line = elem.getElement(lineNumber);
	     int startL = line.getStartOffset();
	     int endL = line.getEndOffset();	
	     yyreset(new ScilabDocumentReader(doc, startL, endL));
	     switch (type) {
	     	 case ' ':
	      	 yybegin(WHITE);
	      	 break;
	    	 case '\t':
	     	 yybegin(TAB);
	      	 break;
	     default:
		 yybegin(WHITE);
	     }
	     return yylex();
	} catch (IOException e) {
	     return 0;
	}
    }	

    public int getTabsAtBeginning(int pos) {
    	Element line = elem.getElement(pos);
	int startL = line.getStartOffset();
	int endL = line.getEndOffset();
	try {
	    yyreset(new ScilabDocumentReader(doc, startL, endL));
	    yybegin(BEG);
	    if (yylex() == 1) {
	        return yylength();
       	    }
	} catch (IOException e) {  
	    return 0;	
    	}
	return 0;
    }	    

    public void getIndentLevel(int pos, int[] level) {
    	Element line = elem.getElement(elem.getElementIndex(pos));
	int startL = line.getStartOffset();
	int endL = line.getEndOffset();
	level[0] = 0;
	level[1] = 0;
	try {
	    yyreset(new ScilabDocumentReader(doc, startL, endL));
	    yybegin(INDENT);
	    do {
	        switch (yylex()) {
	       	case 0:
	              break;
	       	case 1:
	             level[1]++;
		     break;
	       	case 2:
	             if (level[1] > 0) {
		         level[1]--;
		     } else {
	        	 level[0]++;
		     }
	             break;
	       	case 3:
	             if (level[1] == 0) {
		         level[0]++;
		         level[1]++;
		     }
                }	         
	    } while (zzMarkedPos != 0);
        } catch (IOException e) { } 	  
    }
%}

%eofval{
  return 0;
%eofval}

/* main character classes */

eol = \n

comment = "//".*{eol}

spec = [a-zA-Z0-9%_#!$?0-9]

string = (([^\'\"\r\n]*)|([\'\"]{2}))+
qstring = (\"|\'){string}(\"|\')
transp = ({spec} | ")" | "]" | "}") "'"

indentP = ("function" | "if" | "for" | "while" | "try" | "select" | "switch")
indentPx = {indentP}{spec}+
xindentP = {spec}{indentP}

indentM = ("endfunction" | "end")
indentMx = {indentM}{spec}+
xindentM = {spec}{indentM}
indentMP = ("else" | "elseif" | "catch" | "case" | "otherwise")
indentMPx = {indentMP}{spec}+
xindentMP = {spec}{indentMP}

%x INDENT, WHITE, TAB, BEG

%%

<INDENT> {
  {transp}			 |
  {comment}			 |
  {indentPx}			 |
  {qstring}			 { }

  {indentP}			 {
  				   return 1;
 				 }

  {indentM}			 {
  				   return 2;
 				 }

  {indentMP}			 {
  				   return 3;
 				 }

  .				 |
  {xindentP}			 |
  {xindentM}			 |
  {xindentMP}			 |
  {indentMPx}			 |
  {indentMx}			 { }

  {eol} 			 {
  				   return 0;
				 }
}

<WHITE> {
  " "*				 {
				   return yylength();
				 }

  .				 |
  {eol}				 {
  				   return 0;
 				 }
}

<TAB> {
  "\t"*				 {
				   return yylength();
				 }

  .				 |
  {eol}				 {
  				   return 0;
 				 }
}

<BEG> {
  ^[ \t]+			 {
				   return 1;
				 }

  .				 |
  {eol}				 { }
}
