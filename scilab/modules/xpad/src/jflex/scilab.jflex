//CHECKSTYLE:OFF

package org.scilab.modules.xpad;

import java.util.Arrays;
import java.util.Set;
import java.util.HashSet;
import java.io.IOException;
import javax.swing.text.Element;

%%

%public
%class ScilabLexer
%extends ScilabLexerConstants
%final
%unicode
%char
%type int
%switch

%{
    public int start = 0;
    public static Set<String> commands = new HashSet();
    public static Set<String> variables = new HashSet();

    private ScilabDocument doc = null;
    private boolean transposable = false;
    private Element elem;

    public ScilabLexer(ScilabDocument doc) {
        this.doc = doc;
        this.elem = doc.getDefaultRootElement();
        variables.addAll(Arrays.asList(ScilabKeywords.GetVariablesName()));
        commands.addAll(Arrays.asList(ScilabKeywords.GetFunctionsName()));
        commands.addAll(Arrays.asList(ScilabKeywords.GetMacrosName()));
    }

    public void setRange(int p0, int p1) {
        start = p0;
        yyreset(new ScilabDocumentReader(doc, p0, p1));
    }

    public int yychar() {
        return yychar;
    }

    public int getKeyword(int pos, boolean strict) {
        Element line = elem.getElement(elem.getElementIndex(pos));
        int end = line.getEndOffset();
        int tok = -1;
        start = line.getStartOffset();
        int startL = start;
        int s = -1;

        try {
           yyreset(new ScilabDocumentReader(doc, start, end));
           if (!strict) {
              pos++;
           }

           while (startL < pos && s != startL) {
               s = startL;
               tok = yylex();
               startL = start + yychar + yylength();
           }

           return tok;
        } catch (IOException e) {
           return ScilabLexerConstants.DEFAULT;
        }
     }

%}

/* main character classes */
eol = \n

open = "[" | "(" | "{"
close = "]" | ")" | "}"

comment = "//"

quote = "'"

cstes = "%t" | "%T" | "%f" | "%F" | "%e" | "%pi" | "%inf" | "%i" | "%z" | "%s" | "%nan" | "%eps" | "SCI" | "WSCI" | "SCIHOME" | "TMPDIR"

operator = ".'" | ".*" | "./" | ".\\" | ".^" | ".**" | "+" | "-" | "/" | "\\" | "*" | "^" | "**" | "==" | "~=" | "<>" | "<" | ">" | "<=" | ">=" | ".*." | "./." | ".\\." | "/." | "=" | "&" | "|" | "@" | "@=" | "~"

functionKwds = "function" | "endfunction"

structureKwds = "then" | "else" | "elseif" | "end" | "do" | "catch" | "case"

openstructureKwds = "if" | "for" | "while" | "try" | "select"

controlKwds = "abort" | "break" | "quit" | "return" | "resume" | "pause" | "continue" | "exit"

authors = "Calixte Denizet" | "Calixte DENIZET" | "Sylvestre Ledru" | "Sylvestre LEDRU" | "Yann Collette" | "Yann COLLETTE" | "Allan Cornet" | "Allan CORNET" | "Allan Simon" | "Allan SIMON" | "Antoine Elias" | "Antoine ELIAS" | "Bernard Hugueney" | "Bernard HUGUENEY" | "Bruno Jofret" | "Bruno JOFRET" | "Claude Gomez" | "Claude GOMEZ" | "Clement David" | "Clement DAVID" | "Jerome Picard" | "Jerome PICARD" | "Manuel Juliachs" | "Manuel JULIACHS" | "Michael Baudin" | "Michael BAUDIN" | "Pierre Lando" | "Pierre LANDO" | "Pierre Marechal" | "Pierre MARECHAL" | "Serge Steer" | "Serge STEER" | "Vincent Couvert" | "Vincent COUVERT" | "Vincent Liard" | "Vincent LIARD" | "Zhour Madini-Zouine" | "Zhour MADINI-ZOUINE" | "Vincent Lejeune" | "Vincent LEJEUNE" | "Sylvestre Koumar" | "Sylvestre KOUMAR" | "Inria" | "INRIA" | "DIGITEO" | "Digiteo" | "ENPC"

break = ".."(".")*

special = "$" | ":" | {break}

