package org.scilab.modules.gui.utils;

import javax.swing.Icon;

public class SwingScilabIconItem {
    private String text;
    private Icon icon;

    /**
     * Constructor
     * @param text the text displayed in the item
     */
    public SwingScilabIconItem(String text, Icon icon) {
        this.text = text;
        this.icon = icon;
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

}
