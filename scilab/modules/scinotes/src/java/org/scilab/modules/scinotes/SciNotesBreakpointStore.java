/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.scinotes;

import java.io.File;
import java.io.IOException;
import java.io.Reader;
import java.io.Writer;
import java.nio.charset.StandardCharsets;
import java.nio.file.AtomicMoveNotSupportedException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;
import java.util.ArrayList;
import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

import org.scilab.modules.commons.ScilabConstants;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

/** Persists the breakpoint definitions desired by SciNotes across processes. */
public final class SciNotesBreakpointStore {

    private static final int FORMAT_VERSION = 1;
    private static final String FILE_NAME = "scinotes-breakpoints.json";
    private static final Gson GSON = new GsonBuilder().setPrettyPrinting().create();
    /** Compact form: the result is inlined in a one-line executeJavaScript call. */
    private static final Gson COMPACT_GSON = new Gson();
    private static final SciNotesBreakpointStore INSTANCE = new SciNotesBreakpointStore();

    private final Path storagePath;
    private StoreData data;

    private SciNotesBreakpointStore() {
        storagePath = ScilabConstants.SCIHOME.toPath().resolve(FILE_NAME);
        data = load();
    }

    public static SciNotesBreakpointStore getInstance() {
        return INSTANCE;
    }

    /** Return a detached snapshot for one canonical source path. */
    public synchronized List<Breakpoint> getBreakpoints(String sourcePath) {
        String canonical = canonicalPath(sourcePath);
        List<Breakpoint> breakpoints = data.sources.get(canonical);
        if (breakpoints == null) {
            return Collections.emptyList();
        }
        return copyBreakpoints(breakpoints);
    }

    /** Replace and persist the complete desired list for one source. */
    public synchronized void replaceBreakpoints(String sourcePath, List<Breakpoint> breakpoints) {
        String canonical = canonicalPath(sourcePath);
        if (canonical.isEmpty()) {
            return;
        }

        List<Breakpoint> valid = sanitize(breakpoints);
        if (valid.isEmpty()) {
            data.sources.remove(canonical);
        } else {
            data.sources.put(canonical, valid);
        }
        save();
    }

    /**
     * Give the file a Save As wrote the breakpoints of the file it copied.
     * The original is still on disk, unchanged, and keeps its own.
     */
    public synchronized void copy(String oldSourcePath, String newSourcePath) {
        String oldCanonical = canonicalPath(oldSourcePath);
        String newCanonical = canonicalPath(newSourcePath);
        if (oldCanonical.isEmpty() || newCanonical.isEmpty() || oldCanonical.equals(newCanonical)) {
            return;
        }

        List<Breakpoint> copied = data.sources.get(oldCanonical);
        if (copied != null && !copied.isEmpty()) {
            data.sources.put(newCanonical, new ArrayList<Breakpoint>(copied));
            save();
        }
    }

    public synchronized String getBreakpointsJson(String sourcePath) {
        return COMPACT_GSON.toJson(getBreakpoints(sourcePath));
    }

    /**
     * The whole store, as one compact JSON object keyed by canonical path.
     * A breakpoint belongs to the file, not to an open tab: the debug session
     * is told about every file the store knows, so a line still stops the
     * execution when no editor shows that file.
     * @return the complete desired list of every known source
     */
    public synchronized String getAllBreakpointsJson() {
        return COMPACT_GSON.toJson(data.sources);
    }

