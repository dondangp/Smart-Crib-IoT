import requests

def start_client():

    server_url = 'http://13.88.157.6:65432'  # Replace with your server's public IP address and port

    while True:
       
        id = int(input("id ( 0 for controler 1 for device): "))            
        device = int(input("devices 1 = light, 2 = fan, 3 = lock: "))
        data = None
        
        if id == 0: # if we want to act as a controler
            if device == 1: # grab brightness from input
                brightness = int(input("brightness (0 - 255): "))
                data = {"id": id,   # let them know we are a controler
                        "device": device,   #let em know what device we want to control
                        "brightness": brightness}   # send brightness update
            elif device == 2:   # grab fanspeed from input
                fanSpeed = int(input("fanspeed (0-100): "))
                data = {"id": id,
                        "device": device,
                        "fanSpeed": fanSpeed} # send fanspeed update
            elif device == 3:   # grab doorlock from input
                doorlock = int(input("O for unlock 1 for lock: "))
                data = {"id": id,
                        "device": device,
                        "doorlock": doorlock}   # send updated doorlock status
                
        if id == 1: # if we want to act as a device
            if device == 1: 
                # device wont update anything
                data = {"id": id,   # let them know we are a device
                        "device": device}   #let em know what device we are
            elif device == 2:   # grab fanspeed from input
                # dont update fanspeed
                temp = int(input("Current temp (0-100): "))
                humidity = int(input("Current humidity (0 - 100): "))
                data = {"id": id,
                        "device": device,
                        "temp": temp,         # send temp update
                        "humidity": humidity} # send humidity update
            elif device == 3:   
                # device wont update anything
                data = {"id": id,
                        "device": device}
        
        try:    #try with input values
               
            
            print("outgoing json data")
            print(data)
            
            response = requests.post(server_url, json=data)

            if response.status_code == 200:
                print("incoming json data")
                print(response.text)
            else:
                print(f"Server error: {response.json().get('error', 'Unknown error')}")

        except ValueError:
            print("Please enter a valid number.")
        except requests.exceptions.RequestException as e:
            print(f"Failed to connect to the server: {e}")

if __name__ == "__main__":

    start_client()
