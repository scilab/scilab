//CHECKSTYLE:OFF

package org.scilab.modules.scinotes;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
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
    private boolean breakcomment;
    private MatchingBlockScanner matchBlock;

    static {
	// For SciNotes colors in preferences
	commands.add("cos");
	macros.add("sind");
    }

    public ScilabLexer(ScilabDocument doc) {
	this(doc, new MatchingBlockScanner(doc), true);
    }

    public ScilabLexer(ScilabDocument doc, boolean update) {
	this(doc, new MatchingBlockScanner(doc), update);
    }

    public ScilabLexer(ScilabDocument doc, MatchingBlockScanner matchBlock, boolean update) {
        this.doc = doc;
        this.elem = doc.getDefaultRootElement();
        this.infile = doc.getFunctionsInDoc();
	this.matchBlock = matchBlock;
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
        if (currentLine != 0) {
	   ScilabDocument.ScilabLeafElement e = (ScilabDocument.ScilabLeafElement) elem.getElement(currentLine - 1);
	   if (e.isBrokenString()) {
              yybegin(QSTRING);
	   } else if (e.isBlockComment()) {
	      yybegin(BLOCKCOMMENT);
	   }
        }
    }

    public int yychar() {
        return yychar;
    }

    public int scan() throws IOException {
        int ret = yylex();
	int lastPos = start + yychar + yylength();
        if (lastPos == end - 1) {
           ((ScilabDocument.ScilabLeafElement) elem.getElement(elem.getElementIndex(start))).setBrokenString(breakstring);
           breakstring = false;
        } else if (lastPos == end) {
	   ((ScilabDocument.ScilabLeafElement) elem.getElement(elem.getElementIndex(start))).setBlockComment(yystate() == BLOCKCOMMENT);
        }
	return ret;
    }

    public boolean isLineFinishedByBlockComment(int start, int end) {
        this.start = start;
	this.end = end;
	try {		
           yyreset(new ScilabDocumentReader(doc, start, end));
	   int tok = 0;
	   while (tok != ScilabLexerConstants.EOF) {
	      tok = yylex();
	   }
        } catch (Exception e) { }
	
	return yystate() == BLOCKCOMMENT;
    }

    public int getKeyword(int pos, boolean strict) {
        // Pre condition
        if (elem == null) {
            return ScilabLexerConstants.DEFAULT;
        }

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
startcomment = ("/" "*"+)
endcomment = ("*"+) "/"

quote = "'"

dquote = "\""

cstes = "%t" | "%T" | "%f" | "%F" | "%e" | "%pi" | "%inf" | "%i" | "%z" | "%s" | "%nan" | "%eps" | "SCI" | "WSCI" | "SCIHOME" | "TMPDIR"

operator = ".'" | ".*" | "./" | ".\\" | ".^" | ".**" | "+" | "-" | "/" | "\\" | "*" | "^" | "**" | "==" | "~=" | "<>" | "<" | ">" | "<=" | ">=" | ".*." | "./." | ".\\." | "/." | "=" | "&" | "|" | "@" | "@=" | "~" | "&&" | "||"

functionKwds = "function" | "endfunction"

structureKwds = "then" | "do" | "catch" | "case" | "otherwise"

elseif = "elseif" | "else"

openCloseStructureKwds = "if" | "for" | "while" | "try" | "select" | "switch"

end = "end"

controlKwds = "abort" | "break" | "quit" | "return" | "resume" | "pause" | "continue" | "exit"

authors = "Calixte Denizet" | "Calixte DENIZET" | "Sylvestre Ledru" | "Sylvestre LEDRU" | "Yann Collette" | "Yann COLLETTE" | "Allan Cornet" | "Allan CORNET" | "Allan Simon" | "Allan SIMON" | "Antoine Elias" | "Antoine ELIAS" | "Bernard Hugueney" | "Bernard HUGUENEY" | "Bruno Jofret" | "Bruno JOFRET" | "Claude Gomez" | "Claude GOMEZ" | "Clement David" | "Clement DAVID" | "Jerome Picard" | "Jerome PICARD" | "Manuel Juliachs" | "Manuel JULIACHS" | "Michael Baudin" | "Michael BAUDIN" | "Pierre Lando" | "Pierre LANDO" | "Pierre Marechal" | "Pierre MARECHAL" | "Serge Steer" | "Serge STEER" | "Vincent Couvert" | "Vincent COUVERT" | "Vincent Liard" | "Vincent LIARD" | "Zhour Madini-Zouine" | "Zhour MADINI-ZOUINE" | "Vincent Lejeune" | "Vincent LEJEUNE" | "Sylvestre Koumar" | "Sylvestre KOUMAR" | "Simon Gareste" | "Simon GARESTE" | "Cedric Delamarre" | "Cedric DELAMARRE" | "Inria" | "INRIA" | "DIGITEO" | "Digiteo" | "ENPC"

break = ".."(".")*
breakinstring = {break}[ \t]*{comment}?

special = "$" | ":" | {break}

string = (([^ \t\'\"\r\n\.]*)|([\'\"]{2}))+

id = ([a-zA-Z%_#!?][a-zA-Z0-9_#!$?]*)|("$"[a-zA-Z0-9_#!$?]+)

badid = ([0-9$][a-zA-Z0-9_#!$?]+)
whitabs = (" "+"\t" | "\t"+" ")[ \t]*
badop = [+-]([\*\/\\\^] | "."[\*\+\-\/\\\^]) | ":=" | "->" | " !=" | ("&&" "&"+) | ("||" "|"+) | ([*+-/\\\^]"=")

dot = "."

url = "http://"[^ \t\f\n\r\'\"]+
mail = "<"[ \t]*[a-zA-Z0-9_\.\-]+"@"([a-zA-Z0-9\-]+".")+[a-zA-Z]{2,5}[ \t]*">"

latex = "$"(([^$]*|"\\$")+)"$"
latexinstring = (\"|\')"$"(([^$\'\"]*|"\\$"|([\'\"]{2}))+)"$"(\"|\')

digit = [0-9]
exp = [dDeE][+-]?{digit}*
number = ({digit}+"."?{digit}*{exp}?)|("."{digit}+{exp}?)

%x QSTRING, COMMENT, BLOCKCOMMENT, FIELD, COMMANDS, COMMANDSWHITE, BREAKSTRING

%%

<YYINITIAL> {
  {comment}                      {
                                   transposable = false;
                                   yypushback(2);
                                   yybegin(COMMENT);
                                 }

  {startcomment}                 {
                                   transposable = false;
                                   yypushback(2);
                                   yybegin(BLOCKCOMMENT);
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

  {end}       			 {
                                   transposable = false;
				   if (matchBlock != null) {
				      MatchingBlockScanner.MatchingPositions pos = matchBlock.getMatchingBlock(start + yychar + yylength(), false);
				      if (pos != null) {
				         try {
				      	     String match = doc.getText(pos.secondB, pos.secondE - pos.secondB);
				      	     if (match.equals("function")) {
					        return ScilabLexerConstants.FKEYWORD;
					     }
				      	 } catch (BadLocationException e) { }
				      }
				   }
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
                                   return ScilabLexerConstants.LATEX;
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
  .
                                 {
                                   yypushback(1);
                                   yybegin(YYINITIAL);
                                 }

  {eol}                          { }
}

<FIELD> {
  {id}                           {
                                   transposable = true;
                                   return ScilabLexerConstants.FIELD;
                                 }

  .                              {
                                   yypushback(1);
                                   yybegin(YYINITIAL);
                                 }

  {eol}                          { }
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

<BLOCKCOMMENT> {
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

  {endcomment}			 {
				   yybegin(YYINITIAL);
				   return ScilabLexerConstants.COMMENT;
  				 }

  .                              |
  {eol}                          {
                                   return ScilabLexerConstants.COMMENT;
                                 }
}

<COMMENT> {
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

  .                              |
  {eol}                          {
                                   return ScilabLexerConstants.COMMENT;
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
