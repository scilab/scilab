/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2024-2026 - Dassault Systèmes S.E. - Antoine ELIAS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

package org.scilab.modules.console;

import com.google.gson.Gson;
import java.util.ArrayList;
import java.util.List;

public class CompletionEntries {
    private String pattern;
    private List<CompletionEntry> entries = new ArrayList<CompletionEntry>();
    public CompletionEntries(String pattern) {
        this.pattern = pattern;
    }

    public void add(CompletionEntry msg) {
        entries.add(msg);
    }

    public void add(String type, int kind, String[] array) {
        if (array == null) return;

        for (int i = 0; i < array.length; ++i) {
            entries.add(new CompletionEntry(array[i], type, kind));
        }
    }

    public void add(String type, int kind, ArrayList<String> array) {
        if (array == null) return;

        for (String a : array) {
            entries.add(new CompletionEntry(a, type, kind));
        }
    }

    public void empty() {
        entries.clear();
    }

    public Integer size() {
        return entries.size();
    }

    public String toJson() {
        Gson gson = new Gson();
        return gson.toJson(this);
    }
}
