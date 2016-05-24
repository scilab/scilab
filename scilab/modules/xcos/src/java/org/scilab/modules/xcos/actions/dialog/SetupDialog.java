/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 * Copyright (C) 2013 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.actions.dialog;

import java.awt.Component;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Image;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.beans.PropertyVetoException;
import java.math.BigDecimal;
import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;
import java.util.Arrays;
import java.util.EnumSet;
import java.util.logging.Logger;

import javax.swing.DefaultListCellRenderer;
import javax.swing.ImageIcon;
import javax.swing.InputVerifier;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.JFormattedTextField;
import javax.swing.JLabel;
import javax.swing.JList;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.VectorOfDouble;
import org.scilab.modules.xcos.actions.SetupAction;
import org.scilab.modules.xcos.graph.ScicosParameters;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Dialog associated with the {@link SetupAction}.
 *
 * Note that this dialog break the Data Abstraction Coupling metric because of
 * the numbers of graphical components involved in the GUI creation. For the
 * same reason (GUI class), constants are not used on this code.
 */
// CSOFF: ClassDataAbstractionCoupling
// CSOFF: ClassFanOutComplexity
// CSOFF: MagicNumber
@SuppressWarnings(value = { "serial" })
public class SetupDialog extends JDialog {
    private static final DecimalFormatSymbols FORMAT_SYMBOL = DecimalFormatSymbols.getInstance();
    private static final DecimalFormat CURRENT_FORMAT = new DecimalFormat("0.0####E00", FORMAT_SYMBOL);
    private static final BigDecimal MAX_DOUBLE = BigDecimal.valueOf(Double.MAX_VALUE);

    /**
     * Modifiers used to disable some parameters for a specific solver
     */
    private static enum SolverModifiers {
        ABSOLUTE_TOLERANCE,
        RELATIVE_TOLERANCE,
        TOLERANCE_ON_TIME,
        MAX_INTEGRATION_TIME,
        MAX_STEP_SIZE
    }

    /**
     * Describe a solver, its compilation value, name and tooltip.
     *
     * Also contains a field with enable modifier set.
     */
    protected static class SolverDescriptor implements Comparable<SolverDescriptor> {
        private final int number;
        private final String name;
        private final String tooltip;
        private final EnumSet<SolverModifiers> modifiers;

        public SolverDescriptor(int number, String name, String tooltip, EnumSet<SolverModifiers> modifiers) {
            super();
            this.number = number;
            this.name = name;
            this.tooltip = tooltip;
            this.modifiers = modifiers;
        }

        public SolverDescriptor(double number) {
            super();
            this.number = (int) number;
            this.name = null;
            this.tooltip = null;
            this.modifiers = null;
        }

        public int getNumber() {
            return number;
        }

        public String getToolTip() {
            return tooltip;
        }

        @Override
        public String toString() {
            return name;
        }

        public void applyModifiers(
            JFormattedTextField integrator,
            JFormattedTextField integratorRel,
            JFormattedTextField toleranceOnTime,
            JFormattedTextField maxIntegrationTime,
            JFormattedTextField maxStepSize) {
            integrator.setEnabled(modifiers.contains(SolverModifiers.ABSOLUTE_TOLERANCE));
            integratorRel.setEnabled(modifiers.contains(SolverModifiers.RELATIVE_TOLERANCE));
            toleranceOnTime.setEnabled(modifiers.contains(SolverModifiers.TOLERANCE_ON_TIME));
            maxIntegrationTime.setEnabled(modifiers.contains(SolverModifiers.MAX_INTEGRATION_TIME));
            maxStepSize.setEnabled(modifiers.contains(SolverModifiers.MAX_STEP_SIZE));
        }

        @Override
        public int compareTo(SolverDescriptor o) {
            // inline Integer.compare(number, o.number) to be java 1.6 compatible
            return (number < o.number) ? -1 : ((number == o.number) ? 0 : 1);
        }
    }