string = (([^\'\"\r\n]*)|([\'\"]{2}))+
dqstring = \"{string}(\"|\')

id = ([a-zA-Z%_#!?][a-zA-Z0-9_#!$?]*)|("$"[a-zA-Z0-9_#!$?]+)

dot = "."

url = "http://"[^ \t\f\n\r]+

latex = "$"(([^$]*|"\\$")+)"$"

digit = [0-9]
exp = [eE][+-]?{digit}+
number = ({digit}+"."?{digit}*{exp}?)|("."{digit}+{exp}?)

%x QSTRING, COMMENT, FIELD, COMMANDS, COMMANDSWHITE

%%

<YYINITIAL> {
  {comment}                      {
                                   transposable = false;
                                   yybegin(COMMENT);
                                 }

  {operator}                     {
                                   transposable = false;
                                   return ScilabLexerConstants.OPERATOR;
                                 }

  {functionKwds}                 {
                                   transposable = false;
                                   return ScilabLexerConstants.FKEYWORD;
                                 }

  {openstructureKwds}            {
                                   transposable = false;
                                   return ScilabLexerConstants.OSKEYWORD;
                                 }

  {structureKwds}                {
                                   transposable = false;
                                   return ScilabLexerConstants.SKEYWORD;
                                 }

  {controlKwds}                  {
                                   transposable = false;
                                   return ScilabLexerConstants.CKEYWORD;
                                 }

  {cstes}                        {
                                   transposable = true;
                                   return ScilabLexerConstants.CONSTANTES;
                                 }

  {id}                           {
                                   transposable = true;
                                   String str = yytext();
                                   if (commands.contains(str)) {
                                       yybegin(COMMANDS);
                                       return ScilabLexerConstants.COMMANDS;
                                   } else if (variables.contains(str)) {
                                       return ScilabLexerConstants.VARIABLES;
                                   }
                                   return ScilabLexerConstants.ID;
                                 }

  {number}                       {
                                   transposable = true;
                                   return ScilabLexerConstants.NUMBER;
                                 }

  {special}                      {
                                   transposable = false;
                                   return ScilabLexerConstants.SPECIAL;
                                 }

  {dot}                          {
                                   transposable = false;
                                   yybegin(FIELD);
                                   return ScilabLexerConstants.OPERATOR;
                                 }

  {quote}                        {
                                    if (transposable) {
                                       return ScilabLexerConstants.TRANSP;
                                    } else {
                                       yybegin(QSTRING);
                                    }
                                 }

  {open}                         {
                                   transposable = false;
                                   return ScilabLexerConstants.OPEN;
                                 }

  {close}                        {
                                   transposable = true;
                                   return ScilabLexerConstants.CLOSE;
                                 }

  {dqstring}                     {
                                   transposable = false;
                                   return ScilabLexerConstants.STRING;
                                 }

  " "                            {
                                   return ScilabLexerConstants.WHITE;
                                 }

  "\t"                           {
                                   return ScilabLexerConstants.TAB;
                                 }

  .                              |
  {eol}                          {
                                   transposable = false;
                                   return ScilabLexerConstants.DEFAULT;
                                 }

}

<COMMANDS> {

  " "                            {
                                   yybegin(COMMANDSWHITE);
                                   return ScilabLexerConstants.WHITE;
                                 }

  "\t"                           {
                                   yybegin(COMMANDSWHITE);
                                   return ScilabLexerConstants.TAB;
                                 }
  .
                                 {
                                   yypushback(1);
                                   yybegin(YYINITIAL);
                                 }

  {eol}                          { }
}

<COMMANDSWHITE> {
  [^ \t,;]*                      {
                                   return ScilabLexerConstants.STRING;
                                 }

  " "                            {
                                   return ScilabLexerConstants.WHITE;
                                 }

  "\t"                           {
                                   return ScilabLexerConstants.TAB;
                                 }
  .
                                 {
                                   yypushback(1);
                                   yybegin(YYINITIAL);
                                 }

  {eol}                          { }
}

<FIELD> {
  {id}                           {
                                   return ScilabLexerConstants.FIELD;
                                 }

  .                              {
                                   yypushback(1);
                                   yybegin(YYINITIAL);
                                 }

  {eol}                          { }
}

<QSTRING> {
  {string}                       { }

  (\'|\")                        {
                                   transposable = false;
                                   yybegin(YYINITIAL);
                                   return ScilabLexerConstants.STRING;
                                 }

  .                              |
  {eol}                          {
                                   return ScilabLexerConstants.DEFAULT;
                                 }

}

<COMMENT> {
  {authors}                      {
                                   return ScilabLexerConstants.AUTHORS;
                                 }

  {url}                          {
                                   return ScilabLexerConstants.URL;
                                 }

  {latex}                        {
                                   return ScilabLexerConstants.LATEX;
                                 }

  .                              |
  {eol}                          {
                                   return ScilabLexerConstants.COMMENT;
                                 }
}

<<EOF>>                          {
                                   return ScilabLexerConstants.EOF;
                                 }