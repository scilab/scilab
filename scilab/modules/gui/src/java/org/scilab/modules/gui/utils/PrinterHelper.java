/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.utils;

import java.awt.print.PrinterException;
import java.awt.print.PrinterJob;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import javax.swing.JEditorPane;

import org.scilab.modules.localization.Messages;
import org.scilab.modules.gui.messagebox.MessageBox;
import org.scilab.modules.gui.messagebox.ScilabMessageBox;

/**
 *
 * @author Allan CORNET - DIGITEO
 *
 */
public class PrinterHelper {
    /**
     * default constructor
     */
    protected PrinterHelper() {

    }

    /**
     * Print a string
     * @param theString String to print
     * @param pageHeader header page added for first page
     * @return true if printed
     */
    public static boolean printString(String theString, String pageHeader) {
        String stringWithHeader = pageHeader + "\n\n" + theString;
        return printString(stringWithHeader);
    }

    /**
     * Print a string
     * @param theString String to print
     * @return true if printed
     */
    public static boolean printString(String theString) {
        JEditorPane pageToPrint = new JEditorPane("Text/html", theString);
        pageToPrint.setEditable(false);
        PrinterJob printTask = PrinterJob.getPrinterJob();
        printTask.setPrintable(new PrinterWriter(pageToPrint));
        try {
            printTask.print();
        } catch (PrinterException e) {
            MessageBox messageBox = ScilabMessageBox.createMessageBox();
            messageBox.setMessage(Messages.gettext("An error occurred: ") + e.getLocalizedMessage());
            messageBox.setModal(true);
            messageBox.setIcon("error");
            messageBox.displayAndWait();
            return false;
        }
        return true;
    }

    /**
     * print a file
     * @param fileName input filename
     * @return true if printed
     */
    public static boolean printFile(String fileName) {
        JEditorPane pageToPrint = new JEditorPane();

        URL url;
        try {
            url = new URL("file:///" + fileName);
            pageToPrint.setEditable(false);
            pageToPrint.setContentType("text/plain;charset=UTF-8");
            try {
                pageToPrint.setPage(url);
            } catch (IOException e) {
                return false;
            }
        } catch (MalformedURLException e1) {
            e1.printStackTrace();
            return false;
        }

        PrinterJob printTask = PrinterJob.getPrinterJob();
        printTask.setPrintable(new PrinterWriter(pageToPrint));
        try {
            printTask.print();
        } catch (PrinterException e) {
            e.printStackTrace();
            return false;
        }
        return true;
    }

    /*
    public static void main(String args[]) {
    	printString("Scilab\nTest");
    	printFile("D:/GIT-scilab-master/scilab/scilab/etc/scilab.start");
    }
    */

}

