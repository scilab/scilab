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
    public static final int NUMBEROFTOKENS = 25;

    /**
     * DEFAULT : tokens which are not recognized
     */
    public static final int DEFAULT = 0;

    /**
     * OPERATOR : tokens like '+', '-', ...
     */
    public static final int OPERATOR = 1;

    /**
     * SKEYWORD : Structure keywords like 'then', 'else' or 'catch' ...
     */
    public static final int SKEYWORD = 2;

    /**
     * SKEYWORD : Structure keywords like 'if', 'for' or 'while'
     */
    public static final int OSKEYWORD = 3;

    /**
     * CKEYWORD : Control keywords like 'abort', 'break' or 'return'
     */
    public static final int CKEYWORD = 4;

    /**
     * CONSTANTES : Constantes like '%pi' or '%T'
     */
    public static final int CONSTANTES = 5;

    /**
     * ID : Identifiers like 'myvar' or 'myfun'
     */
    public static final int ID = 6;

    /**
     * NUMBER : I don't know ;)
     */
    public static final int NUMBER = 7;

    /**
     * SPECIAL : Special keywords like '$', ':' or '...'
     */
    public static final int SPECIAL = 8;

    /**
     * DOT : the dot in a field context, e.g. 'object.myfield'
     */
    public static final int DOT = 9;

    /**
     * TRANSP : just the transpose operator
     */
    public static final int TRANSP = 10;

    /**
     * OPEN : an opening char like '(', '[' or '{'
     */
    public static final int OPEN = 11;

    /**
     * CLOSE : a closing char like ')', ']' or '}'
     */
    public static final int CLOSE = 12;

    /**
     * STRING : "bla bla bla" or 'bli bli bli' (or fucking exotic way to write a string)
     */
    public static final int STRING = 13;

    /**
     * COMMENT : Comments like // An interesting commentary
     */
    public static final int COMMENT = 14;

    /**
     * FKEYWORD : For keywords 'function' and 'endfunction'
     */
    public static final int FKEYWORD = 15;

    /**
     * COMMANDS : Functions in Scilab
     */
    public static final int COMMANDS = 16;

    /**
     * FIELD : Field of an object, e.g. myobject.myfield
     */
    public static final int FIELD = 17;

    /**
     * AUTHORS : Authors in Scilab
     */
    public static final int AUTHORS = 18;

    /**
     * URL : http://...
     */
    public static final int URL = 19;

    /**
     * WHITE : A white char ' '
     */
    public static final int WHITE = 20;

    /**
     * TAB : A tabulation '\t'
     */
    public static final int TAB = 21;

    /**
     * LATEX : $\frac\pi\alpha$
     */
    public static final int LATEX = 22;

    /**
     * VARIABLES : A variable in Scilab
     */
    public static final int VARIABLES = 23;

    /**
     * EOF : End Of File
     */
    public static final int EOF = 24;

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
        TOKENS.put("Variable", VARIABLES);
        TOKENS.put("Identifier", ID);
        TOKENS.put("Comment", COMMENT);
        TOKENS.put("Field", FIELD);
        TOKENS.put("FunctionKwd", FKEYWORD);
        TOKENS.put("Url", URL);
        TOKENS.put("Number", NUMBER);
        TOKENS.put("Control", CKEYWORD);
        TOKENS.put("Structure", SKEYWORD);
        TOKENS.put("Constantes", CONSTANTES);
        TOKENS.put("White", WHITE);
        TOKENS.put("Tabulation", TAB);
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
            || type == OSKEYWORD
            || type == SKEYWORD
            || type == CONSTANTES;
    }

    /**
     * Can we open the source file of the keyword with type ?
     * @param type the type of the keyword
     * @return true if the keyword is openable
     */
    public static boolean isOpenable(int type) {
        return type == COMMANDS;
    }

    /**
     * To know if a keyword is a part of a matching keywords
     * @param type the type of the keyword
     * @return true if the keyword is a part of a matching block
     */
    public static boolean isMatchable(int type) {
        return type == OPEN
            || type == CLOSE
            || type == FKEYWORD
            || type == CKEYWORD
            || type == OSKEYWORD
            || type == SKEYWORD;
    }

    /**
     * To know if a keyword is a part of a matching keywords
     * @param type the type of the keyword
     * @return true if the keyword is a part of a matching block
     */
    public static boolean isOpenClose(int type) {
        return type == OPEN
            || type == CLOSE;
    }
}
