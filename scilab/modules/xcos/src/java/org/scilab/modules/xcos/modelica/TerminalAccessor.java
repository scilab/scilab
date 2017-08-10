/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2010 - DIGITEO - Clement DAVID <clement.david@scilab.org>
 * Copyright (C) 2011-2011 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.modelica;

import java.util.ArrayList;
import java.util.EventListener;
import java.util.EventObject;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.scilab.modules.xcos.modelica.model.ModelicaValue;
import org.scilab.modules.xcos.modelica.model.Terminal;

/**
 * Class used to access fields of {@link Terminal}.
 *
 * To get the typed data, use {@link #getData(TerminalAccessor, Terminal)}.
 */
public enum TerminalAccessor {
    // CSOFF: JavadocVariable
    // CSOFF: JavadocMethod
    /**
     * Name of the terminal
     */
    NAME(ModelicaMessages.NAME, String.class, false) {
        @Override
        protected Object getData(Terminal terminal) {
            return terminal.getName();
        }

        @Override
        public void setData(Object aValue, Terminal terminal) {
            final Object value = getData(terminal);
            terminal.setName(aValue.toString());
            firePropertyChange(terminal, value, aValue);
        }
    },
    /**
     * ID of the terminal
     */
    ID(ModelicaMessages.ID, String.class, false) {
        @Override
        protected Object getData(Terminal terminal) {
            return terminal.getId();
        }

        @Override
        public void setData(Object aValue, Terminal terminal) {
            final Object value = getData(terminal);
            terminal.setId(aValue.toString());
            firePropertyChange(terminal, value, aValue);
        }
    },
    /**
     * Kind of the terminal
     */
    KIND(ModelicaMessages.KIND, String.class, false) {
        @Override
        protected Object getData(Terminal terminal) {
            return terminal.getKind();
        }

        @Override
        public void setData(Object aValue, Terminal terminal) {
            final Object value = getData(terminal);
            terminal.setKind(aValue.toString());
            firePropertyChange(terminal, value, aValue);

        }
    },
    /**
     * Is this value fixed ?
     */
    FIXED(ModelicaMessages.FIXED, Boolean.class, false) {
        @Override
        protected Object getData(Terminal terminal) {
            if (terminal.getFixed() == null) {
                terminal.setFixed(new ModelicaValue());
            }

            if (terminal.getFixed().getValue().isEmpty()) {
                return Boolean.valueOf(false);
            }

            final String value = terminal.getFixed().getValue();
            return Boolean.valueOf(value);
        }

        @Override
        public void setData(Object aValue, Terminal terminal) {
            final Object value = getData(terminal);
            terminal.getFixed().setValue(aValue.toString());
            firePropertyChange(terminal, value, aValue);
        }
    },
    /**
     * Initial value
     */
    INITIAL(ModelicaMessages.INITIAL, Double.class, true) {
        @Override
        protected Object getData(Terminal terminal) {
            if (terminal.getInitialValue() == null) {
                terminal.setInitialValue(new ModelicaValue());
            }

            if (terminal.getInitialValue().getValue().isEmpty()) {
                return Double.valueOf(0.0);
            }

            final String value = terminal.getInitialValue().getValue();
            return Double.valueOf(value);
        }

        @Override
        public void setData(Object aValue, Terminal terminal) {
            final Object value = getData(terminal);
            terminal.getInitialValue().setValue(aValue.toString());
            firePropertyChange(terminal, value, aValue);
        }
    },
    /**
     * Weight of the nominal value
     */
    WEIGHT(ModelicaMessages.WEIGHT, Double.class, true) {
        @Override
        protected Object getData(Terminal terminal) {
            if (terminal.getWeight() == null) {
                terminal.setWeight(new ModelicaValue());
            }

            if (terminal.getWeight().getValue().isEmpty()) {
                final Boolean fixed = getData(FIXED, terminal);

                final Double initData;
                if (fixed.booleanValue()) {
                    initData = Double.valueOf(1.0);
                } else {
                    initData = Double.valueOf(0.0);
                }

                return initData;
            }

            final String value = terminal.getWeight().getValue();
            return Double.valueOf(value);
        }

        @Override
        public void setData(Object aValue, Terminal terminal) {
            final Object value = getData(terminal);
            terminal.getWeight().setValue(aValue.toString());
            firePropertyChange(terminal, value, aValue);
        }
    },
    //    /**
    //     * Max of the value
    //     */
    //    MAX(ModelicaMessages.MAX, Double.class, true) {
    //        @Override
    //        protected Object getData(Terminal terminal) {
    //            if (terminal.getMax() == null) {
    //                terminal.setMax(new ModelicaValue());
    //            }
    //
    //            if (terminal.getMax().getValue().isEmpty()) {
    //                return Double.valueOf(0.0);
    //            }
    //
    //            final String value = terminal.getMax().getValue();
    //            return Double.valueOf(value);
    //        }
    //
    //        @Override
    //        public void setData(Object aValue, Terminal terminal) {
    //            final Object value = getData(terminal);
    //            terminal.getMax().setValue(aValue.toString());
    //            firePropertyChange(terminal, value, aValue);
    //        }
    //    },
    //    /**
    //     * Min of the value
    //     */
    //    MIN(ModelicaMessages.MIN, Double.class, true) {
    //        @Override
    //        protected Double getData(Terminal terminal) {
    //            if (terminal.getMin() == null) {
    //                terminal.setMin(new ModelicaValue());
    //            }
    //
    //            if (terminal.getMin().getValue().isEmpty()) {
    //                return Double.valueOf(0.0);
    //            }
    //
    //            final String value = terminal.getMin().getValue();
    //            return Double.valueOf(value);
    //        }
    //
    //        @Override
    //        public void setData(Object aValue, Terminal terminal) {
    //            final Object value = getData(terminal);
    //            terminal.getMin().setValue(aValue.toString());
    //            firePropertyChange(terminal, value, aValue);
    //        }
    //    },
    //    /**
    //     * Nominal value
    //     */
    //    NOMINAL(ModelicaMessages.NOMINAL, Double.class, true) {
    //        @Override
    //        protected Double getData(Terminal terminal) {
    //            if (terminal.getNominalValue() == null) {
    //                terminal.setNominalValue(new ModelicaValue());
    //            }
    //
    //            if (terminal.getNominalValue().getValue().isEmpty()) {
    //                return Double.valueOf(0.0);
    //            }
    //
    //            final String value = terminal.getNominalValue().getValue();
    //            return Double.valueOf(value);
    //        }
    //
    //        @Override
    //        public void setData(Object aValue, Terminal terminal) {
    //            final Object value = getData(terminal);
    //            terminal.getNominalValue().setValue(aValue.toString());
    //            firePropertyChange(terminal, value, aValue);
    //        }
    //    },
    /**
     * Comment to the terminal
     */
    COMMENT(ModelicaMessages.COMMENT, String.class, true) {
        @Override
        protected Object getData(Terminal terminal) {
            if (terminal.getComment() == null) {
                terminal.setComment(new ModelicaValue());
            }

            return terminal.getComment().getValue();
        }

        @Override
        public void setData(Object aValue, Terminal terminal) {
            final Object value = getData(terminal);
            terminal.getComment().setValue(aValue.toString());
            firePropertyChange(terminal, value, aValue);
        }
    },
    /**
     * Is the terminal selected ?
     */
    SELECTED(ModelicaMessages.SELECTED, Boolean.class, true) {
        private static final String TRUE = "y";
        private static final String FALSE = "n";

        @Override
        protected Boolean getData(Terminal terminal) {
            if (terminal.getSelected() == null) {
                terminal.setSelected(new ModelicaValue());
            }

            return TRUE.equals(terminal.getSelected().getValue());
        }

        @Override
        public void setData(Object aValue, Terminal terminal) {
            final Object oldValue = getData(terminal);

            final String value;
            if ((Boolean) aValue) {
                value = TRUE;
            } else {
                value = FALSE;
            }
            terminal.getSelected().setValue(value);
            firePropertyChange(terminal, oldValue, aValue);
        }
    };
    // CSON: JavadocVariable
    // CSON: JavadocMethod

