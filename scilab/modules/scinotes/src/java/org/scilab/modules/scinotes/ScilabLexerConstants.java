/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
    public static final int NUMBEROFTOKENS = 36;

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
     * LATEXINSTRING : $\frac\pi\alpha$
     */
    public static final int LATEXINSTRING = 25;

    /**
     * VARIABLES : A variable in Scilab
     */
    public static final int VARIABLES = 26;

    /**
     * INPUTOUTPUTARGS : A variable as arg in function declaration
     */
    public static final int INPUTOUTPUTARGS = 27;

    /**
     * WHITE_COMMENT : A white char ' ' in comments
     */
    public static final int WHITE_COMMENT = 28;

    /**
     * TAB_COMMENT : A tabulation '\t' in comments
     */
    public static final int TAB_COMMENT = 29;

    /**
     * WHITE_STRING : A white char ' ' in strings
     */
    public static final int WHITE_STRING = 30;

    /**
     * TAB_STRING : A tabulation '\t' in strings
     */
    public static final int TAB_STRING = 31;

    /**
     * ELSEIF : elseif keyword
     */
    public static final int ELSEIF = 32;

    /**
     * ERROR : a syntax error for example
     */
    public static final int ERROR = 33;

    /**
     * EOF : End Of File
     */
    public static final int TODO = 34;

    /**
     * EOF : End Of File
     */
    public static final int EOF = 35;

    /**
     * TOKENS : A Map which contains the names of keywords (useful in scinotesConfiguration.xml)
     */
    public static final Map<String, Integer> TOKENS = new HashMap<String, Integer>(28);

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
        TOKENS.put("LaTeXInString", LATEXINSTRING);
        TOKENS.put("TabulationInString", TAB_STRING);
        TOKENS.put("OpenClose", OPENCLOSE);
        TOKENS.put("Todo", TODO);
    }

    /**
     * getStringRep
     * @param id the type of a token
     * @return the string representation in config file of this token
     */
    public final static String getStringRep(int id) {
        if (idTokens == null) {
            idTokens = new HashMap<Integer, String>(TOKENS.size());
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
    public static final boolean isLaTeX(final int type) {
        return type == LATEX || type == LATEXINSTRING;
    }

    /**
     * Have we a piece of a Scilab string ?
     * @param type the type of the keyword
     * @return true if the keyword is a piece of Scilab string
     */
    public static final boolean isString(final int type) {
        return type == STRING || type == WHITE_STRING || type == TAB_STRING;
    }

    /**
     * Have we a LaTeX string ?
     * @param type the type of the keyword
     * @return true if the keyword is a LaTeX string
     */
    public static final boolean isComment(final int type) {
        return type == COMMENT || type == AUTHORS || type == URL || type == MAIL || type == LATEX || type == WHITE_COMMENT || type == TAB_COMMENT || type == TODO;
    }

    /**
     * Can we have help on the keyword with type ?
     * @param type the type of the keyword
     * @return true if the keyword is helpable
     */
    public static final boolean isHelpable(final int type) {
        return type == ID
               || type == COMMANDS
               || type == MACROS
               || type == OPERATOR
               || type == FKEYWORD
               || type == CKEYWORD
               || type == OSKEYWORD
               || type == SKEYWORD
               || type == ELSEIF
               || type == CONSTANTES
               || type == VARIABLES
               || type == FIELD;
    }

    public static final boolean isSearchable(final int type) {
        return type == ID
               || type == COMMANDS
               || type == MACROS
               || type == MACROINFILE
               || type == INPUTOUTPUTARGS
               || type == OPERATOR
               || type == FKEYWORD
               || type == CKEYWORD
               || type == OSKEYWORD
               || type == SKEYWORD
               || type == ELSEIF
               || type == NUMBER
               || type == CONSTANTES
               || type == VARIABLES
               || type == FIELD;
    }

    /**
     * Can we open the source file of the keyword with type ?
     * @param type the type of the keyword
     * @return true if the keyword is openable
     */
    public static final boolean isOpenable(final int type) {
        return type == MACROS || type == MACROINFILE;
    }

    /**
     * To know if a keyword is a part of a matching keywords
     * @param type the type of the keyword
     * @return true if the keyword is a part of a matching block
     */
    public static final boolean isMatchable(final int type) {
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
    public static final boolean isClickable(final int type) {
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
    public static final boolean isOpenClose(final int type) {
        return type == OPENCLOSE;
    }
}
