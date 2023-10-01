
let WebSocket = require('ws');
let http = require('http');
let server = http.createServer();
let wss = new WebSocket.Server({ server });

let circuit = {
    ceiling_light_1: false,
    ceiling_light_2: false,
    strip_light_1: false,
    strip_light_2: false,
    fan_1: false,
    fan_2: false,
    fan_3: false,
    fan_4: false,
    fan_5: false,
    fan_6: false,   
};

// print the details of client handshake
wss.on('headers', (headers, req) => {
    console.log(headers);
});

wss.on('connection', (ws) => {
    
    console.log('A new client connected...');
    
    // Send the current circuit status to the newly connected client
    ws.send(JSON.stringify(circuit));

    ws.on('message', (message) => {
        try {

            let data = JSON.parse(message);
            console.log('Received:', data);

            if (data.action === 'toggle') {
                circuit[data.circuit] = !circuit[data.circuit];

                // Broadcast the new circuit status to all connected clients
                wss.clients.forEach((client) => {
                    if (client.readyState === WebSocket.OPEN) {
                        client.send(JSON.stringify(circuit));
                    }
                });
            }

        } catch (error) {
            console.error('Error processing message:', error);
        }
    });

    // When the client closes the connection
    ws.on('close', () => {
        console.log('Someone disconnected...');
    });
});

server.listen(3000, () => {
    console.log(`Server started on port 3000`);
});
