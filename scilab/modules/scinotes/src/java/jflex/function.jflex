//CHECKSTYLE:OFF

package org.scilab.modules.scinotes;

import java.io.IOException;
import java.util.List;
import java.util.ArrayList;
import javax.swing.text.Element;

@javax.annotation.Generated("JFlex")
@SuppressWarnings("fallthrough")
%%

%public
%class FunctionScanner
%final
%unicode
%char
%type int

%switch

%{
    List<String> returnValues;
    List<String> argsValues;
    String functionName;
    int end;

    private ScilabDocument doc;
    private String id;
    private boolean inRETS;

    public FunctionScanner(ScilabDocument doc) {
        this.doc = doc;
        returnValues = new ArrayList();
        argsValues = new ArrayList();
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
            returnValues = new ArrayList();
            argsValues = new ArrayList();
            end = p1;
            yyreset(new ScilabDocumentReader(doc, p0, p1));
            yybegin(BROKEN);
            int broken = yylex();

            yyreset(new ScilabDocumentReader(doc, p0, p1));
            while (true) {
                int ret = yylex();
                if (ret != ScilabDocument.ScilabLeafElement.BROKEN) {
                   return ret | broken;
                }
                Element elem = doc.getDefaultRootElement();
                int start = end + 1;
                elem = elem.getElement(elem.getElementIndex(end + 1));
                end = elem.getEndOffset();
                yyreset(new ScilabDocumentReader(doc, elem.getStartOffset(), end));
		if (inRETS) {
		   inRETS = false;
		   yybegin(RETS);
		} else {
                   yybegin(ARGS);
		}
            }
        } catch (IOException e) {
            return ScilabDocument.ScilabLeafElement.NOTHING;
        }
    }

    public FunctionInfo getFunctionInfo() {
        return new FunctionInfo(functionName, returnValues, argsValues);
    }

    public class FunctionInfo {
        public String functionName;
        public List<String> returnValues;
        public List<String> argsValues;

        public FunctionInfo(String name, List ret, List args) {
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

comments = ("//".*)?{eol}
break = ".."(".")*{white}*{comments}

brokenline = ([^\.]* | ([^\.]*"."[^\.]+)+){break}

id = [a-zA-Z%_#!$?][a-zA-Z0-9_#!$?]*
spec = [^a-zA-Z0-9_#!$?]

equal = {white}* "=" {white}*

rpar = ")"[,; \t]*{comments}

fun = {white}* "function" {white}
funb = {white}* "function["
endfun = {white}* "endfunction" {spec}

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

  {break}			 {
  				   inRETS = true;
  				   return ScilabDocument.ScilabLeafElement.BROKEN;
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
