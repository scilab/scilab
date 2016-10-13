/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Pierre GRADIT
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

package org.scilab.modules.preferences.Component;

import java.awt.Color;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;

import javax.swing.JScrollPane;
import javax.swing.text.DefaultCaret;

import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import org.scilab.modules.commons.ScilabCommonsUtils;
import org.scilab.modules.preferences.XChooser;
import org.scilab.modules.preferences.XComponent;
import org.scilab.modules.preferences.XConfigManager;
import org.scilab.modules.scinotes.KeywordAdapter;
import org.scilab.modules.scinotes.KeywordEvent;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.ScilabEditorKit;
import org.scilab.modules.scinotes.ScilabLexerConstants;

/** Implementation of Panel compliant with extended management.
 *
 * @author Pierre GRADIT
 *
 */
@SuppressWarnings(value = { "serial" })
public class PreviewCode extends Panel implements XComponent, XChooser {

    /** Universal identifier for serialization.
     *
     */
    private static final long serialVersionUID = 3462302313959678932L;
    private static final int GAP = 5;

    private final ScilabEditorPane previewEditorPane;

    /** Constructor.
     *
     * @param peer : associated view DOM node.
     */
    public PreviewCode(final Node peer) {
        super(peer);
        fixedHeight = false;
        previewEditorPane = new ScilabEditorPane(null);
        ScilabEditorKit kit = new ScilabEditorKit();
        previewEditorPane.setEditorKit(kit);
        previewEditorPane.setFont(kit.getStylePreferences().tokenFonts[0]);
        String codeSample = "// A comment with whites    and tabulations \t\t\n"
                            + "// Email: <support@@scilab-enterprises.com>\n"
                            + "// Scilab editor: http://www.scilab.io\n"
                            + "//\n"
                            + "// LaTeX $$\\sum_{n=1}^{+\\infty}\\frac1{n^2}=\\frac{\\pi^2}6$$\n"
                            + "// TODO: blah blah\n"
                            + "function [a, b] = myfunction(d, e, f)\n"
                            + "\ta = 2.71828 + %pi + f($, :);\n"
                            + "\tb = cos(a) + sind(a) + \"$\\frac12$\";\n"
                            + "\tif d == e then\n"
                            + "\t\tb = 10 - e.field;\n"
                            + "\telse\n"
                            + "\t\tb = \"\t\ttest     \" + home\n"
                            + "\t\treturn\n"
                            + "\tend\n"
                            + "\tmyvar = 1.23e-45;\n"
                            + "endfunction";
        previewEditorPane.setText(codeSample);
        previewEditorPane.setCaret(new DefaultCaret() {
            public void mouseDragged(MouseEvent e) {
                e.consume();
            }
        });
        previewEditorPane.setBackground(Color.WHITE);
        previewEditorPane.getCaret().setBlinkRate(500);
        previewEditorPane.getCaret().setVisible(true);
        previewEditorPane.setEditable(false);
        previewEditorPane.addKeywordListener(new KeywordAdapter.MouseOverAdapter() {
            public void caughtKeyword(KeywordEvent e) {
                previewEditorPane.setToolTipText(ScilabLexerConstants.getStringRep(e.getType()));
            }
        });
        previewEditorPane.addKeywordListener(new KeywordAdapter.MouseClickedAdapter() {
            public void caughtKeyword(KeywordEvent e) {
                PreviewCode.this.keywordClicked(e);
            }
        });
        JScrollPane scrollPane = new JScrollPane(previewEditorPane);
        previewEditorPane.setRequestFocusEnabled(true);
        previewEditorPane.setFocusable(true);

        add(scrollPane);
    }

    /** Define the set of actuators.
     *
     * @return array of actuator names.
     */
    public final String [] actuators() {
        String [] actuators = {};
        return actuators;
    }

    /** Refresh the component by the use of actuators.
     *
     * @param peer the corresponding view DOM node
     */
    public void refresh(final Node peer) {
        NodeList nodelist = peer.getChildNodes();
        for (int i = 0; i < nodelist.getLength(); i++) {
            Node node = nodelist.item(i);
            if (node.getNodeName().equals("tableRow")) {
                String name = XConfigManager.getAttribute(node , "name");
                String xColor = XConfigManager.getAttribute(node , "color");
                Color color = XConfigManager.getColor(xColor);
                previewEditorPane.resetColor(name, color);
                int style = 0;
                if (XConfigManager.getAttribute(node , "underline").equals("true")) {
                    style = 1;
                }
                if (XConfigManager.getAttribute(node , "strike-through").equals("true")) {
                    style += 2;
                }
                previewEditorPane.resetAttribute(name, style);
                int bold = -1;
                if (XConfigManager.getAttribute(node , "bold").equals("true")) {
                    bold = 1;
                }
                int italic = -2;
                if (XConfigManager.getAttribute(node , "italic").equals("true")) {
                    italic = 2;
                }
                previewEditorPane.resetFont(name, bold);
                previewEditorPane.resetFont(name, italic);
            }
        }
        previewEditorPane.repaint();
    }

    /** Row selection management.
     *
     */
    private ActionListener actionListener = null;
    String chosenItem;

    /** Actual response read by the listener.
     *
     * @return index of the selected line.
     */
    public final Object choose() {
        return chosenItem;
    }

    public void keywordClicked(KeywordEvent e) {
        if (actionListener != null) {
            int choosedIndex = e.getType();
            chosenItem = ScilabLexerConstants.getStringRep(choosedIndex);
            ActionEvent transmit  = new ActionEvent(this, 0, "Keyword changed", System.currentTimeMillis(), 0);
            actionListener.actionPerformed(transmit);
        }
    }

    /** Registration of a single listener.
     * @param listener for the unique actionListener.
     */
    public final void addActionListener(final ActionListener listener) {
        actionListener = listener;
    }

    /** Developer serialization method.
     *
     * @return equivalent signature.
     */
    public final String toString() {
        return "PreviewCode";
    }
}
