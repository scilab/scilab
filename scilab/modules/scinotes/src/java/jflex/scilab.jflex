//CHECKSTYLE:OFF

package org.scilab.modules.scinotes;

import java.util.Arrays;
import java.util.HashSet;
import java.util.Iterator;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.io.IOException;
import javax.swing.text.BadLocationException;
import javax.swing.text.Element;

import org.scilab.modules.commons.ScilabCommonsUtils;

@javax.annotation.Generated("JFlex")
@SuppressWarnings("fallthrough")

%%

%public
%class ScilabLexer
%extends ScilabLexerConstants
%final
%unicode
%char
%type int
%switch
%pack

%{
    public int start;
    public int end;
    public int beginString;
    public static Set<String> commands = new HashSet<String>();
    public static Set<String> macros = new HashSet<String>();
    public static Set<String> variables = new HashSet<String>();
    public Set<String> infile;

    private ScilabDocument doc;
    private boolean transposable;
    private Element elem;
    private boolean breakstring;

    public ScilabLexer(ScilabDocument doc) {
    	this(doc, true);
    }

    public ScilabLexer(ScilabDocument doc, boolean update) {
        this.doc = doc;
        this.elem = doc.getDefaultRootElement();
        this.infile = doc.getFunctionsInDoc();
	if (update) {
	   update();
    	}
    }	   

    public static void update() {
    	if (ScilabCommonsUtils.isScilabThread()) {
            String[] vars = ScilabKeywords.GetVariablesName();
	    String[] funs = ScilabKeywords.GetFunctionsName();
	    String[] macs = ScilabKeywords.GetMacrosName();
	    variables.clear();
            commands.clear();
            macros.clear();
	    if (vars != null) {
	        variables.addAll(Arrays.asList(vars));
	    }
            if (funs != null) {
	        commands.addAll(Arrays.asList(funs));
	    }
	    if (macs != null) {
	        macros.addAll(Arrays.asList(macs));
	    }
	}
    }

    public void setRange(int p0, int p1) {
        start = p0;
        end = p1;
        transposable = false;
        breakstring = false;
        yyreset(new ScilabDocumentReader(doc, p0, p1));
        int currentLine = elem.getElementIndex(start);
        if (currentLine != 0 && ((ScilabDocument.ScilabLeafElement) elem.getElement(currentLine - 1)).isBrokenString()) {
           yybegin(QSTRING);
        }
    }

    public int yychar() {
        return yychar;
    }

    public int scan() throws IOException {
        int ret = yylex();
        if (start + yychar + yylength() == end - 1) {
           ((ScilabDocument.ScilabLeafElement) elem.getElement(elem.getElementIndex(start))).setBrokenString(breakstring);
           breakstring = false;
        }
        return ret;
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

           while (startL < pos && (s != startL || yystate() == BREAKSTRING)) {
               s = startL;
               tok = yylex();
               startL = start + yychar + yylength();
           }

           return tok;
        } catch (IOException e) {
           return ScilabLexerConstants.DEFAULT;
        }
     }

     public static ScilabTokens getScilabTokens(String str) {
     	 ScilabDocument doc = new ScilabDocument(false);
	 try {
	     doc.insertString(0, str, null);
         } catch (BadLocationException e) { }
	 return getScilabTokens(doc);
     }

     public static ScilabTokens getScilabTokens(ScilabDocument doc) {
     	 ScilabLexer lexer = new ScilabLexer(doc);
	 lexer.yyreset(new ScilabDocumentReader(doc, 0, doc.getLength()));
	 ScilabTokens tokens = new ScilabTokens();
	 int tok = -1;
	 try {
	    while (tok != ScilabLexerConstants.EOF) {
               tok = lexer.yylex();
    	       tokens.add(tok, lexer.yychar + lexer.yylength());
	    }
	 } catch (IOException e) { }
 
	 return tokens;
     }

     public static class ScilabTokens {
        private List<Integer> tokenType = new ArrayList<Integer>();
        private List<Integer> tokenPos = new ArrayList<Integer>();

	ScilabTokens() { }

	void add(final int type, final int pos) {
	   tokenType.add(type);
	   tokenPos.add(pos);
	}

	public final List<Integer> getTokenType() {
	   return tokenType;
	}

	public final List<Integer> getTokenPos() {
	   return tokenPos;
	}
     }
%}

/* main character classes */
eol = \n

open = "[" | "(" | "{"
close = "]" | ")" | "}"

comment = "//"

quote = "'"

dquote = "\""

cstes = "%t" | "%T" | "%f" | "%F" | "%e" | "%pi" | "%inf" | "%i" | "%z" | "%s" | "%nan" | "%eps" | "SCI" | "WSCI" | "SCIHOME" | "TMPDIR"

