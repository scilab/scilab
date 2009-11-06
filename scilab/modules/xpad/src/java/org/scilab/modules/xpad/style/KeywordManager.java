package org.scilab.modules.xpad.style;


import java.util.Hashtable;

import org.scilab.modules.xpad.ScilabKeywords;

/**
 * The class which handles the various keyword of Scilab
 */
public class KeywordManager {
	private static final String[] QUOTATIONS = {"(\"|')([^\\n])*?(\"|')"};

	private static final String[] BOOLS = {"%T", "%F", "%t", "%f",	"%e","%pi",
			"%inf", "%i", "%z", "%s", "%nan", "%eps","SCI", "SCIHOME", "TMPDIR", "MSDOS"};
	
	private static final String[] COMMENTS = {"//[^{\n}]*"};
	
	private static final String[] OPERATORS = {"=", "\\+", "-", "\\*", "/", "\\\\", "\\^", 
			"\\./", "\\.\\\\", "\\.\\^", 
			"\\.\\*\\.", "\\./\\.", "\\.\\\\\\.",
			"==", "<", ">", "<=", ">=", "~=", "@=",
			"&", "\\|", "@", "~",
	"\\.\\.[\\.]*"};
	
	Hashtable<String, String[]> keywords = new Hashtable<String, String[]>();

	
	/**
	 * constructor of the object
	 * It loads all the keywords values from the Scilab engine 
	 */
	public KeywordManager(){
		//Get all Scilab keywords with SWIG
		if (true==true) { /* @TODO: update it to allow the text editor as standalone */
			String[] commands =  ScilabKeywords.GetCommandsName();
			String[] functions =  ScilabKeywords.GetFunctionsName();
			String[] macros =  ScilabKeywords.GetMacrosName();
		}else{
			String[] commands =  {""};//ScilabKeywords.GetCommandsName();
			String[] functions =  {""};// ScilabKeywords.GetFunctionsName();
			String[] macros =  {""};//ScilabKeywords.GetMacrosName();
			
		}
		//String[] variables =  ScilabKeywords.GetVariablesName();

		for (int i = 0; i < macros.length; i++) {
			keywords.put("macro", macros);
		}
		for (int i = 0; i < commands.length; i++) {
			keywords.put("command", commands);
		}
		for (int i = 0; i < functions.length; i++) {
			keywords.put("function", functions);
		}	
	}

	public static String[] getQuotations() {
		return QUOTATIONS;
	}
	
	static public String[] getBools() {
		return BOOLS;
	}
	
	public static String[] getComments() {
		return COMMENTS;
	}
	
	public static String[] getOperators() {
		return OPERATORS;
	}
	


	/**
	 * Get all Scilab's keywords into a hashtable
	 * @return the hashtable of the keywords
	 */
	public Hashtable<String, String[]> getScilabKeywords() {
		return keywords;
	}

}
