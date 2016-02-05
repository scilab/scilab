package org.scilab.modules.gui.utils;


public class SwingScilabTextItem {
    /**
     * Class created as a workaround for bug:
     * http://bugzilla.scilab.org/show_bug.cgi?id=7898 This bug is a Java bug:
     * http://bugs.sun.com/bugdatabase/view_bug.do?bug_id=4133743
     *
     * This workaround has been proposed by a user on Java bug tracker.
     *
     * The toString method will be used to display the elements, but because the
     * class inherits its equals method from Object instead of String, none of
     * the elements are considered duplicates.
     *
     */
    private String textOfItem;

    /**
     * Constructor
     * @param text the text displayed in the item
     */
    public SwingScilabTextItem(String text) {
        textOfItem = text;
    }

    /**
     * Overload Object toString() method
     * @return the item converted to String
     * @see java.lang.Object#toString()
     */
    @Override
    public String toString() {
        return textOfItem;
    }
}