operator = ".'" | ".*" | "./" | ".\\" | ".^" | ".**" | "+" | "-" | "/" | "\\" | "*" | "^" | "**" | "==" | "~=" | "<>" | "<" | ">" | "<=" | ">=" | ".*." | "./." | ".\\." | "/." | "=" | "&" | "|" | "@" | "@=" | "~"

functionKwds = "function" | "endfunction"

structureKwds = "then" | "do" | "catch" | "case"

elseif = "elseif" | "else"

openCloseStructureKwds = "if" | "for" | "while" | "try" | "select" | "end"

controlKwds = "abort" | "break" | "quit" | "return" | "resume" | "pause" | "continue" | "exit"

authors = "Calixte Denizet" | "Calixte DENIZET" | "Sylvestre Ledru" | "Sylvestre LEDRU" | "Antoine Elias" | "Antoine ELIAS" | "Bruno Jofret" | "Bruno JOFRET" | "Claude Gomez" | "Claude GOMEZ" | "Clement David" | "Clement DAVID" | "Manuel Juliachs" | "Manuel JULIACHS" | "Sheldon Cooper" | "Leonard Hofstadter" | "Serge Steer" | "Serge STEER" | "Vincent Couvert" | "Vincent COUVERT" | "Adeline Carnis" | "Adeline CARNIS" | "Charlotte Hecquet" | "Charlotte HECQUET" | "Paul Bignier" | "Paul BIGNIER" | "Alexandre Herisse" | "Alexandre HERISSE" | "Simon Marchetto" | "Simon MARCHETTO" | "Vladislav Trubkin" | "Vladislav TRUBKIN" | "Cedric Delamarre" | "Cedric DELAMARRE" | "Inria" | "INRIA" | "DIGITEO" | "Digiteo" | "Scilab Enterprises" | "ENPC"

error = "Scilab Entreprises" | "Scilab Entreprise" | "Scilab Enterprise"
todo = ("TODO" | "todo" | "Todo")[ \t]*:[^\n]*

break = ".."(".")*
breakinstring = {break}[ \t]*({comment} | {eol})

special = "$" | ":" | {break}