    private static final SolverDescriptor[] AVAILABLE_SOLVERS = new SolverDescriptor[] {
        new SolverDescriptor(0, "LSodar",
                             "Method: dynamic, Nonlinear solver= dynamic",
                             EnumSet.allOf(SetupDialog.SolverModifiers.class)),

        new SolverDescriptor(1, "Sundials/CVODE - BDF - NEWTON",
                             "Method: BDF, Nonlinear solver= NEWTON",
                             EnumSet.allOf(SetupDialog.SolverModifiers.class)),

        new SolverDescriptor(2, "Sundials/CVODE - BDF - FUNCTIONAL",
                             "Method: BDF, Nonlinear solver= FUNCTIONAL",
                             EnumSet.allOf(SetupDialog.SolverModifiers.class)),

        new SolverDescriptor(3, "Sundials/CVODE - ADAMS - NEWTON",
                             "Method: ADAMS, Nonlinear solver= NEWTON",
                             EnumSet.allOf(SetupDialog.SolverModifiers.class)),

        new SolverDescriptor(4, "Sundials/CVODE - ADAMS - FUNCTIONAL",
                             "Method: ADAMS, Nonlinear solver= FUNCTIONAL",
                             EnumSet.allOf(SetupDialog.SolverModifiers.class)),

        new SolverDescriptor(5, "DOPRI5 - Dormand-Prince 4(5)",
                             "Method: Fixed step",
                             EnumSet.of(SetupDialog.SolverModifiers.MAX_STEP_SIZE)),

        new SolverDescriptor(6, "RK45 - Runge-Kutta 4(5)",
                             "Method: Fixed step",
                             EnumSet.of(SetupDialog.SolverModifiers.MAX_STEP_SIZE)),

        new SolverDescriptor(7, "Implicit RK45 - Implicit Runge-Kutta 4(5)",
                             "Method: Fixed step, Nonlinear solver= FIXED-POINT",
                             EnumSet.of(SetupDialog.SolverModifiers.MAX_STEP_SIZE,
                                        SetupDialog.SolverModifiers.RELATIVE_TOLERANCE)),

        new SolverDescriptor(8, "CRANI - Crank-Nicolson 2(3)",
                             "Method: Fixed step, Nonlinear solver= FIXED-POINT",
                             EnumSet.of(SetupDialog.SolverModifiers.MAX_STEP_SIZE,
                                        SetupDialog.SolverModifiers.RELATIVE_TOLERANCE)),

        new SolverDescriptor(100, "Sundials/IDA",
                             "Method: BDF, Nonlinear solver= NEWTON",
                             EnumSet.allOf(SetupDialog.SolverModifiers.class)),

        new SolverDescriptor(101, "DDaskr - Newton",
                             "Method: BDF, Nonlinear solver= NEWTON",
                             EnumSet.allOf(SetupDialog.SolverModifiers.class)),

        new SolverDescriptor(102, "DDaskr - GMRes",
                             "Method: BDF, Nonlinear solver= GMRES",
                             EnumSet.allOf(SetupDialog.SolverModifiers.class))
    };

    /**
     * Validate the user entry and format it.
     *
     * Without formatting the entry, bug #7143 appears on jdk6.
     */
    private static final InputVerifier VALIDATE_POSITIVE_DOUBLE = new InputVerifier() {

        @Override
        public boolean verify(javax.swing.JComponent arg0) {
            boolean ret = false;
            JFormattedTextField textField = (JFormattedTextField) arg0;
            try {
                BigDecimal value = new BigDecimal(textField.getText());
                if (value.compareTo(BigDecimal.ZERO) >= 0 && value.compareTo(MAX_DOUBLE) <= 0) {
                    ret = true;
                }

                // bug #7143 workaround
                textField.setValue(value);
            } catch (NumberFormatException e) {
                return ret;
            }
            return ret;

        };
    };

    /**
     * Initialize static final fields
     */
    static {
        FORMAT_SYMBOL.setDecimalSeparator('.');
        CURRENT_FORMAT.setDecimalFormatSymbols(FORMAT_SYMBOL);
        CURRENT_FORMAT.setParseIntegerOnly(false);
        CURRENT_FORMAT.setParseBigDecimal(true);

        // assert that the array is sorted
        Arrays.sort(AVAILABLE_SOLVERS);
    }

    private final ScicosParameters parameters;
    private final XcosDiagram rootGraph;

    private JFormattedTextField integration;
    private JFormattedTextField rts;
    private JFormattedTextField integrator;
    private JFormattedTextField integratorRel;
    private JFormattedTextField toleranceOnTime;
    private JFormattedTextField maxIntegrationTime;
    private JComboBox<SolverDescriptor> solver;
    private JFormattedTextField maxStepSize;

