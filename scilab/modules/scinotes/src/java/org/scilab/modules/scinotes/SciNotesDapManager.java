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

/** Owns the authenticated DAP endpoint shared by SciNotes Monaco views. */
public final class SciNotesDapManager {

    private static final String LOCALHOST = "127.0.0.1";

    private static Endpoint endpoint;
    private static boolean startRequested;

    private SciNotesDapManager() {
    }

    /** Return the endpoint only if it is already running, without starting it. */
    public static synchronized Endpoint getEndpoint() {
        return endpoint;
    }

    public static synchronized Endpoint getOrStartEndpoint() {
        if (endpoint != null) {
            return endpoint;
        }

        int port = findFreePort();
        if (port < 0) {
            return null;
        }

        endpoint = new Endpoint(LOCALHOST, port, UUID.randomUUID().toString(), UUID.randomUUID().toString());
        startEndpoint(endpoint);
        return endpoint;
    }

    private static void startEndpoint(Endpoint value) {
        if (startRequested) {
            return;
        }

        startRequested = true;
        try {
            SciNotesLspJNI.startDapWebSocket(value.getPort(), value.getToken(), false);
        } catch (RuntimeException | UnsatisfiedLinkError e) {
            startRequested = false;
            endpoint = null;
            System.err.println("SciNotes DAP: unable to start DAP endpoint: " + e.getMessage());
        }
    }

    private static int findFreePort() {
        try (ServerSocket socket = new ServerSocket(0)) {
            socket.setReuseAddress(true);
            return socket.getLocalPort();
        } catch (IOException e) {
            System.err.println("SciNotes DAP: unable to allocate a local port: " + e.getMessage());
            return -1;
        }
    }

    /** Immutable endpoint description injected into each Monaco browser. */
    public static final class Endpoint {
        private final String host;
        private final int port;
        private final String token;
        private final String sessionId;

        private Endpoint(String host, int port, String token, String sessionId) {
            this.host = host;
            this.port = port;
            this.token = token;
            this.sessionId = sessionId;
        }

        public String getUrl() {
            return "ws://" + host + ":" + port + "/dap?token=" + token;
        }

        public int getPort() {
            return port;
        }

        public String getToken() {
            return token;
        }

        public String getSessionId() {
            return sessionId;
        }
    }
}
