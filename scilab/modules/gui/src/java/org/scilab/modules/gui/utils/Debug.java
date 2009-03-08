package org.scilab.modules.gui.utils;

public class Debug {

    private static final boolean enableDebug = false;
    /**
     * DEBUG function
     */
    public static void DEBUG(String callingClass, String msg) {
	if (enableDebug) {
	    System.err.println("[DEBUG] __"+callingClass+"__ "+Thread.currentThread().toString()+" : "+msg);
	}
    }

    public static void DEBUG(String msg) {
	if (enableDebug) {
	    System.err.println("[DEBUG] __unknown_class__ "+Thread.currentThread().toString()+" : "+msg);
	}
    }

}
