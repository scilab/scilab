package org.scilab.modules.gui.utils;

import java.awt.Color;

public class SwingScilabColorItem {
    private String text;
    private Color color;

    /**
     * Constructor
     * @param text the text displayed in the item
     */
    public SwingScilabColorItem(String text, Color color) {
        this.text = text;
        this.color = color;
    }

    /**
     * Overload Object toString() method
     * @return the item converted to String
     * @see java.lang.Object#toString()
     */
    public String toString() {
        return text;
    }

    public Color getColor() {
        return color;
    }
}
