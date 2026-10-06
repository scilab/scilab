/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2024-2026 - Dassault Systèmes S.E. - Antoine ELIAS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

package org.scilab.modules.console;

public class CompletionEntry {
    // LSP CompletionItemKind values, as sent by the LSP server to SciNotes
    public static final int FUNCTION = 3;
    public static final int FIELD = 5;
    public static final int VARIABLE = 6;
    public static final int KEYWORD = 14;
    public static final int FILE = 17;
    public static final int FOLDER = 19;

    private String entry;
    private String type;
    private int kind;

    CompletionEntry(String entry, String type, int kind) {
        this.entry = entry;
        this.type = type;
        this.kind = kind;
    }
}
