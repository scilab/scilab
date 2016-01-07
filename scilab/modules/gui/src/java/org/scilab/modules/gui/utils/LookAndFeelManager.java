/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2007 - INRIA - Sylvestre Ledru
 * Copyright (C) 2015 - Scilab Enterprises - Clement DAVID
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 * === LICENSE_END ===
 *
 */

package org.scilab.modules.gui.utils;

import java.awt.Font;
import java.awt.FontFormatException;
import java.awt.GraphicsEnvironment;
import java.io.File;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;

import javax.swing.SwingUtilities;
import javax.swing.UIDefaults;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;
import javax.swing.plaf.FontUIResource;

/**
 * Class used to modify look and feel in Scilab GUIs
 * @author Allan CORNET
 * @author Sylvestre Ledru
 */
public class LookAndFeelManager {
    private static LookAndFeelManager instance;

    public static LookAndFeelManager getInstance() {
        if (instance == null) {
            instance = new LookAndFeelManager();
        }
        return instance;
    }

    private final UIManager.LookAndFeelInfo[] availableLookAndFeels;
    private boolean ret;

    private Font serif;
    private Font sansSerif;
    private Font monospaced;
    private Font dialog;
    private Font dialogInput;


    /**
     * Constructor
     */
    public LookAndFeelManager() {
        availableLookAndFeels = UIManager.getInstalledLookAndFeels();
    }

    public Font getSerifFont() {
        if (serif == null) {
            serif = new Font(Font.SERIF, Font.PLAIN, 12);
        }
        return serif;
    }

    public Font getSansSerifFont() {
        if (sansSerif == null) {
            sansSerif = new Font(Font.SANS_SERIF, Font.PLAIN, 12);
        }
        return sansSerif;
    }

    public Font getMonospacedFont() {
        if (monospaced == null) {
            monospaced = new Font(Font.MONOSPACED, Font.PLAIN, 12);
        }
        return monospaced;
    }

    public Font getDialogFont() {
        if (dialog == null) {
            dialog = new Font(Font.DIALOG, Font.PLAIN, 12);
        }
        return dialog;
    }

    public Font getDialogInputFont() {
        if (dialogInput == null) {
            dialogInput = new Font(Font.DIALOG_INPUT, Font.PLAIN, 12);
        }
        return dialogInput;
    }




    /**
     * Get Installed Look and Feels
     * @return an array of String
     */
    public String[] getInstalledLookAndFeels() {
        String[] retStrings = new String[availableLookAndFeels.length];
        for (int i = 0; i < availableLookAndFeels.length; i++) {
            retStrings[i] =  availableLookAndFeels[i].getClassName();
        }
        return retStrings;
    }

    /**
     * Get numbers of Installed Look and Feels
     * @return size of array of String
     */
    public int numbersOfInstalledLookAndFeels() {
        return availableLookAndFeels.length;
    }

    /**
     * Get Look and Feel
     * @return a String
     */
    public String getCurrentLookAndFeel() {
        return UIManager.getLookAndFeel().getClass().getName();
    }

    /**
     * Is this look and feel exists or not
     * @param lookandfeel the name of the lookandfeel we want check if exists
     * @return if it exists or not
     */
    public boolean isSupportedLookAndFeel(String lookandfeel) {
        for (int i = 0; i < availableLookAndFeels.length; i++) {
            if (availableLookAndFeels[i].getClassName().equals(lookandfeel)) {
                return true;
            }
        }
        return false;
    }

    /**
     * Set Look and Feel
     * @param lookandfeel a String
     * @return a boolean
     */
    public boolean setLookAndFeel(final String lookandfeel) {
        ret = false;
        try {
            SwingUtilities.invokeAndWait(new Runnable() {
                public void run() {
                    try {
                        UIManager.setLookAndFeel(lookandfeel);
                        registerEmbeddedFonts();
                        storeEmbeddedFonts();
                        initComponentDefaults(UIManager.getDefaults());

                        ret = true;
                    } catch (UnsupportedLookAndFeelException e) {
                        System.err.println("Cannot find this look and feel:");
                        System.err.println(e.getLocalizedMessage());
                    } catch (ClassNotFoundException e) {
                        System.err.println("LookAndFeel class could not be found:");
                        System.err.println(e.getLocalizedMessage());
                    } catch (IllegalAccessException e) {
                        System.err.println("Illegal access while setting the Look And Feel:");
                        System.err.println(e.getLocalizedMessage());
                    } catch (InstantiationException e) {
                        System.err.println("Instantiation error while setting the Look And Feel:");
                        System.err.println(e.getLocalizedMessage());
                    } catch (FontFormatException e) {
                        System.err.println("Invalid Font format:");
                        System.err.println(e.getLocalizedMessage());
                    } catch (IOException e) {
                        System.err.println("Unable to open Font:");
                        System.err.println(e.getLocalizedMessage());
                    }
                }
            });
        } catch (InterruptedException e) { }
        catch (InvocationTargetException e) {
            System.err.println(e);
        }

        return ret;
    }

