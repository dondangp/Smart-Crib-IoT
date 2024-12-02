import time
import network
import dht
import sh1106
import uasyncio as asyncio
from machine import I2C, Pin
import urequests as requests  # Use `urequests` for MicroPython

# Blink on-board LED
led = Pin("LED", Pin.OUT)

# Pin 2 for DHT22 sensor data
sensor = dht.DHT22(Pin(2))

# GPIO 19, 20, 21 for the fan relay speed low, medium, high
relay_low = Pin(19, Pin.OUT)
relay_medium = Pin(20, Pin.OUT)
relay_high = Pin(21, Pin.OUT)

# SCLK is GPIO 9, SDA is GPIO 8 for I2C
# The dimensions of the display are 128 x 64
i2c = I2C(0, scl=Pin(9), sda=Pin(8), freq=400000)
oled = sh1106.SH1106_I2C(128, 64, i2c)

# Rotate the screen by 180 degrees
oled.rotate(True)

# Wi-Fi connection setup
ssid = 'Z'
password = 'Password2024'

wlan = network.WLAN(network.STA_IF)
wlan.active(True)
wlan.connect(ssid, password)

while not wlan.isconnected():
    print('Connecting to Wi-Fi...')
    time.sleep(1)

print('Wi-Fi connected')
print('Network config:', wlan.ifconfig())

# Function to read sensor data
def read_sensor():
    try:
        sensor.measure()
        temp = sensor.temperature()
        # Convert to Fahrenheit
        temp = temp * (9 / 5) + 32
        humidity = sensor.humidity()
        return temp, humidity
    except OSError as e:
        print("Sensor error:", e)
        return None, None

# Function to display data on OLED
def display_data(temp, humidity):
    oled.fill(0)
    if temp is not None and humidity is not None:
        oled.text("Temp: {:.1f} F".format(temp), 0, 0)
        oled.text("Humidity: {:.1f}%".format(humidity), 0, 10)
    else:
        oled.text("Failed to read", 0, 0)
    oled.show()

# Function to control fan speed
def control_fan_speed(fan_speed):
    relay_low.off()
    relay_medium.off()
    relay_high.off()
    if fan_speed == 1:
        relay_low.on()
        print("Fan set to LOW")
    elif fan_speed == 2:
        relay_medium.on()
        print("Fan set to MEDIUM")
    elif fan_speed == 3:
        relay_high.on()
        print("Fan set to HIGH")
    elif fan_speed == 0:
        print("Fan is OFF")

# Async task to read, display, and send data to the server
async def read_and_send_data():
    server_url = 'http://13.88.157.6:65432'  # Server's public IP address and port
    
    while True:
        temp, humidity = read_sensor()

        # Display on OLED
        display_data(temp, humidity)

        # Send data to server
        if temp is not None and humidity is not None:
            data = {
                "id": 1,  # Device ID
                "device": 2,  # Device type 2 for smart fan
                "temp": temp,
                "humidity": humidity
            }
            
            try:
                response = requests.post(server_url, json=data)
                if response.status_code == 200:
                    fan_speed = int(response.json().get("fanSpeed", 0))
                    control_fan_speed(fan_speed)
                else:
                    print(f"Server error: {response.status_code}")
                response.close()
            except Exception as e:  # Generic exception for MicroPython compatibility
                print("Failed to connect to the server:", e)
        
        led.on()
        await asyncio.sleep(1)
        led.off()
        await asyncio.sleep(1)

# Main function to run the async task
async def main():
    task = asyncio.create_task(read_and_send_data())
    await asyncio.gather(task)

# Run the main function
try:
    asyncio.run(main())
except KeyboardInterrupt:
    print("Program stopped.")

