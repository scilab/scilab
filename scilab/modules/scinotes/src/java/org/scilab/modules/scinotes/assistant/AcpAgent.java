package org.scilab.modules.scinotes.assistant;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

/**
 * One ACP agent of the catalogue ({@link AgentConfig}, agents.json): how to
 * find the program the user installed and start it in ACP mode. Pure data —
 * nothing here depends on which agent it is.
 */
final class AcpAgent {

    final String id;
    final String name;
    final List<String> command;
    final List<String> args;
    final List<String> dirs;
    final Map<String, String> env;

    private AcpAgent(String id) {
        this.id = id;
        String n = AgentConfig.string(id, "name");
        this.name = n != null && !n.isEmpty() ? n : id;
        this.command = AgentConfig.stringList(id, "command");
        this.args = AgentConfig.stringList(id, "args");
        this.dirs = AgentConfig.stringList(id, "dirs");
        this.env = AgentConfig.env(id);
    }

    /** The agent of this id, null when the catalogue has none. */
    static AcpAgent find(String id) {
        return id != null && AgentConfig.ids().contains(id) ? new AcpAgent(id) : null;
    }

    /** All agents, in catalogue order. */
    static List<AcpAgent> all() {
        List<AcpAgent> list = new ArrayList<>();
        for (String id : AgentConfig.ids()) {
            list.add(new AcpAgent(id));
        }
        return list;
    }

    /** Executable set by the user in the settings page, or null. */
    String customPath() {
        String p = AssistantSettings.getString("acp_path." + id);
        return p == null || p.trim().isEmpty() ? null : p.trim();
    }

    /**
     * The program to run: the user's path when set (null when it no longer
     * exists), else the first command name found in "dirs" then on the
     * PATH (and in the usual install directories on Linux/macOS). Null when
     * the agent is not installed.
     */
    String binary() {
        String custom = customPath();
        if (custom != null) {
            File f = new File(custom);
            return f.isFile() ? f.getAbsolutePath() : null;
        }
        List<String> names = new ArrayList<>();
        for (String c : command) {
            if (AgentPlatform.isWindows()) {
                names.add(c + ".exe");
                names.add(c + ".cmd");
            }
            names.add(c);
        }
        for (String d : dirs) {
            File dir = new File(expand(d));
            for (String n : names) {
                File f = new File(dir, n);
                if (f.isFile()) {
                    return f.getAbsolutePath();
                }
            }
        }
        String[] arr = names.toArray(new String[0]);
        return AgentPlatform.isWindows() ? AgentPlatform.findOnPath(arr) : AgentPlatform.findUnix(arr);
    }

    /** The full command line, or null when the agent is not installed. */
    List<String> commandLine() {
        return commandLine(binary());
    }

    /** The command line with this program (or the first command name when null). */
    List<String> commandLine(String bin) {
        if (bin == null && command.isEmpty()) {
            return null;
        }
        List<String> cmd = new ArrayList<>();
        cmd.add(bin != null ? bin : command.get(0));
        cmd.addAll(args);
        return cmd;
    }

    /** Expand ~ and %VAR% in a directory of the catalogue. */
    private static String expand(String d) {
        if (d.startsWith("~")) {
            d = AgentPlatform.userHome() + d.substring(1);
        }
        StringBuilder sb = new StringBuilder();
        int i = 0;
        while (i < d.length()) {
            int a = d.indexOf('%', i);
            int b = a < 0 ? -1 : d.indexOf('%', a + 1);
            if (b < 0) {
                sb.append(d, i, d.length());
                break;
            }
            String v = System.getenv(d.substring(a + 1, b));
            sb.append(d, i, a).append(v != null ? v : "");
            i = b + 1;
        }
        return sb.toString();
    }
}
