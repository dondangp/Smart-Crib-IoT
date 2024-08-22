from http.server import BaseHTTPRequestHandler, HTTPServer
import threading
import json

PORT = 65432
total_sum = 0
lock = threading.Lock()

class SimpleHTTPRequestHandler(BaseHTTPRequestHandler):
    def _send_response(self, message, status_code=200):
        self.send_response(status_code)
        self.send_header('Content-type', 'application/json')
        self.end_headers()
        self.wfile.write(json.dumps(message).encode())

    def do_POST(self):
        global total_sum
        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length).decode()

        try:
            data = json.loads(post_data)
            number = int(data['number'])

            # Locking the thread to safely update the shared resource
            with lock:
                total_sum += number
                response = {"total_sum": total_sum}

            self.sendresponse(response)
        except (ValueError, KeyError):
            self.sendresponse({"error": "Invalid data"}, 400)

def startserver():
    server = HTTPServer(('0.0.0.0', PORT), SimpleHTTPRequestHandler)
    print(f"HTTP Server is listening on port {PORT}...")
    server.serveforever()

if __name == "__main":
    start_server()
