import time
import dht
import sh1106
from machine import I2C, Pin

# Blink on-board LED
led = machine.Pin("LED", machine.Pin.OUT)

# Pin 4 for data in
sensor = dht.DHT22(Pin(4))

# SCLK is I2C1 GPIO 27. SDA is I2C1 GPIO 26
# The dimensions of the display are 128 x 64
i2c = I2C(1,scl=Pin(27), sda=Pin(26), freq=400000)
oled = sh1106.SH1106_I2C(128, 64, i2c)

def read_sensor():
    try:
        sensor.measure()
        temp = sensor.temperature()
        humidity = sensor.humidity()
        return temp, humidity
    except OSError as e:
        return None, None
    
def display_data(temp, humidity):
    oled.fill(0)
    if temp is not None and humidity is not None:
        oled.text("Temp: {:.1f} C".format(temp),0,0)
        oled.text("Humidity: {:.1f}%".format(humidity),0,10)
    else:
        oled.text("Failed to read", 0, 0)
    oled.show()
    
while True:
    temp, humidity = read_sensor()
    print("Temp: {:.1f} C".format(temp))
    print("Humidity: {:.1f}%".format(humidity))

    # publish_sensor_data(client)
    display_data(temp, humidity)

    led.on()
    time.sleep(1)
    led.off()
    time.sleep(1)