    /**
     * Instantiate a new dialog.
     *
     * @param parent
     *            the current selected graph component
     * @param parameters
     *            the current parameters
     */
    public SetupDialog(Component parent, XcosDiagram graph, ScicosParameters parameters) {
        super();

        this.parameters = parameters;

        ImageIcon scilabIcon = new ImageIcon(FindIconHelper.findIcon("scilab"));
        Image imageForIcon = scilabIcon.getImage();
        setLayout(new GridBagLayout());
        setIconImage(imageForIcon);
        setTitle(XcosMessages.SETUP_TITLE);
        setModal(false);
        setLocationRelativeTo(parent);
        rootGraph = graph;
        setDefaultCloseOperation(DISPOSE_ON_CLOSE);
        ScilabSwingUtilities.closeOnEscape(this);

        initComponents();
    }

    /**
     * Initialize the dialog components.
     *
     * As this method perform a complex initialization, It doesn't pass NCSS.
     */
    // CSOFF: JavaNCSS
    private void initComponents() {
        JavaController controller = new JavaController();

        JLabel integrationLabel = new JLabel(XcosMessages.FINAL_INTEGRATION_TIME);
        integration = new JFormattedTextField(CURRENT_FORMAT);
        integration.setInputVerifier(VALIDATE_POSITIVE_DOUBLE);
        integration.setValue(new BigDecimal(parameters.getProperties(controller).get(ScicosParameters.FINAL_INTEGRATION_TIME)));

        JLabel rtsLabel = new JLabel(XcosMessages.REAL_TIME_SCALING);
        rts = new JFormattedTextField(CURRENT_FORMAT);
        rts.setInputVerifier(VALIDATE_POSITIVE_DOUBLE);
        rts.setValue(new BigDecimal(parameters.getProperties(controller).get(ScicosParameters.REAL_TIME_SCALING)));

        JLabel integratorAbsLabel = new JLabel(XcosMessages.INTEGRATOR_ABSOLUTE_TOLERANCE);
        integrator = new JFormattedTextField(CURRENT_FORMAT);
        integrator.setInputVerifier(VALIDATE_POSITIVE_DOUBLE);
        integrator.setValue(new BigDecimal(parameters.getProperties(controller).get(ScicosParameters.INTEGRATOR_ABSOLUTE_TOLERANCE)));

        JLabel integratorRelLabel = new JLabel(XcosMessages.INTEGRATOR_RELATIVE_TOLERANCE);
        integratorRel = new JFormattedTextField(CURRENT_FORMAT);
        integratorRel.setInputVerifier(VALIDATE_POSITIVE_DOUBLE);
        integratorRel.setValue(new BigDecimal(parameters.getProperties(controller).get(ScicosParameters.INTEGRATOR_RELATIVE_TOLERANCE)));

        JLabel toleranceOnTimeLabel = new JLabel(XcosMessages.TOLERANCE_ON_TIME);
        toleranceOnTime = new JFormattedTextField(CURRENT_FORMAT);
        toleranceOnTime.setInputVerifier(VALIDATE_POSITIVE_DOUBLE);
        toleranceOnTime.setValue(new BigDecimal(parameters.getProperties(controller).get(ScicosParameters.TOLERANCE_ON_TIME)));

        JLabel maxIntegrationTimeLabel = new JLabel(XcosMessages.MAX_INTEGRATION_TIME_INTERVAL);
        maxIntegrationTime = new JFormattedTextField(CURRENT_FORMAT);
        maxIntegrationTime.setInputVerifier(VALIDATE_POSITIVE_DOUBLE);
        maxIntegrationTime.setValue(new BigDecimal(parameters.getProperties(controller).get(ScicosParameters.MAX_INTEGRATION_TIME_INTERVAL)));

        JLabel solverLabel = new JLabel(XcosMessages.SOLVER_CHOICE);


        solver = new JComboBox<SolverDescriptor>(AVAILABLE_SOLVERS);
        double solverValue = parameters.getProperties(controller).get(ScicosParameters.SOLVER);
        final int currentIndex = Arrays.binarySearch(AVAILABLE_SOLVERS, new SolverDescriptor(solverValue));
        final SolverDescriptor current = AVAILABLE_SOLVERS[currentIndex];
        solver.setSelectedIndex(currentIndex);

        final class ComboboxToolTipRenderer extends DefaultListCellRenderer {
            @Override
            public Component getListCellRendererComponent(JList<?> list, Object value, int index, boolean isSelected, boolean cellHasFocus) {
                JComponent comp = (JComponent) super.getListCellRendererComponent(list, value, index, isSelected, cellHasFocus);

                if (-1 < index && null != value) {
                    list.setToolTipText(AVAILABLE_SOLVERS[index].getToolTip());
                }
                return comp;
            }
        }
        solver.setRenderer(new ComboboxToolTipRenderer());

        solver.addItemListener(new ItemListener() {
            @Override
            public void itemStateChanged(ItemEvent e) {
                SolverDescriptor current = (SolverDescriptor) e.getItem();
                current.applyModifiers(integrator, integratorRel, toleranceOnTime, maxIntegrationTime, maxStepSize);
            }
        });

        JLabel maxStepSizeLabel = new JLabel(XcosMessages.MAXIMUN_STEP_SIZE);
        maxStepSize = new JFormattedTextField(CURRENT_FORMAT);
        maxStepSize.setInputVerifier(VALIDATE_POSITIVE_DOUBLE);
        maxStepSize.setValue(new BigDecimal(parameters.getProperties(controller).get(ScicosParameters.MAXIMUM_STEP_SIZE)));

        JButton cancelButton = new JButton(XcosMessages.CANCEL);
        JButton okButton = new JButton(XcosMessages.OK);
        JButton defaultButton = new JButton(XcosMessages.DEFAULT);
        JButton setContextButton = new JButton(XcosMessages.SET_CONTEXT);
        okButton.setPreferredSize(cancelButton.getPreferredSize());

        GridBagConstraints gbc = new GridBagConstraints();

        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.gridheight = 1;
        gbc.gridwidth = 1;
        gbc.insets = new Insets(0, 10, 0, 0);

        gbc.gridx = 0;
        gbc.gridy = 4;
        gbc.gridheight = 1;
        gbc.gridwidth = 1;
        gbc.fill = GridBagConstraints.NONE;
        gbc.insets = new Insets(0, 10, 0, 0);

        add(integrationLabel, gbc);

        gbc.gridy = 5;
        add(rtsLabel, gbc);

        gbc.gridy = 6;
        add(integratorAbsLabel, gbc);

        gbc.gridy = 7;
        add(integratorRelLabel, gbc);

        gbc.gridy = 8;
        add(toleranceOnTimeLabel, gbc);

        gbc.gridy = 9;
        add(maxIntegrationTimeLabel, gbc);

        gbc.gridy = 10;
        add(solverLabel, gbc);

        gbc.gridy = 11;
        add(maxStepSizeLabel, gbc);

        gbc.gridy = 12;
        add(setContextButton, gbc);

        gbc.gridx = 1;
        gbc.gridy = 4;
        gbc.gridwidth = GridBagConstraints.REMAINDER;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.insets = new Insets(5, 10, 0, 10);
        add(integration, gbc);

        gbc.gridy = 5;
        add(rts, gbc);

        gbc.gridy = 6;
        add(integrator, gbc);

        gbc.gridy = 7;
        add(integratorRel, gbc);

        gbc.gridy = 8;
        add(toleranceOnTime, gbc);

        gbc.gridy = 9;
        add(maxIntegrationTime, gbc);

        gbc.gridy = 10;
        add(solver, gbc);

        gbc.gridy = 11;
        add(maxStepSize, gbc);

        gbc.gridx = 1;
        gbc.gridy = 14;
        gbc.gridheight = 1;
        gbc.gridwidth = 1;
        gbc.weightx = 1.;
        gbc.fill = GridBagConstraints.NONE;
        gbc.insets = new Insets(5, 0, 10, 5);
        add(okButton, gbc);

        gbc.gridx = 2;
        gbc.weightx = 0.;
        gbc.insets = new Insets(5, 0, 10, 10);
        add(cancelButton, gbc);

        gbc.gridx = 3;
        gbc.weightx = 0.;
        gbc.insets = new Insets(5, 0, 10, 10);
        add(defaultButton, gbc);

        installActionListeners(cancelButton, okButton, defaultButton, setContextButton);

        // at the end, update the enable status of some components
        current.applyModifiers(integrator, integratorRel, toleranceOnTime, maxIntegrationTime, maxStepSize);
    }

