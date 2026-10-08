package org.scilab.modules.scinotes.assistant;

/**
 * Minimal JSON helpers — no external dependency.
 */
final class JsonUtil {

    private JsonUtil() {
    }

    static String extractString(String json, String key) {
        String search = "\"" + key + "\"";
        int idx = json.indexOf(search);
        if (idx < 0) {
            return null;
        }
        idx = json.indexOf(':', idx + search.length());
        if (idx < 0) {
            return null;
        }
        idx++;
        while (idx < json.length() && json.charAt(idx) == ' ') {
            idx++;
        }
        if (idx >= json.length()) {
            return null;
        }
        // String value: "..."
        if (json.charAt(idx) == '"') {
            int end = findClosingQuote(json, idx + 1);
            if (end < 0) {
                return null;
            }
            return unescape(json.substring(idx + 1, end));
        }
        // Number/literal value (for id fields): 123, true, false, null
        int end = idx;
        while (end < json.length() && json.charAt(end) != ',' && json.charAt(end) != '}'
               && json.charAt(end) != ' ' && json.charAt(end) != '\n') {
            end++;
        }
        String val = json.substring(idx, end);
        return "null".equals(val) ? null : val;
    }

    private static int findClosingQuote(String s, int from) {
        for (int i = from; i < s.length(); i++) {
            if (s.charAt(i) == '\\') {
                i++;
                continue;
            }
            if (s.charAt(i) == '"') {
                return i;
            }
        }
        return -1;
    }

    static String escape(String s) {
        if (s == null) {
            return "";
        }
        return s.replace("\\", "\\\\")
                .replace("\"", "\\\"")
                .replace("\n", "\\n")
                .replace("\r", "\\r")
                .replace("\t", "\\t");
    }

    static String unescape(String s) {
        if (s == null) {
            return null;
        }
        StringBuilder sb = new StringBuilder(s.length());
        for (int i = 0; i < s.length(); i++) {
            if (s.charAt(i) == '\\' && i + 1 < s.length()) {
                char next = s.charAt(i + 1);
                switch (next) {
                    case 'n':  sb.append('\n'); i++; break;
                    case 'r':  sb.append('\r'); i++; break;
                    case 't':  sb.append('\t'); i++; break;
                    case '"':  sb.append('"');  i++; break;
                    case '\\': sb.append('\\'); i++; break;
                    default:   sb.append(s.charAt(i)); break;
                }
            } else {
                sb.append(s.charAt(i));
            }
        }
        return sb.toString();
    }
}
