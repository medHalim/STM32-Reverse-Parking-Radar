from PyQt5 import QtWidgets
from distance_logic import get_zone

class RadarUI(QtWidgets.QWidget):
    """
    PyQt5 Widget class to manage the Reversing Radar GUI display.

    Attributes:
        ui (object): The UI object generated from Qt Designer (Ui_Form), containing widgets like
                     lcdNumber and color indicators.
    """

    def __init__(self, ui):
        """
        Initialize the RadarUI widget.

        Args:
            ui (object): The UI object containing the radar GUI elements.
        """
        super().__init__()
        self.ui = ui

    def update_display(self, dist):
        """
        Update the radar GUI based on the current distance measurement.

        - Determines which color zone the distance falls into using `get_zone`.
        - Updates the LCD display.
        - Shows or hides red, yellow, and green indicators accordingly.

        Args:
            dist (float): The distance measured by the radar (in meters).
        """
        # Determine the zone for the given distance
        zone = get_zone(dist)

        # Show the LCD display if the zone is not "NONE"
        self.ui.lcdNumber.setVisible(zone != "NONE")
        self.ui.lcdNumber.display(dist)

        def set_group(group, state):
            """
            Helper function to show or hide a group of QLabel widgets.

            Args:
                group (list): List of QLabel widgets to update.
                state (bool): True to show the widgets, False to hide.
            """
            for item in group:
                item.setVisible(state)

        # Group widgets by color for easy management
        red = [
            self.ui.redLeft1, self.ui.redLeft2,
            self.ui.redRight1, self.ui.redRight2
        ]
        yellow = [
            self.ui.yellowLeft1, self.ui.yellowLeft2,
            self.ui.yellowRight1, self.ui.yellowRight2
        ]
        green = [
            self.ui.greenLeft1, self.ui.greenLeft2,
            self.ui.greenRight1, self.ui.greenRight2
        ]

        # Update visibility for each color group based on the zone
        set_group(red, zone in ["RED_FULL", "RED"])
        set_group(yellow, zone in ["RED_FULL", "RED", "YELLOW_FULL", "YELLOW"])
        set_group(green, zone != "NONE")
