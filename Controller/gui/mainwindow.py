# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'mainwindow.ui'
#
# Created by: PyQt4 UI code generator 4.11.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui
from .sendMessage import *

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

class Ui_MainWindow(object):
    def setupUi(self, MainWindow, client):
        MainWindow.setObjectName(_fromUtf8("MainWindow"))
        MainWindow.setFixedSize(580, 476)
        self.tags = []
        self.title = 'client '+str(client.id)
        self.client = client
        self.MainWindow = MainWindow
        self.centralWidget = QtGui.QWidget(MainWindow)
        self.centralWidget.setObjectName(_fromUtf8("centralWidget"))
        self.plainTextEdit = QtGui.QPlainTextEdit(self.centralWidget)
        self.plainTextEdit.setEnabled(True)
        self.plainTextEdit.setReadOnly(True)
        self.plainTextEdit.setGeometry(QtCore.QRect(20, 30, 381, 391))
        self.plainTextEdit.setObjectName(_fromUtf8("plainTextEdit"))
        self.listWidget = QtGui.QListWidget(self.centralWidget)
        self.listWidget.setGeometry(QtCore.QRect(410, 30, 151, 361))
        self.listWidget.setObjectName(_fromUtf8("listWidget"))
        self.label = QtGui.QLabel(self.centralWidget)
        self.label.setGeometry(QtCore.QRect(20, 10, 381, 16))
        self.label.setObjectName(_fromUtf8("label"))
        self.label_2 = QtGui.QLabel(self.centralWidget)
        self.label_2.setGeometry(QtCore.QRect(410, 10, 151, 16))
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.pushButton = QtGui.QPushButton(self.centralWidget)
        self.pushButton.setGeometry(QtCore.QRect(110, 430, 151, 32))
        self.pushButton.setObjectName(_fromUtf8("pushButton"))
        self.lineEdit = QtGui.QLineEdit(self.centralWidget)
        self.lineEdit.setGeometry(QtCore.QRect(410, 400, 151, 21))
        self.lineEdit.setObjectName(_fromUtf8("lineEdit"))
        self.pushButton_2 = QtGui.QPushButton(self.centralWidget)
        self.pushButton_2.setGeometry(QtCore.QRect(410, 430, 151, 32))
        self.pushButton_2.setObjectName(_fromUtf8("pushButton_2"))
        self.pushButton_3 = QtGui.QPushButton(self.centralWidget)
        self.pushButton_3.setGeometry(QtCore.QRect(260, 430, 151, 32))
        self.pushButton_3.setObjectName(_fromUtf8("pushButton_3"))
        MainWindow.setCentralWidget(self.centralWidget)
        self.pushButton.clicked.connect(self.sendMessage)
        self.pushButton_2.clicked.connect(self.add_tag)
        self.pushButton_3.clicked.connect(self.remove_tag)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(_translate(self.title, self.title, None))
        self.label.setText(_translate("MainWindow", "Messages", None))
        self.label_2.setText(_translate("MainWindow", "Tags", None))
        self.pushButton.setText(_translate("MainWindow", "Send Message", None))
        self.pushButton_2.setText(_translate("MainWindow", "Add Tag", None))
        self.pushButton_3.setText(_translate("MainWindow", "Remove Tag", None))

    def add_tag(self):
        tag = self.lineEdit.text().strip()
        if tag in self.tags:
            return
        self.tags.append(tag)
        if len(tag) > 0:
            tags = tag.split(',')
            tags_strip = []
            for t in tags:
                tags_strip.append(t.strip())
            self.client.add_tags(0,tags_strip)
            self.listWidget.addItem(tag)
        self.lineEdit.setText('')

    def remove_tag(self):
        items = self.listWidget.selectedItems()
        if not items: return
        for item in items:
            self.listWidget.takeItem(self.listWidget.row(item))
            tag = item.text().strip()
            i = self.tags.index(tag)
            del self.tags[i]
            self.client.remove_tag([tag])

    def sendMessage(self):
        dialog = QtGui.QDialog(self.MainWindow)
        sm = Ui_SendMessage()
        sm.setupUi(dialog, self.client)
        print(dialog.exec_())

    def received(self, msg):
        self.plainTextEdit.appendPlainText(msg)
        self.MainWindow.update()




if __name__ == "__main__":
    import sys
    app = QtGui.QApplication(sys.argv)
    MainWindow = QtGui.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    Main2 = QtGui.QMainWindow()
    Main2.show()
    sys.exit(app.exec_())
