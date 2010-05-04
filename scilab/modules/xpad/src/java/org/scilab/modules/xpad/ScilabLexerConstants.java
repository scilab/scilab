/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xpad;

import java.util.Map;
import java.util.HashMap;

/**
 * This class contains constants used in the lexer
 * @author Calixte DENIZET
 */
public class ScilabLexerConstants {
    
    /**
     * Number of known tokens
     */
    public static final int NUMBEROFTOKENS = 23;
    
    /**
     * DEFAULT : tokens which are not recognized
     */
    public static final int DEFAULT = 0;
    
    /**
     * OPERATOR : tokens like '+', '-', ...
     */
    public static final int OPERATOR = 1;
    
    /**
     * SKEYWORD : Structure keywords like 'if', 'for' or 'while'
     */
    public static final int SKEYWORD = 2;
    
    /**
     * CKEYWORD : Control keywords like 'abort', 'break' or 'return'
     */
    public static final int CKEYWORD = 3;
    
    /**
     * CONSTANTES : Constantes like '%pi' or '%T'
     */
    public static final int CONSTANTES = 4;
    
    /**
     * ID : Identifiers like 'myvar' or 'myfun'
     */
    public static final int ID = 5;
    
    /**
     * NUMBER : I don't know ;)
     */
    public static final int NUMBER = 6;
    
    /**
     * SPECIAL : Special keywords like '$', ':' or '...' 
     */
    public static final int SPECIAL = 7;
    
    /**
     * DOT : the dot in a field context, e.g. 'object.myfield' 
     */
    public static final int DOT = 8;
    
    /**
     * TRANSP : just the transpose operator
     */
    public static final int TRANSP = 9;
    
    /**
     * OPEN : an opening char like '(', '[' or '{'
     */
    public static final int OPEN = 10;
    
    /**
     * CLOSE : a closing char like ')', ']' or '}'
     */
    public static final int CLOSE = 11;
    
    /**
     * STRING : "bla bla bla" or 'bli bli bli' (or fucking exotic way to write a string)
     */
    public static final int STRING = 12;
    
    /**
     * COMMENT : Comments like // An interesting commentary
     */
    public static final int COMMENT = 13;
    
    /**
     * FKEYWORD : For keywords 'function' and 'endfunction'
     */
    public static final int FKEYWORD = 14;
    
    /**
     * COMMANDS : Functions in Scilab
     */
    public static final int COMMANDS = 15;
    
    /**
     * FIELD : Field of an object, e.g. myobject.myfield
     */
    public static final int FIELD = 16;
    
    /**
     * AUTHORS : Authors in Scilab 
     */
    public static final int AUTHORS = 17;
    
    /**
     * URL : http://...
     */
    public static final int URL = 18;
    
    /**
     * WHITE : A white char ' '
     */
    public static final int WHITE = 19;
    
    /**
     * TAB : A tabulation '\t'
     */
    public static final int TAB = 20;
    
    /**
     * LATEX : $\frac\pi\alpha$
     */
    public static final int LATEX = 21;
    
    /**
     * EOF : End Of File
     */
    public static final int EOF = 22;
    
    /**
     * TOKENS : A Map which contains the names of keywords (useful in xpadConfiguration.xml)
     */
    public static final Map<String, Integer> TOKENS = new HashMap();

    static {
	TOKENS.put("Default", DEFAULT);
	TOKENS.put("Special", SPECIAL);
	TOKENS.put("Operator", OPERATOR);
	TOKENS.put("Command", COMMANDS);
	TOKENS.put("String", STRING);
	TOKENS.put("Variable", ID);
	TOKENS.put("Comment", COMMENT);
	TOKENS.put("Field", FIELD);
	TOKENS.put("FunctionKwd", FKEYWORD);
	TOKENS.put("Url", URL);
	TOKENS.put("Number", NUMBER);
	TOKENS.put("Control", CKEYWORD);
	TOKENS.put("Structure", SKEYWORD);
	TOKENS.put("Constantes", CONSTANTES);
	TOKENS.put("WhiteIndentation", WHITE);
	TOKENS.put("TabIndentation", TAB);
    }
    
    /**
     * Can we have help on the keyword with type ?
     * @param type the type of the keyword
     * @return true if the keyword is helpable
     */
    public static boolean isHelpable(int type) {
	return type == ID 
	    || type == COMMANDS
	    || type == OPERATOR
	    || type == FKEYWORD
	    || type == CKEYWORD
	    || type == SKEYWORD
	    || type == CONSTANTES;
    }
}
