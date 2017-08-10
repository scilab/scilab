/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
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

package org.scilab.modules.action_binding.highlevel;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import javax.swing.SwingUtilities;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.action_binding.utils.ActionBindingMessages;
import org.scilab.modules.action_binding.utils.Signal;

/**
 * Implements useful methods to communicate with the Scilab interpreter.
 */
public final class ScilabInterpreterManagement extends InterpreterManagement {

    private static ExecutorService executor = Executors
            .newSingleThreadExecutor();
    private static Set<String> runningTasks = Collections
            .synchronizedSet(new HashSet<String>());

    private static final String NOTIFY = ";notify(\"";
    private static final String CLOSE = "\");";

    /** This class is a static singleton, thus it must not be instantiated */
    private ScilabInterpreterManagement() {
    }

    /**
     * Throw when there is a problem to communicate with the scilab interpreter.
     */
    public static class InterpreterException extends Exception {

        private static final long serialVersionUID = -7222406936060866827L;

        /**
         * Default constructor
         *
         * @param string
         *            Useful message
         */
        public InterpreterException(String string) {
            super(string);
        }
    }

    /**
     * Execute the command and wait for the end of the execution.
     *
     * @param command
     *            The scilab command
     * @throws InterpreterException
     *             when the command cannot be executed on the interpreter.
     */
    public static void synchronousScilabExec(String command)
    throws InterpreterException {
        final String uidDesc = Integer.toString(command.hashCode());
        final String fullCommand = protectStatements(command, uidDesc);

        if (runningTasks.contains(uidDesc)) {
            throw new InterpreterException(
                ActionBindingMessages.SCILAB_SAMECOMMAND);
        }

        int ret = InterpreterManagement.requestScilabExec(fullCommand);
        if (ret != 0) {
            throw new InterpreterException(ActionBindingMessages.SCILAB_UNABLE);
        }
        runningTasks.add(uidDesc);
        Signal.wait(uidDesc);
        runningTasks.remove(uidDesc);
    }

    /**
     * Execute the command and wait for the end of the execution.
     *
     * @param function
     *            The scilab function
     * @param args
     *            The arguments to the Scilab function.
     *
     *            This method will escape any {@link CharSequence} instance but
     *            not char arrays. Thus if you want to pass a raw identifier,
     *            just transform it to a char array.
     * @throws InterpreterException
     *             when the command cannot be executed on the interpreter.
     */
    public static void synchronousScilabExec(String function, Object... args)
    throws InterpreterException {
        synchronousScilabExec(buildCall(function, args));
    }

    /**
     * Execute the command asynchronously and call the callback actionPerformed
     * method when the execution ends.
     *
     * The ActionEvent send to the callback actionPerformed method is composed
     * of :
     * <ul>
     * <li>source: The XcosInterpreterManagement.class value which is unique.</li>
     * <li>id: the command.hashCode() value use it to compare to your own.</li>
     * <li>command: the command passed to the interpreter.</li>
     * </ul>
     *
     * Note: the callback is called on the EDT thread so your are free to
     * manipulate AWT/SWING objects on it.
     *
     * @param command
     *            The command to execute
     * @param callback
     *            The callback which is called at the end of the execution.
     * @throws InterpreterException
     *             when the command cannot be executed on the interpreter.
     * @deprecated This function as been deprecated in favor of the
     *             {@link #asynchronousScilabExec(ActionListener, String)}
     *             method to be coherent with
     *             {@link #asynchronousScilabExec(ActionListener, String, Object...)}
     *             .
     */
    @Deprecated
    public static void asynchronousScilabExec(String command,
            final ActionListener callback) throws InterpreterException {
        final int uid = command.hashCode();
        final String uidDesc = Integer.toString(uid);
        final String fullCommand = protectStatements(command, uidDesc);
        final ActionEvent event = new ActionEvent(
            ScilabInterpreterManagement.class, uid, command);

        if (runningTasks.contains(uidDesc)) {
            throw new InterpreterException(
                ActionBindingMessages.SCILAB_SAMECOMMAND);
        }

        executor.submit(new Callable<Void>() {
            @Override
            public Void call() throws Exception {
                int ret = InterpreterManagement
                          .putCommandInScilabQueue(fullCommand);
                if (ret != 0) {
                    throw new InterpreterException(
                        ActionBindingMessages.SCILAB_UNABLE);
                }
                runningTasks.add(uidDesc);
                Signal.wait(uidDesc);
                runningTasks.remove(uidDesc);
                SwingUtilities.invokeLater(new Runnable() {
                    @Override
                    public void run() {
                        callback.actionPerformed(event);
                    }
                });
                return null;
            }
        });
    }

