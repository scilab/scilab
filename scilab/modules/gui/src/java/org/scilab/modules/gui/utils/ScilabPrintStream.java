/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.utils;

import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintStream;

import org.scilab.modules.gui.bridge.CallScilabBridge;

/**
 * @author Calixte DENIZET
 */
public final class ScilabPrintStream extends PrintStream {

    private static ScilabPrintStream stream;
    private static SciOutputStream output;

    /**
     * Constructor
     */
    private ScilabPrintStream() {
        super(output, true);
    }

    /**
     * A stream is available only if the console is available
     * @return true if a stream is available
     */
    public static boolean isAvailable() {
        return false;//ScilabConsole.isExistingConsole();
    }

    /**
     * @return an instance of ScilabPrintStream (singleton)
     */
    public static ScilabPrintStream getInstance() {
        if (stream == null) {
            output = new SciOutputStream();
            stream = new ScilabPrintStream();
        }

        return stream;
    }

    /**
     * @param out the PrintStream where to redirect
     */
    public static void setRedirect(PrintStream out) {
        output.setRedirect(out);
    }

    /**
     * Inner class to simulate an outputstream which display data in console
     */
    private static final class SciOutputStream extends OutputStream {

        private PrintStream out;

        /**
         * Constructor
         */
        private SciOutputStream() { }

        /**
         * @param out the OutputStream where to redirect
         */
        public void setRedirect(PrintStream out) {
            this.out = out;
        }

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
        public void write(byte[] b) throws IOException {
            CallScilabBridge.display(new String(b));
            if (out != null) {
                out.write(b);
            }
        }

        /**
         * {@inheritDoc}
         */
        public void write(byte[] b, int off, int len) throws IOException {
            CallScilabBridge.display(new String(b, off, len));
            if (out != null) {
                out.write(b, off, len);
            }
        }

        /**
         * {@inheritDoc}
         */
        public void write(int b) throws IOException {
            CallScilabBridge.display(new String (new byte[] {(byte) b}));
            if (out != null) {
                out.write(b);
            }
        }
    }
}
