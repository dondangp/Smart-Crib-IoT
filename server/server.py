from http.server import BaseHTTPRequestHandler, HTTPServer
import threading
import json

PORT = 65432

# smartlight data
brightness = 0

# thermal data
fanSpeed = 0
temp = 0
humidity = 0

# smartlock data
doorlock = 0  # 0 means door is unlocked, 1 means door is locked
newPassword = 1234

lock = threading.Lock()

class SimpleHTTPRequestHandler(BaseHTTPRequestHandler):

    # I believe this is where the request is sent 
    def _send_response(self, message, status_code=200):
        self.send_response(status_code)
        self.send_header('Content-type', 'application/json')
        self.end_headers()
        self.wfile.write(json.dumps(message).encode())

    # I believe this is where the request is sent
    def do_POST(self):
        global brightness, fanSpeed, temp, humidity, doorlock, newPassword  # Make sure to declare global variables
        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length).decode()

        try:
            # first, get the data as from json 
            data = json.loads(post_data)
            print("incoming json data")
            print(data)

            # identify who made the request, device or controller
            id = int(data['id'])

            if id == 0:  #  CONTROLER REQUEST
                # find what device controller is aiming to control
                device = int(data['device'])
                if device == 1:  # if target device is the LightDevice adjust brightness
                    with lock:
                        brightness = int(data["brightness"]) #user updates brightness
                        response = {"brightness": brightness}# respond with updated brightness
                elif device == 2:  # if target device is the thermal device
                    with lock:
                        fanSpeed = int(data["fanSpeed"]) #user update   
                        # temp = int(data["temp"])         #device update
                        # humidity = int(data["humidity"])    #device update
                        response = {"fanSpeed": fanSpeed,   #respond with updated fan speed
                                    "temp": temp,           #respond with current temp
                                    "humidity": humidity}   #respond with current humidity
                elif device == 3:  # if target device is the smartlock device
                    with lock:
                        doorlock = int(data["doorlock"])    # user update
                        response = {"doorlock": doorlock}   # respond with updated lock state
                elif device == 31: # if target device is smart lock device 
                    with lock:
                        newPassword = int(data["newPassword"])
                        response = {}

                else:
                    self._send_response({"error": "Invalid target"}, 400)
                    return

            elif id == 1:  # DEVICE REQUEST
                
                device = int(data['device'])    #what device made request

                if device == 1:
                    with lock:
                        # device wont update any data
                        response = {"brightness": brightness}   # respond with updated data
                elif device == 2:
                    with lock:
                        #device wont update fanspeed
                        temp = int(data['temp'])    # device must update temp
                        humidity = int(data['humidity'])    # device must update humidity
                        response = {"fanSpeed": fanSpeed, 
                                    "temp": temp,
                                    "humidity": humidity}   # respond with updated data
                elif device == 3:
                    with lock:
                        # device wont update data
                        response = {"doorlock": doorlock,
                                    "newPassword": newPassword}   #respond with updated data
                else:
                    self._send_response({"error": "Invalid device"}, 400)
                    return
            else:
                self._send_response({"error": "Invalid id"}, 400)
                return

            print("outgoing json data")
            print(response)
            self._send_response(response)
        except (ValueError, KeyError):
            self._send_response({"error": "Invalid data"}, 400)


def start_server():
    server = HTTPServer(('0.0.0.0', PORT), SimpleHTTPRequestHandler)
    print(f"HTTP Server is listening on port {PORT}...")
    server.serve_forever()

# starts the program overall
if __name__ == "__main__":
    start_server()



