/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.block.actions;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Arrays;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.swing.BorderFactory;
import javax.swing.DefaultCellEditor;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.WindowConstants;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;
import javax.swing.table.DefaultTableModel;
import javax.swing.table.TableColumn;

import org.scilab.modules.graph.ScilabComponent;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfDouble;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.ScilabTypeCoder;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Customize the mask of the {@link SuperBlock}.
 */
@SuppressWarnings(value = { "serial" })
public final class SuperblockMaskCustomizeAction extends DefaultAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.CUSTOMIZE;
    /** Icon name of the action */
    public static final String SMALL_ICON = "";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = 0;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = 0;

    private static final String TERMINATE_EDIT_ON_FOCUS_LOST = "terminateEditOnFocusLost";

    /**
     * Private constructor
     *
     * @param scilabGraph
     *            the associated graph
     */
    public SuperblockMaskCustomizeAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * Create the menu associated with this action
     *
     * @param scilabGraph
     *            the associated graph
     * @return the newly created menu
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, SuperblockMaskCustomizeAction.class);
    }

    /**
     * Function to be performed by this action.
     *
     * @param e
     *            The associated event
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        final XcosDiagram graph = (XcosDiagram) getGraph(e);

        // action disabled when the cell is edited
        final ScilabComponent comp = ((ScilabComponent) graph.getAsComponent());
        if (comp.isEditing()) {
            return;
        }

        SuperBlock block = (SuperBlock) graph.getSelectionCell();

        CustomizeFrame frame = new CustomizeFrame(graph);
        CustomizeFrame.CustomizeFrameModel model = frame.getController().getModel();
        model.setBlock(block);
        model.importFromBlock();
        frame.setVisible(true);
    }

    /**
     * Frame used to customize fields and variables default values. DAC: this
     * class is tightly coupled to Swing
     */
    // CSOFF: ClassDataAbstractionCoupling
    @SuppressWarnings(value = { "serial" })
    private class CustomizeFrame extends JFrame {

        private final CustomizeFrameControler controler;

        private javax.swing.JPanel buttonBlob;
        private javax.swing.JButton cancelButton;
        private javax.swing.JPanel customizeMainPanel;
        private javax.swing.JScrollPane customizeScrollPane;
        private javax.swing.JTabbedPane tabbedPane;
        private javax.swing.JTable defaultValueTable;
        private javax.swing.JPanel defaultValues;
        private javax.swing.JScrollPane defaultValuesScrollPane;
        private javax.swing.JButton delete;
        private javax.swing.JButton insert;
        private javax.swing.JPanel mainPanel;
        private javax.swing.JButton moveDown;
        private javax.swing.JButton moveUp;
        private javax.swing.JButton okButton;
        private javax.swing.JLabel rowLabel;
        private javax.swing.JPanel rowManagement;
        private javax.swing.JSpinner rowSpinner;
        private javax.swing.JPanel tableManagement;
        private javax.swing.JPanel validationPanel;
        private javax.swing.JTable varCustomizeTable;
        private javax.swing.JPanel varSettings;

        /**
         * Constructor
         *
         * @param diagram
         *            the container diagram
         */
        public CustomizeFrame(XcosDiagram diagram) {
            setTitle(XcosMessages.MASK_TITLE);
            setDefaultCloseOperation(WindowConstants.HIDE_ON_CLOSE);
            controler = new CustomizeFrameControler(diagram);
            initComponents();
        }

        /**
         * @return The associated controller
         */
        public CustomizeFrameControler getController() {
            return controler;
        }

        /**
         * Construct the UI and install the listeners.
         */
        // CSOFF: JavaNCSS
        // CSOFF: MagicNumber
        private void initComponents() {

            /* Construct the components */
            mainPanel = new javax.swing.JPanel();
            tabbedPane = new javax.swing.JTabbedPane();
            varSettings = new javax.swing.JPanel();
            customizeMainPanel = new javax.swing.JPanel();
            customizeScrollPane = new javax.swing.JScrollPane();
            varCustomizeTable = new javax.swing.JTable();
            tableManagement = new javax.swing.JPanel();
            insert = new javax.swing.JButton();
            delete = new javax.swing.JButton();
            buttonBlob = new javax.swing.JPanel();
            moveUp = new javax.swing.JButton();
            moveDown = new javax.swing.JButton();
            rowManagement = new javax.swing.JPanel();
            rowLabel = new javax.swing.JLabel();
            rowSpinner = new javax.swing.JSpinner();
            defaultValues = new javax.swing.JPanel();
            defaultValuesScrollPane = new javax.swing.JScrollPane();
            defaultValueTable = new javax.swing.JTable();
            validationPanel = new javax.swing.JPanel();
            okButton = new javax.swing.JButton();
            cancelButton = new javax.swing.JButton();

            setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);

            /* Initialize the components */
            mainPanel.setLayout(new java.awt.BorderLayout());
            mainPanel.setBorder(BorderFactory.createEmptyBorder(2, 2, 2, 2));

            varSettings.setLayout(new javax.swing.BoxLayout(varSettings, javax.swing.BoxLayout.PAGE_AXIS));

            varCustomizeTable.setModel(controler.getModel().customizeTableModel);
            varCustomizeTable.putClientProperty(TERMINATE_EDIT_ON_FOCUS_LOST, Boolean.TRUE);
            customizeScrollPane.setViewportView(varCustomizeTable);
            // setAutoCreateRowSorter is java 1.6
            // varCustomizeTable.setAutoCreateRowSorter(true);

            /* Update the default value table */
            varCustomizeTable.getModel().addTableModelListener(controler.updateValuesTable);

            /*
             * Activate and deactivate insertion/deletion sensible
             * buttons/spinner
             */
            varCustomizeTable.getModel().addTableModelListener(controler.updateButtonsSensibleForModifications);

            /* Activate and deactivate selection sensible buttons */
            varCustomizeTable.getSelectionModel().addListSelectionListener(controler.updateButtonsSensibleForSelectionChange);

            customizeMainPanel.add(customizeScrollPane);

            tableManagement.setBorder(javax.swing.BorderFactory.createEmptyBorder(2, 2, 2, 2));
            tableManagement.setLayout(new java.awt.GridLayout(5, 1));

            /* Install the insert action */
            insert.setMnemonic('n');
            insert.setText(XcosMessages.MASK_INSERT);
            tableManagement.add(insert);
            insert.addActionListener(controler.insertActionListener);

            /* Install the delete action */
            delete.setMnemonic('l');
            delete.setText(XcosMessages.MASK_DELETE);
            tableManagement.add(delete);
            delete.addActionListener(controler.deleteActionListener);

            /* Install empty area */
            tableManagement.add(buttonBlob);

            /* Install the move-up action */
            moveUp.setMnemonic('u');
            moveUp.setText(XcosMessages.MASK_MOVEUP);
            tableManagement.add(moveUp);
            moveUp.addActionListener(controler.moveUpActionListener);

            /* Install the move-down action */
            moveDown.setMnemonic('w');
            moveDown.setText(XcosMessages.MASK_MOVEDOWN);
            tableManagement.add(moveDown);
            moveDown.addActionListener(controler.moveDownActionListener);

            /* add the table management */
            customizeMainPanel.add(tableManagement);

            varSettings.add(customizeMainPanel);

            rowLabel.setText(XcosMessages.MASK_ROWS + " :");
            rowManagement.add(rowLabel);

            rowSpinner.setModel(new javax.swing.SpinnerNumberModel(1, 1, Integer.MAX_VALUE, 1));
            rowSpinner.setEditor(new javax.swing.JSpinner.NumberEditor(rowSpinner, "######0"));
            rowSpinner.setValue(varCustomizeTable.getModel().getRowCount());
            rowManagement.add(rowSpinner);
            rowSpinner.addChangeListener(controler.rowSpinnerChangeListener);

            varSettings.add(rowManagement);

            tabbedPane.addTab(XcosMessages.MASK_VARSETTINGS, varSettings);

            defaultValueTable.setModel(controler.getModel().valuesTableModel);
            defaultValueTable.putClientProperty(TERMINATE_EDIT_ON_FOCUS_LOST, Boolean.TRUE);
            defaultValuesScrollPane.setViewportView(defaultValueTable);

            // setAutoCreateRowSorter is java 1.6
            // defaultValueTable.setAutoCreateRowSorter(false);

            defaultValues.add(defaultValuesScrollPane);

            tabbedPane.addTab(XcosMessages.MASK_DEFAULTVALUES, defaultValues);

            mainPanel.add(tabbedPane, java.awt.BorderLayout.CENTER);

            okButton.setText(XcosMessages.OK);
            validationPanel.add(okButton);
            okButton.addActionListener(controler.okActionListener);

            cancelButton.setText(XcosMessages.CANCEL);
            validationPanel.add(cancelButton);
            cancelButton.addActionListener(controler.cancelActionListener);

            mainPanel.add(validationPanel, java.awt.BorderLayout.PAGE_END);

            add(mainPanel);

            pack();
            cancelButton.requestFocusInWindow();

            /* Evaluate the context and set up the variable name selection */
            TableColumn vars = varCustomizeTable.getColumnModel().getColumn(0);
            JComboBox validVars = new JComboBox();
            final Set<String> keys = controler.context.keySet();
            for (String key : keys) {
                validVars.addItem(key);
            }
            vars.setCellEditor(new DefaultCellEditor(validVars));

            insert.setEnabled(validVars.getModel().getSize() != 0);
        }

        // CSON: JavaNCSS
        // CSON: MagicNumber

        /**
         * Implements the models used on the frame.
         */
        private class CustomizeFrameModel {
            private SuperBlock block;

            /**
             * Model used on the customize table.
             */
            private final DefaultTableModel customizeTableModel = new DefaultTableModel(new Object[][] { new Object[] { XcosMessages.MASK_WINTITLEVAR,
                        XcosMessages.MASK_WINTITLE, false
                                                                                                                          }
            }, new String[] { XcosMessages.MASK_VARNAME, XcosMessages.MASK_VARDESC, XcosMessages.MASK_EDITABLE }) {
                private final Class<?>[] types = new Class[] { java.lang.String.class, java.lang.String.class, java.lang.Boolean.class };
                private final boolean[] canEdit = new boolean[] { true, true, true };

                @Override
                public Class<?> getColumnClass(int columnIndex) {
                    return types[columnIndex];
                }

                @Override
                public boolean isCellEditable(int rowIndex, int columnIndex) {
                    if (rowIndex != 0) {
                        return canEdit[columnIndex];
                    }
                    return false;
                }
            };

            /**
             * Model used for the values table
             */
            private final DefaultTableModel valuesTableModel = new javax.swing.table.DefaultTableModel(new Object[][] { new Object[] {
                    XcosMessages.MASK_WINTITLE, ""
                }
            }, new String[] { XcosMessages.MASK_VARDESC, XcosMessages.MASK_VARVALUES }) {
                private final Class<?>[] types = new Class[] { java.lang.String.class, java.lang.String.class };
                private final boolean[] canEdit = new boolean[] { false, true };

                @Override
                public Class<?> getColumnClass(int columnIndex) {
                    return types[columnIndex];
                }

                @Override
                public boolean isCellEditable(int rowIndex, int columnIndex) {
                    return canEdit[columnIndex];
                }
            };

            /**
             * Default constructor
             */
            protected CustomizeFrameModel() {
                // Does nothing as the fields are final.
            }

            /**
             * @param block
             *            This model associated block.
             */
            public void setBlock(SuperBlock block) {
                this.block = block;
            }

            /**
             * @return This model associated block.
             */
            public SuperBlock getBlock() {
                return block;
            }

            /**
             * Export the table models to the block exprs.
             */
            public void exportToBlock() {
                /** Data vectors are typed when parsing */
                final List<?> customModel = customizeTableModel.getDataVector();
                final List<?> valuesModel = valuesTableModel.getDataVector();

                /* We have one content that is not a variable : Window Title */
                final int nbOfVar = valuesModel.size() - 1;

                final String[][] values = new String[nbOfVar][1];
                final String[][] varNames = new String[nbOfVar][1];
                final String[][] varDesc = new String[nbOfVar + 1][1];
                final ScilabList polFields = new ScilabList();

                /* Title */
                varDesc[0][0] = (String) ((List<?>) valuesModel.get(0)).get(1);

                /* Other fields */
                for (int i = 0; i < nbOfVar; i++) {
                    values[i][0] = (String) ((List<?>) valuesModel.get(i + 1)).get(1);
                    varNames[i][0] = (String) ((List<?>) customModel.get(i + 1)).get(0);
                    varDesc[i + 1][0] = (String) ((List<?>) customModel.get(i + 1)).get(1);

                    /*
                     * reconstruct pol fields. The default types of the values.
                     *
                     * This field indicate the dimension of each entry (-1.0 is
                     * automatic). TODO: type the data there instead of using
                     * the generic "pol".
                     */
                    polFields.add(new ScilabString("pol"));
                    polFields.add(new ScilabDouble(-1.0));
                }

                /* Construct fields from data */
                final ScilabList exprs;
                if (nbOfVar == 0) {
                    /* Set default values */
                    exprs = new ScilabList(Arrays.asList(
                                               new ScilabDouble(),
                                               new ScilabList(Arrays.asList(new ScilabDouble(), new ScilabString(XcosMessages.MASK_DEFAULTWINDOWNAME),
                                                       new ScilabList(Arrays.asList(new ScilabDouble()))))));
                } else {
                    /* set the values */
                    exprs = new ScilabList(Arrays.asList(new ScilabString(values),
                                                         new ScilabList(Arrays.asList(new ScilabString(varNames), new ScilabString(varDesc), polFields))));
                }

                JavaController controller = new JavaController();
                VectorOfDouble vec = new ScilabTypeCoder().var2vec(exprs);
                controller.setObjectProperty(block.getUID(), block.getKind(), ObjectProperties.EXPRS, vec);

                /*
                 * Trace the exprs update.
                 */
                if (Logger.getLogger(SuperblockMaskCustomizeAction.class.getName()).isLoggable(Level.FINEST)) {
                    Logger.getLogger(SuperblockMaskCustomizeAction.class.getName()).finest("exprs=" + exprs);
                }
            }

            /**
             * Import the model exprs to the table models.
             */
            public void importFromBlock() {
                ScilabString values;
                ScilabString varNames;
                ScilabString varDesc;

                JavaController controller = new JavaController();
                VectorOfDouble vec = new VectorOfDouble();
                controller.getObjectProperty(block.getUID(), block.getKind(), ObjectProperties.EXPRS, vec);
                ScilabType rawExprs = new ScilabTypeCoder().vec2var(vec);

                // Xcos from Scilab 5.2.0 version
                // so set default values
                if (rawExprs instanceof ScilabDouble) {
                    rawExprs = new ScilabList(Arrays.asList(
                                                  new ScilabDouble(),
                                                  new ScilabList(Arrays.asList(new ScilabDouble(), new ScilabString(XcosMessages.MASK_DEFAULTWINDOWNAME),
                                                          new ScilabList(Arrays.asList(new ScilabDouble()))))));
                }
                DefaultTableModel customModel = customizeTableModel;
                DefaultTableModel valuesModel = valuesTableModel;

                /*
                 * rawExprs have to be typed as
                 * list([],list([],"Set block parameters",list())) or as
                 * list([""],list([""],"Set block parameters",list([""])))
                 */
                assert rawExprs instanceof ScilabList;
                ScilabList exprs = (ScilabList) rawExprs;

                assert (exprs.get(0) instanceof ScilabDouble) || (exprs.get(0) instanceof ScilabString);
                if (exprs.get(0) instanceof ScilabDouble) {
                    values = new ScilabString("");
                } else { /* exprs.get(0) instanceof ScilabString) */
                    values = (ScilabString) exprs.get(0);
                }

                assert exprs.get(1) instanceof ScilabList;
                ScilabList lvalues = (ScilabList) exprs.get(1);

                assert (lvalues.get(0) instanceof ScilabDouble) || (lvalues.get(0) instanceof ScilabString);
                if (lvalues.get(0) instanceof ScilabDouble) {
                    varNames = new ScilabString();
                } else { /* exprs.get(0) instanceof ScilabString) */
                    varNames = (ScilabString) lvalues.get(0);
                }

                assert lvalues.get(1) instanceof ScilabString;
                varDesc = (ScilabString) lvalues.get(1);

                /*
                 * Check if the data are stored as columns or as row.
                 */
                if (varDesc.getHeight() >= varDesc.getWidth()) {

                    /* Title */
                    valuesModel.setValueAt(varDesc.getData()[0][0], 0, 1);

                    /* Loop all over the data */
                    for (int i = 1; i < varDesc.getHeight(); i++) {
                        customModel.addRow(new Object[] { varNames.getData()[i - 1][0], varDesc.getData()[i][0], true });
                        valuesModel.setValueAt(values.getData()[i - 1][0], i, 1);
                    }
                } else {
                    /* Title */
                    valuesModel.setValueAt(varDesc.getData()[0][0], 0, 1);

                    /* Loop all over the data */
                    for (int i = 1; i < varDesc.getHeight(); i++) {
                        customModel.addRow(new Object[] { varNames.getData()[0][i - 1], varDesc.getData()[0][i], true });
                        valuesModel.setValueAt(values.getData()[0][i - 1], i, 1);
                    }
                }
            }
        }

        /**
         * Implement the action listeners for the frame
         */
        private class CustomizeFrameControler {
            private final CustomizeFrameModel model;

            private final ActionListener cancelActionListener = new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    dispose();
                }
            };

            private final ActionListener okActionListener = new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent arg0) {
                    model.exportToBlock();
                    dispose();
                }
            };

            private final ChangeListener rowSpinnerChangeListener = new ChangeListener() {
                @Override
                public void stateChanged(ChangeEvent e) {
                    int rowCount = varCustomizeTable.getRowCount();
                    int value = (Integer) rowSpinner.getModel().getValue();

                    for (; rowCount < value; rowCount++) {
                        insertActionListener.actionPerformed(new ActionEvent(this, -1, ACTION_COMMAND_KEY));
                    }

                    for (; rowCount > value; rowCount--) {
                        deleteActionListener.actionPerformed(new ActionEvent(this, -1, ACTION_COMMAND_KEY));
                    }
                }
            };

            private final ActionListener moveDownActionListener = new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent arg0) {
                    int selectedRow = varCustomizeTable.getSelectedRow();

                    swapTableRow(model.customizeTableModel, selectedRow, selectedRow + 1);
                    swapTableRow(model.valuesTableModel, selectedRow, selectedRow + 1);

                    /* Keep the same row selected */
                    varCustomizeTable.changeSelection(selectedRow + 1, varCustomizeTable.getSelectedColumn(), false, false);
                }
            };

            private final ActionListener moveUpActionListener = new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent arg0) {
                    int selectedRow = varCustomizeTable.getSelectedRow();

                    swapTableRow(model.customizeTableModel, selectedRow, selectedRow - 1);
                    swapTableRow(model.valuesTableModel, selectedRow, selectedRow - 1);

                    /* Keep the same row selected */
                    varCustomizeTable.changeSelection(selectedRow - 1, varCustomizeTable.getSelectedColumn(), false, false);
                }
            };

            private final ActionListener deleteActionListener = new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    int selected = varCustomizeTable.getSelectedRow();
                    int nbOfRows = varCustomizeTable.getRowCount();

                    if (selected > 1) {
                        model.customizeTableModel.removeRow(selected);
                        varCustomizeTable.changeSelection(selected - 1, 1, false, false);
                    } else if (nbOfRows > 1) {
                        model.customizeTableModel.removeRow(nbOfRows - 1);
                        varCustomizeTable.changeSelection(nbOfRows - 2, 1, false, false);
                    }
                }
            };

            private final ActionListener insertActionListener = new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    final String nextKey;
                    if (keyIterator.hasNext()) {
                        nextKey = keyIterator.next();
                    } else {
                        // at the end of the iteration loop again
                        keyIterator = context.keySet().iterator();

                        if (keyIterator.hasNext()) {
                            nextKey = keyIterator.next();
                        } else {
                            nextKey = ""; // no defined vars.
                        }
                    }

                    model.customizeTableModel.addRow(new Object[] { nextKey, nextKey, true });
                    varCustomizeTable.changeSelection(model.customizeTableModel.getRowCount() - 1, 1, false, false);
                }
            };

            /**
             * Update the buttons on selection change.
             */
            private final ListSelectionListener updateButtonsSensibleForSelectionChange = new ListSelectionListener() {
                @Override
                public void valueChanged(ListSelectionEvent e) {
                    /* We cannot move up anymore */
                    final boolean isFirst;
                    /* We cannot move down anymore */
                    final boolean isLast;
                    /* The second row is selected */
                    final boolean isSecond;

                    int selectedRow = varCustomizeTable.getSelectedRow();
                    int rowCount = varCustomizeTable.getRowCount();

                    isFirst = selectedRow < 1;
                    isSecond = selectedRow == 1;
                    isLast = selectedRow == rowCount - 1;

                    moveUp.setEnabled(!isFirst && !isSecond);
                    moveDown.setEnabled(!isFirst && !isLast);

                    delete.setEnabled(!isFirst);
                }
            };

            /**
             * Update the buttons/spinner on modifications
             */
            private final TableModelListener updateButtonsSensibleForModifications = new TableModelListener() {
                @Override
                public void tableChanged(TableModelEvent e) {
                    int rowCount = model.customizeTableModel.getRowCount();
                    rowSpinner.setValue(rowCount);
                }
            };

            /**
             * Update the values table on change on the customize table.
             */
            private final TableModelListener updateValuesTable = new TableModelListener() {
                @Override
                public void tableChanged(TableModelEvent e) {
                    final DefaultTableModel valuesModel = model.valuesTableModel;
                    final DefaultTableModel customModel = model.customizeTableModel;
                    int row = e.getFirstRow();
                    int column = e.getColumn();

                    switch (e.getType()) {
                        case TableModelEvent.INSERT:
                            String key = (String) customModel.getValueAt(row, 0);
                            String value = context.get(key);
                            valuesModel.addRow(new Object[] { key, value });
                            break;

                        case TableModelEvent.DELETE:
                            valuesModel.removeRow(row);
                            break;

                        case TableModelEvent.UPDATE:
                            if (column == 0) {
                                // update the description when it was set to a var
                                // name and the variable change
                                // the description should not change when it is not
                                // the default.
                                String varName = (String) customModel.getValueAt(row, 0);
                                String varDesc = (String) customModel.getValueAt(row, 1);
                                if (context.containsKey(varDesc)) {
                                    customModel.setValueAt(varName, row, 1);
                                }
                            } else if (column == 1) {
                                // update the description on the default value table
                                valuesModel.setValueAt(customModel.getValueAt(row, 1), row, 0);
                            }
                            break;

                        default:
                            break;
                    }
                }
            };

            private final Map<String, String> context;
            private Iterator<String> keyIterator;

            /**
             * Cstr
             *
             * @param diagram
             *            the calling diagram
             */
            public CustomizeFrameControler(XcosDiagram diagram) {
                model = new CustomizeFrameModel();
                context = diagram.evaluateContext();
                keyIterator = context.keySet().iterator();
            }

            /**
             * @return the model
             */
            public CustomizeFrameModel getModel() {
                return model;
            }

            /**
             * Swap two table rows
             *
             * @param model
             *            the model
             * @param row1
             *            the first row
             * @param row2
             *            th second row
             */
            private void swapTableRow(DefaultTableModel model, int row1, int row2) {
                /*
                 * doesn't need to be checked as the operation doesn't depend on
                 * it
                 */
                @SuppressWarnings("unchecked")
                final List < List<? >> data = model.getDataVector();

                /* Inverting data */
                List<?> tmp = data.get(row2);
                data.set(row2, data.get(row1));
                data.set(row1, tmp);
            }
        }
    }
    // CSON: ClassDataAbstractionCoupling
}