    /**
     * Event on model change
     */
    @SuppressWarnings(value = { "serial" })
    public static class ChangeEvent extends EventObject {
        private final Terminal terminal;
        private final Object oldData;
        private final Object newData;

        /**
         * Default constructor.
         *
         * @param source
         *            the source
         * @param terminal
         *            the terminal
         * @param oldData
         *            the old data
         * @param newData
         *            the new data
         */
        public ChangeEvent(TerminalAccessor source, Terminal terminal,
        Object oldData, Object newData) {
            super(source);

            this.terminal = terminal;
            this.oldData = oldData;
            this.newData = newData;
        }

        /**
         * @return the terminal
         */
        public Terminal getTerminal() {
            return terminal;
        }

        /**
         * @return the oldData
         */
        public Object getOldData() {
            return oldData;
        }

        /**
         * @return the newData
         */
        public Object getNewData() {
            return newData;
        }
    }

    /**
     * Interface used to propagated change event.
     */
    public static interface ChangeListener extends EventListener {
        /**
         * Called on change.
         *
         * @param event
         *            the event
         */
        void change(ChangeEvent event);
    }

    /**
     * Support event notification.
     */
    public static class ChangeSupport {
        private final Map<TerminalAccessor, List<ChangeListener>> listeners;

        /**
         * Default constructor.
         */
        public ChangeSupport() {
            listeners = new HashMap<TerminalAccessor, List<ChangeListener>>();
        }

