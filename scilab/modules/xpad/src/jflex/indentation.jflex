package org.scilab.modules.xpad;

import java.util.Stack;
import java.io.IOException;
 
%% 

%public
%class SingleScanner
%final
%unicode
%char
%type int

%switch

%{
    private int indent = 0;
    private ScilabDocument doc;

    public SingleScanner(ScilabDocument doc) {
    	this.doc = doc;
    }
   
    public int getMatchingClosed(int p0, int p1) throws IOException {
    	yyreset(new ScilabDocumentReader(doc, p0, p1));
        yybegin(MATCHINGCLOSED);
	return yylex();
    }

    public int getLevelIndent(int p0, int p1) throws IOException {
    	yyreset(new ScilabDocumentReader(doc, p0, p1));
	yybegin(INDENT);
	return yylex();
    }
    
%}

%eofval{
	return -1;
%eofval}

/* main character classes */
eol = \r|\n|\r\n

white = [ \t\f]*

open = "(" | "[" | "{"
close = ")" | "]" | "}"

indentP = "function" | "if" | "for" | "while" | "try" | "select"
indentM = "endfunction" | "end"
indentMP = "else" | "elseif" | "catch" | "case"

%x INDENT, INDENTPLUS, OPENCLOSEMATCH, MATCHINGCLOSED

%%

<MATCHINGCLOSED> {
  {open}			 {
  				   indent = 1;
				   yybegin(OPENCLOSEMATCH);
				 }

  .				 |
  {eol}				 {
  				   return -1;
				 }
}

<INDENT> {
  {indentP}			 {
  				   indent = 1;
				   yybegin(INDENTPLUS);
 				 }

  {white} 			 { }
}
				 


<INDENTPLUS> {
  {indentP}			 { 
  				   indent++;
				 }

  {indentM}			 { 
  				   indent--;
				 }
  				 
  .*				 { }

  {eol}				 {
				   return indent;
				 }
}

<OPENCLOSEMATCH> {
  {open}			 {
  				   indent++;
				 }

  {close}			 {
  				   if (indent == 1) {
				      return yychar;
				   } else {
				      indent--;
				   }
				 }

  .*				 |
  {eol} 			 { }
}
