/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU
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
package org.scilab.tests.modules.javasci;

import org.junit.*;
import static org.junit.Assert.*;

import java.io.File;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;

import java.awt.GraphicsEnvironment;

import org.scilab.modules.javasci.Scilab;
import org.scilab.modules.javasci.JavasciException;
import org.scilab.modules.javasci.JavasciException.InitializationException;
import org.scilab.modules.javasci.JavasciException.UnsupportedTypeException;
import org.scilab.modules.javasci.JavasciException.UndefinedVariableException;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.types.ScilabInteger;
import org.scilab.modules.types.ScilabTypeEnum;
import org.scilab.modules.commons.ScilabCommons;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.SwingUtilities;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;

public class testBug9544 {
    private Scilab sci;
    private Test_sci x;

    /*
     * This method will be called for each test.
     * with @After, this ensures that all the time the engine is closed
     * especially in case of error.
     * Otherwise, the engine might be still running and all subsequent tests
     * would fail.
     */
    @Before
    public void open() throws NullPointerException, JavasciException {
        sci = new Scilab(true);
        assertTrue(sci.open());
    }

    @Test()
    public void nonRegBug9544Working() throws NullPointerException, JavasciException, IOException {
        if (!GraphicsEnvironment.isHeadless()) {
            x = new Test_sci();
        }
    }

    /**
     * See #open()
     */
    @After
    public void close() {
        if (!GraphicsEnvironment.isHeadless()) {
            x.dispose();
        }
        sci.close();

    }

    public class Test_sci extends JFrame {

        private JPanel container = new JPanel();
        private JButton b = new JButton ("Auto clicked button");
        private JLabel resultat;

        private Scilab sci;



        public Test_sci() { /*throws IOException*/
            try {
                sci = new Scilab(true);
            } catch (org.scilab.modules.javasci.JavasciException f) {
                System.err.println("An exception occurred: " + f.getLocalizedMessage());
            }
            this.setTitle("bug 9544 non reg test");
            this.setSize(200, 100);
            this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

            JPanel top = new JPanel();
            b.addActionListener(new BoutonListener());

            top.add(b);

            this.setContentPane(top);
            this.setVisible(true);

            new Thread(new Runnable() {
                public void run() {
                    SwingUtilities.invokeLater(new Runnable() {
                        public void run() {
                            b.doClick();
                        }
                    });
                }
            }).start();

        }

        class BoutonListener implements ActionListener {

            public void actionPerformed(ActionEvent g) {
                List<String> commands = new ArrayList<String>();
                commands.add("X = [1,2];");
                commands.add("Y = [3,4];");
                commands.add("plot(X,Y);");
                assertTrue(sci.exec(commands.toArray(new String[commands.size()])));
            }

        }
    }
}
