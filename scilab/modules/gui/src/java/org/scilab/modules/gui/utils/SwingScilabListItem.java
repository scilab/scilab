package org.scilab.modules.gui.utils;

import java.awt.Color;

import javax.swing.Icon;

public class SwingScilabListItem {
    private String text = null;
    private Icon icon = null;
    private Color background = null;
    private Color foreground = null;

    /**
     * Constructor
     * @param text the text displayed in the item
     */
    public SwingScilabListItem(String text, Icon icon, Color background, Color foreground) {
        this.text = text;
        this.icon = icon;
        this.background = background;
        this.foreground = foreground;
    }

    /**
     * Overload Object toString() method
     * @return the item converted to String
     * @see java.lang.Object#toString()
     */
    public String toString() {
        return text;
    }

    public Icon getIcon() {
        return icon;
    }

    public Color getBackground() {
        return background;
    }

    public Color getForeground() {
        return foreground;
    }
}
