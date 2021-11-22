import serial

def auto_connect() -> serial.Serial:
    import serial.tools.list_ports
    for port in serial.tools.list_ports.comports():
        if port.manufacturer and "arduino" in port.manufacturer.lower() or 'AI06BDLX' in port.device.upper():
            return serial.Serial(port=port.device, baudrate=9600, timeout=0)
    raise RuntimeError("No Arduino device available")
