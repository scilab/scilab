//CHECKSTYLE:OFF

package org.scilab.modules.scinotes;

import java.io.IOException;
import java.util.List;
import java.util.ArrayList;
import javax.swing.text.BadLocationException;
import javax.swing.text.Element;

@javax.annotation.Generated("JFlex")
%%

%public
%class FunctionScanner
%final
%unicode
%char
%type int

%{
    List<String> returnValues;
    List<String> argsValues;
    String functionName;
    int end;
    int start;

    private ScilabDocument doc;
    private String id;
    private MatchingBlockScanner matchBlock;

    public FunctionScanner(ScilabDocument doc) {
        this.doc = doc;
        returnValues = new ArrayList<>();
        argsValues = new ArrayList<>();
	this.matchBlock = new MatchingBlockScanner(doc);
    }

    public int yychar() {
        return (int) yychar;
    }

    public String getFunctionName() {
        return functionName;
    }

    public List<String> getReturnValues() {
        return returnValues;
    }

    public List<String> getArgsValues() {
        return argsValues;
    }

    public int getLineType(int p0, int p1) {
        try {
            returnValues = new ArrayList<>();
            argsValues = new ArrayList<>();
	    start = p0;
            end = p1;
            yyreset(new ScilabDocumentReader(doc, p0, p1));
            yybegin(BROKEN);
            int broken = yylex();

            int implicitEnd = getImplicitEndFunctionType(p0, p1);
            if (implicitEnd != ScilabDocument.ScilabLeafElement.NOTHING) {
                return implicitEnd | broken;
            }

            yyreset(new ScilabDocumentReader(doc, p0, p1));
            while (true) {
                int ret = yylex();
                if (ret != ScilabDocument.ScilabLeafElement.BROKEN) {
                   return ret | broken;
                }
                Element elem = doc.getDefaultRootElement();
                elem = elem.getElement(elem.getElementIndex(end + 1));
                end = elem.getEndOffset();
                yyreset(new ScilabDocumentReader(doc, elem.getStartOffset(), end));
                yybegin(ARGS);
            }
        } catch (IOException e) {
            return ScilabDocument.ScilabLeafElement.NOTHING;
        }
    }

    /**
     * Detect an indented implicit function end. The regular lexer rule consumes
     * indentation one character at a time, which makes its matching-block
     * position unreliable. This is notably visible for methods in classdef.
     */
    private int getImplicitEndFunctionType(int p0, int p1) {
        try {
            String line = doc.getText(p0, p1 - p0);
            int offset = 0;
            while (offset < line.length() && (line.charAt(offset) == ' ' || line.charAt(offset) == '\t')) {
                offset++;
            }

            if (!line.startsWith("end", offset)) {
                return ScilabDocument.ScilabLeafElement.NOTHING;
            }

            int afterEnd = offset + 3;
            if (afterEnd < line.length()) {
                char next = line.charAt(afterEnd);
                if (Character.isLetterOrDigit(next) || "_#!$?".indexOf(next) >= 0) {
                    return ScilabDocument.ScilabLeafElement.NOTHING;
                }
            }

            MatchingBlockScanner.MatchingPositions pos = matchBlock.getMatchingBlock(p0 + afterEnd, false);
            if (pos != null) {
                String match = doc.getText(pos.secondB, pos.secondE - pos.secondB);
                if (match.equals("function")) {
                    return ScilabDocument.ScilabLeafElement.ENDFUN;
                }
            }
        } catch (BadLocationException e) { }

        return ScilabDocument.ScilabLeafElement.NOTHING;
    }

    public FunctionInfo getFunctionInfo() {
        return new FunctionInfo(functionName, returnValues, argsValues);
    }

    public class FunctionInfo {
        public String functionName;
        public List<String> returnValues;
        public List<String> argsValues;

        public FunctionInfo(String name, List<String> ret, List<String> args) {
            this.functionName = name;
            this.returnValues = ret;
            this.argsValues = args;
        }
    }
%}

%eofval{
  return ScilabDocument.ScilabLeafElement.NOTHING;
%eofval}

/* main character classes */
white = [ \t]+
eol = \n

