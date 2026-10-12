/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
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
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import java.awt.GraphicsEnvironment;

import org.scilab.modules.javasci.Scilab;
import org.scilab.modules.javasci.JavasciException;
import org.scilab.modules.javasci.JavasciException.InitializationException;
import org.scilab.modules.javasci.JavasciException.UnsupportedTypeException;
import org.scilab.modules.javasci.JavasciException.UndefinedVariableException;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.types.ScilabDouble;
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
    private CountDownLatch clicked = new CountDownLatch(1);

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
            x = new Test_sci(false);
            // wait for the Swing callback completion to avoid closing the engine with a job in flight
            try {
                clicked.await(30, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
                fail("interrupted while waiting for the Swing callback");
            }
        }
    }

    @Test()
    public void nonRegBug9544PlotFromThread() throws NullPointerException, JavasciException, IOException {
        if (!GraphicsEnvironment.isHeadless()) {
            x = new Test_sci(true);
            try {
                clicked.await(30, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
                fail("interrupted while waiting for the plot thread");
            }
            // the synchronous exec above covers plot(): Z proves the whole job ran
            assertTrue(sci.exec("Z = X .* Y;"));
            ScilabDouble z = (ScilabDouble) sci.get("Z");
            assertTrue(z.equals(new ScilabDouble(new double[][] {{3.0, 8.0}})));
            assertTrue(sci.exec("delete(gcf());"));
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

        private boolean plot;

        public Test_sci(boolean plot) { /*throws IOException*/
            this.plot = plot;
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
                if (plot) {
                    // the engine will block on the event dispatch thread while the
                    // graphics are created: run the job on a worker thread so that
                    // the event dispatch thread stays free
                    commands.add("plot(X, Y);");
                    commands.add("Z = X .* Y;");
                    new Thread(new Runnable() {
                        public void run() {
                            try {
                                assertTrue(sci.exec(commands.toArray(new String[commands.size()])));
                            } finally {
                                clicked.countDown();
                            }
                        }
                    }).start();
                    return;
                }
                // no graphics command here: the engine would wait for the event dispatch
                // thread, which is itself blocked in this synchronous exec
                commands.add("Z = X .* Y;");
                try {
                    assertTrue(sci.exec(commands.toArray(new String[commands.size()])));
                } finally {
                    clicked.countDown();
                }
            }

        }
    }
}
