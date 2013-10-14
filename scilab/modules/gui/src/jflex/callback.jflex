// jflex --nobak -d ../java/org/scilab/modules/gui/uiwidget/callback callback.jflex

//CHECKSTYLE:OFF

package org.scilab.modules.gui.uiwidget.callback;

import java.io.IOException;
import java.io.StringReader;

@javax.annotation.Generated("JFlex")
@SuppressWarnings("fallthrough")
%%

%public
%class CallbackParser
%final
%unicode
%char
%type UICallback.Type

%switch

%{
    public static final UICallback.Type guessCallbackType(final String code) {
    	try {
	   CallbackParser parser = new CallbackParser(new StringReader(code));
	   parser.yybegin(YYINITIAL);
	   UICallback.Type type = parser.yylex();
	   if (parser.yylength() != code.length()) {
	      type = UICallback.Type.SCI_CODE;
	   }

	   parser.yyclose();

	   return type;
	} catch (IOException e) {
	   return UICallback.Type.SCI_CODE;
	}
    }
%}

%eofval{
  return UICallback.Type.SCI_CODE;
%eofval}

/* main character classes */
scilab = ([a-zA-Z%_#!?][a-zA-Z0-9_#!$?]*)|("$"[a-zA-Z0-9_#!$?]+)
java = ([a-zA-Z]+".")+[a-zA-Z_]+
url = ("http"|"file"|"https"|"ftp"|"mailto")"://".*

%%

<YYINITIAL> {
  {scilab}                       {
  				    return UICallback.Type.SCI_FUN;
                                 }

  {java}                         {
			            return UICallback.Type.JAVA;
                                 }

  {url}			         {
			            return UICallback.Type.URL;
  				 }

  .                              {
  				    return UICallback.Type.SCI_CODE;
                                 }
}
