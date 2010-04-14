package org.scilab.modules.xpad;

import java.util.Map;
import java.util.HashMap;

public class ScilabLexerConstants {
    
    public static final int NUMBER_OF_TOKENS = 20;

    public static final int DEFAULT = 0;
    public static final int OPERATOR = 1;
    public static final int SKEYWORD = 2;
    public static final int CKEYWORD = 3;
    public static final int BOOLEAN = 4;
    public static final int ID = 5;
    public static final int NUMBER = 6;
    public static final int SPECIAL = 7;
    public static final int DOT = 8;
    public static final int TRANSP = 9;
    public static final int OPEN = 10;
    public static final int CLOSE = 11;
    public static final int STRING = 12;
    public static final int COMMENT = 13;
    public static final int FKEYWORD = 14;
    public static final int COMMANDS = 15;
    public static final int FIELD = 16;
    public static final int AUTHORS = 17;
    public static final int URL = 18;
    public static final int EOF = 19;

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
	TOKENS.put("Boolean", BOOLEAN);
    }
}
