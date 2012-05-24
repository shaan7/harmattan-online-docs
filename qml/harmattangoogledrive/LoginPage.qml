/***************************************************************************
 *  Copyright 2012 Shantanu Tushar <shaan7in@gmail.com>                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    id: loginPage
    signal login(string username, string password)
    property string selected_username

    Column {
        id: accountColumn
        anchors.fill: parent

        AppHeader {
            id: selectAccountLabel
            width: parent.width; height: UiConstants.HeaderDefaultHeightPortrait
            text: qsTr("Select a Google account")
        }

        Item { width: parent.width; height: UiConstants.DefaultMargin }

        Image {
            anchors.horizontalCenter: parent.horizontalCenter
            width: 128; height: width
            source: "qrc:///general/appicon.svg"
        }

        ListView {
            width: parent.width
            height: parent.height - selectAccountLabel.height
            model: accountsModel
            delegate: Button {
                text: display
                width: parent.width
                onClicked: {
                    accountColumn.visible = false
                    passwordColumn.visible = true
                    usernameText.text = display
                }
            }
        }
    }

    Column {
        id: passwordColumn
        visible: false
        anchors.fill: parent

        AppHeader {
            id: usernameText
            width: parent.width; height: UiConstants.HeaderDefaultHeightPortrait
        }

        Item { width: parent.width; height: UiConstants.DefaultMargin }

        Image {
            anchors.horizontalCenter: parent.horizontalCenter
            width: 128 ; height: width
            source: "qrc:///general/appicon.svg"
        }

        TextField {
            width: parent.width
            id: passwordText
            placeholderText: qsTr("password")
            echoMode: TextInput.Password
        }

        Button {
            id: loginButton
            width: parent.width
            text: qsTr("Login")
            onClicked: loginPage.login(usernameText.text, passwordText.text)
        }
    }
}
