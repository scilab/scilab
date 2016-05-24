//CHECKSTYLE:OFF

package org.scilab.modules.scinotes;

import java.io.IOException;
import javax.swing.text.Element;

@javax.annotation.Generated("JFlex")
@SuppressWarnings("fallthrough")
%%

%public
%class MatchingBlockScanner
%final
%unicode
%char
%type int
%buffer 256

%{
    private ScilabDocument doc;
    private Element elem;
    private boolean transp = false;
    private ScilabLexer scilabLexer;
    private int start;
    private int end;
    private int savePos;

    public MatchingBlockScanner(ScilabDocument doc) {
        this.doc = doc;
        this.elem = doc.getDefaultRootElement();
        this.scilabLexer = new ScilabLexer(doc, null, false);
    }

    public MatchingPositions getNextBlock(int pos, boolean lr) {
        int s = 1;
        transp = false;
	try {
      	    if (lr) {
                start =  pos;
                end = doc.getEndPosition().getOffset();
                yyreset(new ScilabDocumentReader(doc, start, end));
                yybegin(LR);
	    } else {
                start = pos - 1;
                end = 0;
                yyreset(new ScilabDocumentReader(doc, true, start, end));
              	yybegin(RL);
	    }
	    do {
               if (yylex() == 0) {
                  s--;
               } else {
                  s++;
               }
            } while (zzMarkedPos != 0 && s != 0);
        } catch (IOException e) {
            return null;
        }
        if (s == 0) {
            if (lr) {
                return new MatchingPositions(pos, pos, pos + yychar, pos + yychar + (transp?(yylength()-1):yylength()));
            } else {
                return new MatchingPositions(pos, pos, pos - yychar - yylength(), pos - yychar);
            }
        }

        return null;
    }

    public MatchingPositions getMatchingBlock(int pos, boolean lr) {
        int p1, s = 1;
        transp = false;
        try {
            if (lr) {
                start =  pos;
                end = doc.getEndPosition().getOffset();
                yyreset(new ScilabDocumentReader(doc, start, end));
                yybegin(OPENCLOSE);
                if (yylex() != 1) {
                   return null;
                }

                p1 = pos + yylength();
                yybegin(LR);
            } else {
                 start = pos - 1;
                 end = 0;
                 yyreset(new ScilabDocumentReader(doc, true, start, end));
                 yybegin(CLOSEOPEN);
                 if (yylex() != 1) {
                    return null;
                 }
                 p1 = pos - yylength();
                 yybegin(RL);
            }

            do {
               if (yylex() == 0) {
                  s--;
               } else {
                  s++;
               }
            } while (zzMarkedPos != 0 && s != 0);
        } catch (IOException e) {
            return null;
        }
        if (s == 0) {
            if (lr) {
                return new MatchingPositions(pos, p1, pos + yychar, pos + yychar + (transp?(yylength()-1):yylength()));
            } else {
                return new MatchingPositions(p1, pos, pos - yychar - yylength(), pos - yychar);
            }
        }

        return null;
    }

    public final class MatchingPositions {
        public int firstB;
        public int firstE;
        public int secondB;
        public int secondE;

        private MatchingPositions(int x1, int x2, int y1, int y2) {
            firstB = x1;
            firstE = x2;
            secondB = y1;
            secondE = y2;
        }
    }
%}

%eofval{
  return -1;
%eofval}

/* main character classes */
eol = \n

comment = "//".*{eol}

tnemmoc = {eol}([^\r\n]*"//")+

spec = [a-zA-Z0-9_#!$?]

id = [a-zA-Z%_#!?][a-zA-Z0-9_#!$?]*

string = (([^\'\"\r\n]*)|([\'\"]{2}))*
qstring = (\"|\'){string}(\"|\')
gnirtsq = \"{string}(\"|\')
transp = ({spec} | ")" | "]" | "}") "'"

openK = ("if" | "for" | "while" | "select" | "try" | "function" | "switch")
openKx = {openK}{spec}+

elseif = "elseif" | "else"

openS = "(" | "[" | "{"
closeK = ("end" | "endfunction")
closeKx = {closeK}{spec}+
xcloseK = ({spec} | "%" ){closeK}
closeS =  ")" | "]" | "}"

esolcK = ("fi" | "rof" | "elihw" | "tceles" | "yrt" | "noitcnuf" | "hctiws")
esolcKx = {spec}{esolcK}
xesolcK = {esolcK}{spec}
nepoK = ("dne" | "noitcnufdne")
nepoKx = {spec}{nepoK}

%x LR, RL, OPENCLOSE, CLOSEOPEN, PSNART, SPEC

%%

<LR> {
  {transp}                       {
                                   char c = yycharat(yylength() - 2);
                                   if (c == ')' || c == ']' || c == '}') {
                                      transp = true;
                                      return 0;
                                   }
                                 }

  "elseif"                       |
  {comment}                      |
  {openKx}                       |
  {qstring}                      { }

  {openS}                        |
  {openK}                        {
                                   return 1;
                                 }

  {closeS}                       |
  {closeK}                       {
                                   return 0;
                                 }

  {id}                           |
  {closeKx}                      |
  {xcloseK}                      |
  .                              |
  {eol}                          { }
}

<RL> {
  \'                             {
                                   yypushback(1);
                                   yybegin(PSNART);
                                 }

  "fiesle"                       |
  {tnemmoc}                      |
  {esolcKx}                      |
  {xesolcK}                      |
  {gnirtsq}                      { }

  {closeS}                       |
  {nepoK}                        {
                                   return 1;
                                 }

  {openS}                        |
  {esolcK}                       {
                                   return 0;
                                 }

  {nepoKx}                       |
  .                              |
  {eol}                          { }
}

<PSNART> {
  \'                             {
                                   if (scilabLexer.getKeyword(start - yychar, false) == ScilabLexerConstants.STRING) {
                                      savePos = start - yychar - scilabLexer.beginString - scilabLexer.start;
                                      yybegin(SPEC);
                                   } else {
                                      yybegin(RL);
                                   }
                                 }
}

<SPEC> {
  .                              |
  {eol}                          {
                                   if (--savePos == 0) {
                                      yybegin(RL);
                                   }
                                 }
}

<OPENCLOSE> {
  {openKx}                       {
                                   return 0;
                                 }

  {elseif}                       |
  {openS}                        |
  {openK}                        {
                                   return 1;
                                 }

  .                              |
  {eol}                          {
                                   return 0;
                                 }
}

<CLOSEOPEN> {
  {nepoKx}                       {
                                   return 0;
                                 }

  {closeS}                       |
  {nepoK}                        {
                                   return 1;
                                 }

  .                              |
  {eol}                          {
                                   return 0;
                                 }
}
