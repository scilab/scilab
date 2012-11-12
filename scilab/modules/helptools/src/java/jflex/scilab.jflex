//CHECKSTYLE:OFF

package org.scilab.modules.helptools.scilab;

import java.util.Arrays;
import java.util.Set;
import java.util.Map;
import java.util.HashSet;
import java.util.List;
import java.util.ArrayList;
import java.util.Iterator;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.Reader;
import java.io.StringReader;
import java.io.IOException;

@javax.annotation.Generated("JFlex")
@SuppressWarnings("fallthrough")
%%

%public
%class ScilabLexer
%final
%unicode
%char
%type void
%switch

%{
    private static Set<String> commands;
    private static Set<String> macros;

    private int beginString;
    private boolean transposable;
    private boolean breakstring;
    private boolean breakargs;
    private int whitesOnFirstLine;
    private int saveLexState;

    private String id;
    private List<String> returnValues = new ArrayList<String>();
    private List<String> argsValues = new ArrayList<String>();
    private List<String> localFun = new ArrayList<String>();

    private AbstractScilabCodeHandler handler;

    public ScilabLexer(String[] primFile, String[] macroFile) {
       if (commands == null) {
          commands = new HashSet<String>();
          macros = new HashSet<String>();
          loadNames(primFile, commands);
          loadNames(macroFile, macros);
       }
    }

    public ScilabLexer(String primFile, String macroFile) {
       this(new String[]{primFile}, new String[]{macroFile});
    }

    public ScilabLexer(Set<String> primitives, Set<String> macros) {
       commands = primitives;
       this.macros = macros;
    }

    private void loadNames(String[] files, Set<String> set) {
       for (int i = 0; i < files.length; i++) {
          loadNames(files[i], set);
       }
    }

    private void loadNames(String file, Set<String> set) {
       if (file == null) {
          return;
       }
       BufferedReader input = null;
       try {
          input = new BufferedReader(new FileReader(file));
          String line = null;
          while ((line = input.readLine()) != null) {
             set.add(line);
          }
       } catch (IOException e) {
          System.err.println(e);
       }
       if (input != null) {
          try {
             input.close();
          } catch (IOException e) {
             System.err.println(e);
          }
       }
    }

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

    public String convert(AbstractScilabCodeHandler h, String code) {
        Reader reader = new StringReader(trimEnd(code));
        String str = convert(h, reader, true);
        try {
          reader.close();
        } catch (IOException e) { }

        return str;
    }

    public String convert(AbstractScilabCodeHandler h, Reader code, boolean ret) {
        if (code == null) {
           return null;
        } else {
           handler = h;
           transposable = false;
           breakargs = false;
           breakstring = false;
           whitesOnFirstLine = 0;
           localFun.clear();
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

/* main character classes */
eol = "\r" | "\n" | "\r\n"

open = "[" | "(" | "{"
close = "]" | ")" | "}"

comment = "//"

quote = "'"

dquote = "\""

cstes = "%t" | "%T" | "%f" | "%F" | "%e" | "%pi" | "%inf" | "%i" | "%z" | "%s" | "%nan" | "%eps" | "SCI" | "WSCI" | "SCIHOME" | "TMPDIR"

dottransp = ".'"
lt = "<"
gt = ">"
neq = "<>"
leq = "<="
geq = ">="
amp = "&"

operator = ".*" | "./" | ".\\" | ".^" | ".**" | "+" | "-" | "/" | "\\" | "*" | "^" | "**" | "==" | "~=" | ".*." | "./." | ".\\." | "/." | "=" | "|" | "@" | "@=" | "~"

structureKwds = "if" | "for" | "while" | "try" | "select" | "end" | "then" | "do" | "catch" | "case" | "elseif" | "else"

controlKwds = "abort" | "break" | "quit" | "return" | "resume" | "pause" | "continue" | "exit"

break = ".."(".")*
breakinstring = {break}[ \t]*({comment} | {eol})

special = "$" | ":" | {break}

string = (([^\t\'\"\r\n<>&\.]*)|([\'\"]{2})|("."[^\t\'\"\r\n<>&\.]))+

argstring = ([^ \t,;/\n\r<>&]*) | ("/"[^ \t,;/\n\r<>&]*)

id = ([a-zA-Z%_#!?][a-zA-Z0-9_#!$?]*)|("$"[a-zA-Z0-9_#!$?]+)

dot = "."

digit = [0-9]
exp = [dDeE][+-]?{digit}*
number = ({digit}+"."?{digit}*{exp}?)|("."{digit}+{exp}?)

/* function declaration */
white = [ \t]+
funcomments = {white}* "//"
fun = "function" {white}
funb = "function["
endfun = "endfunction"

htmlentity = "&"[#a-zA-Z0-9]*";"

%x QSTRING, COMMENT, FIELD, COMMANDS, COMMANDSWHITE, BREAKSTRING, FUNCTION, TYPEID, FUNNAME, RETS, ARGS, BREAKINARGS, WHITESEOL, CLEANFIRST, CLEAN

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
  {htmlentity}                   {
                                   transposable = false;
                                   handler.handleDefault(yytext());
                                 }

  {comment}                      {
                                   transposable = false;
                                   yypushback(2);
                                   yybegin(COMMENT);
                                 }

  {dottransp}                    {
                                   transposable = false;
                                   handler.handleOperator(".&#0039;");
                                 }

  {lt}                           {
                                   transposable = false;
                                   handler.handleOperator("&#0060;");
                                 }

  {gt}                           {
                                   transposable = false;
                                   handler.handleOperator("&#0062;");
                                 }

  {leq}                          {
                                   transposable = false;
                                   handler.handleOperator("&#0060;=");
                                 }

  {geq}                          {
                                   transposable = false;
                                   handler.handleOperator("&#0062;=");
                                 }

  {neq}                          {
                                   transposable = false;
                                   handler.handleOperator("&#0060;&#0062;");
                                 }

  {amp}                          {
                                   transposable = false;
                                   handler.handleOperator("&#0038;");
                                 }

  {operator}                     {
                                   transposable = false;
                                   handler.handleOperator(yytext());
                                 }

  {funb}                         {
                                   transposable = false;
                                   handler.handleFKeywords("function");
                                   handler.handleNothing(" ");
                                   handler.handleOpenClose("[");
                                   yybegin(RETS);
                                 }

  {fun}                          {
                                   transposable = false;
                                   handler.handleFKeywords("function");
                                   handler.handleNothing(" ");
                                   yybegin(FUNCTION);
                                 }

  {endfun}                       {
                                   transposable = false;
                                   returnValues.clear();
                                   argsValues.clear();
                                   handler.handleFKeywords("endfunction");
                                 }

  {structureKwds}                {
                                   transposable = false;
                                   handler.handleSKeywords(yytext());
                                 }

  {controlKwds}                  {
                                   transposable = false;
                                   handler.handleCKeywords(yytext());
                                 }

  {cstes}                        {
                                   transposable = true;
                                   handler.handleConstants(yytext());
                                 }

  {id}                           {
                                   transposable = true;
                                   String str = yytext();
                                   if (commands.contains(str)) {
                                       yybegin(COMMANDS);
                                       handler.handleCommand(str);
                                   } else if (macros.contains(str)) {
                                       yybegin(COMMANDS);
                                       handler.handleMacro(str);
                                   } else if (localFun.contains(str)) {
                                        yybegin(COMMANDS);
                                        handler.handleFunctionId(str);
                                   } else {
                                       if (returnValues.contains(str) || argsValues.contains(str)) {
                                           handler.handleInputOutputArgs(str);
                                       }  else {
                                           handler.handleId(str);
                                       }
                                   }
                                 }

  {number}                       {
                                   transposable = true;
                                   handler.handleNumber(yytext());
                                 }

  {special}                      {
                                   transposable = false;
                                   handler.handleSpecial(yytext());
                                 }

  {dot}                          {
                                   transposable = false;
                                   yybegin(FIELD);
                                   handler.handleOperator(yytext());
                                 }

  {quote}                        {
                                   if (transposable) {
                                       handler.handleOperator("&#0039;");
                                   } else {
                                       beginString = zzStartRead;
                                       yybegin(QSTRING);
                                       handler.handleString("&#0039;");
                                   }
                                 }

  {open}                         {
                                   transposable = false;
                                   handler.handleOpenClose(yytext());
                                 }

  {close}                        {
                                   transposable = true;
                                   handler.handleOpenClose(yytext());
                                 }

  {dquote}                       {
                                   transposable = false;
                                   beginString = zzStartRead;
                                   yybegin(QSTRING);
                                   handler.handleString("&#0034;");
                                 }

  " "                            {
                                   transposable = false;
                                   handler.handleNothing(" ");
                                 }

  "\t"                           {
                                   transposable = false;
                                   handler.handleNothing("    ");
                                 }

  "\0"                           {
                                   return;
                                 }

  .                              {
                                   transposable = false;
                                   handler.handleDefault(yytext());
                                 }

  {eol}                          {
                                   handler.handleNothing("\n");
                                   saveLexState = YYINITIAL;
                                   yybegin(CLEAN);
                                 }

}

<FUNCTION> {
  "["                            {
                                   handler.handleOpenClose("[");
                                   yybegin(RETS);
                                 }

  {id}                           {
                                   id = yytext();
                                   yybegin(TYPEID);
                                 }

  .                              |
  {eol}                          {
                                   yypushback(1);
                                   yybegin(YYINITIAL);
                                 }
}

<TYPEID> {
  "="                            {
                                   returnValues.add(id);
                                   handler.handleInputOutputArgsDecl(id);
                                   handler.handleOperator("=");
                                   yybegin(FUNNAME);
                                 }

  "("                            {
                                   localFun.add(id);
                                   handler.handleFunctionIdDecl(id);
                                   handler.handleOpenClose("(");
                                   yybegin(ARGS);
                                 }

  [ \t]                          { }

  {funcomments}                  {
                                   localFun.add(id);
                                   handler.handleFunctionIdDecl(id);
                                   yypushback(yylength());
                                   yybegin(YYINITIAL);
                                 }

  .                              |
  {eol}                          {
                                   yypushback(1);
                                   yybegin(YYINITIAL);
                                 }
}

<FUNNAME> {
  {id}                           {
                                   id = yytext();
                                   localFun.add(id);
                                   handler.handleFunctionIdDecl(id);
                                 }

  [ \t]                          { }

  "("                            {
                                   handler.handleOpenClose("(");
                                   yybegin(ARGS);
                                 }

  {funcomments}                  {
                                   yypushback(yylength());
                                   yybegin(YYINITIAL);
                                 }

  .                              |
  {eol}                          {
                                   yypushback(1);
                                   yybegin(YYINITIAL);
                                 }
}

<ARGS> {
  {id}                           {
                                   id = yytext();
                                   argsValues.add(id);
                                   handler.handleInputOutputArgsDecl(id);
                                 }

  ","                            {
                                   handler.handleDefault(", ");
                                 }

  {white}                        { }

  {break}                        {
                                   handler.handleSpecial(yytext());
                                   yybegin(BREAKINARGS);
                                 }

  ")"                            {
                                   handler.handleOpenClose(")");
                                   yybegin(YYINITIAL);
                                 }

  .                              |
  {eol}                          {
                                   yypushback(1);
                                   yybegin(YYINITIAL);
                                 }
}

<BREAKINARGS> {
  " "                            {
                                   handler.handleNothing(" ");
                                 }

  {comment}                      {
                                   breakargs = true;
                                   yypushback(2);
                                   yybegin(COMMENT);
                                 }

  {white}*{eol}                  {
                                   handler.handleNothing("\n");
                                   saveLexState = WHITESEOL;
                                   yybegin(CLEAN);
                                 }

  .                              {
                                   yypushback(1);
                                   yybegin(YYINITIAL);
                                 }
}

<WHITESEOL> {
  " "                            {
                                   handler.handleNothing(" ");
                                 }

  .                              |
  {eol}                          {
                                   yypushback(1);
                                   yybegin(ARGS);
                                 }
}

<RETS> {
  {id}                           {
                                   id = yytext();
                                   returnValues.add(id);
                                   handler.handleInputOutputArgsDecl(id);
                                 }

  ","                            {
                                   handler.handleDefault(", ");
                                 }

  "]"                            {
                                   handler.handleOpenClose("]");
                                 }

  "["                            {
                                   handler.handleOpenClose("[");
                                 }

  "="                            {
                                   handler.handleOperator("=");
                                   yybegin(FUNNAME);
                                 }

  "..."                          {
                                   handler.handleNothing("...");
                                 }

  {white}                        { }

  .                              |
  {eol}                          {
                                   yypushback(1);
                                   yybegin(YYINITIAL);
                                 }
}

<COMMANDS> {
  [ \t]*"("                      {
                                   yypushback(yylength());
                                   yybegin(YYINITIAL);
                                 }

  " "                            {
                                   yybegin(COMMANDSWHITE);
                                   handler.handleNothing(" ");
                                 }

  "\t"                           {
                                   yybegin(COMMANDSWHITE);
                                   handler.handleNothing("    ");
                                 }
  .
                                 {
                                   yypushback(1);
                                   yybegin(YYINITIAL);
                                 }

  {eol}                          {
                                   handler.handleNothing("\n");
                                   saveLexState = YYINITIAL;
                                   yybegin(CLEAN);
                                 }
}

<COMMANDSWHITE> {
  {comment}                      {
                                   transposable = false;
                                   yypushback(2);
                                   yybegin(COMMENT);
                                 }

  {argstring}                    {
                                   handler.handleString(yytext());
                                 }

  {lt}                           {
                                   transposable = false;
                                   handler.handleString("&#0060;");
                                 }

  {gt}                           {
                                   transposable = false;
                                   handler.handleString("&#0062;");
                                 }

  {amp}                          {
                                   transposable = false;
                                   handler.handleString("&#0038;");
                                 }

  " "                            {
                                   handler.handleNothing(" ");
                                 }

  "\t"                           {
                                   handler.handleNothing("    ");
                                 }

  {eol}                          {
                                   handler.handleNothing("\n");
                                   saveLexState = YYINITIAL;
                                   yybegin(CLEAN);
                                 }
  .
                                 {
                                   yypushback(1);
                                   yybegin(YYINITIAL);
                                 }

}

<FIELD> {
  {id}                           {
                                   handler.handleField(yytext());
                                 }

  .                              {
                                   yypushback(1);
                                   yybegin(YYINITIAL);
                                 }

 {eol}                           {
                                   handler.handleNothing("\n");
                                   saveLexState = YYINITIAL;
                                   yybegin(CLEAN);
                                 }
}

<QSTRING> {
  {breakinstring}                {
                                   yypushback(yylength());
                                   yybegin(BREAKSTRING);
                                   transposable = false;
                                   handler.handleString(yytext());
                                 }

  {lt}                           {
                                   transposable = false;
                                   handler.handleString("&#0060;");
                                 }

  {gt}                           {
                                   transposable = false;
                                   handler.handleString("&#0062;");
                                 }

  {amp}                          {
                                   transposable = false;
                                   handler.handleString("&#0038;");
                                 }

  {string}                       |
  "."                            {
                                   handler.handleString(yytext());
                                 }

  {quote}                        {
                                   transposable = false;
                                   yybegin(YYINITIAL);
                                   handler.handleString("&#0039;");
                                 }

  {dquote}                       {
                                   transposable = false;
                                   yybegin(YYINITIAL);
                                   handler.handleString("&#0034;");
                                 }

  "\0"                           {
                                   return;
                                 }

  .                              {
                                   handler.handleString(yytext());
                                 }

 {eol}                           {
                                   handler.handleNothing("\n");
                                   saveLexState = YYINITIAL;
                                   yybegin(CLEAN);
                                 }
}

<COMMENT> {
  [^&<>\'\"\n\0]*                {
                                   handler.handleComment(yytext());
                                 }

  {lt}                           {
                                   handler.handleComment("&#0060;");
                                 }

  {gt}                           {
                                   handler.handleComment("&#0062;");
                                 }

  {amp}                          {
                                   handler.handleComment("&#0038;");
                                 }

  {quote}                        {
                                   handler.handleComment("&#0039;");
                                 }

  {dquote}                       {
                                   handler.handleComment("&#0034;");
                                 }

  {eol}                          {
                                   if (breakstring) {
                                     saveLexState = QSTRING;
                                     breakstring = false;
                                   } else if (breakargs) {
                                     saveLexState = WHITESEOL;
                                     breakargs = false;
                                   } else {
                                     saveLexState = YYINITIAL;
                                   }
                                   handler.handleNothing("\n");
                                   yybegin(CLEAN);
                                 }

  "\0"                           {
                                   return;
                                 }
}

<BREAKSTRING> {
  {break}                        {
                                   breakstring = true;
                                   handler.handleSpecial(yytext());
                                 }

  " "                            {
                                   handler.handleNothing(" ");
                                 }

  "\t"                           {
                                   handler.handleNothing("    ");
                                 }

  {comment}                      {
                                   transposable = false;
                                   yypushback(2);
                                   yybegin(COMMENT);
                                 }

  .                              {
                                   handler.handleDefault(yytext());
                                 }

  {eol}                          {
                                   if (breakstring) {
                                      breakstring = false;
                                      saveLexState = QSTRING;
                                   } else {
                                      saveLexState = YYINITIAL;
                                   }
                                   handler.handleNothing("\n");
                                   yybegin(CLEAN);
                                 }

  "\0"                           {
                                   return;
                                 }
}

<<EOF>>                          {
                                   return;
                                 }
