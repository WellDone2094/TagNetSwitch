# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'sendmessage.ui'
#
# Created by: PyQt4 UI code generator 4.11.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

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

class Ui_SendMessage(object):
    def setupUi(self, SendMessage, client):
        self.client = client
        SendMessage.setObjectName(_fromUtf8("SendMessage"))
        SendMessage.resize(400, 352)
        self.dialog = SendMessage
        self.lineEdit = QtGui.QLineEdit(SendMessage)
        self.lineEdit.setGeometry(QtCore.QRect(10, 30, 381, 21))
        self.lineEdit.setObjectName(_fromUtf8("lineEdit"))
        self.plainTextEdit = QtGui.QPlainTextEdit(SendMessage)
        self.plainTextEdit.setGeometry(QtCore.QRect(10, 80, 381, 241))
        self.plainTextEdit.setObjectName(_fromUtf8("plainTextEdit"))
        self.label = QtGui.QLabel(SendMessage)
        self.label.setGeometry(QtCore.QRect(10, 10, 59, 16))
        self.label.setObjectName(_fromUtf8("label"))
        self.label_2 = QtGui.QLabel(SendMessage)
        self.label_2.setGeometry(QtCore.QRect(10, 60, 59, 16))
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.pushButton = QtGui.QPushButton(SendMessage)
        self.pushButton.setGeometry(QtCore.QRect(280, 320, 113, 32))
        self.pushButton.setObjectName(_fromUtf8("pushButton"))
        self.pushButton.clicked.connect(self.send)

        self.retranslateUi(SendMessage)
        QtCore.QMetaObject.connectSlotsByName(SendMessage)

    def retranslateUi(self, SendMessage):
        SendMessage.setWindowTitle(_translate("SendMessage", "Send Message", None))
        self.label.setText(_translate("SendMessage", "Tags", None))
        self.label_2.setText(_translate("SendMessage", "Message", None))
        self.pushButton.setText(_translate("SendMessage", "Send", None))

    def send(self):
        tagString = self.lineEdit.text().strip()
        tags = tagString.split(',');
        for i in range(len(tags)):
            tags[i] = tags[i].strip()
        self.client.send(0,self.plainTextEdit.document().toPlainText().__str__(),tags)
        self.dialog.done(0)
