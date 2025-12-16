from PyQt5.QtCore import QThread, pyqtSignal
import serial

class SerialWorker(QThread):
    """
    QThread subclass to handle asynchronous reading of radar distance data from a serial port.

    Signals:
        distance_received (float): Emitted whenever a valid distance reading is received.
    """

    distance_received = pyqtSignal(float)

    def __init__(self, port="COM8", baudrate=115200):
        """
        Initialize the SerialWorker thread.

        Args:
            port (str): Serial port to connect to (default "COM8").
            baudrate (int): Baud rate for the serial connection (default 115200).
        """
        super().__init__()
        self.port = port
        self.baudrate = baudrate
        self.running = True  # Flag to control thread execution

    def run(self):
        """
        Main thread execution method.

        - Opens the serial port.
        - Continuously reads lines from the serial buffer.
        - Converts lines to float and emits `distance_received`.
        - Ignores lines that cannot be converted to float.
        """
        try:
            ser = serial.Serial(self.port, self.baudrate, timeout=1)
        except serial.SerialException:
            # Serial port could not be opened; exit thread
            return

        while self.running:
            if ser.in_waiting:
                try:
                    # Read a line, decode, strip whitespace, convert to float
                    dist = float(ser.readline().decode().strip())
                    # Emit the distance to connected slots
                    self.distance_received.emit(dist)
                except ValueError:
                    # Ignore invalid lines that cannot be converted to float
                    pass

        # Close the serial port when the thread stops
        ser.close()

    def stop(self):
        """
        Stop the serial reading thread.

        Sets `running` to False, which causes the `run` loop to exit.
        """
        self.running = False
