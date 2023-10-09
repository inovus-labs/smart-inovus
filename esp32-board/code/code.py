import uwebsocket
import ussl
import network
import machine
import json
import socket
import time

# Define your WiFi credentials
WIFI_SSID = "master"
WIFI_PASS = "123456789"

# WebSocket server URL
WS_SERVER = "ws://smart-inovus-server.glitch.me"

# SSL certificate

SSL_CERT = """-----BEGIN CERTIFICATE-----
MIIFYDCCBEigAwIBAgIQQAF3ITfU6UK47naqPGQKtzANBgkqhkiG9w0BAQsFADA/
MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT
DkRTVCBSb290IENBIFgzMB4XDTIxMDEyMDE5MTQwM1oXDTI0MDkzMDE4MTQwM1ow
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwggIiMA0GCSqGSIb3DQEB
AQUAA4ICDwAwggIKAoICAQCt6CRz9BQ385ueK1coHIe+3LffOJCMbjzmV6B493XC
ov71am72AE8o295ohmxEk7axY/0UEmu/H9LqMZshftEzPLpI9d1537O4/xLxIZpL
wYqGcWlKZmZsj348cL+tKSIG8+TA5oCu4kuPt5l+lAOf00eXfJlII1PoOK5PCm+D
LtFJV4yAdLbaL9A4jXsDcCEbdfIwPPqPrt3aY6vrFk/CjhFLfs8L6P+1dy70sntK
4EwSJQxwjQMpoOFTJOwT2e4ZvxCzSow/iaNhUd6shweU9GNx7C7ib1uYgeGJXDR5
bHbvO5BieebbpJovJsXQEOEO3tkQjhb7t/eo98flAgeYjzYIlefiN5YNNnWe+w5y
sR2bvAP5SQXYgd0FtCrWQemsAXaVCg/Y39W9Eh81LygXbNKYwagJZHduRze6zqxZ
Xmidf3LWicUGQSk+WT7dJvUkyRGnWqNMQB9GoZm1pzpRboY7nn1ypxIFeFntPlF4
FQsDj43QLwWyPntKHEtzBRL8xurgUBN8Q5N0s8p0544fAQjQMNRbcTa0B7rBMDBc
SLeCO5imfWCKoqMpgsy6vYMEG6KDA0Gh1gXxG8K28Kh8hjtGqEgqiNx2mna/H2ql
PRmP6zjzZN7IKw0KKP/32+IVQtQi0Cdd4Xn+GOdwiK1O5tmLOsbdJ1Fu/7xk9TND
TwIDAQABo4IBRjCCAUIwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAQYw
SwYIKwYBBQUHAQEEPzA9MDsGCCsGAQUFBzAChi9odHRwOi8vYXBwcy5pZGVudHJ1
c3QuY29tL3Jvb3RzL2RzdHJvb3RjYXgzLnA3YzAfBgNVHSMEGDAWgBTEp7Gkeyxx
+tvhS5B1/8QVYIWJEDBUBgNVHSAETTBLMAgGBmeBDAECATA/BgsrBgEEAYLfEwEB
ATAwMC4GCCsGAQUFBwIBFiJodHRwOi8vY3BzLnJvb3QteDEubGV0c2VuY3J5cHQu
b3JnMDwGA1UdHwQ1MDMwMaAvoC2GK2h0dHA6Ly9jcmwuaWRlbnRydXN0LmNvbS9E
U1RST09UQ0FYM0NSTC5jcmwwHQYDVR0OBBYEFHm0WeZ7tuXkAXOACIjIGlj26Ztu
MA0GCSqGSIb3DQEBCwUAA4IBAQAKcwBslm7/DlLQrt2M51oGrS+o44+/yQoDFVDC
5WxCu2+b9LRPwkSICHXM6webFGJueN7sJ7o5XPWioW5WlHAQU7G75K/QosMrAdSW
9MUgNTP52GE24HGNtLi1qoJFlcDyqSMo59ahy2cI2qBDLKobkx/J3vWraV0T9VuG
WCLKTVXkcGdtwlfFRjlBz4pYg1htmf5X6DYO8A4jqv2Il9DjXA6USbW1FzXSLr9O
he8Y4IWS6wY7bCkjCWDcRQJMEhg76fsO3txE+FiYruq9RUWhiF1myv4Q6W+CyBFC
Dfvp7OOGAN6dEOM4+qR9sdjoSYKEBpsr6GtPAQw4dy753ec5
-----END CERTIFICATE-----
"""
# Define GPIO pins for controlling circuits
CIRCUITS = {
    "ceiling_light_1": 4,
    "ceiling_light_2": 5,
    "strip_light_1": 6,
    "strip_light_2": 7,
    "fan_1": 8,
    "fan_2": 9,
    "fan_3": 10,
    "fan_4": 11,
    "fan_5": 12,
    "fan_6": 13
}

# Initialize WiFi
wifi = network.WLAN(network.STA_IF)
wifi.active(True)

# Try to connect to WiFi
if not wifi.isconnected():
    print("Connecting to WiFi...")
    wifi.connect(WIFI_SSID, WIFI_PASS)
    while not wifi.isconnected():
        time.sleep(1)

print("WiFi connected.")

# Create a WebSocket secure connection with SSL certificate
try:
    import ssl  # Import the ssl module
    ssl_sock = ssl.wrap_socket(socket.socket())
    ssl_sock.connect(socket.getaddrinfo(WS_SERVER, 443)[0][-1])
    ws = uwebsocket.websocket(ssl_sock, WS_SERVER)
    print("WebSocket connected.")

except Exception as e:
    print("Error establishing WebSocket connection:", e)
    ws = None  # Set ws to None to indicate a failed connection

MAX_RETRIES = 3  # Maximum number of connection retry attempts

for retry_count in range(MAX_RETRIES):
    try:
        import ssl
        ssl_sock = ssl.wrap_socket(socket.socket())
        ssl_sock.connect(socket.getaddrinfo(WS_SERVER, 443)[0][-1])
        ws = uwebsocket.websocket(ssl_sock, WS_SERVER)
        print("WebSocket connected.")
        break  # Successfully connected, exit retry loop

    except Exception as e:
        print(f"Error establishing WebSocket connection (Attempt {retry_count + 1}):", e)
        ws = None  # Set ws to None to indicate a failed connection
        time.sleep(5)  # Wait for a few seconds before retrying
        
        
        
        
        
        3
# Main loop
while True:
    if ws:
        try:
            data = ws.recv()
            if data:
                # Parse received JSON data
                try:
                    data_json = json.loads(data)
                    print("Received JSON data:", data_json)

                    # Iterate through circuits and control them based on JSON data
                    for circuit, state in data_json.items():
                        if circuit in CIRCUITS:
                            pin = machine.Pin(CIRCUITS[circuit], machine.Pin.OUT)
                            pin.value(state)

                except Exception as e:
                    print("Error parsing JSON:", e)

        except Exception as e:
            print("An error occurred during WebSocket communication:", e)
            # You can add further error handling or exit the program if necessary
    else:
        print("WebSocket is not connected. Retrying...")
        # You can add reconnection logic here if needed, such as waiting and retrying.
        time.sleep(5)  # Wait for 5 seconds before attempting to reconnect