        /**
         * Add a listener for a defined accessor
         *
         * @param field
         *            the accessor
         * @param listener
         *            the listener to add
         */
        public void addChangeListener(TerminalAccessor field,
        ChangeListener listener) {
            if (!listeners.containsKey(field)) {
                listeners.put(field, new ArrayList<ChangeListener>());
            }

            final List<ChangeListener> listenerList = listeners.get(field);
            listenerList.add(listener);
        }

        /**
         * Remove the listener from the listener list
         *
         * @param field
         *            the accessor
         * @param listener
         *            the listener
         */
        public void removeChangeListener(TerminalAccessor field,
                                         ChangeListener listener) {
            final List<ChangeListener> listenerList = listeners.get(field);
            if (listenerList != null) {
                listenerList.remove(listener);
            }
        }

        /**
         * Fire a change
         *
         * @param field
         *            the field
         * @param event
         *            the event
         */
        protected void fireChangeEvent(TerminalAccessor field, ChangeEvent event) {
            if (listeners.containsKey(field)) {
                for (ChangeListener listener : listeners.get(field)) {
                    listener.change(event);
                }
            }
        }
    }

    private static final String VARIABLE = "variable";
    private static final String FIXED_PARAMETER = "fixed_parameter";

    private static ChangeSupport pcs = new ChangeSupport();

    /**
     * static constructor used to setup the Terminal change support
     */
    static {
        // update the fixed state on update
        // WEIGHT.addChangeListener(new ChangeListener() {
        // @Override
        // public void change(TerminalAccessor.ChangeEvent event) {
        // final Terminal terminal = event.getTerminal();
        //
        // if (terminal.getKind().equals(FIXED_PARAMETER)
        // || terminal.getKind().equals(VARIABLE)) {
        // if (((Double) getData(WEIGHT,terminal)) >= 1.0) {
        // FIXED.setData(Boolean.TRUE,
        // terminal);
        // } else {
        // FIXED.setData(Boolean.FALSE,
        // terminal);
        // }
        // } else {
        // FIXED.setData(Boolean.FALSE,
        // terminal);
        // }
        // }
        // });

        // set a default initial value on weight change
        TerminalAccessor.WEIGHT.addChangeListener(new ChangeListener() {
            @Override
            public void change(ChangeEvent event) {
                // will update the default initial on access value
                @SuppressWarnings("unused")
                final Double notUsed = getData(INITIAL, event.getTerminal());
            }
        });
    }

    private String name;
    private Class<?> klass;
    private boolean editable;

    /**
     * Default constructor
     *
     * @param name
     *            the name
     * @param klass
     *            the klass
     * @param editable
     *            is editable flag
     */
    private TerminalAccessor(String name, Class<?> klass, boolean editable) {
        this.name = name;
        this.klass = klass;
        this.editable = editable;
    }

    /**
     * Get the typed data
     *
     * @param <T>
     *            the type
     * @param identifier
     *            the current identifier
     * @param terminal
     *            the current data to parse
     * @return the data of the identifier
     */
    @SuppressWarnings({ "unchecked" })
    public static <T> T getData(TerminalAccessor identifier, Terminal terminal) {
        return (T) identifier.getData(terminal);
    }

    /**
     * @return the name
     */
    public String getName() {
        return name;
    }

    /**
     * @return the klass
     */
    public Class<?> getKlass() {
        return klass;
    }

    /**
     * @return the editable status
     */
    public boolean isEditable() {
        return editable;
    }

    /**
     * Get the data for the current identifier.
     *
     * @param terminal
     *            the terminal data
     * @return the identifier data
     */
    protected abstract Object getData(Terminal terminal);

    /**
     * Set the data for the current identifier
     *
     * @param aValue
     *            the identifier new value
     * @param terminal
     *            the terminal data
     */
    public abstract void setData(Object aValue, Terminal terminal);

    /*
     * Property change support.
     */

    /**
     * Report a bound property update to any registered listeners. No event is
     * fired if old and new are equal and non-null.
     *
     * <p>
     * This is merely a convenience wrapper around the more general fireChange
     * method that takes {@code ChangeEvent} value.
     *
     * @param propertyName
     *            The programmatic name of the property that was changed.
     * @param oldValue
     *            The old value of the property.
     * @param newValue
     *            The new value of the property.
     */
    protected void firePropertyChange(Terminal propertyName, Object oldValue,
                                      Object newValue) {
        if (oldValue != null && newValue != null && oldValue.equals(newValue)) {
            return;
        }
        pcs.fireChangeEvent(this, new ChangeEvent(this, propertyName, oldValue,
                            newValue));
    }

    /**
     * Add a ChangeListener to the listener list. The same listener object may
     * be added more than once, and will be called as many times as it is added.
     * If <code>listener</code> is null, no exception is thrown and no action is
     * taken.
     *
     * @param listener
     *            The PropertyChangeListener to be added
     */
    public void addChangeListener(ChangeListener listener) {
        pcs.addChangeListener(this, listener);
    }
}
