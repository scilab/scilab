/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2021-2022 - UTC - Stéphane MOTTELET
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.gui.bridge.filechooser;

import java.io.File;
import java.lang.reflect.Method;
import java.util.StringTokenizer;
import java.util.concurrent.*;
import java.util.List;
import java.util.ArrayList;

import java.awt.Toolkit;
import java.awt.Dimension;

import javafx.application.Platform;
import javafx.embed.swing.JFXPanel;
import javafx.stage.Stage;
import javafx.stage.Modality;
import javafx.stage.StageStyle;
import javafx.stage.FileChooser;
import javafx.stage.FileChooser.ExtensionFilter;
import javafx.stage.DirectoryChooser;

import javax.swing.JFrame;

import org.scilab.modules.commons.OS;
import org.scilab.modules.gui.filechooser.FileChooserInfos;
import org.scilab.modules.gui.filechooser.ScilabFileChooserBridge;
import org.scilab.modules.gui.filechooser.SimpleFileChooser;
import org.scilab.modules.gui.utils.ConfigManager;
import org.scilab.modules.gui.utils.SciFileFilter;

/**
 * JavaFX implementation of a Scilab File Chooser
 * @author Stéphane Mottelet
 */

public class JFXScilabFileChooser implements SimpleFileChooser {
    private static final long serialVersionUID = 1L;

    public static final int OPEN_DIALOG = 0;
    public static final int SAVE_DIALOG = 1;
    private String[] selection; // Path + filenames
    private String selectionPath; // Path
    private String title;
    private static Double xPos = Double.NaN;
    private static Double yPos = Double.NaN;
    private String[] selectionFileNames; // Filenames
    private int selectionSize;
    private int filterIndex;
    private boolean acceptAllFileFilterUsed = true; // JFileChooser default
    private int dialogType = 0;
    private boolean directorySelectionOnly = false;
    private boolean multipleSelection = false;
    private DirectoryChooser directoryChooser;
    private FileChooser fileChooser;
    private final ExtensionFilter acceptAllFilter;
    private final List<ExtensionFilter> maskFilters = new ArrayList<ExtensionFilter>();
    // Raw settings, replayed on a Swing chooser if the JavaFX dialog fails
    private final List<String[][]> masks = new ArrayList<String[][]>();
    private String initialDirectory;
    private String initialFileName;
    private Stage stage;
    private static JFXPanel jfxpanel; // Instantiated only to start the JavaFX toolkit
    // GTK matches "*.*" literally, hiding files without extension
    private static final String ALL_FILES_PATTERN = OS.get() == OS.UNIX ? "*" : "*.*";


    public JFXScilabFileChooser() {
        startJavaFXToolkit();
        directoryChooser = new DirectoryChooser();
        fileChooser = new FileChooser();
        acceptAllFilter = new ExtensionFilter(new SciFileFilter("*.*", null, 0).getDescription(), ALL_FILES_PATTERN);
        setCurrentDirectory(new File(ConfigManager.getLastOpenedDirectory()));
    }

