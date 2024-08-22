import socket

def startclient():
    clientsocket = socket.socket(socket.AFINET, socket.SOCKSTREAM)
    # Replace with your server's public IP address and port
    client_socket.connect(('52.160.88.33', 65432))

    while True:
        number = input("Enter a number (or type 'exit' to quit): ")
        if number.lower() == 'exit':
            break
        client_socket.sendall(number.encode())
        response = client_socket.recv(1024).decode()
        print(f"Current total sum from server: {response}")

    client_socket.close()

if __name == "__main":
    start_client()
import socket

def startclient():
    clientsocket = socket.socket(socket.AFINET, socket.SOCKSTREAM)
    # Replace with your server's public IP address and port
    client_socket.connect(('52.160.88.33', 65432))

    while True:
        number = input("Enter a number (or type 'exit' to quit): ")
        if number.lower() == 'exit':
            break
        client_socket.sendall(number.encode())
        response = client_socket.recv(1024).decode()
        print(f"Current total sum from server: {response}")

    client_socket.close()

if __name == "__main":
    start_client()
