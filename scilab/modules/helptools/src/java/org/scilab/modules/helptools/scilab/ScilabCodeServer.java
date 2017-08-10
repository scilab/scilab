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

package org.scilab.modules.helptools.scilab;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;
import java.io.Reader;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.net.Socket;
import java.net.ServerSocket;
import java.nio.charset.Charset;
import java.util.HashMap;
import java.util.Map;
import java.util.Stack;

/**
 * Server to colorize Scilab code.
 * The string passed to the server must be terminated with an EOF or a null char "\0".
 */
public final class ScilabCodeServer {

    private static final int DEFAULTMAXHANDLERS = 5;

    private Stack<HTMLWithStyleScilabCodeHandler> stack = new Stack<HTMLWithStyleScilabCodeHandler>();
    private String prim;
    private String macro;
    private int maxHandler;

    /**
     * Main method
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        Map<String, String> map = parseCommandLine(args);
        if (map.containsKey("help")) {
            System.out.println("Usage scicodeserver [OPTION]...");
            System.out.println("Create a multi-thread server to colorize Scilab code");
            System.out.println("");
            System.out.println("-port          An integer giving the port where to listen to");
            System.out.println("-sciprim       A file containing the list of the Scilab primitives");
            System.out.println("-scimacro      A file containing the list of the Scilab macros");
            System.out.println("-maxhandlers   An integer giving the maximum of Scilab code handlers");
            System.out.println("               which must be kept in memory (5 by default)");
            System.out.println("");
            System.out.println("Report bugs on: <http://bugzilla.scilab.org>");
            System.out.println("Project page: <http://forge.scilab.org/index.php/p/scidoc>");
            return;
        }

        if (!map.containsKey("port")) {
            System.err.println("No port");
            System.err.println("Use the option -help");
            return;
        }

        int max = DEFAULTMAXHANDLERS;
        if (map.containsKey("maxhandlers")) {
            try {
                max = Integer.parseInt(map.get("maxhandlers"));
            } catch (Exception e) {
                System.err.println("The option -maxhandlers expects a positive integer");
                return;
            }
        }

        int port = 0;
        if (map.containsKey("port")) {
            try {
                port = Integer.parseInt(map.get("port"));
            } catch (Exception e) {
                System.err.println("The option -port expects a positive integer");
                return;
            }
        }

        ScilabCodeServer scs = new ScilabCodeServer(map.get("sciprim"), map.get("scimacro"), max);
        try {
            ServerSocket server = new ServerSocket(port);
            while (true) {
                new ScilabCodeThread(server.accept(), scs);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * Private constructor
     * @param primFile the filename containing the list of the scilab built-in commands
     * @param macroFile the filename containing the list of the scilab macros
     * @param maxHandler the maximum handlers which must be cached
     */
    private ScilabCodeServer(String primFile, String macroFile, int maxHandler) {
        this.prim = primFile;
        this.macro = macroFile;
        this.maxHandler = maxHandler;
    }

    /**
     * Get an handler. The stack is synchronized.
     * @return an handler
     */
    private HTMLWithStyleScilabCodeHandler getHandler() {
        synchronized (stack) {
            if (stack.isEmpty()) {
                return new HTMLWithStyleScilabCodeHandler(prim, macro);
            }
            return stack.pop();
        }
    }

    /**
     * Push an handler in the stack to cach it. The stack is synchronized.
     * @param h the handler
     */
    private void pushHandler(HTMLWithStyleScilabCodeHandler h) {
        synchronized (stack) {
            if (stack.size() < maxHandler) {
                stack.push(h);
            }
        }
    }

    /**
     * @param in the input
     * @param out the output
     * @throws IOException
     */
    private void convert(Reader in, Writer out) throws IOException {
        HTMLWithStyleScilabCodeHandler handler = getHandler();
        handler.convert(in, out);
        pushHandler(handler);
    }

    /**
     * Parse the command line
     * @param args the arguments
     * @return the options with their argument in a map
     */
    private static Map<String, String> parseCommandLine(String[] args) {
        String option = null;
        boolean in = false;
        Map<String, String> map = new HashMap<String, String>();
        for (int i = 0; i < args.length; i++) {
            if (args[i].length() >= 2 && args[i].charAt(0) == '-') {
                if (option != null) {
                    map.put(option, "");
                    option = null;
                    option = args[i];
                }
                if (args[i].charAt(1) == '-') {
                    option = args[i].substring(2);
                } else {
                    option = args[i].substring(1);
                }
            } else {
                if (option != null) {
                    map.put(option, args[i]);
                    option = null;
                } else if (!in) {
                    map.put("input", args[i]);
                    in = true;
                } else {
                    System.err.println("Not an argument " + args[i]);
                    return null;
                }
            }
        }
        if (option != null) {
            map.put(option, "");
        }

        return map;
    }

    /**
     * Inner class
     */
    private static class ScilabCodeThread implements Runnable {

        private Reader in;
        private Writer out;
        private Socket sock;
        private ScilabCodeServer scs;

        /**
         * Default constructor
         * @param sock the socket where to read and write
         * @param scs the server
         */
        ScilabCodeThread(Socket sock, ScilabCodeServer scs) {
            this.sock = sock;
            this.scs = scs;
            try {
                in = new BufferedReader(new InputStreamReader(sock.getInputStream()));
                out = new OutputStreamWriter(sock.getOutputStream(), Charset.forName("UTF-8"));
            } catch (IOException e) {
                e.printStackTrace();
            }
            new Thread(this).start();
        }

        public void run() {
            try {
                scs.convert(in, out);
                out.flush();
                out.close();
                in.close();
                sock.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}