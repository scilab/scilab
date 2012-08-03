//CHECKSTYLE:OFF

package org.scilab.modules.helptools.c;

import java.io.Reader;
import java.io.StringReader;
import java.io.IOException;

@javax.annotation.Generated("JFlex")
@SuppressWarnings("fallthrough")
%%

%public
%class CLexer
%final
%unicode
%char
%type void
%switch

%{

    private int whitesOnFirstLine;
    private int saveLexState;
    private AbstractCCodeHandler handler;

    public CLexer() { }

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

    public String convert(AbstractCCodeHandler h, String code) {
        Reader reader = new StringReader(trimEnd(code));
        String str = convert(h, reader, true);
        try {
          reader.close();
        } catch (IOException e) { }

        return str;
    }

    public String convert(AbstractCCodeHandler h, Reader code, boolean ret) {
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

%x COMMENT, STRING, CLEAN, CLEANFIRST

/* main character classes */
eol = \r | \n | \r\n
white = [ \t\f]+

commentBeg = "/" "*"+
commentEnd = "*"+ "/"
comment = ([^\r\n*/] | "*"+ [^\r\n*/] | [^\r\n*]"/")*

oneLineComment = "//" [^\r\n]*

id = [a-zA-Z]([0-9a-zA-Z_])*

hex = [a-fA-F0-9]
is = (u|U|l|L)*
fs = (f|F|l|L)
digit = [0-9]
exp = [Ee] [+-]? {digit}+
number = 0 [xX] {hex}+ {is}? | "0"? {digit}+ {is}? | {digit}* "." {digit}+ {exp}? {fs}? | {digit}+ "." {digit}* {exp}? {fs}? | "NULL"

dquote = "\""
string = ([^\r\n\\\"]* | "\\"[^\r\n])+
char = "'" "\\"? [^\r\n\t] "'"

keyword = "auto" | "break" | "case" | "catch" | "cin" | "const_cast" | "continue" | "cout" | "default" | "delete" | "do" | "dynamic_cast" | "else" | "enum" | "false" | "for" | "goto" | "if" | "namespace" | "new" | "reinterpret_cast" | "return" | "sizeof" | "static_cast" | "switch" | "template" | "this" | "throw" | "true" | "try" | "typedef" | "using" | "while"

type = "bool" | "char" | "double" | "float" | "int" | "long" | "short" | "signed" | "struct" | "union" | "unsigned" | "void"

modifier = "const" | "extern" | "friend" | "inline" | "mutable" | "noinline" | "private" | "protected" | "public" | "register" | "static" | "virtual" | "volatile"

preprocessor = "#define" | "#elif" | "#else" | "#endif" | "#error" | "#if" | "#ifdef" | "#ifndef" | "#import" | "#include" | "#line" | "#pragma" | "#undef" | "#using"

openclose = [(){}\[\]]

operator = [=;,.><!~?:+-*/%&|\^] | ">>" | "<<" | "and" | "and_eq" | "bitand" | "bitor" | "compl" | "not" | "not_eq" | "operator" | "or" | "or_eq" | "typeid" | "xor" | "xor_eq"

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
  {oneLineComment}               {
                                   handler.handleComment(yytext());
                                 }

  {commentBeg}                   {
                                   yybegin(COMMENT);
                                   handler.handleComment(yytext());
                                 }

  {dquote}                       {
                                   yybegin(STRING);
                                   handler.handleString(yytext());
                                 }

  {char}                         {
                                   handler.handleString(yytext());
                                 }

  {number}                       {
                                   handler.handleNumber(yytext());
                                 }

  {keyword}                      {
                                   handler.handleKeyword(yytext());
                                 }

  {type}                         {
                                   handler.handleType(yytext());
                                 }

  {modifier}                     {
                                   handler.handleModifier(yytext());
                                 }

  {preprocessor}                 {
                                   handler.handlePreprocessor(yytext());
                                 }

  {openclose}                    {
                                   handler.handleOpenClose(yytext());
                                 }

  {operator}                     {
                                   handler.handleOperator(yytext());
                                 }

  {id}                           {
                                   handler.handleId(yytext());
                                 }

  .                              {
                                   handler.handleDefault(yytext());
                                 }

  {eol}                          {
                                   handler.handleNothing("\n");
                                   saveLexState = YYINITIAL;
                                   yybegin(CLEAN);
                                 }
}

<COMMENT> {
  {commentEnd}                   {
                                   yybegin(YYINITIAL);
                                   handler.handleComment(yytext());
                                 }

  {comment}                      {
                                   handler.handleComment(yytext());
                                 }

  "*"{eol}                       {
  				   handler.handleComment("*");
                                   handler.handleNothing("\n");
                                   saveLexState = COMMENT;
                                   yybegin(CLEAN);
                                 }

  {eol}                          {
                                   handler.handleNothing("\n");
                                   saveLexState = COMMENT;
                                   yybegin(CLEAN);
                                 }
}

<STRING> {
  {string}                       {
                                   handler.handleString(yytext());
                                 }

  {dquote}                       {
                                   yybegin(YYINITIAL);
                                   handler.handleString(yytext());
                                 }

  {eol}                          {
                                   handler.handleNothing("\n");
                                   saveLexState = STRING;
                                   yybegin(CLEAN);
                                 }
}

<<EOF>>                          {
                                   return;
                                 }
