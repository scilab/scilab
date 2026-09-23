/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.scinotes;

/**
 * Native SciNotes entry points used to start editor protocol endpoints.
 */
public final class SciNotesLspJNI {

    /**
     * Constructor.
     */
    private SciNotesLspJNI() {
    }

    static {
        try {
            System.loadLibrary("sciscinotes");
        } catch (SecurityException e) {
            System.err.println("A security manager exists and does not allow the loading of the specified dynamic library.");
            System.err.println(e.getLocalizedMessage());
            e.printStackTrace(System.err);
        } catch (UnsatisfiedLinkError e) {
            if (System.getenv("CONTINUE_ON_JNI_ERROR") == null) {
                System.err.println("The native library sciscinotes does not exist or cannot be found.");
                System.err.println(e.getLocalizedMessage());
                e.printStackTrace(System.err);
            }
        }
    }

    /**
     * Start the LSP WebSocket endpoint.
     *
     * @param port the listening port
     * @param token the expected WebSocket token
     * @param spawned true when Scilab was spawned by an external client
     */
    public static native void startLspWebSocket(int port, String token, boolean spawned);

    /**
     * Start the DAP WebSocket endpoint.
     *
     * @param port the listening port
     * @param token the expected WebSocket token
     * @param spawned true when Scilab was spawned by an external client
     */
    public static native void startDapWebSocket(int port, String token, boolean spawned);

    public static native void indexDocument(String uri, String text, int version);

    public static native void removeIndexedDocument(String uri);
}