    private StoreData load() {
        if (!Files.isRegularFile(storagePath)) {
            return new StoreData();
        }

        try (Reader reader = Files.newBufferedReader(storagePath, StandardCharsets.UTF_8)) {
            StoreData loaded = GSON.fromJson(reader, StoreData.class);
            if (loaded == null || loaded.version != FORMAT_VERSION || loaded.sources == null) {
                System.err.println("SciNotes DAP: unsupported breakpoint store format: " + storagePath);
                return new StoreData();
            }

            StoreData cleaned = new StoreData();
            boolean changed = false;
            for (Map.Entry<String, List<Breakpoint>> entry : loaded.sources.entrySet()) {
                String canonical = canonicalPath(entry.getKey());
                if (canonical.isEmpty() || !Files.isRegularFile(Path.of(canonical))) {
                    changed = true;
                    continue;
                }

                List<Breakpoint> valid = sanitize(entry.getValue());
                if (valid.isEmpty()) {
                    changed = true;
                    continue;
                }
                if (!canonical.equals(entry.getKey()) || valid.size() != entry.getValue().size()) {
                    changed = true;
                }
                cleaned.sources.put(canonical, valid);
            }

            data = cleaned;
            if (changed) {
                save();
            }
            return cleaned;
        } catch (IOException | RuntimeException e) {
            System.err.println("SciNotes DAP: unable to load breakpoint store: " + e.getMessage());
            return new StoreData();
        }
    }

    private void save() {
        Path parent = storagePath.getParent();
        if (parent == null) {
            return;
        }

        Path temporary = null;
        try {
            Files.createDirectories(parent);
            temporary = Files.createTempFile(parent, FILE_NAME, ".tmp");
            try (Writer writer = Files.newBufferedWriter(temporary, StandardCharsets.UTF_8)) {
                GSON.toJson(data, writer);
            }
            try {
                Files.move(temporary, storagePath, StandardCopyOption.ATOMIC_MOVE, StandardCopyOption.REPLACE_EXISTING);
            } catch (AtomicMoveNotSupportedException e) {
                Files.move(temporary, storagePath, StandardCopyOption.REPLACE_EXISTING);
            }
        } catch (IOException e) {
            System.err.println("SciNotes DAP: unable to save breakpoint store: " + e.getMessage());
        } finally {
            if (temporary != null) {
                try {
                    Files.deleteIfExists(temporary);
                } catch (IOException e) {
                    // The next SCIHOME cleanup can remove an abandoned temporary file.
                }
            }
        }
    }

    private static List<Breakpoint> sanitize(List<Breakpoint> breakpoints) {
        List<Breakpoint> valid = new ArrayList<Breakpoint>();
        if (breakpoints == null) {
            return valid;
        }
        for (Breakpoint breakpoint : breakpoints) {
            if (breakpoint != null && breakpoint.line > 0) {
                valid.add(new Breakpoint(breakpoint));
            }
        }
        return valid;
    }

    private static List<Breakpoint> copyBreakpoints(List<Breakpoint> breakpoints) {
        List<Breakpoint> copy = new ArrayList<Breakpoint>(breakpoints.size());
        for (Breakpoint breakpoint : breakpoints) {
            copy.add(new Breakpoint(breakpoint));
        }
        return copy;
    }

    private static String canonicalPath(String sourcePath) {
        if (sourcePath == null || sourcePath.isEmpty()) {
            return "";
        }
        try {
            return new File(sourcePath).getCanonicalPath();
        } catch (IOException e) {
            return new File(sourcePath).getAbsoluteFile().toPath().normalize().toString();
        }
    }

    private static final class StoreData {
        private int version = FORMAT_VERSION;
        private Map<String, List<Breakpoint>> sources = new LinkedHashMap<String, List<Breakpoint>>();
    }

    /** Durable fields of one source breakpoint; verification remains runtime-only. */
    public static final class Breakpoint {
        private int line;
        private boolean enabled;
        private String condition;
        private String hitCondition;

        public Breakpoint(int line, boolean enabled, String condition, String hitCondition) {
            this.line = line;
            this.enabled = enabled;
            this.condition = condition == null ? "" : condition;
            this.hitCondition = hitCondition == null ? "" : hitCondition;
        }

        private Breakpoint(Breakpoint other) {
            this(other.line, other.enabled, other.condition, other.hitCondition);
        }

        public int getLine() {
            return line;
        }

        public boolean isEnabled() {
            return enabled;
        }

        public String getCondition() {
            return condition;
        }

        public String getHitCondition() {
            return hitCondition;
        }
    }
}
