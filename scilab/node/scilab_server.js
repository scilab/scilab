var fork = require('child_process').fork;
var L = console.log;
var express = require('express');
var app = express();
var server = require('http').Server(app);
var io = require('socket.io')(server);

app.use(express.static('static'));

server.listen(1337);

app.get('/', function (req, res) {
    res.sendFile(__dirname + '/static/index.html');
});

var processAlive = false;

var prcSocket;
io.on('connection', function (socket) {
    //incoming connection from web users
    
    if(processAlive === false) {
        //start process
        fork('./scilab_process.js'/*, [], {execArgv: ['--debug=5859']}*/);
        processAlive = true;
    
        var prcAddr = 'http://127.0.0.1:10002';
        prcSocket = require('socket.io-client')(prcAddr);
        L('dispatcher connected');
    } else {
        //send ready message to client
        socket.emit('status', {data:'reconnection'});
        prcSocket.emit('reconnection');
    }

    prcSocket.emit('imagepath', {path:__dirname + '/static/'});
    
    prcSocket.on('command_end', function() {
        socket.emit('command_end');
    });
        
    prcSocket.on('graphic_create', function(msg) {
        //L(msg);
        socket.emit('graphic_create', msg);
    });
        
    prcSocket.on('graphic_delete', function(msg) {
        socket.emit('graphic_delete', msg);
    });
        
    prcSocket.on('graphic_update', function(msg) {
        socket.emit('graphic_update', msg);
    });
 
    prcSocket.on('graphic_reconnection', function(msg) {
        //L(msg);
        socket.emit('graphic_reconnection', msg);
    });
        
 
    prcSocket.on('status', function(msg) {
        if(msg.data === 'ready') {
            //send ready message to client
            socket.emit('status', msg);
            //send execution of initial script to scilab
            prcSocket.emit('command', {data:"exec(getenv('SCIFILES') + '" + "/start" + ".sce', -1);"});
        }
    });

	prcSocket.on('connection', function(msg) {
        L('dispatcher connected');
    });

	prcSocket.on('reconnect', function(msg) {
        L('dispatcher reconnect');
    });

	prcSocket.on('reconnection', function(msg) {
        L('dispatcher connected');
    });

    prcSocket.on('disconnect', function(msg) {
        L('dispatcher disconnected');
        prcSocket.disconnect();
        processAlive = false;
    });
        
    //receive command from client
    socket.on('command', function (msg) {
        prcSocket.emit('command', msg);
    });
    
    socket.on('callback', function (msg) {
        prcSocket.emit('callback', msg);
    });
    
    socket.on('callback', function (msg) {
        prcSocket.emit('callback', msg);
    });
    
    socket.on('force_reload', function () {
        L('force reload');
		prcSocket.emit('force_reload');
		prcSocket.emit('command', {data:"exec(getenv('SCIFILES') + '" + "/start" + ".sce', -1);"});
    });
});

L('Server running on port '+server.address().port);
