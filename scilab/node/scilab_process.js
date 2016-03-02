var spawn = require('child_process').spawn;

var L = console.log;

var debug = false;
var command_ready = false;
var graphic_ready = false;

var cmdPort = 10000 + process.pid;
var grpPort = 10001 + process.pid;
var dspPort = 10002;

//start server to chat with dispatcher
var dispatchio = require('socket.io')(dspPort);
dispatchio.on('connection', function (dspSocket) {
    
    dspSocket.on('command', function (msg) {
        L('command (' + process.pid + ') : ' + msg.data);
        commandio.emit('command', msg);
    });

    dspSocket.on('callback', function (msg) {
        graphicio.emit('callback', msg);
    });

    dspSocket.on('quit', function () {
        L('quit' + '(' + process.pid + ')');
        commandio.emit('command', {data:'quit'});
        commandio.close();
        graphicio.close();
        process.exit(0);
    });
    
    //start command server to chat with Scilab
    var commandio = require('socket.io')(cmdPort);
    commandio.on('connection', function (socket) {
        L('Scilab command connected');

        socket.on('command_end', function () {
            L('command_end');
            dspSocket.emit('command_end');
        });

        socket.on('disconnect', function () {
            L('scilab command disconnected'+ '(' + process.pid + ')');
            command_ready = false;
        });

        //send to server scilab is ready
        command_ready = true;
        if(graphic_ready) {
            dspSocket.emit('status', {data:'ready'});
        }
    });

    //start graphic server to chat with graphic MVC
    var graphicio = require('socket.io')(grpPort);
    graphicio.on('connection', function (socket) {
        L('Scilab graphic connected');

        socket.on('graphic_create', function (msg) {
            dspSocket.emit('graphic_create', msg);
        });

        socket.on('graphic_delete', function (msg) {
            dspSocket.emit('graphic_delete', msg);
        });

        socket.on('graphic_update', function (msg) {
            dspSocket.emit('graphic_update', msg);
        });

        socket.on('disconnect', function () {
            L('scilab graphic disconnected'+ '(' + process.pid + ')');
            graphic_ready = false;
        });
        
        //send to server scilab is ready
        graphic_ready = true;
        if(command_ready) {
            dspSocket.emit('status', {data:'ready'});
        }
    });
});

//launch scilab with init command
var app;
if(process.platform == "win32") {
    app = process.env.SCIPATH + "/bin/wscilex.exe";
} else {
    app = process.env.SCIPATH + "/bin/scilab";
}

var addr = 'http://127.0.0.1';
var commandaddr = addr + ':' + cmdPort;
var graphicaddr = addr + ':' + grpPort;

var scilabApp = spawn(app, ['-nw', '-commandaddr', commandaddr, '-graphicaddr', graphicaddr]);

//to debug
if(debug) {
/*
    scilabApp.stdout.on('data', function(data) {
        L('scilab out :' + data.toString());
    });
*/                
    scilabApp.stderr.on('data', function(data) {
        L('scilab err :' + data.toString());
    });
}
