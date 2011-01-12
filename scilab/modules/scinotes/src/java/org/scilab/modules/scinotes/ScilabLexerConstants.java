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

package org.scilab.modules.scinotes;

import java.util.Map;
import java.util.HashMap;
import java.util.Iterator;

/**
 * This class contains constants used in the lexer
 * @author Calixte DENIZET
 */
public class ScilabLexerConstants {

    /**
     * Number of known tokens
     */
    public static final int NUMBEROFTOKENS = 34;

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
     * OSKEYWORD : Structure keywords like 'if', 'for' or 'while'
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
     * OPENCLOSE : an opening char like '(', '[', '{' or ')'
     */
    public static final int OPENCLOSE = 11;

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
     * COMMANDS : Built-in functions in Scilab
     */
    public static final int COMMANDS = 15;

    /**
     * MACROS : Macros in Scilab
     */
    public static final int MACROS = 16;

    /**
     * MACROINFILE : Macros in Scilab
     */
    public static final int MACROINFILE = 17;

    /**
     * FIELD : Field of an object, e.g. myobject.myfield
     */
    public static final int FIELD = 18;

    /**
     * AUTHORS : Authors in Scilab
     */
    public static final int AUTHORS = 19;

    /**
     * URL : http://...
     */
    public static final int URL = 20;

    /**
     * MAIL : <pierre.marechal@scilab.org>
     */
    public static final int MAIL = 21;

    /**
     * WHITE : A white char ' '
     */
    public static final int WHITE = 22;

    /**
     * TAB : A tabulation '\t'
     */
    public static final int TAB = 23;

    /**
     * LATEX : $\frac\pi\alpha$
     */
    public static final int LATEX = 24;

    /**
     * VARIABLES : A variable in Scilab
     */
    public static final int VARIABLES = 25;

    /**
     * INPUTOUTPUTARGS : A variable as arg in function declaration
     */
    public static final int INPUTOUTPUTARGS = 26;

    /**
     * WHITE_COMMENT : A white char ' ' in comments
     */
    public static final int WHITE_COMMENT = 27;

    /**
     * TAB_COMMENT : A tabulation '\t' in comments
     */
    public static final int TAB_COMMENT = 28;

    /**
     * WHITE_STRING : A white char ' ' in strings
     */
    public static final int WHITE_STRING = 29;

    /**
     * TAB_STRING : A tabulation '\t' in strings
     */
    public static final int TAB_STRING = 30;

    /**
     * ELSEIF : elseif keyword
     */
    public static final int ELSEIF = 31;

    /**
     * ERROR : a syntax error for example
     */
    public static final int ERROR = 32;

    /**
     * EOF : End Of File
     */
    public static final int EOF = 33;

    /**
     * TOKENS : A Map which contains the names of keywords (useful in scinotesConfiguration.xml)
     */
    public static final Map<String, Integer> TOKENS = new HashMap(27);

    private static Map<Integer, String> idTokens;

    static {
        TOKENS.put("Default", DEFAULT);
        TOKENS.put("Special", SPECIAL);
        TOKENS.put("Operator", OPERATOR);
        TOKENS.put("Primitive", COMMANDS);
        TOKENS.put("Macro", MACROS);
        TOKENS.put("MacroInFile", MACROINFILE);
        TOKENS.put("String", STRING);
        TOKENS.put("ExternalVariable", VARIABLES);
        TOKENS.put("InputOutputArgs", INPUTOUTPUTARGS);
        TOKENS.put("Identifier", ID);
        TOKENS.put("Comment", COMMENT);
        TOKENS.put("Field", FIELD);
        TOKENS.put("FunctionKwd", FKEYWORD);
        TOKENS.put("Url", URL);
        TOKENS.put("Mail", MAIL);
        TOKENS.put("Number", NUMBER);
        TOKENS.put("Control", CKEYWORD);
        TOKENS.put("Structure", SKEYWORD);
        TOKENS.put("Constantes", CONSTANTES);
        TOKENS.put("White", WHITE);
        TOKENS.put("WhiteInComment", WHITE_COMMENT);
        TOKENS.put("WhiteInString", WHITE_STRING);
        TOKENS.put("Tabulation", TAB);
        TOKENS.put("TabulationInComment", TAB_COMMENT);
        TOKENS.put("LaTeXInComment", LATEX);
        TOKENS.put("TabulationInString", TAB_STRING);
        TOKENS.put("OpenClose", OPENCLOSE);
    }

    /**
     * getStringRep
     * @param id the type of a token
     * @return the string representation in config file of this token
     */
    public static String getStringRep(int id) {
        if (idTokens == null) {
            idTokens = new HashMap(TOKENS.size());
            Iterator<String> iterator = TOKENS.keySet().iterator();
            while (iterator.hasNext()) {
                String key = iterator.next();
                idTokens.put(TOKENS.get(key), key);
            }
        }

        String rep = idTokens.get(id);
        if (rep != null) {
            return rep;
        }
        if (id == OSKEYWORD || id == ELSEIF) {
            return "Structure";
        }
        return "Default";
    }

    /**
     * Have we a LaTeX string ?
     * @param type the type of the keyword
     * @return true if the keyword is a LaTeX string
     */
    public static boolean isLaTeX(int type) {
        return type == LATEX;
    }

    /**
     * Can we have help on the keyword with type ?
     * @param type the type of the keyword
     * @return true if the keyword is helpable
     */
    public static boolean isHelpable(int type) {
        return type == ID
            || type == COMMANDS
            || type == MACROS
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
        return type == MACROS || type == MACROINFILE;
    }

    /**
     * To know if a keyword is a part of a matching keywords
     * @param type the type of the keyword
     * @return true if the keyword is a part of a matching block
     */
    public static boolean isMatchable(int type) {
        return type == OPENCLOSE
            || type == FKEYWORD
            || type == OSKEYWORD
            || type == ELSEIF;
    }

    /**
     * To know if a keyword is a clickable
     * @param type the type of the keyword
     * @return true if the keyword is clickable
     */
    public static boolean isClickable(int type) {
        return type == URL
            || type == MAIL
            || type == MACROS
            || type == MACROINFILE;
    }

    /**
     * To know if a keyword is a part of a matching keywords
     * @param type the type of the keyword
     * @return true if the keyword is a part of a matching block
     */
    public static boolean isOpenClose(int type) {
        return type == OPENCLOSE;
    }
}
