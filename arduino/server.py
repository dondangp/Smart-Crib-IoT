from http.server import BaseHTTPRequestHandler, HTTPServer
import threading
import json

PORT = 65432

#smartlight data 
brightness = 0

#thermal data
fanSpeed = 1
temp = 0
humidity = 0

#smartlock data
lock = 0 # 0 means door is unlocked 1 means door is locked



lock = threading.Lock()

class SimpleHTTPRequestHandler(BaseHTTPRequestHandler):

    # I believe this is where the request is sent 
    def _send_response(self, message, status_code=200):
        self.send_response(status_code)
        self.send_header('Content-type', 'application/json')
        self.end_headers()
        self.wfile.write(json.dumps(message).encode())


    # I belive this is where the request is sent
    def do_POST(self):
        # global lightLevel
        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length).decode()

        try:
            # first get the data as from json 
            data = json.loads(post_data)
            print(data)

            # identify who made the request device or controller
            id = int(data['id'])
            print('control connected id = %d' % (id))


            if id == 0: # if this is a controler request
                
                # find what device controler is aiming to control
                target = int(data['target'])
                print('target: %d' % (target))
                if target == 1: # if target device is the LightDevice adjust brightness
                    
                    
                    # update brightness 
                    # respond with updated brightness level
                    with lock:
                        
                        brightness = int(data['brightness'])
                        print('brightness: %d' % (brightness))
                        response = {"brightness": brightness}
                elif target == 2: #if target device is thermal device
                    with lock:
                        fanSpeed = int(data["fanSpeed"])
                        temp = int(data["temp"])
                        humidity = int(data["humidity"])
                        print('fanSpeed: %d' % (fanSpeed))
                        response = {"fanSpeed": fanSpeed}
                elif target == 3: # if target device is smartlock device 
                    with lock:
                        lock = int(data["lock"])
                        response = {"lock": lock}
                        
                    

                        
            elif id == 1: # if this is a device request
                
                # find out what device this is aka what target device
                device = int(data['target'])

                if device == 1:
                    # respond with updated lightlevel
                    with lock:
                        response = {"brightness": brightness}
                if device == 2:
                    #respond with User updated fan speed

                    with lock:
                        response = {"fanSpeed": fanSpeed}
                if device == 3:
                    with lock:
                        response = {"lock": lock}
                        
                        
            self._send_response(response)
        except (ValueError, KeyError):
            self.send_response({"error": "Invalid data"}, 400)



# 
def start_server():
    server = HTTPServer(('0.0.0.0', PORT), SimpleHTTPRequestHandler)
    print(f"HTTP Server is listening on port {PORT}...")
    server.serve_forever()



# starts the program overall
if __name__ == "__main__":
    start_server()