    // CSON: JavaNCSS

    /**
     * Install the action listeners on the buttons
     *
     * @param cancelButton
     *            the cancel button (Cancel)
     * @param okButton
     *            the OK button (Validate)
     * @param defaultButton
     *            the default button (Reset)
     * @param setContextButton
     *            the context button (Shortcut to set context)
     */
    private void installActionListeners(JButton cancelButton, JButton okButton, JButton defaultButton, JButton setContextButton) {
        cancelButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                dispose();
            }
        });

        defaultButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                integration.setValue(new BigDecimal(ScicosParameters.DEFAULT_PARAMETERS.get(ScicosParameters.FINAL_INTEGRATION_TIME)));
                integrator.setValue(new BigDecimal(ScicosParameters.DEFAULT_PARAMETERS.get(ScicosParameters.INTEGRATOR_ABSOLUTE_TOLERANCE)));
                integratorRel.setValue(new BigDecimal(ScicosParameters.DEFAULT_PARAMETERS.get(ScicosParameters.INTEGRATOR_RELATIVE_TOLERANCE)));
                toleranceOnTime.setValue(new BigDecimal(ScicosParameters.DEFAULT_PARAMETERS.get(ScicosParameters.TOLERANCE_ON_TIME)));
                maxIntegrationTime.setValue(new BigDecimal(ScicosParameters.DEFAULT_PARAMETERS.get(ScicosParameters.MAX_INTEGRATION_TIME_INTERVAL)));
                maxStepSize.setValue(new BigDecimal(ScicosParameters.DEFAULT_PARAMETERS.get(ScicosParameters.MAXIMUM_STEP_SIZE)));
                rts.setValue(new BigDecimal(ScicosParameters.DEFAULT_PARAMETERS.get(ScicosParameters.REAL_TIME_SCALING)));
                solver.setSelectedIndex((int) ScicosParameters.DEFAULT_PARAMETERS.get(ScicosParameters.SOLVER));
            }
        });

        okButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    /*
                     * TODO This logic must be deported to a vetoable
                     * handler
                     */
                    int solverSelectedIndex = solver.getSelectedIndex();

                    VectorOfDouble v = new VectorOfDouble(8);
                    v.set(ScicosParameters.FINAL_INTEGRATION_TIME, ((BigDecimal) integration.getValue()).doubleValue());
                    v.set(ScicosParameters.INTEGRATOR_ABSOLUTE_TOLERANCE, ((BigDecimal) integrator.getValue()).doubleValue());
                    v.set(ScicosParameters.INTEGRATOR_RELATIVE_TOLERANCE, ((BigDecimal) integratorRel.getValue()).doubleValue());
                    v.set(ScicosParameters.TOLERANCE_ON_TIME, ((BigDecimal) toleranceOnTime.getValue()).doubleValue());
                    v.set(ScicosParameters.MAX_INTEGRATION_TIME_INTERVAL, ((BigDecimal) maxIntegrationTime.getValue()).doubleValue());
                    v.set(ScicosParameters.REAL_TIME_SCALING, ((BigDecimal) rts.getValue()).doubleValue());
                    v.set(ScicosParameters.SOLVER, AVAILABLE_SOLVERS[solverSelectedIndex].getNumber());
                    v.set(ScicosParameters.MAXIMUM_STEP_SIZE, ((BigDecimal) maxStepSize.getValue()).doubleValue());

                    parameters.setProperties(new JavaController(), v);

                    dispose();

                } catch (PropertyVetoException ex) {
                    Logger.getLogger(SetupAction.class.getName()).severe(ex.toString());
                }
            }
        });

        setContextButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                final SetContextDialog dialog = new SetContextDialog(SetupDialog.this, rootGraph, parameters);

                dialog.pack();
                dialog.setVisible(true);
            }
        });
    }
}
// CSON: ClassDataAbstractionCoupling
// CSON: ClassFanOutComplexity
// CSON: MagicNumber
