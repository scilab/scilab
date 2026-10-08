"""Minimal MCP server mimicking the scilab tools, on both transports of the
real McpSseServer: stateless streamable HTTP (POST /mcp: codex, agy, vibe,
cursor) and legacy SSE (GET /sse + POST /messages: claude).
Logs every request so we can see what an AI CLI sends.

Usage: python fake_mcp_server.py [port] [logfile]
"""
import json, queue, sys, uuid
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

PORT = int(sys.argv[1]) if len(sys.argv) > 1 else 8765
LOG = open(sys.argv[2] if len(sys.argv) > 2 else "fake_mcp.log", "a", encoding="utf-8")


def T(name, desc, props, req):
    return {"name": name, "description": desc,
            "inputSchema": {"type": "object", "properties": {k: {"type": v} for k, v in props.items()}, "required": req}}

TOOLS = [
    T("execute_scilab_code", "Execute Scilab code in the active Scilab session. Use for computations, tests, inspections, plots.", {"code": "string"}, ["code"]),
    T("get_variable", "Get the value of a Scilab variable.", {"name": "string"}, ["name"]),
    T("list_variables", "List the user variables in the Scilab workspace with their type and size.", {}, []),
    T("last_error", "Get the last error message recorded by the Scilab interpreter.", {}, []),
    T("get_figure", "Capture a Scilab graphic figure as a PNG image (mode export or screen).", {"figure_id": "integer", "mode": "string"}, []),
    T("scilab_help", "Get full documentation for a Scilab function.", {"function": "string"}, ["function"]),
    T("scilab_help_search", "Search Scilab documentation by keyword.", {"query": "string", "limit": "integer"}, ["query"]),
    T("load_skill", "Load the full body of a skill listed in your system prompt.", {"name": "string"}, ["name"]),
    T("get_editor_content", "Get the content of the editor file you are working on (or the open tab given by path).", {"path": "string"}, []),
    T("edit_editor_text", "Replace an exact text fragment in the editor file you are working on.", {"old_string": "string", "new_string": "string", "replace_all": "boolean", "path": "string"}, ["old_string", "new_string"]),
    T("set_editor_content", "Replace the entire content of the editor file you are working on.", {"content": "string", "path": "string"}, ["content"]),
    T("replace_editor_selection", "Replace the selected text in SciNotes editor.", {"content": "string"}, ["content"]),
    T("open_file_in_editor", "Open a file in a new SciNotes editor tab.", {"path": "string", "line": "integer"}, ["path"]),
]

SESSIONS = {}


def respond(msg):
    """JSON-RPC answer to one message, None for a notification."""
    mid = msg.get("id")
    method = msg.get("method")
    if method == "initialize":
        ver = msg.get("params", {}).get("protocolVersion", "2025-03-26")
        return {"jsonrpc": "2.0", "id": mid, "result": {
            "protocolVersion": ver, "capabilities": {"tools": {}},
            "serverInfo": {"name": "scilab-assistant", "version": "1.0.0"}}}
    if method == "tools/list":
        return {"jsonrpc": "2.0", "id": mid, "result": {"tools": TOOLS}}
    if method == "tools/call":
        name = msg.get("params", {}).get("name")
        text = f"OK ({name})"
        if name == "execute_scilab_code":
            text = "   42.\n"
        elif name == "get_editor_content":
            text = "File: C:/work/demo.sce\n\nx = 1:10;\ny = x.^2;\nplot(x, y)\n"
        return {"jsonrpc": "2.0", "id": mid, "result": {"content": [{"type": "text", "text": text}]}}
    if method == "ping":
        return {"jsonrpc": "2.0", "id": mid, "result": {}}
    if mid is None:
        return None
    return {"jsonrpc": "2.0", "id": mid, "error": {"code": -32601, "message": "method not found: " + str(method)}}


class H(BaseHTTPRequestHandler):
    def log_message(self, *a):
        pass

    def _send(self, code, body=None):
        self.send_response(code)
        self.send_header("Access-Control-Allow-Origin", "*")
        if body is not None:
            data = json.dumps(body).encode()
            self.send_header("Content-Type", "application/json")
            self.send_header("Content-Length", str(len(data)))
            self.end_headers()
            self.wfile.write(data)
        else:
            self.send_header("Content-Length", "0")
            self.end_headers()

    def do_GET(self):
        LOG.write(f"GET {self.path} {dict(self.headers)}\n"); LOG.flush()
        if not self.path.startswith("/sse"):
            self._send(405)
            return
        # Legacy SSE transport: announce the POST endpoint, then push the
        # answers of the messages posted there
        sid = uuid.uuid4().hex
        q = SESSIONS[sid] = queue.Queue()
        self.send_response(200)
        self.send_header("Content-Type", "text/event-stream")
        self.send_header("Cache-Control", "no-cache")
        self.end_headers()
        try:
            self.wfile.write(f"event: endpoint\ndata: /messages?sessionId={sid}\n\n".encode())
            self.wfile.flush()
            while True:
                try:
                    reply = q.get(timeout=15)
                    self.wfile.write(f"event: message\ndata: {json.dumps(reply)}\n\n".encode())
                except queue.Empty:
                    self.wfile.write(b": keepalive\n\n")
                self.wfile.flush()
        except (BrokenPipeError, ConnectionResetError, ConnectionAbortedError):
            SESSIONS.pop(sid, None)

    def do_DELETE(self):
        self._send(200)

    def do_POST(self):
        n = int(self.headers.get("Content-Length", 0))
        raw = self.rfile.read(n).decode()
        LOG.write(f"POST {self.path} accept={self.headers.get('Accept')} body={raw}\n"); LOG.flush()
        try:
            msg = json.loads(raw)
        except Exception:
            self._send(400, {"error": "bad json"}); return
        reply = respond(msg)
        if self.path.startswith("/messages"):
            sid = self.path.split("sessionId=")[-1]
            if reply is not None and sid in SESSIONS:
                SESSIONS[sid].put(reply)
            self._send(202)
        elif reply is None:
            self._send(202)
        else:
            self._send(200, reply)


print(f"fake MCP on http://127.0.0.1:{PORT}/mcp and /sse", flush=True)
ThreadingHTTPServer(("127.0.0.1", PORT), H).serve_forever()
