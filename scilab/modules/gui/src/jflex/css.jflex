// jflex --nobak -d ../java/org/scilab/modules/gui/uiwidget/ css.jflex

//CHECKSTYLE:OFF

package org.scilab.modules.gui.uiwidget;

import java.io.IOException;
import java.io.File;
import java.io.FileReader;
import java.io.Reader;
import java.io.StringReader;
import java.util.HashMap;
import java.util.Map;

@javax.annotation.Generated("JFlex")
@SuppressWarnings("fallthrough")
%%

%public
%class CSSParser
%final
%unicode
%char
%type int

%switch

%{
    private String identifier;
    private String key;
    private String value;
    private Map<String, Map<String, String>> map;
    private Map<String, String> kv;
    
    public static Map<String, Map<String, String>> parseFile(String file) throws CSSParserException {
        try {
	   return parse(new FileReader(new File(file)));
        } catch (IOException e) {
   	    throw new CSSParserException(e.getMessage());  
        }
    }

    public static Map<String, Map<String, String>> parseBlock(String block) throws CSSParserException {
    	return parse(new StringReader(block));
    }

    private static Map<String, Map<String, String>> parse(Reader reader) throws CSSParserException {
        try {
	    CSSParser parser = new CSSParser(reader);
	    parser.map = new HashMap<String, Map<String, String>>();
            parser.yybegin(YYINITIAL);
            int ret = parser.yylex();

	    if (ret == -1) {
	       parser.yyclose();
	       throw new CSSParserException("Invalid CSS file");  
	    }

	    parser.yyclose();
	       
	    return parser.map;
        } catch (IOException e) {
	    throw new CSSParserException(e.getMessage());  
        }
    }

    public static Map<String, String> parseLine(String str) throws CSSParserException {
        try {
	    CSSParser parser = new CSSParser(new StringReader(str));
	    parser.kv = new HashMap<String, String>();
            parser.yybegin(LINE);
            int ret = parser.yylex();

	    if (ret == -1) {
	       parser.yyclose();
	       throw new CSSParserException("Invalid CSS instructions");  
	    }

	    parser.yyclose();

	    return parser.kv;
        } catch (IOException e) {
	    throw new CSSParserException(e.getMessage());  
        }
    }

    private void openBlock() {
        kv = new HashMap<String, String>();
        map.put(identifier, kv);
    }

    private void closeBlock() {
        kv = null;
    }

    public void pushKeyValue() {
        kv.put(key.toLowerCase(), value);
	key = null;
	value = null;        
    }
%}

%eofval{
  return 0;
%eofval}

/* main character classes */
white = [ \f\t\r\n]*
eol = \n | \r\n | \r | \f

ident = [.#]?[a-zA-Z]*
key = [a-zA-Z0-9_-]*
colon = ":"{white}
semicolon = {white}";"
value = [^ \t:;\r\n]({white}[^ \t:;\r\n])*

comment = \/\*[^*]*\*+([^/*][^*]*\*+)*\/

blockBeg = {white} "{" {white}
blockEnd = {white} "}" {white}

%x BLOCK, VALUE, LINE, VALUELINE

%%

<YYINITIAL> {
  {white}                        |
  {comment}			 {

                                 }

  {ident}                        {
			           identifier = yytext();
                                 }

  {blockBeg}			 {
  				   openBlock();
  				   yybegin(BLOCK);
  				 }

  .                              {
  				   return -1;
                                 }
}

<BLOCK> {
  {white}                        |
  {comment}		         {

  				 }

  {key}                          {
			           key = yytext();
                                 }

  {colon}     			 {
  				   yybegin(VALUE);
  				 }

  {blockEnd}                     {
				   closeBlock();
				   yybegin(YYINITIAL);	
 				 }

  .                              {
                                   return -1;
                                 }
}

<VALUE> {
  {value}                        {
                                   value = yytext();
				   pushKeyValue();
                                 }

  {semicolon} | {eol}            {
                                   yybegin(BLOCK);
  	      			 }

  .                              {
                                   return -1;
                                 }
}

<LINE> {
  {white}                        {

  				 }

  {key}                          {
			           key = yytext();
				 }

  {colon}        		 {
				   yybegin(VALUELINE);
  				 } 				  

  .                              {
				   return -1;
				 }
}

<VALUELINE> {
  {value}                        {
			           value = yytext();
				   pushKeyValue();
				 }

  {semicolon}			 {
				   yybegin(LINE);
  				 }

  .                              {
				   return -1;
				 }
}
