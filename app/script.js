
// Get all the elements
let ceiling_light_1 = document.getElementById('ceiling_light_1');
let ceiling_light_2 = document.getElementById('ceiling_light_2');

let strip_light_1 = document.getElementById('strip_light_1');
let strip_light_2 = document.getElementById('strip_light_2');

let fan_1 = document.getElementById('fan_1');
let fan_2 = document.getElementById('fan_2');
let fan_3 = document.getElementById('fan_3');
let fan_4 = document.getElementById('fan_4');
let fan_5 = document.getElementById('fan_5');
let fan_6 = document.getElementById('fan_6');


// Connect to the server
let ws = new WebSocket('wss://smart-inovus-server.glitch.me/');


// When the connection is open
ws.onopen = () => {
    console.log('Connected to the server');
};


// When we receive a message from the server
ws.onmessage = (event) => {
    let circuit = JSON.parse(event.data);
    
    ceiling_light_1.innerHTML = `${circuit.ceiling_light_1 ? 'ON' : 'OFF'}`;
    ceiling_light_2.innerHTML = `${circuit.ceiling_light_2 ? 'ON' : 'OFF'}`;
    strip_light_1.innerHTML = `${circuit.strip_light_1 ? 'ON' : 'OFF'}`;
    strip_light_2.innerHTML = `${circuit.strip_light_2 ? 'ON' : 'OFF'}`;
    fan_1.innerHTML = `${circuit.fan_1 ? 'ON' : 'OFF'}`;
    fan_2.innerHTML = `${circuit.fan_2 ? 'ON' : 'OFF'}`;
    fan_3.innerHTML = `${circuit.fan_3 ? 'ON' : 'OFF'}`;
    fan_4.innerHTML = `${circuit.fan_4 ? 'ON' : 'OFF'}`;
    fan_5.innerHTML = `${circuit.fan_5 ? 'ON' : 'OFF'}`;
    fan_6.innerHTML = `${circuit.fan_6 ? 'ON' : 'OFF'}`;

};


// trigger the toggle action on the server
let toggleState = (circuit) => {
    ws.send(JSON.stringify({ action: 'toggle', circuit }));
}
