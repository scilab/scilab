var L = console.log;

var express = require('express');
var app = express();
var server = require('http').Server(app);
var io = require('socket.io')(server);

var client = require('socket.io-client');

var fork = require('child_process').fork;


server.listen(1337);

var script = '/loader';

app.use('/favicon.ico', express.static('./favicon.ico'));
app.get("/favicon.ico", function(req, res) {
});

app.get("*", function(req, res) {
    res.sendFile(__dirname + '/html/index.html');
    if(req.url != "/") {
        script = req.url;
    } else {
        script = '/loader';
    }
});

io.on('connection', function (socket) {
    //start process
    var child = fork('./scilab_process.js');
    //in future version, it must be a docker so we can't use 'internal' pipe
    //so we need to connect a socket, another ...
    var prcAddr = 'http://127.0.0.1:10002';
    var prcSocket = require('socket.io-client')(prcAddr);
    prcSocket.on('connect', function() {
        L('Dispatcher connected');

        prcSocket.on('command_end', function() {
            socket.emit('command_end');
        });
        
        prcSocket.on('graphic_create', function(msg) {
            socket.emit('graphic_create', msg);
        });
        
        prcSocket.on('graphic_delete', function(msg) {
            socket.emit('graphic_delete', msg);
        });
        
        prcSocket.on('graphic_update', function(msg) {
            socket.emit('graphic_update', msg);
        });
        
        prcSocket.on('status', function(msg) {
            if(msg.data == 'ready') {
                //send ready message to client
                socket.emit('status', msg);
                //send execution of initial script to scilab
                prcSocket.emit('command', {data:"exec(getenv('SCIFILES') + '" + script + ".sce', -1);"});
            }
        });
    });
    
    //receive command from client
    socket.on('command', function (msg) {
        prcSocket.emit('command', msg);
    });
    
    socket.on('callback', function (msg) {
        prcSocket.emit('callback', msg);
    });
    
    socket.on('disconnect', function () {
        L('disconnect');
        prcSocket.emit('quit');
        prcSocket.close();
    });

});

L('Server running on port '+server.address().port);