    /**
     * Register the shipped fonts
     */
    private void registerEmbeddedFonts() throws FontFormatException, IOException {
        GraphicsEnvironment ge = GraphicsEnvironment.getLocalGraphicsEnvironment();

        File[] possibleDirs = {
            new File(System.getenv("SCI"), "/../fonts/google-droid"),   // binary version (eg.  where we have a "share")
            new File(System.getenv("SCI"), "/desktop"),                 // Linux dev. version or Windows install
        };

        // detect existing dir
        File dir = null;
        for (File d : possibleDirs) {
            if (d.isDirectory()) { // also check if it exists implicitly
                dir = d;
                break;
            }
        }
        if (dir == null) {
            return;
        }

        // register fonts
        String[] filenames = {"DroidSans-Bold.ttf", "DroidSansMono.ttf", "DroidSans.ttf", "DroidSerif-BoldItalic.ttf", "DroidSerif-Bold.ttf", "DroidSerif-Italic.ttf", "DroidSerif-Regular.ttf"}; ;
        final Float size = 11f;

        for (String filename : filenames) {
            File f  = new File(dir, filename);

            if (f.canRead()) {
                Font base = Font.createFont(Font.TRUETYPE_FONT, f);

                Font derived = base.deriveFont(size);
                ge.registerFont(derived);
            }
        }
    }

    /**
     * Store on local fields the default used fonts
     */
    private void storeEmbeddedFonts() {
        serif = new Font("Droid Serif", Font.PLAIN, 12);
        sansSerif = new Font("Droid Sans", Font.PLAIN, 12);
        monospaced = new Font("Droid Sans Mono", Font.PLAIN, 12);
        dialog = new Font("Droid Sans", Font.PLAIN, 12);
        dialogInput = new Font("Droid Sans Mono", Font.PLAIN, 12);
    }

    /**
     * Override some defaults previously set in GTKLookAndFell#initComponentDefaults(UIDefaults)
     *
     * @param table the default settings
     */
    private void initComponentDefaults(UIDefaults table) {
        final FontUIResource defaultFontUIResource = new FontUIResource(getDialogFont());
        final FontUIResource defaultMonospacedFontUIResource = new FontUIResource(getMonospacedFont());

        table.put("ArrowButton.font", defaultFontUIResource);
        table.put("Button.font", defaultFontUIResource);
        table.put("CheckBox.font", defaultFontUIResource);
        table.put("CheckBoxMenuItem.font", defaultFontUIResource);
        table.put("ColorChooser.font", defaultFontUIResource);
        table.put("ComboBox.font", defaultFontUIResource);
        table.put("DesktopPane.font", defaultFontUIResource);
        table.put("DesktopIcon.font", defaultFontUIResource);
        table.put("EditorPane.font", defaultFontUIResource);
        table.put("FileChooser.font", defaultFontUIResource);
        table.put("FormattedTextField.font", defaultFontUIResource);
        table.put("InternalFrame.font", defaultFontUIResource);
        table.put("InternalFrameTitlePane.font", defaultFontUIResource);
        table.put("Label.font", defaultFontUIResource);
        table.put("List.font", defaultFontUIResource);
        table.put("Menu.font", defaultFontUIResource);
        table.put("MenuBar.font", defaultFontUIResource);
        table.put("MenuItem.font", defaultFontUIResource);
        table.put("OptionPane.font", defaultFontUIResource);
        table.put("Panel.font", defaultFontUIResource);
        table.put("PasswordField.font", defaultFontUIResource);
        table.put("PopupMenu.font", defaultFontUIResource);
        table.put("PopupMenuSeparator.font", defaultFontUIResource);
        table.put("ProgressBar.font", defaultFontUIResource);
        table.put("RadioButton.font", defaultFontUIResource);
        table.put("RadioButtonMenuItem.font", defaultFontUIResource);
        table.put("RootPane.font", defaultFontUIResource);
        table.put("ScrollBar.font", defaultFontUIResource);
        table.put("ScrollBarTrack.font", defaultFontUIResource);
        table.put("ScrollBarThumb.font", defaultFontUIResource);
        table.put("ScrollPane.font", defaultFontUIResource);
        table.put("Separator.font", defaultFontUIResource);
        table.put("Slider.font", defaultFontUIResource);
        table.put("SliderTrack.font", defaultFontUIResource);
        table.put("SliderThumb.font", defaultFontUIResource);
        table.put("Spinner.font", defaultFontUIResource);
        table.put("SplitPane.font", defaultFontUIResource);
        table.put("TabbedPane.font", defaultFontUIResource);
        table.put("Table.font", defaultFontUIResource);
        table.put("TableHeader.font", defaultFontUIResource);
        table.put("TextArea.font", defaultFontUIResource);
        table.put("TextField.font", defaultMonospacedFontUIResource);
        table.put("TextPane.font", defaultMonospacedFontUIResource);
        table.put("ToggleButton.font", defaultFontUIResource);
        table.put("ToolBar.font", defaultFontUIResource);
        table.put("ToolTip.font", defaultFontUIResource);
        table.put("Tree.font", defaultFontUIResource);
        table.put("Viewport", defaultFontUIResource);
    }

    /**
     * Set System Look and Feel
     * @return a boolean
     */
    public boolean setSystemLookAndFeel() {
        return setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
    }
}
