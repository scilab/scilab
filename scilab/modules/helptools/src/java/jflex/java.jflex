//CHECKSTYLE:OFF

package org.scilab.modules.helptools.java;

import java.io.Reader;
import java.io.StringReader;
import java.io.IOException;

import org.scilab.modules.helptools.c.AbstractCCodeHandler;

@javax.annotation.Generated("JFlex")
@SuppressWarnings("fallthrough")
%%

%public
%class JavaLexer
%final
%unicode
%char
%type void
%pack

%{

    private int whitesOnFirstLine;
    private int saveLexState;
    private AbstractCCodeHandler handler;

    public JavaLexer() { }

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

%x COMMENT, CLEAN, CLEANFIRST

/* main character classes */
eol = \r | \n | \r\n
white = [ \t\f]+

commentBeg = "/" "*"+
commentEnd = "*"+ "/"
comment = ([^\r\n*/] | "*"+ [^\r\n*/] | [^\r\n*]"/")*

oneLineComment = "//" [^\r\n]*

id = [a-zA-Z]([0-9a-zA-Z_])*

hex = [a-fA-F0-9]
is = (l|L)?
fs = [fFdD]?
digit = [0-9]
exp = [Ee] [+-]? {digit}+
number = 0 [xX] {hex}+ {is}? | "0"? {digit}+ {is}? | {digit}* "." {digit}+ {exp}? {fs}? | {digit}+ "." {digit}* {exp}? {fs}?

string = ("\"" ([^\r\n\\\"]* | "\\"[^\r\n])+ "\"") | ("'" "\\"? [^\r\n\t] "'")

keyword = "break" | "case" | "catch" | "class" | "const" | "continue" | "default" | "do" | "else" | "enum" | "extends" | "false" | "finally" | "for" | "goto" | "if" | "implements" | "import" | "interface" | "native" | "new" | "package" | "return" | "strictfp" | "super" | "switch" | "this" | "throw" | "throws" | "true" | "try" | "while"

type = [Bb]"oolean" | [Bb]"yte" | "char" | "Character" | "Class" | [Dd]"ouble" | [Ff]"loat" | "int" | "Integer" | [Ll]"ong" | "Object" | [Ss]"hort" | "String" | "StringBuffer" | "StringBuilder" | "Thread" | "void" | "Void"

exception = "ArithmeticException" | "ArrayIndexOutOfBoundsException" | "ClassCastException" | "ClassNotFoundException" | "CloneNotSupportedException" | "Exception" | "IllegalAccessException" | "IllegalArgumentException" | "IllegalStateException" | "IllegalThreadStateException" | "IndexOutOfBoundsException" | "InstantiationException" | "InterruptedException" | "NegativeArraySizeException" | "NoSuchFieldException" | "NoSuchMethodException" | "NullPointerException" | "NumberFormatException" | "RuntimeException" | "SecurityException" | "StringIndexOutOfBoundsException" | "UnsupportedOperationException"

modifier = "abstract" | "final" | "private" | "protected" | "public" | "static" | "transient" | "volatile"

openclose = [(){}\[\]]

operator = [=;,.><!~?:+-*/%&|\^] | ">>" | "<<" | ">>>"

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

  {string}                       {
                                   handler.handleString(yytext());
                                 }

  {number}                       {
                                   handler.handleNumber(yytext());
                                 }

  {keyword}                      {
                                   handler.handleKeyword(yytext());
                                 }

  {type}                         |
  {exception}		         {
                                   handler.handleType(yytext());
                                 }

  {modifier}                     {
                                   handler.handleModifier(yytext());
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

<<EOF>>                          {
                                   return;
                                 }
