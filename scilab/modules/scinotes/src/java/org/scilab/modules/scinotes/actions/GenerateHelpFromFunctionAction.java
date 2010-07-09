/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.scinotes.actions;

import java.util.List;

import javax.swing.KeyStroke;
import javax.swing.text.Element;
import javax.swing.text.BadLocationException;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.FunctionScanner;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * Generate help from a function definition
 * @author Calixte DENIZET
 */
public class GenerateHelpFromFunctionAction extends DefaultAction {

    /**
     * The help template
     */
    public static final String template = "//\n"
        + "//\n"
        + "// Calling Sequence\n"
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
     * @param editor SciNotes
     */
    private GenerateHelpFromFunctionAction(SciNotes editor) {
        super(SciNotesMessages.GENERATE_HELP, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        ScilabEditorPane sep = (ScilabEditorPane) getEditor().getTextPane();
        int pos = sep.getCaretPosition();
        ScilabDocument doc = (ScilabDocument) sep.getDocument();
        Element root = doc.getDefaultRootElement();
        Element e = root.getElement(root.getElementIndex(pos));
        if (e instanceof ScilabDocument.ScilabLeafElement) {
            ScilabDocument.ScilabLeafElement se = (ScilabDocument.ScilabLeafElement) e;
            if (se.isFunction()) {
                FunctionScanner.FunctionInfo info = se.getFunctionInfo();
                String help = template.replaceFirst("functionName", info.functionName);
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
        for(int i = 0; i < list.size() - 1; i++) {
            str += list.get(i) + sep;
        }
        if (list.size() >= 1) {
            str += list.get(list.size() - 1) + end;
        }

        return str;
    }

    /**
     * createMenu
     * @param editor SciNotes
     * @param key Keystroke
     * @return MenuItem
     */
    public static MenuItem createMenu(SciNotes editor, KeyStroke key) {
        return createMenu(SciNotesMessages.GENERATE_HELP, null, new GenerateHelpFromFunctionAction(editor), key);
    }
}
