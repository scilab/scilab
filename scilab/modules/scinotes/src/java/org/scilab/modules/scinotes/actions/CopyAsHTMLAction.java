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

import java.awt.Toolkit;
import java.awt.datatransfer.Clipboard;
import java.awt.datatransfer.ClipboardOwner;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.awt.im.InputContext;
import java.io.IOException;

import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.KeyStroke;
import javax.swing.TransferHandler;

import org.scilab.modules.jvm.LoadClassPath;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.utils.HTMLCodeConverter;

/**
 * CopyAsHTMLAction Class
 * @author Calixte Denizet
 */
public class CopyAsHTMLAction extends DefaultAction {

    private static final long serialVersionUID = 5363720520521282071L;

    protected boolean printLineNumber;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public CopyAsHTMLAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        if (!getEditor().getTextPane().copyColumnSelectionInClipBoard()) {
            String selection = getEditor().getTextPane().getSelectedText();
            if (selection != null) {
                HTMLSelection sel = new HTMLSelection(getEditor().getTextPane(), selection, printLineNumber);
                Toolkit.getDefaultToolkit().getSystemClipboard().setContents(sel, sel);
            }
        }
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return createMenu
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new CopyAsHTMLAction(label, editor), key);
    }

    /**
     * createButton
     * @param tooltip the tooltip
     * @param icon an icon name searched in SCI/modules/gui/images/icons/
     * @param editor SciNotes
     * @return PushButton
     */
    public static JButton createButton(String tooltip, String icon, SciNotes editor) {
        return createButton(tooltip, icon, new CopyAsHTMLAction(tooltip, editor));
    }

    /**
     * Inner class to handle HTML selectoin
     */
    public static class HTMLSelection implements Transferable, ClipboardOwner {

        private DataFlavor htmlFlavor;
        private String data;
        private boolean printLineNumber;
        private ScilabEditorPane pane;
        private boolean codeConverterLoaded;

        /**
         * Default constructor
         * @param data the data to copy
         */
        HTMLSelection(ScilabEditorPane pane, String data, boolean printLineNumber) {
            this.data = data;
            this.printLineNumber = printLineNumber;
            this.pane = pane;
            try {
                htmlFlavor = new DataFlavor("text/html;class=java.lang.String");
            } catch (ClassNotFoundException e) {
                e.printStackTrace();
            }
        }

        /**
         * {@inheritDoc}
         */
        public Object getTransferData(DataFlavor flavor) {
            Object ret = null;
            if (flavor.equals(htmlFlavor)) {
                if (!codeConverterLoaded) {
                    LoadClassPath.loadOnUse("copyAsHTMLinScinotes");
                    codeConverterLoaded = true;
                }
                ret = new HTMLCodeConverter(pane).convert(data, printLineNumber);
            } else if (flavor.equals(DataFlavor.stringFlavor)) {
                ret = data;
            }

            return ret;
        }

        /**
         * {@inheritDoc}
         */
        public DataFlavor[] getTransferDataFlavors() {
            return new DataFlavor[] {htmlFlavor, DataFlavor.stringFlavor};
        }

        /**
         * {@inheritDoc}
         */
        public boolean isDataFlavorSupported(DataFlavor flavor) {
            return flavor.equals(htmlFlavor) || flavor.equals(DataFlavor.stringFlavor);
        }

        /**
         * {@inheritDoc}
         */
        public void lostOwnership(Clipboard clipboard, Transferable contents) { }
    }

    /**
     * Inner class to handle the HTML transfers
     */
    public static class HTMLTransferHandler extends TransferHandler {

        private static final long serialVersionUID = -5089826958820112637L;

        /**
         * Default constructor
         */
        public HTMLTransferHandler() { }

        /**
         * {@inheritDoc}
         */
        protected Transferable createTransferable(JComponent c) {
            ScilabEditorPane pane = (ScilabEditorPane) c;
            String selection = pane.getSelectedText();
            if (selection != null) {
                return new HTMLSelection(pane, selection, false);
            } else {
                return null;
            }
        }

        /**
         * {@inheritDoc}
         */
        public boolean importData(JComponent c, Transferable t) {
            if (c instanceof ScilabEditorPane) {
                DataFlavor[] flavors = t.getTransferDataFlavors();
                if (flavors != null) {
                    int i = 0;
                    for (; i < flavors.length; i++) {
                        if (flavors[i].equals(DataFlavor.stringFlavor)) {
                            break;
                        }
                    }
                    if (i != flavors.length) {
                        InputContext ic = c.getInputContext();
                        if (ic != null) {
                            ic.endComposition();
                        }
                        try {
                            String data = (String) t.getTransferData(DataFlavor.stringFlavor);
                            ((ScilabEditorPane) c).replaceSelection(data);
                            return true;
                        } catch (UnsupportedFlavorException e) { }
                        catch (IOException ex) { }
                    }
                }
            }
            return false;
        }

        /**
         * {@inheritDoc}
         */
        public int getSourceActions(JComponent c) {
            return COPY_OR_MOVE;
        }
    }
}
