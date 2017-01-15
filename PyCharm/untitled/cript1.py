import serial
import time

port1 = serial.Serial(port='COM3', baudrate=9600, bytesize=serial.EIGHTBITS, stopbits=serial.STOPBITS_ONE, timeout=None)


while 1:
    try:
        indicator = port1.read()
        if indicator == b'v':
            temp_a = port1.read()
            temp_b = port1.read()
            analog = (temp_b[0] << 8) | temp_a[0]
            print(analog)
            #voltage = (analog/1024 * 5)
            #print('Voltage: {:f}'.format(voltage))

            #x = MyClass()
            #read_voltage()
    except port1.SerialTimeoutException:
        print('Data could not be read')
        time.sleep(1)

#class MyClass:
#    def __init__(self):
#        self.data = []
#
#    def read_voltage(self):
#        data2 = ord(port1.read(1))
#        print('Voltage: {:d}'.format(data2))