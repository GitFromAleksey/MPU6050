import serial


ports = ['COM%s'%(i+1) for i in range(256)]
baud_rates = ['1200','2400', '4800', '9600', '19200', '38400', '57600', '115200']

def GetAllPorts():
    avaliable_result = []
    for port in ports:
        message = ''
        try:
            message = str(port)
            s = serial.Serial(port)
            s.close()
            avaliable_result.append(port)
            message += ' - avaliable'
        except (OSError, serial.SerialException):
            message += ' - not avaliable'

    return avaliable_result
##    print(message)

##print(GetAllPorts())

def PortOpen(port_name):
    port = serial.Serial(
        port=port_name,
        baudrate=9600,
        parity=serial.PARITY_ODD,
        stopbits=serial.STOPBITS_TWO,
        bytesize=serial.SEVENBITS)
    return port


def main():
    print('main()')
##    print(GetAllPorts())
    port = PortOpen('COM4')
    print(port.isOpen())


if __name__ == '__main__':
    main()
