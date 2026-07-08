/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2024-2026 - Dassault Systèmes S.E. - Antoine ELIAS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

package org.scilab.modules.console;

public class CompletionEntry {
    private String entry;
    private String type;

    CompletionEntry(String entry, String type) {
        this.entry = entry;
        this.type = type;
    }
}
