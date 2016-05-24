/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.scinotes.actions;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.util.List;

import javax.swing.JMenuItem;
import javax.swing.KeyStroke;
import javax.swing.text.Element;
import javax.swing.text.BadLocationException;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.FunctionScanner;

/**
 * Generate help from a function definition
 * @author Calixte DENIZET
 */
public class GenerateHelpFromFunctionAction extends DefaultAction {

    private static final long serialVersionUID = 5008914832562621484L;

    /**
     * The help template
     */
    public static final String TEMPLATE = "//\n"
                                          + "//\n"
                                          + "// Syntax\n"
                                          + "// [returnValues] = functionName(argsValues) // \n"
                                          + "// Parameters\n"
                                          + "// argsValuesOnColumn\n"
                                          + "// returnValuesOnColumn\n"
                                          + "// \n"
                                          + "// Description\n"
                                          + "// \n"
                                          + "// \n"
                                          + "// Examples\n"
                                          + "// \n"
                                          + "// \n"
                                          + "// See Also\n"
                                          + "// \n"
                                          + "// \n"
                                          + "// Authors\n"
                                          + "// \n"
                                          + "\n";

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public GenerateHelpFromFunctionAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        ScilabEditorPane sep = getEditor().getTextPane();
        int pos = sep.getCaretPosition();
        ScilabDocument doc = (ScilabDocument) sep.getDocument();
        Element root = doc.getDefaultRootElement();
        Element e = root.getElement(root.getElementIndex(pos));
        if (e instanceof ScilabDocument.ScilabLeafElement) {
            ScilabDocument.ScilabLeafElement se = (ScilabDocument.ScilabLeafElement) e;
            if (se.isFunction()) {
                FunctionScanner.FunctionInfo info = se.getFunctionInfo();
                String help = TEMPLATE.replaceFirst("functionName", info.functionName);
                help = help.replaceFirst("returnValues", generateList(info.returnValues, ", ", ""));
                help = help.replaceFirst("argsValues", generateList(info.argsValues, ", ", ""));
                help = help.replaceFirst("returnValuesOnColumn", generateList(info.returnValues, ": \n// ", ": "));
                help = help.replaceFirst("argsValuesOnColumn", generateList(info.argsValues, ": \n// ", ": "));
                try {
                    doc.mergeEditsEnd();
                    doc.insertString(e.getEndOffset(), help, null);
                } catch (BadLocationException exc) { }
            }
        }
    }

    /**
     * Generate a string containing the list gave as argument in using sep as separator
     * @param list the list
     * @param sep the separator
     * @param end a String to close
     * @return the generated String
     */
    private String generateList(List<String> list, String sep, String end) {
        String str = "";
        for (int i = 0; i < list.size() - 1; i++) {
            str += list.get(i) + sep;
        }
        if (list.size() >= 1) {
            str += list.get(list.size() - 1) + end;
        }

        return str;
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key Keystroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, final SciNotes editor, KeyStroke key) {
        final MenuItem menuitem = createMenu(label, null, new GenerateHelpFromFunctionAction(label, editor), key);
        ((JMenuItem) menuitem.getAsSimpleMenuItem()).addPropertyChangeListener(new PropertyChangeListener() {
            public void propertyChange(PropertyChangeEvent e) {
                if (editor.getTextPane() != null) {
                    Element root = editor.getTextPane().getDocument().getDefaultRootElement();
                    int pos = editor.getTextPane().getCaretPosition();
                    ScilabDocument.ScilabLeafElement elem = (ScilabDocument.ScilabLeafElement) root.getElement(root.getElementIndex(pos));
                    menuitem.setEnabled(elem.isFunction());
                }
            }
        });

        return menuitem;
    }
}
