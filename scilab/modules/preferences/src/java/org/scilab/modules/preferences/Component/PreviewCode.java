/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Pierre GRADIT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.preferences.Component;

import org.scilab.modules.preferences.XComponent;


import java.awt.event.MouseEvent;
import javax.swing.JPanel;
import org.w3c.dom.Node;
import org.scilab.modules.preferences.XConfigManager;
import java.awt.BorderLayout;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.ScilabEditorKit;

import javax.swing.text.DefaultCaret;
import org.scilab.modules.scinotes.KeywordAdaptater;
import org.scilab.modules.scinotes.utils.SciNotesMessages;
import javax.swing.BorderFactory;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;
import org.scilab.modules.scinotes.KeywordEvent;
import org.scilab.modules.scinotes.ScilabLexerConstants;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import org.scilab.modules.preferences.XChooser;
import java.awt.Color;
import org.w3c.dom.NodeList;

/** Implementation of Panel compliant with extended management.
*
* @author Pierre GRADIT
*
*/
public class PreviewCode extends JPanel implements XComponent, XChooser {

    /** Universal identifier for serialization.
     *
     */
    private static final long serialVersionUID = 3462302313959678932L;

    private static final int GAP = 5;

    /** Define the set of actuators.
     *
     * @return array of actuator names.
     */
    public final String [] actuators() {
        String [] actuators = {};
        return actuators;
    }


    final ScilabEditorPane previewEditorPane;

    /** Constructor.
    *
    * @param peer : associated view DOM node.
    */
    public PreviewCode(final Node peer) {
        super();
        setLayout(new BorderLayout());
        XConfigManager.setDimension(this, peer);
        XConfigManager.drawConstructionBorders(this);


        JPanel previewPanel = new JPanel(new BorderLayout(GAP, GAP));
        previewEditorPane   = new ScilabEditorPane(null);
        previewPanel.setBorder(BorderFactory.createTitledBorder(SciNotesMessages.PREVIEW));
        previewEditorPane.setEditorKit(new ScilabEditorKit());
        String codeSample = "// A comment with whites    and tabulations \t\t\n"
            + "// Email: <scilab.support@scilab.org>\n"
            + "// Scilab editor: http://www.scilab.org/\n"
            + "function [a, b] = myfunction(d, e, f)\n"
            + "\ta = 2.71828 + %pi + f($, :);\n"
            + "\tb = cos(a) + cosh(a);\n"
            + "\tif d == e then\n"
            + "\t\tb = 10 - e.field;\n"
            + "\telse\n"
            + "\t\tb = \"\t\ttest     \" + home\n"
            + "\t\treturn\n"
            + "\tend\n"
            + "\tmyvar = 1.23e-45;\n"
            + "endfunction";
        previewEditorPane.setText(codeSample);
        //previewEditorPane.setBackground(bgColorButton.getBackground());
        //previewEditorPane.setFont(ConfigSciNotesManager.getFont());
        previewEditorPane.setCaret(new DefaultCaret() {
                public void mouseDragged(MouseEvent e) {
                    e.consume();
                }
            });
        previewEditorPane.getCaret().setBlinkRate(500);
        //previewEditorPane.setCaretColor(fgColorButton.getBackground());
        previewEditorPane.getCaret().setVisible(true);
        previewEditorPane.setEditable(false);
        previewEditorPane.addKeywordListener(new KeywordAdaptater.MouseOverAdaptater() {
                public void caughtKeyword(KeywordEvent e) {
                    previewEditorPane.setToolTipText(ScilabLexerConstants.getStringRep(e.getType()));
                }
            });
        previewEditorPane.addKeywordListener(new KeywordAdaptater.MouseClickedAdaptater() {
                public void caughtKeyword(KeywordEvent e) {
                    PreviewCode.this.keywordClicked(e);
                }
            });
        previewPanel.add(previewEditorPane, BorderLayout.CENTER);
        add(previewPanel);
    }

    /** Refresh the component by the use of actuators.
    *
    * @param peer the corresponding view DOM node
    */
    public void refresh(final Node peer) {
        System.out.println(" " + peer);
        NodeList nodelist = peer.getChildNodes();
        for (int i = 0; i < nodelist.getLength(); i++) {
            Node node     = nodelist.item(i);
            if (node.getNodeName().equals("tableRow")) {
                String name   = XConfigManager.getAttribute(node , "name");
                String xColor = XConfigManager.getAttribute(node , "color");
                Color color   = XConfigManager.getColor(xColor);
                previewEditorPane.resetColor(name, color);
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
    public final String choose() {
        return chosenItem;
    }

    public void keywordClicked(KeywordEvent e) {
        if (actionListener != null) {
            int choosedIndex = e.getType();
            chosenItem = ScilabLexerConstants.getStringRep(choosedIndex);
            ActionEvent transmit  = new ActionEvent(
                 this, 0,"Keyword changed", choosedIndex, 0);
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
        String signature = "PreviewCode";

        return signature;
    }


}