comments = {white}*("//".*)?{eol}
break = ".."(".")*{comments}

brokenline = ([^\.]* | ([^\.]*"."[^\.]+)+){break}

id = [a-zA-Z%_#!$?][a-zA-Z0-9_#!$?]*
spec = [^a-zA-Z0-9_#!$?]?

equal = {white}* "=" {white}*

rpar = ")"{comments}

fun = {white}* "function" {white}
funb = {white}* "function["
endfun = {white}* "endfunction" {spec}
end = "end"

%x FUNCTION, TYPEID, FUNNAME, RETS, ARGS, BROKEN

%%

<YYINITIAL> {
  {funb}                         {
                                   yybegin(RETS);
                                 }

  {fun}                          {
                                   yybegin(FUNCTION);
                                 }

  {endfun}                       {
  				   return ScilabDocument.ScilabLeafElement.ENDFUN;
			         }

  {end}				 {
                                   int afterEnd = start + yychar() + yylength();
                                   if (afterEnd < doc.getLength()) {
                                      try {
                                         char next = doc.getText(afterEnd, 1).charAt(0);
                                         if (Character.isLetterOrDigit(next) || "_#!$?".indexOf(next) >= 0) {
                                            return ScilabDocument.ScilabLeafElement.NOTHING;
                                         }
                                      } catch (BadLocationException e) { }
                                   }
				   MatchingBlockScanner.MatchingPositions pos = matchBlock.getMatchingBlock(afterEnd, false);
				   if (pos != null) {
				      try {
				      	 String match = doc.getText(pos.secondB, pos.secondE - pos.secondB);
				      	 if (match.equals("function")) {
					    return ScilabDocument.ScilabLeafElement.ENDFUN;
					 }
				      } catch (BadLocationException e) { }
				   }
                                 }

  .                              |
  {eol}                          {
                                   return ScilabDocument.ScilabLeafElement.NOTHING;
                                 }
}

<BROKEN> {
  {brokenline}                   {
                                   return ScilabDocument.ScilabLeafElement.BROKEN;
                                 }

  .                              |
  {eol}                          {
                                   return 0;
                                 }
}

<FUNCTION> {
  "["                            {
                                   yybegin(RETS);
                                 }

  {id}                           {
                                   id = yytext();
                                   yybegin(TYPEID);
                                 }

  .                              |
  {eol}                          {
                                   return ScilabDocument.ScilabLeafElement.NOTHING;
                                 }
}

<TYPEID> {
  {equal}                        {
                                   returnValues.add(id);
                                   yybegin(FUNNAME);
                                 }

  "("                            {
                                   functionName = id;
                                   yybegin(ARGS);
                                 }

  {white}                        { }

  {comments}                     {
                                   functionName = id;
                                   return ScilabDocument.ScilabLeafElement.FUN;
                                 }

  .                              {
                                   return ScilabDocument.ScilabLeafElement.NOTHING;
                                 }
}

<FUNNAME> {
  {id}                           {
                                   functionName = yytext();
                                 }

  [ \t]*"("                      {
                                   yybegin(ARGS);
                                 }

  {comments}                     {
                                   return ScilabDocument.ScilabLeafElement.FUN;
                                 }

  .                              {
                                   return ScilabDocument.ScilabLeafElement.NOTHING;
                                 }
}

<RETS> {
  {id}                           {
                                   returnValues.add(yytext());
                                 }

  ","                            |
  {white}                        { }

  "]"{equal}                     {
                                   yybegin(FUNNAME);
                                 }

  .                              |
  {eol}                          {
                                   return ScilabDocument.ScilabLeafElement.NOTHING;
                                 }
}

<ARGS> {
  {id}                           {
                                   argsValues.add(yytext());
                                 }

  ","                            |
  {white}                        { }

  {break}                        {
                                   return ScilabDocument.ScilabLeafElement.BROKEN;
                                 }

  {rpar}                         {
                                   return ScilabDocument.ScilabLeafElement.FUN;
                                 }

  .                              |
  {eol}                          {
                                   return ScilabDocument.ScilabLeafElement.NOTHING;
                                 }
}
