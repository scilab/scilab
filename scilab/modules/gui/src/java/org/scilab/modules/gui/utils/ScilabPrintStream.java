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
