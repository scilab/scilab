/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2024-2026 - Dassault Systèmes S.E. - Antoine ELIAS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

package org.scilab.modules.console;

import com.google.gson.Gson;

public class ConsoleMessage<T, U, V, W> {
    private String type;
    private T data1;
    private U data2;
    private V data3;
    private W data4;

    public ConsoleMessage(String type, T data1, U data2, V data3, W data4) {
        this.type = type;
        this.data1 = data1;
        this.data2 = data2;
        this.data3 = data3;
        this.data4 = data4;
    }

    public String toJson() {
        Gson gson = new Gson();
        return gson.toJson(this);
    }
}