    /**
     * Execute the command asynchronously and call the callback actionPerformed
     * method when the execution ends.
     *
     * The ActionEvent send to the callback actionPerformed method is composed
     * of :
     * <ul>
     * <li>source: The XcosInterpreterManagement.class value which is unique.</li>
     * <li>id: the command.hashCode() value use it to compare to your own.</li>
     * <li>command: the command passed to the interpreter.</li>
     * </ul>
     *
     * Note: the callback is called on the EDT thread so your are free to
     * manipulate AWT/SWING objects on it.
     *
     * @param command
     *            The command to execute
     * @param callback
     *            The callback which is called at the end of the execution.
     * @throws InterpreterException
     *             when the command cannot be executed on the interpreter.
     */
    public static void asynchronousScilabExec(final ActionListener callback,
            String command) throws InterpreterException {
        final int uid = command.hashCode();
        final String uidDesc = Integer.toString(uid);
        final String fullCommand = protectStatements(command, uidDesc);
        final ActionEvent event = new ActionEvent(
            ScilabInterpreterManagement.class, uid, command);

        if (runningTasks.contains(uidDesc)) {
            throw new InterpreterException(
                ActionBindingMessages.SCILAB_SAMECOMMAND);
        }

        executor.submit(new Callable<Void>() {
            @Override
            public Void call() throws Exception {
                int ret = InterpreterManagement
                          .putCommandInScilabQueue(fullCommand);
                if (ret != 0) {
                    throw new InterpreterException(
                        ActionBindingMessages.SCILAB_UNABLE);
                }
                runningTasks.add(uidDesc);
                Signal.wait(uidDesc);
                runningTasks.remove(uidDesc);
                if (callback != null) {
                    SwingUtilities.invokeLater(new Runnable() {
                        @Override
                        public void run() {
                            callback.actionPerformed(event);
                        }
                    });
                }
                return null;
            }
        });
    }

    /**
     * Execute the command asynchronously and call the callback actionPerformed
     * method when the execution ends.
     *
     * The ActionEvent send to the callback actionPerformed method is composed
     * of :
     * <ul>
     * <li>source: The XcosInterpreterManagement.class value which is unique.</li>
     * <li>id: the command.hashCode() value use it to compare to your own.</li>
     * <li>command: the command passed to the interpreter.</li>
     * </ul>
     *
     * Note: the callback is called on the EDT thread so your are free to
     * manipulate AWT/SWING objects on it.
     *
     * @param callback
     *            The callback which is called at the end of the execution.
     * @param function
     *            The Scilab function
     * @param args
     *            The arguments to the Scilab function.
     *
     *            This method will escape any {@link CharSequence} instance but
     *            not char arrays. Thus if you want to pass a raw identifier,
     *            just transform it to a char array.
     *
     * @throws InterpreterException
     *             when the command cannot be executed on the interpreter.
     */
    public static void asynchronousScilabExec(final ActionListener callback,
            String function, Object... args) throws InterpreterException {
        asynchronousScilabExec(callback, buildCall(function, args));
    }

    /**
     * This method halt a command performed asynchronously.
     *
     * @param hashcode
     *            The command.getHashcode() uid.
     */
    public static void stopScilabExec(int hashcode) {
        String uidDesc = Integer.toString(hashcode);

        synchronized (runningTasks) {
            if (runningTasks.contains(uidDesc)) {
                Signal.notify(uidDesc);
                runningTasks.remove(uidDesc);
            }
        }
    }

    /**
     * This method stop all the running scilab execution (sync or async).
     */
    public static void stopAllScilabExec() {
        synchronized (runningTasks) {
            interruptScilab();
            for (String uidDesc : runningTasks) {
                Signal.notify(uidDesc);
            }
        }
    }

    /**
     * Construct a new Scilab command calling function with args as arguments.
     *
     * This method will escape any {@link CharSequence} instance but not char
     * arrays. Thus if you want to pass a raw identifier, just transform it to a
     * char array.
     *
     * @param function
     *            the function to call
     * @param args
     *            the function arguments
     * @return the command
     */
    public static String buildCall(String function, Object... args) {
        StringBuilder b = new StringBuilder(function);

        b.append('(');

        final List<Object> lst = Arrays.asList(args);
        for (Iterator<Object> iterator = lst.iterator(); iterator.hasNext();) {
            Object object = iterator.next();

            /*
             * Appending the object
             */
            if (object instanceof CharSequence) {
                // We escape any CharSequence
                b.append("\"");
                b.append(object);
                b.append("\"");
            } else if (object instanceof char[]) {
                // We don't escape char[]
                b.append((char[]) object);
            } else if (object instanceof Boolean) {
                boolean value = (Boolean) object;
                if (value) {
                    b.append("%t");
                } else {
                    b.append("%f");
                }
            } else {
                b.append(object);
            }

            /*
             * Putting next comma
             */
            if (iterator.hasNext()) {
                b.append(", ");
            }
        }
        b.append(")");

        return b.toString();
    }

    /**
     * Protect some statements of commands within a try-catch block.
     * @param statements the statements to protect
     * @param uid the uid used on the notify(...) call
     * @return the protected statements
     */
    private static String protectStatements(String statements, String uid) {
        return new StringBuilder()

               .insert(0, "try, ")
               .append(statements)
               .append(" ,catch")
               .append(" disp(lasterror()); ")
               .append(" end ")

               .append(NOTIFY).append(uid).append(CLOSE)

               .toString();
    }
}
