//CHECKSTYLE:OFF

package org.scilab.modules.helptools.XML;

import java.io.Reader;
import java.io.StringReader;
import java.io.IOException;

@javax.annotation.Generated("JFlex")
@SuppressWarnings("fallthrough")
%%

%public
%class XMLLexer
%final
%unicode
%char
%type void
%switch

%{

    private int whitesOnFirstLine;
    private int saveLexState;
    private AbstractXMLCodeHandler handler;

    public XMLLexer() { }

    private String trimEnd(String str) {
        int end = str.length() - 1;
        int i = end;
        for (; i >= 0; i--) {
           char c = str.charAt(i);
           if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
              break;
           }
        }
        if (i != end) {
           return str.substring(0, i + 1);
        }

        return str;
    }

    public String convert(AbstractXMLCodeHandler h, String code) {
        Reader reader = new StringReader(trimEnd(code));
        String str = convert(h, reader, true);
        try {
          reader.close();
        } catch (IOException e) { }

        return str;
    }

    public String convert(AbstractXMLCodeHandler h, Reader code, boolean ret) {
        if (code == null) {
           return null;
        } else {
           handler = h;
           whitesOnFirstLine = 0;
           yyreset(code);
           yybegin(CLEANFIRST);
           try {
               yylex();
           } catch (IOException e) {
               return null;
           }
           if (ret) {
               return h.toString();
           }
           return "";
        }
    }
%}

%x COMMENT, CDATA, TAG, PI, CLEAN, CLEANFIRST

/* main character classes */
/* http://www.w3.org/TR/REC-xml/ */

white = [ \t]+

entity = "&"[#a-zA-Z0-9]*";"

nameStartChar = ":" | [A-Z] | "_" | [a-z] | [\u00C0-\u00D6] | [\u00D8-\u00F6] | [\u00F8-\u02FF] | [\u0370-\u037D] | [\u037F-\u1FFF] | [\u200C-\u200D] | [\u2070-\u218F] | [\u2C00-\u2FEF] | [\u3001-\uD7FF] | [\uF900-\uFDCF] | [\uFDF0-\uFFFD]
nameChar = {nameStartChar} | "-" | "." | [0-9] | \u00B7 | [\u0300-\u036F] | [\u0203F-\u2040]
name = {nameStartChar} {nameChar}*

commentBeg = "<!--"
commentEnd = "-->"
comment = ([^\r\n-]* | [^\r\n-]* "-" [^\r\n-] | [^\r\n-]* "--" [^\r\n>])+

cdataBeg = "<![CDATA["
cdataEnd = "]]>"
cdata = ([^\r\n\]]* | [^\r\n\]]* "]" [^\r\n\]] | [^\r\n\]]* "]]" [^\r\n>])+

piBeg = "<?" {name}
piEnd = "?>"

openTagBeg = "<" {name}
autocloseTag = "/>"
openTagEnd = ">"
closeTag = "</" {name} {white}* ">"

attribute = {name} "="
value = ("\"" [^\r\n\"]* "\"") | ("\'" [^\r\n\']* "\'")

eol = [\r\n]+

%%
<CLEANFIRST> {
  {white}*{eol}+                 { }

  {white}                        {
                                     whitesOnFirstLine = yylength();
                                     yybegin(YYINITIAL);
                                 }

  .                              {
                                     yypushback(1);
                                     yybegin(YYINITIAL);
                                 }
}

<CLEAN> {
  {eol}+                         {
                                     handler.handleNothing("\n");
                                 }

  {white}                        {
                                     int len = yylength() - whitesOnFirstLine;
                                     if (len > 0) {
                                        yypushback(len);
                                     }
                                     yybegin(saveLexState);
                                 }

  .                              {
                                     yypushback(1);
                                     yybegin(saveLexState);
                                 }
}

<YYINITIAL> {
  {entity}                       {
                                     handler.handleEntity("&amp;" + yytext().substring(1));
                                 }

  {piBeg}                        {
                                     yybegin(PI);
                                     handler.handleOpenInstr("&#0060;?");
                                     handler.handleInstrName(yytext().substring(2));
                                 }

  {openTagBeg}                   {
                                     yybegin(TAG);
                                     handler.handleLow("&#0060;");
                                     handler.handleOpenTagName(yytext().substring(1));
                                 }

  {closeTag}                     {
                                     handler.handleLowClose("&#0060;/");
                                     String str = yytext();
                                     handler.handleOpenTagName(str.substring(2, str.length() - 1).trim());
                                     handler.handleGreat("&#0062;");
                                 }

  {commentBeg}                   {
                                     yybegin(COMMENT);
                                     handler.handleOpenComment("&#0060;!--");
                                 }

  {cdataBeg}                     {
                                     yybegin(CDATA);
                                     handler.handleOpenCdata("&#0060;![CDATA[");
                                 }

  [^<\r\n]*                      {
                                     handler.handleDefault(yytext());
                                 }

  {eol}                          {
                                     handler.handleNothing("\n");
                                     saveLexState = YYINITIAL;
                                     yybegin(CLEAN);
                                 }
}

<PI> {
  {attribute}                    {
                                     String str = yytext();
                                     handler.handleAttributeName(str.substring(0, str.length() - 1));
                                     handler.handleEqual("=");
                                 }

  {value}                        {
                                     handler.handleAttributeValue(yytext());
                                 }

  {piEnd}                        {
                                     yybegin(YYINITIAL);
                                     handler.handleCloseInstr("?&#0062;");
                                 }

  {white}                        {
                                     handler.handleNothing(" ");
                                 }

  {eol}                          {
                                     handler.handleNothing("\n");
                                     saveLexState = PI;
                                     yybegin(CLEAN);
                                 }

  .                              { }
}

<TAG> {
  {attribute}                    {
                                     String str = yytext();
                                     handler.handleAttributeName(str.substring(0, str.length() - 1));
                                     handler.handleEqual("=");
                                 }

  {value}                        {
                                     handler.handleAttributeValue(yytext());
                                 }

  {autocloseTag}                 {
                                     yybegin(YYINITIAL);
                                     handler.handleAutoClose("/&#0062;");
                                 }

  {openTagEnd}                   {
                                     yybegin(YYINITIAL);
                                     handler.handleGreat("&#0062;");
                                 }

  {white}                        {
                                     handler.handleNothing(" ");
                                 }

  {eol}                          {
                                     handler.handleNothing("\n");
                                     saveLexState = TAG;
                                     yybegin(CLEAN);
                                 }

  .                              { }
}

<COMMENT> {
  {commentEnd}                   {
                                     yybegin(YYINITIAL);
                                     handler.handleCommentEnd("--&#0062;");
                                 }

  {eol}                          {
                                     handler.handleNothing("\n");
                                     saveLexState = COMMENT;
                                     yybegin(CLEAN);
                                 }

  {comment}                      {
                                     handler.handleComment(yytext());
                                 }
}

<CDATA> {
  {cdataEnd}                     {
                                     yybegin(YYINITIAL);
                                     handler.handleCdataEnd("]]&#0062;");
                                 }

  {eol}                          {
                                     handler.handleNothing("\n");
                                     saveLexState = CDATA;
                                     yybegin(CLEAN);
                                 }

  {cdata}                        {
                                     handler.handleCdata(yytext());
                                 }
}

<<EOF>>                          {
                                     return;
                                 }
