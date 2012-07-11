//CHECKSTYLE:OFF

package org.scilab.modules.helptools ;

import java.io.IOException;
import java.io.StringReader;

@javax.annotation.Generated("JFlex")
@SuppressWarnings("fallthrough")
%%

%public
%class SynopsisLexer
%final
%unicode
%char
%type void

%switch

%{
    private static String currentName;
    private static StringBuilder htmlBuffer = new StringBuilder(512);
    private static SynopsisLexer lexer = new SynopsisLexer();

    private SynopsisLexer() { }

    public static String convert(String name, String str) {
        currentName = name;
        try {
            lexer.yyreset(new StringReader(str.trim()));
	    lexer.yybegin(YYINITIAL);
            htmlBuffer.setLength(0);
	    lexer.yylex();
	    
	    return htmlBuffer.toString();
        } catch (IOException e) {
            return null;
        }
    }

%}

%eofval{
  return;
%eofval}

/* main character classes */
white = [ \t]+
eol = ("\r" | "\n" | "\r\n"){white}*

id = ([a-zA-Z%_#!?][a-zA-Z0-9_#!$?]*)|("$"[a-zA-Z0-9_#!$?]+)

comments = "//"[^\r\n]*

nothing = [^a-zA-Z%_#!?/\n\r<>&\"\']+ | "/"

dquote = "\""
quote = "'"
lt = "<"
gt = ">"
amp = "&"

%%

<YYINITIAL> {

  {comments}                     {
                                   htmlBuffer.append("<span class=\"comment\">");
				   htmlBuffer.append(yytext());
				   htmlBuffer.append("</span>");
                                 }

  {id}				 {
  				   String str = yytext();
				   if (str.equals(currentName)) {
				      htmlBuffer.append("<span class=\"functionid\">");
				   } else {
				      htmlBuffer.append("<span class=\"default\">");
				   }				
				   htmlBuffer.append(str);
				   htmlBuffer.append("</span>");
				 }

  {dquote}                       {
				   htmlBuffer.append("&#0034;");
                                 }
     
  {quote}                        {
				   htmlBuffer.append("&#0039;");
                                 }

  {lt}                           {
				   htmlBuffer.append("&lt;");
                                 }

  {gt}                           {
				   htmlBuffer.append("&gt;");
                                 }

  {amp}                          {
				   htmlBuffer.append("&amp;");
                                 }

  {white}			 {
  				   htmlBuffer.append(" ");
				 }

  {eol}				 {
				   htmlBuffer.append("\n");
				 }

  {nothing}                      {
                                   htmlBuffer.append("<span class=\"default\">");
				   htmlBuffer.append(yytext());
				   htmlBuffer.append("</span>");
                                 }
}
