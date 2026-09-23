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

import java.io.IOException;
import java.net.ServerSocket;
import java.util.UUID;

/**
 * Owns the SciNotes LSP endpoint lifecycle shared by Monaco editor panes.
 */
public final class SciNotesLspManager {

    private static final String LOCALHOST = "127.0.0.1";

    private static Endpoint endpoint;
    private static boolean startRequested;

    private SciNotesLspManager() {
    }

    /**
     * Get the shared LSP endpoint, starting it once on demand.
     *
     * @return the endpoint to inject in Monaco, or null if no port is available
     */
    public static synchronized Endpoint getOrStartEndpoint() {
        if (endpoint != null) {
            return endpoint;
        }

        int port = findFreePort();
        if (port < 0) {
            return null;
        }

        String token = UUID.randomUUID().toString();
        endpoint = new Endpoint(LOCALHOST, port, token);
        startEndpoint(endpoint);
        return endpoint;
    }

    private static void startEndpoint(Endpoint endpoint) {
        if (startRequested) {
            return;
        }

        startRequested = true;
        try {
            SciNotesLspJNI.startLspWebSocket(endpoint.getPort(), endpoint.getToken(), false);
        } catch (RuntimeException | UnsatisfiedLinkError e) {
            startRequested = false;
            SciNotesLspManager.endpoint = null;
            System.err.println("SciNotes LSP: unable to start LSP endpoint: " + e.getMessage());
        }
    }

    private static int findFreePort() {
        try {
            ServerSocket socket = new ServerSocket(0);
            try {
                socket.setReuseAddress(true);
                return socket.getLocalPort();
            } finally {
                socket.close();
            }
        } catch (IOException e) {
            System.err.println("SciNotes LSP: unable to allocate a local port: " + e.getMessage());
            return -1;
        }
    }

    /**
     * Immutable LSP endpoint description.
     */
    public static final class Endpoint {
        private final String host;
        private final int port;
        private final String token;

        private Endpoint(String host, int port, String token) {
            this.host = host;
            this.port = port;
            this.token = token;
        }

        public String getUrl() {
            return "ws://" + host + ":" + port + "/lsp?token=" + token;
        }

        public int getPort() {
            return port;
        }

        public String getToken() {
            return token;
        }

    }
}
