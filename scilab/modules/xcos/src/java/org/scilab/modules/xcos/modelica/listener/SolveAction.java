/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2010 - DIGITEO - Clement DAVID <clement.david@scilab.org>
 * Copyright (C) 2011-2011 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.modelica.listener;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;
import java.util.logging.Logger;

import javax.swing.AbstractAction;
import javax.xml.bind.JAXBException;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.xcos.modelica.ModelStatistics;
import org.scilab.modules.xcos.modelica.Modelica;
import org.scilab.modules.xcos.modelica.ModelicaController;
import org.scilab.modules.xcos.modelica.ModelicaMessages;
import org.scilab.modules.xcos.modelica.model.Model;

/**
 * Solve the model.
 */
@SuppressWarnings(value = { "serial" })
public final class SolveAction extends AbstractAction {
    private static final String COMPILE_STRING = "fw='%s'; paremb='%s'; jaco='%s'; " + "if(compile_init_modelica(fw, paremb, jaco)) then "
            + "mopen('%s', 'w'); end ";
    private static final String COMPUTE_STRING = "method='%s'; Nunknowns='%s'; " + "if(Compute_cic(method,Nunknowns)) then " + "mopen('%s', 'w'); " + "end ";

    private static final String IMF_INIT = "_init";
    private static final String EXTENSION = ".xml";
    private static final String INCIDENCE = "i_incidence_matrix";

    private final ModelicaController controller;

    /**
     * Default constructor
     *
     * @param controller
     *            the associated controller
     */
    public SolveAction(final ModelicaController controller) {
        super();

        putValue(NAME, ModelicaMessages.SOLVE);
        this.controller = controller;
    }

    /**
     * action !!!
     *
     * @param e
     *            the event
     * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        final String modelName = controller.getRoot().getName();

        // defensive programming
        if (!controller.isSquare()) {
            Logger.getLogger(SolveAction.class.getName()).severe(ModelicaMessages.MODEL_INVALID);
        }

        initStatus();
        try {
            // store the updated values
            final File updatedInitFile = new File(ScilabConstants.TMPDIR, modelName + IMF_INIT + EXTENSION);
            Modelica.getInstance().save(controller.getRoot(), updatedInitFile);

            final int paremb;
            if (controller.isParameterEmbedded()) {
                paremb = 1;
            } else {
                paremb = 0;
            }

            final int jaco;
            if (controller.isJacobianEnable()) {
                jaco = 1;
            } else {
                jaco = 0;
            }

            // creating a temporary status file
            File statusFile = File.createTempFile(modelName, null);
            statusFile.delete();

            String cmd = String.format(COMPILE_STRING, modelName, paremb, jaco, statusFile.getAbsolutePath());

            Logger.getLogger(SolveAction.class.getName()).finest("Compiling");
            ScilabInterpreterManagement.asynchronousScilabExec(new CompileFinished(statusFile), cmd.toString());
        } catch (InterpreterException e1) {
            Logger.getLogger(SolveAction.class.getName()).severe(e1.toString());
        } catch (IOException e1) {
            Logger.getLogger(SolveAction.class.getName()).severe(e1.toString());
        } catch (JAXBException e1) {
            Logger.getLogger(SolveAction.class.getName()).severe(e1.toString());
        }
    }

    /**
     * Initialize the button and error status.
     */
    private void initStatus() {
        putValue(NAME, ModelicaMessages.SOLVE);
        setEnabled(true);
    }

    /**
     * Action done at the end of the compilation
     */
    private final class CompileFinished implements ActionListener {
        private final File incidence;
        private final File status;

        /**
         * Default constructor
         *
         * @param status
         *            the status file
         */
        public CompileFinished(File status) {
            final String modelName = controller.getRoot().getName();
            incidence = new File(ScilabConstants.TMPDIR, modelName + INCIDENCE + EXTENSION);
            this.status = status;
        }

        /**
         * Update things and compute the compiled model
         *
         * @param e
         *            the event
         * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
         */
        @Override
        public void actionPerformed(ActionEvent e) {
            if (status.exists()) {
                try {
                    // update the model with the computed identifiers
                    final Model incidenceModel = Modelica.getInstance().load(incidence);
                    controller.getRoot().setIdentifiers(incidenceModel.getIdentifiers());
                    controller.getRoot().setOutputs(incidenceModel.getOutputs());

                    // update the statistics
                    controller.getStatistics().fireChange();

                    // update state
                    controller.setCompileNeeded(false);

                    // creating a temporary status file
                    File statusFile = File.createTempFile(controller.getRoot().getName(), null);
                    statusFile.delete();

                    final ModelStatistics stats = controller.getStatistics();
                    long nUnknowns = stats.getUnknowns() - stats.getEquations();
                    String cmd = String.format(COMPUTE_STRING, controller.getComputeMethod(), nUnknowns, statusFile.getAbsolutePath());

                    // compute now
                    Logger.getLogger(SolveAction.class.getName()).finest("Computing");
                    ScilabInterpreterManagement.asynchronousScilabExec(new ComputeFinished(statusFile), cmd);
                } catch (JAXBException e1) {
                    Logger.getLogger(SolveAction.class.getName()).severe(e1.toString());
                } catch (IOException e1) {
                    Logger.getLogger(SolveAction.class.getName()).severe(e1.toString());
                } catch (InterpreterException e1) {
                    Logger.getLogger(SolveAction.class.getName()).severe(e1.toString());
                }
            } else {
                // best effort to alert the user.
                initStatus();
            }
        }
    }

    /**
     * Action done at the end of the computation
     */
    private final class ComputeFinished implements ActionListener {
        /**
         * Default constructor
         *
         * @param status
         *            the status file
         */
        public ComputeFinished(File status) {
        }

        /**
         * Clean the state
         *
         * @param e
         *            the event
         * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
         */
        @Override
        public void actionPerformed(ActionEvent e) {
            initStatus();
        }
    }
}
