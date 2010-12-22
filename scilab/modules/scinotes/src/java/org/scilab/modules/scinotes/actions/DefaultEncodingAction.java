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

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.Charset;
import java.nio.charset.CharsetDecoder;
import java.nio.charset.CharacterCodingException;
import java.nio.charset.CodingErrorAction;
import java.util.Arrays;
import java.util.ArrayList;
import java.util.List;
import java.util.HashMap;
import java.util.Map;
import java.util.StringTokenizer;

import javax.swing.ButtonGroup;
import javax.swing.ImageIcon;
import javax.swing.JMenu;
import javax.swing.JRadioButtonMenuItem;
import javax.swing.KeyStroke;
import javax.swing.text.BadLocationException;
import javax.swing.text.EditorKit;

import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.ButtonType;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * DefaultEncodingAction Class
 * @author Calixte DENIZET
 *
 */
public class DefaultEncodingAction extends DefaultCheckAction {

    /**
     * Default encoding to test
     */
    public static final String[] ENCODINGS = new String[]{"utf-8", "windows-1252", "iso-8859-1"};

    private static final Map<String, Integer> encPos = new HashMap(ENCODINGS.length);

    static {
        for (int i = 0; i < ENCODINGS.length; i++) {
            encPos.put(ENCODINGS[i], i);
        }
    }

    private static final int BUFFERCAPACITY = 8192;
    private String encoding;

    /**
     * Construtor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public DefaultEncodingAction(String name, SciNotes editor) {
        super(name, editor);
        encoding = ConfigSciNotesManager.getDefaultEncoding();
    }

    /**
     * doAction
     */
    public void doAction() {
        ConfigSciNotesManager.saveDefaultEncoding(encoding);
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return createMenu
     */
    public static Menu createMenu(String label, SciNotes editor, KeyStroke key) {
        StringTokenizer tokens = new StringTokenizer(label, ";");
        String labelDefaultEnc = tokens.nextToken();
        String labelUTF8 = tokens.nextToken();
        String labelANSI = tokens.nextToken();
        String labelISO88591 = tokens.nextToken();

        DefaultEncodingAction de = new DefaultEncodingAction(labelDefaultEnc, editor);
        final JRadioButtonMenuItem[] arr = new JRadioButtonMenuItem[3];
        String[] labels = new String[]{labelUTF8, labelANSI, labelISO88591};

        final Menu menu = ScilabMenu.createMenu();
        menu.setText(labelDefaultEnc);

        ButtonGroup group = new ButtonGroup();
        JRadioButtonMenuItem radio;

        for (int i = 0; i < labels.length; i++) {
            radio = createRadioButtonMenuItem(de, labels[i], ENCODINGS[i]);
            group.add(radio);
            ((JMenu) menu.getAsSimpleMenu()).add(radio);
            arr[i] = radio;
        }

        arr[encPos.get(de.encoding)].setSelected(true);

        return menu;
    }

    /**
     * @param file the file to test
     * @return the corresponding charset if exists
     * @throws IOExecption if I/O problems are met
     * @throws CharacterCodingException if no charset is found
     */
    public static Charset tryToGuessEncoding(File file) throws IOException, CharacterCodingException {
        for (int i = 0; i < ENCODINGS.length; i++) {
            if (tryToGuessEncoding(file, Charset.forName(ENCODINGS[i]))) {
                return Charset.forName(ENCODINGS[i]);
            }
        }

        throw new CharacterCodingException();
    }

    /**
     * @param file the file to test
     * @param charset the charset to test
     * @return true if the file can be decoded with the charset
     * @throws IOExecption if I/O problems are met
     */
    public static boolean tryToGuessEncoding(File file, Charset charset) throws IOException {
        char[] cbuf = new char[BUFFERCAPACITY];
        CharsetDecoder decoder = charset.newDecoder().onMalformedInput(CodingErrorAction.REPORT).onUnmappableCharacter(CodingErrorAction.REPORT);
        FileInputStream fis = new FileInputStream(file);
        InputStreamReader inReader = new InputStreamReader(fis, decoder);
        BufferedReader bufReader = new BufferedReader(inReader);
        try {
            while (bufReader.read(cbuf) != -1);
            fis.close();
            inReader.close();
            bufReader.close();
            return true;
        } catch (Exception e) { }

        return false;
    }

    /**
     * createRadioButtonMenuItem
     * @param ln the LineNumbersAction
     * @param title the label of the menuitem
     * @param state the state associated with the menuitem
     * @return JRadioButtonMenuItem
     */
    private static JRadioButtonMenuItem createRadioButtonMenuItem(final DefaultEncodingAction de, String title, final String encoding) {
        JRadioButtonMenuItem radio = new JRadioButtonMenuItem(title);
        radio.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent arg0) {
                    de.encoding = encoding;
                    de.doAction();
                }
            });

        return radio;
    }
}
