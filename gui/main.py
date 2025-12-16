from PyQt5 import QtWidgets
from ui_generated import Ui_Form
from serial_worker import SerialWorker
from ui_main import RadarUI
import sys

"""
Main entry point for the STM32 Reversing Radar GUI application.

- Initializes the PyQt5 application.
- Sets up the UI from Qt Designer.
- Starts the SerialWorker thread to read radar distances.
- Updates the GUI in real-time based on received distances.
"""

# --- Create the PyQt5 application ---
app = QtWidgets.QApplication([])

# --- Set up the main form and UI ---
Form = QtWidgets.QWidget()
ui = Ui_Form()
ui.setupUi(Form)

# --- Initialize RadarUI to manage the display logic ---
radar_ui = RadarUI(ui)

# --- Initialize and start the serial reading thread ---
serial_thread = SerialWorker(port="COM8")
# Connect the distance signal to the GUI update method
serial_thread.distance_received.connect(radar_ui.update_display)
serial_thread.start()

# --- Show the main window ---
Form.show()

# --- Start the Qt event loop ---
exit_code = app.exec_()

# --- Stop the serial thread and wait for it to finish cleanly ---
serial_thread.stop()
serial_thread.wait()

# --- Exit the application with the correct code ---
sys.exit(exit_code)
