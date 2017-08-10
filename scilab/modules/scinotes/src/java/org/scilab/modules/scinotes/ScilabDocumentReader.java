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

package org.scilab.modules.scinotes;

import java.io.Reader;
import java.io.IOException;
import javax.swing.text.BadLocationException;
import javax.swing.text.Segment;

/**
 * Class ScilabDocumentReader provides a read method to read a part of the document
 * from left to right and from right to left (useful to find matching keywords such
 * as function...endfunction or noitcnufdne...noitcnuf)
 * @author Calixte DENIZET
 */
public class ScilabDocumentReader extends Reader {

    private static final String BADLOCATION = "Bad Location in document";

    private int end;
    private int pos;
    private boolean fromEnd;
    private ScilabDocument doc;
    private Segment segment;

    /**
     * Constructor for a reader from left to right
     * @param doc the doc to read
     * @param start the start in the doc
     * @param end the end in the doc
     */
    public ScilabDocumentReader(ScilabDocument doc, int start, int end) {
        this(doc, false, start, end);
    }

    /**
     * Constructor for a reader from left to right (fromEnd == false) or from right to left (fromEnd == true).
     * @param doc the doc to read
     * @param fromEnd LR or RL
     * @param start the start in the doc
     * @param end the end in the doc
     */
    public ScilabDocumentReader(ScilabDocument doc, boolean fromEnd, int start, int end) {
        this.doc = doc;
        this.segment = new Segment();
        if (fromEnd) {
            this.pos = Math.min(start, doc.getLength());
            this.end = end;
        } else {
            this.pos = start;
            this.end = Math.min(end, doc.getLength());
        }
        this.fromEnd = fromEnd;
    }

    /**
     * The read method for a Reader.
     * @param cbuf the buffer where to put the chars
     * @param pos position in the doc
     * @param len the number of chars to be read
     * @return the number of read chars
     * @throws IOException if a problem is encountered
     */
    public int read(char[] cbuf, int pos, int len) throws IOException {
        if (doc == null) {
            throw new IOException("Reader closed");
        }
        int length = len;

        if (!fromEnd) {
            if (this.pos < end) {
                if (length > end - this.pos) {
                    length = end - this.pos;
                }
                try {
                    doc.getText(this.pos, length, segment);
                    System.arraycopy(segment.array, segment.offset, cbuf, pos, length);
                    this.pos += length;
                    return length;
                } catch (BadLocationException e) {
                    throw new IOException(BADLOCATION);
                }
            }
        } else {
            if (this.pos > end) {
                if (length > this.pos - end + 1) {
                    length = this.pos - end + 1;
                }
                try {
                    doc.getText(this.pos - length + 1, length, segment);
                    int ppos = segment.offset + segment.count - 1;
                    for (int i = 0; i < length; i++) {
                        cbuf[pos + i] = segment.array[ppos - i];
                    }
                    this.pos -= length;
                    return length;
                } catch (BadLocationException e) {
                    throw new IOException(BADLOCATION);
                }
            }
        }
        return -1;
    }

    /**
     * The close method of a Reader
     * @throws IOException if a problem is encountered
     */
    public void close() throws IOException {
        doc = null;
    }
}