string = (([^ \t\'\"\r\n\.]*)|([\'\"]{2}))+

id = ([a-zA-Z%_#!?][a-zA-Z0-9_#!$?]*)|("$"[a-zA-Z0-9_#!$?]+)

badid = ([0-9$][a-zA-Z0-9_#!$?]+)
whitabs = (" "+"\t" | "\t"+" ")[ \t]*
badop = [+-]([\*\/\\\^] | "."[\*\+\-\/\\\^]) | ":=" | "->" | ("="[ \t]*">") | ("="[ \t]*"<") | " !=" | "&&" | "||" | ([*+-/\\\^]"=")

dot = "."

url = ("http://"|"https://"|"ftp://"|"dav://"|"davs://"|"sftp://"|"ftps://"|"smb:///"|"file://")[^ \t\f\n\r\'\"]+
mailaddr = [ \t]*[a-zA-Z0-9_\.\-]+"@"([a-zA-Z0-9\-]+".")+[a-zA-Z]{2,5}[ \t]*
mail = ("<" {mailaddr} ">") | ("mailto:" {mailaddr})

latex = "$$"(([^$]*|"\\$")+)"$$"
latexinstring = (\"|\')"$"(([^$\'\"]*|"\\$"|([\'\"]{2}))+)"$"(\"|\')

digit = [0-9]
exp = [dDeE][+-]?{digit}*
number = ({digit}+"."?{digit}*{exp}?)|("."{digit}+{exp}?)

arabic_char = [\u0600-\u06FF\u0750-\u077F\uFB50-\uFDFF\uFE70-\uFEFF]
hebrew_char = [\u0590-\u05FF\uFB1D-\uFB4F]
nko_char = [\u07C0-\u07FF]
thaana_char = [\u0780-\u07BF]
rtl_char = {arabic_char}|{hebrew_char}|{nko_char}|{thaana_char}
rtl_comment = {rtl_char}[^\n]*
rtl_in_string = {rtl_char}(([^\'\"\r\n\.]*)|([\'\"]{2}))+

%x QSTRING, COMMENT, FIELD, COMMANDS, COMMANDSWHITE, BREAKSTRING

%%

<YYINITIAL> {
  {comment}                      {
                                   transposable = false;
                                   yypushback(2);
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

  {openCloseStructureKwds}       {
                                   transposable = false;
                                   return ScilabLexerConstants.OSKEYWORD;
                                 }

  {structureKwds}                {
                                   transposable = false;
                                   return ScilabLexerConstants.SKEYWORD;
                                 }

  {elseif}                       {
                                   transposable = false;
                                   return ScilabLexerConstants.ELSEIF;
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
                                   } else if (macros.contains(str)) {
                                       yybegin(COMMANDS);
                                       return ScilabLexerConstants.MACROS;
                                   } else if (infile.contains(str)) {
                                       yybegin(COMMANDS);
                                       return ScilabLexerConstants.MACROINFILE;
                                   } else {
                                       List<String>[] arr = doc.getInOutArgs(start + yychar);
                                       if (arr != null && (arr[0].contains(str) || arr[1].contains(str))) {
                                           return ScilabLexerConstants.INPUTOUTPUTARGS;
                                       } else if (variables.contains(str)) {
                                           return ScilabLexerConstants.VARIABLES;
                                       }
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

  {latexinstring}                {
                                   return ScilabLexerConstants.LATEXINSTRING;
                                 }

  {quote}                        {
                                    if (transposable) {
                                       return ScilabLexerConstants.TRANSP;
                                    } else {
                                       beginString = zzStartRead;
                                       yybegin(QSTRING);
                                       return ScilabLexerConstants.STRING;
                                    }
                                 }

  {open}                         {
                                   transposable = false;
                                   return ScilabLexerConstants.OPENCLOSE;
                                 }

  {close}                        {
                                   transposable = true;
                                   return ScilabLexerConstants.OPENCLOSE;
                                 }

  {dquote}                       {
                                   transposable = false;
                                   beginString = zzStartRead;
                                   yybegin(QSTRING);
                                   return ScilabLexerConstants.STRING;
                                 }

  {badid}                        |
  {badop}                        |
  {whitabs}                      {
                                   return ScilabLexerConstants.ERROR;
                                 }

  " "                            {
    				   transposable = false;
                                   return ScilabLexerConstants.WHITE;
                                 }

  "\t"                           {
    				   transposable = false;
                                   return ScilabLexerConstants.TAB;
                                 }

  .                              |
  {eol}                          {
                                   transposable = false;
                                   return ScilabLexerConstants.DEFAULT;
                                 }
}

<COMMANDS> {
  [ \t]*"("                      {
                                   yypushback(yylength());
                                   yybegin(YYINITIAL);
                                 }

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
  {comment}                      {
                                   transposable = false;
                                   yypushback(2);
                                   yybegin(COMMENT);
                                 }

  ([^ \t,;/]*) | ("/"[^ /]*)     {
                                   return ScilabLexerConstants.STRING;
                                 }

  " "                            {
                                   return ScilabLexerConstants.WHITE;
                                 }

  "\t"                           {
                                   return ScilabLexerConstants.TAB;
                                 }

  .                              |
  {eol}				 {
                                   yypushback(1);
                                   yybegin(YYINITIAL);
                                 }
}

<FIELD> {
  {id}                           {
                                   transposable = true;
                                   return ScilabLexerConstants.FIELD;
                                 }

  .                              |
  {eol}				 {
                                   yypushback(1);
                                   yybegin(YYINITIAL);
                                 }
}

<QSTRING> {
  {breakinstring}                {
                                   yypushback(yylength());
                                   yybegin(BREAKSTRING);
                                   transposable = false;
                                   return ScilabLexerConstants.STRING;
                                 }

  " "                            {
                                   return ScilabLexerConstants.WHITE_STRING;
                                 }

  "\t"                           {
                                   return ScilabLexerConstants.TAB_STRING;
                                 }

  {rtl_in_string}                |
  {string}                       |
  "."                            {
                                   return ScilabLexerConstants.STRING;
                                 }

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
  {todo}			 {
  				   return ScilabLexerConstants.TODO;
				 }

  {error}			 {
  				   return ScilabLexerConstants.ERROR;
				 }


  {authors}                      {
                                   return ScilabLexerConstants.AUTHORS;
                                 }

  {url}                          {
                                   return ScilabLexerConstants.URL;
                                 }

  {mail}                         {
                                   return ScilabLexerConstants.MAIL;
                                 }

  {latex}                        {
                                   return ScilabLexerConstants.LATEX;
                                 }

  " "                            {
                                   return ScilabLexerConstants.WHITE_COMMENT;
                                 }

  "\t"                           {
                                   return ScilabLexerConstants.TAB_COMMENT;
                                 }
 
  {rtl_comment}                  {
                                    return ScilabLexerConstants.COMMENT;
                                 }

  [^ \t\n]+                      {
                                   return ScilabLexerConstants.COMMENT;
                                 }

  {eol}				 {
  				   yybegin(YYINITIAL);
                                   return ScilabLexerConstants.DEFAULT;
                                 }
}

<BREAKSTRING> {
  {break}                        {
                                   breakstring = true;
                                   return ScilabLexerConstants.SPECIAL;
                                 }

  " "                            {
                                   return ScilabLexerConstants.WHITE;
                                 }

  "\t"                           {
                                   return ScilabLexerConstants.TAB;
                                 }

  {comment}                      {
                                   transposable = false;
                                   yypushback(2);
                                   yybegin(COMMENT);
                                 }

  .                              |
  {eol}                          {
                                   return ScilabLexerConstants.DEFAULT;
                                 }
}

<<EOF>>                          {
                                   return ScilabLexerConstants.EOF;
                                 }
