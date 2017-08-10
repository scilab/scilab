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

package org.scilab.modules.ui_data.variableeditor.actions;

import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.asynchronousScilabExec;

import java.awt.Component;
import java.util.HashMap;
import java.util.Map;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JPopupMenu;
import javax.swing.JTable;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.commons.gui.ScilabLAF;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.types.ScilabTypeEnumDescription;
import org.scilab.modules.ui_data.BrowseVar;
import org.scilab.modules.ui_data.EditVar;
import org.scilab.modules.ui_data.datatable.SwingEditvarTableModel;
import org.scilab.modules.ui_data.variablebrowser.SwingScilabVariableBrowser;
import org.scilab.modules.ui_data.variableeditor.SwingScilabVariableEditor;

/**
 * RefreshAction class
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public final class PlotAction extends CommonCallBack {

    public static final int PLOT2D = 0;
    public static final int MATPLOT = 1;
    public static final int GRAYPLOT = 2;
    public static final int SGRAYPLOT = 3;
    public static final int CHAMP = 4;
    public static final int HISTPLOT = 5;
    public static final int MESH = 6;
    public static final int SURF = 7;
    public static final int HIST3D = 8;
    public static final int CONTOUR2D = 9;
    public static final int PIE = 10;

    private static final Map<String, Integer> map = new HashMap<String, Integer>();
    static {
        map.put("plot2d", 0);
        map.put("Matplot", 1);
        map.put("grayplot", 2);
        map.put("Sgrayplot", 3);
        map.put("champ", 4);
        map.put("histplot", 5);
        map.put("mesh", 6);
        map.put("surf", 7);
        map.put("hist3d", 8);
        map.put("contour2d", 9);
        map.put("pie", 10);
    }

    private static final String COM_PLOT2D = "plot2d(%s)";
    private static final String COM_MATPLOT = "Matplot(%s)";
    private static final String COM_GRAYPLOT = "grayplot(1:%s,1:%s,%s)";
    private static final String COM_SGRAYPLOT = "Sgrayplot(1:%s,1:%s,%s)";
    private static final String COM_CHAMP = "champ(1:%s,1:%s,real(%s),imag(%s))";
    private static final String COM_HISTPLOT = "histplot(10,%s)";
    private static final String COM_MESH = "mesh(%s)";
    private static final String COM_SURF = "surf(%s)";
    private static final String COM_HIST3D = "hist3d(%s)";
    private static final String COM_CONTOUR2D = "L?8625083632641564278=xget(\"fpf\");xset(\"fpf\",\" \");contour2d(1:%s,1:%s,%s,10);xset(\"fpf\",L?8625083632641564278);clear(\"L?8625083632641564278\")";
    private static final String COM_PIE = "L?8625083632641564278=%s;pie(L?8625083632641564278(find(L?8625083632641564278>0&L?8625083632641564278<>%%inf&L?8625083632641564278<>%%nan)))";

    private static final String[] COMMANDS = new String[] {COM_PLOT2D, COM_MATPLOT, COM_GRAYPLOT, COM_SGRAYPLOT, COM_CHAMP, COM_HISTPLOT, COM_MESH, COM_SURF, COM_HIST3D, COM_CONTOUR2D, COM_PIE};
    private static final String[] IMG = new String[] {
        "application-x-scilab-plot2d",
        "application-x-scilab-Matplot",
        "application-x-scilab-grayplot",
        "application-x-scilab-Sgrayplot",
        "application-x-scilab-champ",
        "application-x-scilab-histplot",
        "application-x-scilab-mesh",
        "application-x-scilab-surf",
        "application-x-scilab-hist3d",
        "application-x-scilab-contour2d",
        "application-x-scilab-pie"
    };

    private static final String CREATE = "Create";

    private final SwingScilabDockablePanel editor;
    private final int type;
    private final boolean onSelection;

    /**
     * Constructor
     * @param editor the editor
     * @param name the name of the action
     */
    public PlotAction(SwingScilabDockablePanel editor, String name, boolean onSelection) {
        super(name);
        this.editor = editor;
        this.type = map.get(name);
        this.onSelection = onSelection;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void callBack() {

        String data = null;
        int rowC = 0;
        int colC = 0;
        String modelType = null;
        SwingEditvarTableModel model = null;

        if (editor instanceof SwingScilabVariableEditor) {
            JTable table = ((SwingScilabVariableEditor)editor).getCurrentTable();
            model = (SwingEditvarTableModel) table.getModel();
            modelType = model.getType();

            if (!onSelection) {
                rowC = model.getScilabMatrixRowCount();
                colC = model.getScilabMatrixColCount();
                data = model.getVarName();
            } else {
                int[] cols = table.getSelectedColumns();
                int[] rows = table.getSelectedRows();
                if (cols == null || cols.length == 0 || rows == null || rows.length == 0) {
                    rowC = model.getScilabMatrixRowCount();
                    colC = model.getScilabMatrixColCount();
                    data = model.getVarName();
                } else {
                    if (model.getType().equals(EditVar.STRING)) {
                        return;
                    }

                    int[] rowSize = new int[1];
                    int[] colSize = new int[1];
                    data = model.getScilabSubMatrix(rows, cols, rowSize, colSize);
                    if (data == null) {
                        return;
                    }
                    rowC = rowSize[0];
                    colC = colSize[0];
                }
            }

            if (!modelType.equals(EditVar.DOUBLE)) {
                data = "double(" + data + ")";
            }

        } else {
            // Variable Browser
            int clickedRow = ((SwingScilabVariableBrowser)editor).getTable().getSelectedRow();

            // Does nothing if no variable selected
            if (clickedRow != -1) {
                rowC = Integer.parseInt(((SwingScilabVariableBrowser)editor).getTable().getModel().getValueAt(clickedRow, BrowseVar.NB_ROWS_INDEX).toString());
                colC = Integer.parseInt(((SwingScilabVariableBrowser)editor).getTable().getModel().getValueAt(clickedRow, BrowseVar.NB_COLS_INDEX).toString());
                data = ((SwingScilabVariableBrowser)editor).getTable().getValueAt(clickedRow, BrowseVar.NAME_COLUMN_INDEX).toString();
            }
            int type = Integer.parseInt(((SwingScilabVariableBrowser)editor).getTable().getModel().getValueAt(clickedRow, BrowseVar.TYPE_COLUMN_INDEX).toString());

            if (!ScilabTypeEnumDescription.getTypeDescriptionFromId(type).equals(EditVar.DOUBLE)) {
                data = "double(" + data + ")";
            }

        }

        if (rowC == 0 || colC == 0) {
            return;
        }

        String com = COMMANDS[type];
        switch (type) {
            case 0:
            case 1:
            case 5:
            case 6:
            case 7:
            case 8:
            case 10:
                com = String.format(com, data);
                break;
            case 2:
            case 3:
                com = String.format(com, rowC, colC, data);
                break;
            case 4:
                com = String.format(com, rowC, colC, data, data);
                break;
            case 9:
                if (rowC >= 2 && colC >= 2) {
                    com = String.format(com, rowC, colC, data);
                } else {
                    return;
                }
                break;
        }
        if (editor instanceof SwingScilabVariableEditor) {
            model.execCommand("scf();" + com);
        } else {
            // Browse var case
            try {
                asynchronousScilabExec(null, "scf();" + com);
            } catch (InterpreterException e1) {
                System.err.println("An error in the interpreter has been catched: " + e1.getLocalizedMessage());
            }

        }
    }

    /**
     * Create a button for a tool bar
     * @param editor the associated editor
     * @param title tooltip for the button
     * @return the button
     */
    public static JButton createButton(SwingScilabDockablePanel editor, String title) {
        final JButton button = new JButton();
        ScilabLAF.setDefaultProperties(button);
        button.setToolTipText(title);
        ImageIcon imageIcon = new ImageIcon(FindIconHelper.findIcon("plot"));
        button.setIcon(imageIcon);

        final JPopupMenu popup = new JPopupMenu() {
            @Override
            public void show(Component c, int x, int y) {
                super.show(button, 0, button.getBounds(null).height);
            }
        };
        popup.setBorderPainted(true);

        popup.add(PlotAction.createJMenuItem(editor, "plot2d", true));
        popup.add(PlotAction.createJMenuItem(editor, "Matplot", true));
        popup.add(PlotAction.createJMenuItem(editor, "grayplot", true));
        popup.add(PlotAction.createJMenuItem(editor, "Sgrayplot", true));
        popup.add(PlotAction.createJMenuItem(editor, "champ", true));
        popup.add(PlotAction.createJMenuItem(editor, "histplot", true));
        popup.add(PlotAction.createJMenuItem(editor, "mesh", true));
        popup.add(PlotAction.createJMenuItem(editor, "surf", true));
        popup.add(PlotAction.createJMenuItem(editor, "hist3d", true));
        popup.add(PlotAction.createJMenuItem(editor, "contour2d", true));
        popup.add(PlotAction.createJMenuItem(editor, "pie", true));

        popup.pack();

        button.addActionListener(new CommonCallBack(null) {
            @Override
            public void callBack() {
                if (!popup.isVisible()) {
                    popup.show(null, 0, 0);
                } else {
                    popup.setVisible(false);
                }
            }
        });

        return button;
    }

    /**
     * Create a menu item
     * @param editor the associated editor
     * @param title the menu title
     * @return the menu item
     */
    public static MenuItem createMenuItem(SwingScilabDockablePanel editor, String title, boolean onSelection) {
        MenuItem menu = ScilabMenuItem.createMenuItem();
        menu.setCallback(new PlotAction(editor, title, onSelection));
        menu.setText(title);
        ((SwingScilabMenuItem) menu.getAsSimpleMenuItem()).setIcon(new ImageIcon(FindIconHelper.findIcon(IMG[map.get(title)])));

        return menu;
    }

    /**
     * Create a menu item as a SwingScilabMenuItem
     * @param editor the associated editor
     * @param title the menu title
     * @return the menu item
     */
    public static SwingScilabMenuItem createJMenuItem(SwingScilabDockablePanel editor, String title, boolean onSelection) {
        return (SwingScilabMenuItem) createMenuItem(editor, title, onSelection).getAsSimpleMenuItem();
    }
}
