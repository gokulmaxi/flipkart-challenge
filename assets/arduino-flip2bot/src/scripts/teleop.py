#forward -------homekey
#backward-------endkey
#left-----------insert key
#right----------page_up key
#angular_left---delete key
#angular_right--page_down

import paho.mqtt.client as mqttClient
import time
from pynput import keyboard
import json

 
def on_connect(client, userdata, flags, rc):
 
    if rc == 0:
 
        print("Connected to broker")
 
        global Connected                #Use global variable
        Connected = True                #Signal connection 
 
    else:
 
        print("Connection failed")
 
Connected = False   #global variable for the state of the connection
 
broker_address= "192.168.0.116"
# broker_address= "192.168.0.163"

port = 1883

 
client = mqttClient.Client("Python")               #create new instance
client.on_connect= on_connect                      #attach function to callback
client.connect(broker_address, port=port)          #connect to broker
 
client.loop_start()        #start the loop
 

def on_press(key):
    try:
        # MQTT_MSG=json.dumps({"pwm1": "0","pwm2":  "0","pwm3": "0","pwm4":  "0"})
        if key == key.delete:
            MQTT_MSG=json.dumps({"linear_x":"1"})
            print("Backward") 

        if key == key.insert:
            MQTT_MSG=json.dumps({"angular":"1","linear_y":  "1"})
        if key == key.page_up:
            MQTT_MSG=json.dumps({"angular":"-1","linear_y":  "1"})
        
        if key == key.end:
            MQTT_MSG=json.dumps({"linear_y":"1"})
            print("Left")
        if key == key.home:
            MQTT_MSG=json.dumps({"linear_y": "-1"})
            print("right")
        if key == key.page_down:
            MQTT_MSG=json.dumps({"linear_x":  "-1"})
            print("up")
        if key == key.shift_r:
            MQTT_MSG=json.dumps({"servo":"1"})
            print("left turn")
        if key == key.shift:
            MQTT_MSG=json.dumps({"servo":"-1"})
            print("left turn")
        if key == key.ctrl:
            MQTT_MSG=json.dumps({"colorRequest":"1"})
            print("Servo")
        else:
            print("False")
            
        client.publish("flipkart/bot1", MQTT_MSG)
    except AttributeError:
        print('special key pressed: {0}'.format(
            key))

def on_release(key):
    print('Key released: {0}'.format(
        key))
    if key == keyboard.Key.esc:
        # Stop listener
        return False

# Collect events until released
with keyboard.Listener(
        on_press=on_press,
        on_release=on_release) as listener:
    listener.join()

while Connected != True:    #Wait for connection
    time.sleep(0.1)
 
# try:
#     while True:
#         value = input('Enter the message:')
#         client.publish("flipkart/bot1",value)
 
# except KeyboardInterrupt:
                           
#     client.disconnect()
#     client.loop_stop()