    /**
     * Start the JavaFX toolkit (if not already done) by creating a JFXPanel
     */
    public static synchronized void startJavaFXToolkit() {
        if (jfxpanel != null) {
            return;
        }
        jfxpanel = new JFXPanel();
        Platform.setImplicitExit(false);

        // The JavaFX application thread is not a daemon one and would prevent
        // DestroyJavaVM() from returning, so it has to be stopped when Scilab exits.
        try {
            Class<?> scilab = ClassLoader.getSystemClassLoader().loadClass("org.scilab.modules.core.Scilab");
            Method registerFinalHook = scilab.getDeclaredMethod("registerFinalHook", Runnable.class);
            registerFinalHook.invoke(null, new Runnable() {
                public void run() {
                    Platform.exit();
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    class getChosenFile implements Callable<ArrayList<File>> {
        @Override public ArrayList<File> call() throws Exception {
            // This method is invoked on the JavaFX thread
            ArrayList<File> theResult = new ArrayList<File>();

            fileChooser.setTitle(title);
            directoryChooser.setTitle(title);
            setupFilters();

            if (OS.get() == OS.MAC) {
                if (!(stage instanceof Stage)) {
                    stage = new Stage();
                    stage.setMaxHeight(0);
                    stage.setWidth(0);
                    stage.setResizable(false);
                    stage.setAlwaysOnTop(true);
                    stage.initStyle(StageStyle.UNIFIED);
                    if (xPos.isNaN()) {
                        Dimension d = Toolkit.getDefaultToolkit().getScreenSize(); // get screen size
                        xPos = d.width/2-stage.getWidth()/2;
                        yPos = (double)d.height/3;
                    }
                    stage.setX(xPos);
                    stage.setY(yPos);                    
                }
                stage.setTitle(title);
                stage.initModality(Modality.APPLICATION_MODAL);
                stage.show(); //show stage because you wouldn't be able to get Height & width of the stage
            }
            if (dialogType == JFXScilabFileChooser.OPEN_DIALOG) {
                if (multipleSelection == false) {
                    if (directorySelectionOnly == false)  {
                        File aFile = fileChooser.showOpenDialog(stage);
                        if (aFile != null) {
                            theResult.add(aFile);
                        }
                    } else {
                        File aFile = directoryChooser.showDialog(stage);
                        if (aFile != null) {
                            theResult.add(aFile);
                        }
                    }
                } else {
                    List<File> files = fileChooser.showOpenMultipleDialog(stage);
                    if (files != null) {
                        for (int i=0; i<files.size(); i++) {
                            theResult.add(files.get(i));
                        }
                    }
                }
            } else if (dialogType == JFXScilabFileChooser.SAVE_DIALOG) {
                String fileName = fileChooser.getInitialFileName();
                if (fileName != null && fileName != "")
                {
                    int index = fileName.lastIndexOf('.');
                    ExtensionFilter filter = null;
                    if(index > 0) {
                        String extension = "*"+fileName.substring(index);
                        filter = getFileFilterFromExtension(extension);
                        if (filter != null) {
                            fileChooser.setSelectedExtensionFilter(filter);
                            fileChooser.setInitialFileName(fileName.substring(0,index));
                        }
                    }
                    if (filter == null) {
                         filter = getFileFilterFromExtension(ALL_FILES_PATTERN);
                         if (filter != null) {
                             fileChooser.setSelectedExtensionFilter(filter);
                         }
                     }
                }
                
                File aFile  = fileChooser.showSaveDialog(stage);
                if (aFile != null) {
                    theResult.add(aFile);
                }
            }
            if (OS.get() == OS.MAC) {
                xPos = stage.getX();
                yPos = stage.getY();
                stage.close();
            }
            return theResult;
        }
    }

    /**
     * Set the parent frame
     * @param parent the parent frame
     */
    public void setParentFrame(JFrame parent) {
    }

    /**
     * Display this chooser and wait for user selection
     */
    public void displayAndWait() {
        ArrayList<File> result;
        try {
            FutureTask<ArrayList<File>> futureTask = new FutureTask<ArrayList<File>>(
              new getChosenFile()
            );
            Platform.runLater(futureTask);
            result = futureTask.get();
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            result = new ArrayList<File>();
        } catch (ExecutionException | RuntimeException | LinkageError e) {
            ScilabFileChooserBridge.disableJavaFX(e instanceof ExecutionException ? e.getCause() : e);
            if (stage != null) {
                // macOS owner stage, left open when the dialog failed
                final Stage ownerStage = stage;
                try {
                    Platform.runLater(() -> ownerStage.close());
                } catch (RuntimeException ignored) {
                }
            }
            displaySwingChooser();
            return;
        }

        try {
            selectionSize = result.size();
            if (selectionSize > 0) {
                selection = new String[selectionSize];
                selectionFileNames = new String[selectionSize];
                for (int i=0; i<result.size(); i++) {
                    File file = result.get(i).getAbsoluteFile();
                    selection[i] = file.getPath();
                    File parent = file.getParentFile();
                    selectionPath = (directorySelectionOnly || parent == null) ? file.getPath() : parent.getPath();
                    selectionFileNames[i] = file.getName();
                }
                filterIndex = getScilabFilterIndex();
                ConfigManager.saveLastOpenedDirectory(selectionPath);
            } else {
                // Cancel case
                selection = new String[1];
                selection[0] = "";
                selectionPath = "";
                selectionFileNames = new String[1];
                selectionFileNames[0] = "";
                filterIndex = 0;
            }
            
            //return the filechooser's information
            //they are stocked into FileChooserInfos
            FileChooserInfos.getInstance().setSelection(selection);
            FileChooserInfos.getInstance().setSelectionPathName(selectionPath);
            FileChooserInfos.getInstance().setSelectionFileNames(selectionFileNames);
            FileChooserInfos.getInstance().setSelectionSize(selectionSize);
            FileChooserInfos.getInstance().setFilterIndex(filterIndex);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * Replay this chooser's settings on a Swing chooser, which also fills FileChooserInfos
     */
    private void displaySwingChooser() {
        SwingScilabFileChooser swingChooser = SwingScilabFileChooser.create();
        swingChooser.setTitle(title);
        if (directorySelectionOnly) {
            swingChooser.setDirectorySelectionOnly();
        }
        swingChooser.setAcceptAllFileFilterUsed(acceptAllFileFilterUsed);
        for (String[][] mask : masks) {
            swingChooser.addMask(mask[0], mask[1]);
        }
        if (initialDirectory != null) {
            swingChooser.setInitialDirectory(initialDirectory);
        }
        if (initialFileName != null) {
            swingChooser.setInitialFileName(initialFileName);
        }
        swingChooser.setMultipleSelection(multipleSelection);
        swingChooser.setUiDialogType(dialogType);
        swingChooser.displayAndWait();

        selection = swingChooser.getSelection();
        selectionPath = swingChooser.getSelectionPathName();
        selectionFileNames = swingChooser.getSelectionFileNames();
        selectionSize = swingChooser.getSelectionSize();
        filterIndex = FileChooserInfos.getInstance().getFilterIndex();
    }

    public void setAcceptAllFileFilterUsed(boolean flag) {
        acceptAllFileFilterUsed = flag;
    }

    /**
     * Set the mask & the mask description for the filechooser
     * @param mask the mask to set
     * @param fileMaskDescription the maskDescription to set
     */
    public void addMask(String[] theMask, String[] theFileMaskDescription) {
        masks.add(new String[][] {theMask, theFileMaskDescription});
        boolean noDescription = theFileMaskDescription == null || theFileMaskDescription.length == 0;
        for (int i=0; i<theMask.length; i++) {
            SciFileFilter filter = new SciFileFilter(theMask[i], noDescription ? null : theFileMaskDescription[i], i);
            maskFilters.add(new ExtensionFilter(filter.getDescription(), getPatterns(theMask[i])));
        }
    }

    /**
     * Same filters and default selection as the Swing chooser: "All files" first,
     * then the masks; the last mask is selected when "All files" is listed, the first one otherwise.
     */
    private void setupFilters() {
        List<ExtensionFilter> filters = fileChooser.getExtensionFilters();
        filters.clear();
        if (acceptAllFileFilterUsed) {
            filters.add(acceptAllFilter);
        }
        filters.addAll(maskFilters);
        if (!maskFilters.isEmpty()) {
            fileChooser.setSelectedExtensionFilter(maskFilters.get(acceptAllFileFilterUsed ? maskFilters.size() - 1 : 0));
        } else if (acceptAllFileFilterUsed) {
            fileChooser.setSelectedExtensionFilter(acceptAllFilter);
        }
    }

    /**
     * Swing numbering: masks from 1 in the order given, then "All files".
     */
    private int getScilabFilterIndex() {
        ExtensionFilter selected = fileChooser.getSelectedExtensionFilter();
        int index = maskFilters.indexOf(selected);
        if (index >= 0) {
            return index + 1;
        }
        return (selected != null && selected == acceptAllFilter) ? maskFilters.size() + 1 : 0;
    }

    /**
     * Native dialogs match patterns literally: "all" is a SciFileFilter keyword,
     * not a pattern, and blanks around '|' would become part of the extension.
     */
    private static List<String> getPatterns(String mask) {
        List<String> patterns = new ArrayList<String>();
        if (mask.equals("all")) {
            for (String ext : new SciFileFilter(mask, null, 0).getExtensions()) {
                patterns.add("*." + ext);
            }
        } else {
            for (String pattern : mask.split("\\|")) {
                pattern = pattern.trim();
                if (pattern.equals("*.*")) {
                    patterns.add(ALL_FILES_PATTERN);
                } else if (!pattern.isEmpty()) {
                    patterns.add(pattern);
                }
            }
        }
        return patterns;
    }

    public ExtensionFilter getFileFilterFromExtension(String extension) {
        for (ExtensionFilter filter : fileChooser.getExtensionFilters())   
        {
            for (String filtext : filter.getExtensions())
            {
                if (filtext.equals(extension))
                {                                    
                    return filter;
                }
            }
        }
        return null;
    }

    public void setTitle(String string) {
        title = string;
    }

    public void setCurrentDirectory(File file) {
        // Like JFileChooser, fall back on the nearest existing parent
        File dir = file.getAbsoluteFile();
        while (dir != null && !dir.isDirectory()) {
            dir = dir.getParentFile();
        }
        if (dir == null) {
            dir = new File(System.getProperty("user.dir"));
        }
        // Both, as setDirectorySelectionOnly() may be called after this method
        directoryChooser.setInitialDirectory(dir);
        fileChooser.setInitialDirectory(dir);
    }

    public void setInitialDirectory(String path) {
        // When empty string given
        if (path == null || path.length() == 0) {
            return;
        }
        initialDirectory = path;
        // Replace beginning of the path if is an environment variable
        String newPath = path;
        StringTokenizer tok = new StringTokenizer(path, File.separator);
        if (tok.hasMoreTokens()) {
            /* It is possible that we don't have any more token here when
                                          Scilab is started from / for example */
            String firstToken = tok.nextToken();
            if (firstToken != null && System.getenv(firstToken) != null)  {
                newPath = newPath.replaceFirst(firstToken, System.getenv(firstToken));
            }
        }
        setCurrentDirectory(new File(newPath));
    }

    public void setInitialFileName(String path) {
        if (path == null || path.length() == 0) {
            return;
        }
        initialFileName = path;
        File file = new File(path);
        fileChooser.setInitialFileName(file.getName());
     }

    /**
     * Get the number of files selected
     * @return the number of files selected
     */
    public int getSelectionSize() {
        return selectionSize;
    }

    /**
     * Get the names of selected files
     * @return the names of selected files
     */
    public String[] getSelection() {
        return selection;
    }

    /**
     * Set the flag indicating that we want only select directories
     */
    public void setDirectorySelectionOnly() {
        directorySelectionOnly = true;
    }

    /**
     * Set the flag indicating that we can select multiple files
     * @param multipleSelection enable multiple selection
     */
    public void setMultipleSelection(boolean flag)
    {
        multipleSelection = flag;
    }

    /**
     * Get the path of selected files
     * @return the path of selected files
     */
    public String getSelectionPathName() {
        return selectionPath;
    }

    /**
     * Get the names of selected files
     * @return the names of selected files
     */
    public String[] getSelectionFileNames(){
        return selectionFileNames;
    }

    public void invalidate() {
    }

    /**
     * Set the dialog type (save or open a file ?)
     * @param dialogType the dialog type
     */
    public void setUiDialogType(int type)
    {
        // LOAD = 0, SAVE = 1
        dialogType = type;
    }

}
