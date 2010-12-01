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

package org.scilab.modules.console;

import java.io.OutputStream;
import java.io.PrintStream;

import org.scilab.modules.gui.bridge.CallScilabBridge;
import org.scilab.modules.gui.console.ScilabConsole;

/**
 * @author Calixte DENIZET
 */
public final class ScilabPrintStream extends PrintStream {

    private static ScilabPrintStream stream;

    /**
     * Constructor
     */
    private ScilabPrintStream() {
        super(new SciOutputStream(), true);
    }

    /**
     * A stream is available only if the console is available
     * @return true if a stream is available
     */
    public static boolean isAvailable() {
        return ScilabConsole.isExistingConsole();
    }

    /**
     * @return an instance of ScilabPrintStream (singleton)
     */
    public static ScilabPrintStream getInstance() {
        if (stream == null) {
            stream = new ScilabPrintStream();
        }

        return stream;
    }

    /**
     * Inner class to simulate an outputstream which display datas in console
     */
    private static final class SciOutputStream extends OutputStream {

        /**
         * Constructor
         */
        private SciOutputStream() { }

        /**
         * {@inheritDoc}
         */
        public void close() { }

        /**
         * {@inheritDoc}
         */
        public void flush() { }

        /**
         * {@inheritDoc}
         */
        public void write(byte[] b) {
            CallScilabBridge.display(new String(b));
        }

        /**
         * {@inheritDoc}
         */
        public void write(byte[] b, int off, int len) {
            CallScilabBridge.display(new String(b, off, len));
        }

        /**
         * {@inheritDoc}
         */
        public void write(int b) {
            CallScilabBridge.display(new String (new byte[]{(byte) b}));
        }
    }
}
