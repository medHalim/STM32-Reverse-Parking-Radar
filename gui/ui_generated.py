from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_Form(object):
    """
    UI class generated from Qt Designer for the Reversing Radar Monitor.

    Contains all widgets for displaying:
    - LCD distance readout
    - Red, Yellow, and Green LED indicators
    """

    def setupUi(self, Form):
        """
        Set up the UI widgets and layout for the radar monitor.

        Args:
            Form (QWidget): The main form/window to which widgets are added.
        """
        Form.setObjectName("Form")
        Form.resize(859, 347)
        Form.setStyleSheet("background-color: rgb(42, 39, 39);")  # Dark background

        # --- LCD Number widget ---
        self.lcdNumber = QtWidgets.QLCDNumber(Form)
        self.lcdNumber.setGeometry(QtCore.QRect(340, 100, 181, 131))
        font = QtGui.QFont()
        font.setKerning(True)
        self.lcdNumber.setFont(font)
        self.lcdNumber.setStyleSheet("color:rgb(228, 7, 25);")  # Red digits
        self.lcdNumber.setFrameShape(QtWidgets.QFrame.NoFrame)
        self.lcdNumber.setFrameShadow(QtWidgets.QFrame.Raised)
        self.lcdNumber.setSmallDecimalPoint(False)
        self.lcdNumber.setDigitCount(3)
        self.lcdNumber.setSegmentStyle(QtWidgets.QLCDNumber.Flat)
        self.lcdNumber.setProperty("value", 8.8)  # Initial value
        self.lcdNumber.setObjectName("lcdNumber")

        # --- RED LEDs ---
        self.redLeft1 = QtWidgets.QLabel(Form)
        self.redLeft1.setGeometry(QtCore.QRect(252, 90, 31, 151))
        self.redLeft1.setPixmap(QtGui.QPixmap("assets/redLeft1.png"))
        self.redLeft1.setObjectName("redLeft1")

        self.redLeft2 = QtWidgets.QLabel(Form)
        self.redLeft2.setGeometry(QtCore.QRect(290, 90, 31, 151))
        self.redLeft2.setPixmap(QtGui.QPixmap("assets/redLeft.png"))
        self.redLeft2.setObjectName("redLeft2")

        self.redRight1 = QtWidgets.QLabel(Form)
        self.redRight1.setGeometry(QtCore.QRect(578, 100, 31, 141))
        self.redRight1.setPixmap(QtGui.QPixmap("assets/redRight1.png"))
        self.redRight1.setObjectName("redRight1")

        self.redRight2 = QtWidgets.QLabel(Form)
        self.redRight2.setGeometry(QtCore.QRect(540, 100, 31, 141))
        self.redRight2.setPixmap(QtGui.QPixmap("assets/redRight.png"))
        self.redRight2.setObjectName("redRight2")

        # --- YELLOW LEDs ---
        self.yellowLeft1 = QtWidgets.QLabel(Form)
        self.yellowLeft1.setGeometry(QtCore.QRect(184, 100, 31, 141))
        self.yellowLeft1.setPixmap(QtGui.QPixmap("assets/yellowLeft1.png"))
        self.yellowLeft1.setObjectName("yellowLeft1")

        self.yellowLeft2 = QtWidgets.QLabel(Form)
        self.yellowLeft2.setGeometry(QtCore.QRect(214, 100, 31, 141))
        self.yellowLeft2.setPixmap(QtGui.QPixmap("assets/yellowLeft2.png"))
        self.yellowLeft2.setObjectName("yellowLeft2")

        self.yellowRight1 = QtWidgets.QLabel(Form)
        self.yellowRight1.setGeometry(QtCore.QRect(652, 100, 31, 141))
        self.yellowRight1.setPixmap(QtGui.QPixmap("assets/yellowRight1.png"))
        self.yellowRight1.setObjectName("yellowRight1")

        self.yellowRight2 = QtWidgets.QLabel(Form)
        self.yellowRight2.setGeometry(QtCore.QRect(615, 100, 31, 141))
        self.yellowRight2.setPixmap(QtGui.QPixmap("assets/yellowRight2.png"))
        self.yellowRight2.setObjectName("yellowRight2")

        # --- GREEN LEDs ---
        self.greenLeft0 = QtWidgets.QLabel(Form)
        self.greenLeft0.setGeometry(QtCore.QRect(131, 101, 16, 141))
        self.greenLeft0.setPixmap(QtGui.QPixmap("assets/greenLeft0.png"))
        self.greenLeft0.setObjectName("greenLeft0")

        self.greenLeft1 = QtWidgets.QLabel(Form)
        self.greenLeft1.setGeometry(QtCore.QRect(145, 100, 16, 141))
        self.greenLeft1.setPixmap(QtGui.QPixmap("assets/greenLeft1.png"))
        self.greenLeft1.setObjectName("greenLeft1")

        self.greenLeft2 = QtWidgets.QLabel(Form)
        self.greenLeft2.setGeometry(QtCore.QRect(162, 100, 16, 141))
        self.greenLeft2.setPixmap(QtGui.QPixmap("assets/greenLeft2.png"))
        self.greenLeft2.setObjectName("greenLeft2")

        self.greenRight0 = QtWidgets.QLabel(Form)
        self.greenRight0.setGeometry(QtCore.QRect(719, 100, 31, 141))
        self.greenRight0.setPixmap(QtGui.QPixmap("assets/greenRight0.png"))
        self.greenRight0.setObjectName("greenRight0")

        self.greenRight1 = QtWidgets.QLabel(Form)
        self.greenRight1.setGeometry(QtCore.QRect(703, 100, 16, 141))
        self.greenRight1.setPixmap(QtGui.QPixmap("assets/greenRight1.png"))
        self.greenRight1.setObjectName("greenRight1")

        self.greenRight2 = QtWidgets.QLabel(Form)
        self.greenRight2.setGeometry(QtCore.QRect(681, 100, 21, 141))
        self.greenRight2.setPixmap(QtGui.QPixmap("assets/greenRight2.png"))
        self.greenRight2.setObjectName("greenRight2")

        # Finalize UI setup
        self.retranslateUi(Form)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        """
        Set textual elements (like window title) for the radar monitor UI.

        Args:
            Form (QWidget): The main form/window.
        """
        _translate = QtCore.QCoreApplication.translate
        Form.setWindowTitle(_translate("Form", "Reversing Radar Monitor"))
