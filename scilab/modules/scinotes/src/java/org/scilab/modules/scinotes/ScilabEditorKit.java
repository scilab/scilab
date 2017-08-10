/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
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

package org.scilab.modules.scinotes;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.Reader;
import java.nio.CharBuffer;
import java.nio.charset.CharacterCodingException;
import java.nio.charset.Charset;
import java.nio.charset.CharsetDecoder;
import java.nio.charset.CodingErrorAction;
import java.util.HashMap;
import java.util.Map;

import javax.swing.text.BadLocationException;
import javax.swing.text.DefaultEditorKit;
import javax.swing.text.Document;
import javax.swing.text.ViewFactory;

import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.scinotes.actions.EncodingAction;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * The class ScilabEditorKit provides the minimal things needed to render
 * a Scilab's document.
 * @author Calixte DENIZET
 */
public class ScilabEditorKit extends DefaultEditorKit {

    private static final long serialVersionUID = 8005575461386086815L;

    /**
     * The mimetype for a scilab code
     */
    public static final String MIMETYPE = "text/scilab";
    private static final int BUFFERCAPACITY = 8192;
    public static final String[] ENCODINGS = new String[] {"utf-8", "windows-1252", "iso-8859-1"};
    private static final Map<String, Integer> encPos = new HashMap<String, Integer>(ENCODINGS.length);

    static {
        for (int i = 0; i < ENCODINGS.length; i++) {
            encPos.put(ENCODINGS[i], i);
        }
    }

    private final char[] buffer = new char[BUFFERCAPACITY];

    private ScilabContext preferences;
    private boolean plain;

    /**
     * Default constructor
     */
    public ScilabEditorKit() {
        super();
    }

    /**
     * Constructor
     * @param plain is true to use a plain view (no line-wrapping)
     */
    public ScilabEditorKit(boolean plain) {
        super();
        this.plain = plain;
    }

    public boolean isPlain() {
        return plain;
    }

    /**
     * @return the mime type
     */
    public String getContentType() {
        return MIMETYPE;
    }

    /**
     * @return a new ScilabDocument
     */
    public Document createDefaultDocument() {
        return new ScilabDocument();
    }

    /**
     * @return the context associated with the ScilabDocument
     */
    public ScilabContext getStylePreferences() {
        if (preferences == null) {
            preferences = new ScilabContext(plain);
        }

        return preferences;
    }

    /**
     * Set the context to render the document
     * @param prefs the context to use
     */
    public void setStylePreferences(ScilabContext prefs) {
        preferences = prefs;
    }

    /**
     * @return the ViewFactory used to create a View to render a ScilabDocument
     */
    public ViewFactory getViewFactory() {
        return getStylePreferences();
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
            while (bufReader.read(cbuf) != -1) {
                ;
            }
            fis.close();
            inReader.close();
            bufReader.close();
            return true;
        } catch (Exception e) { }

        return false;
    }

    /**
     * The read method is used to read the file and to write its contents
     * in the document at position pos
     * @param file the file to read
     * @param doc the Document where to write
     * @param pos position where to write
     * @throws CharacterCodingException if an unreadable char is encountered
     * @throws IOException if a problem is encountered in reading the stream
     * @throws BadLocationException if the pos is invalid
     */
    public void read(SciNotes editor, File file, Document doc, int pos) throws IOException, BadLocationException {
        Charset charset = Charset.forName(SciNotesOptions.getSciNotesPreferences().encoding);
        try {
            charset = tryToGuessEncoding(file);
        } catch (CharacterCodingException e) {
            ScilabModalDialog.show(editor, SciNotesMessages.CANNOT_GUESS_ENCODING, SciNotesMessages.SCINOTES_ERROR, IconType.ERROR_ICON);
        }
        ((ScilabDocument) doc).setEncoding(charset.toString());
        EncodingAction.updateEncodingMenu((ScilabDocument) doc);
        FileInputStream fis = new FileInputStream(file);
        InputStreamReader isr = new InputStreamReader(fis, charset);
        BufferedReader br = new BufferedReader(isr);
        read(br, doc, pos);
        try {
            br.close();
        } catch (IOException e) { }
    }

    /**
     * The read method is used to read the Reader and to write its contents
     * in the document at position pos
     * @param in the Reader to read
     * @param doc the Document where to write
     * @param pos position where to write
     * @throws IOException if a problem is encountered in reading the stream
     * @throws BadLocationException if the pos is invalid
     */
    public void read(Reader in, Document doc, int pos) throws IOException, BadLocationException {
        DocString docString = read(in);
        ((ScilabDocument) doc).setEOL(docString.eol);
        ((ScilabDocument) doc).setBinary(docString.isBinary);
        doc.insertString(pos, docString.content, null);
    }

    /**
     * The read method is used to read the Reader and to write its contents
     * in the document at position pos
     * @param in the Reader to read
     * @throws IOException if a problem is encountered in reading the stream
     */
    public DocString read(Reader in) throws IOException {
        int nch;
        int i;
        int prev;
        int inc = 0;
        boolean win = false;
        boolean mac = false;
        boolean first = true;
        boolean binary = false;
        DocString docString = new DocString();
        StringBuilder sbuf = new StringBuilder(buffer.length);
        while ((nch = in.read(buffer, 0, buffer.length)) != -1) {
            if (first) {
                /* We try to know if we have a binary file
                   The rule is : two \0 in the first 8ko : it's binary ! */
                CharBuffer cb = CharBuffer.wrap(buffer);
                byte[] bytes = Charset.forName("utf-8").encode(cb).array();
                for (i = 0; i < nch; i++) {
                    if (bytes[i] == 0 ) {
                        inc++;
                        if (inc == 2) {
                            binary = true;
                            break;
                        }
                    }
                }
                first = false;
            }

            prev = 0;
            if (!binary) {
                for (i = 0; i < nch - 1; i++) {
                    if (buffer[i] == '\r') {
                        buffer[i] = '\n';
                        sbuf.append(buffer, prev, i - prev + 1);
                        if (buffer[i + 1] == '\n') {
                            i++;
                            if (!win && !mac) {
                                docString.eol = ScilabDocument.EOLWIN;
                                win = true;
                            }
                        } else {
                            if (!win && !mac) {
                                docString.eol = ScilabDocument.EOLMAC;
                                mac = true;
                            }
                        }

                        prev = i + 1;
                    }
                }

                if (i == nch - 1) {
                    if (buffer[i] == '\r') {
                        if (!win && !mac) {
                            docString.eol = ScilabDocument.EOLMAC;
                        }
                        buffer[i] = '\n';
                    }
                    sbuf.append(buffer, prev, i - prev + 1);
                }
            } else {
                sbuf.append(buffer, 0, nch);
            }
        }
        if (!win && !mac) {
            docString.eol = ScilabDocument.EOLUNIX;
        }

        docString.isBinary = inc == 2;
        docString.content = sbuf.toString();

        return docString;
    }

    public static class DocString {

        public String eol;
        public boolean isBinary;
        public String content;

        DocString() { }
    }
}
