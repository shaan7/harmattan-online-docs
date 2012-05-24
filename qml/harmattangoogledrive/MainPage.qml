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
    id: mainPage
    signal loginFailed

    BusyIndicator {
         id: indicator3
         platformStyle: BusyIndicatorStyle { size: "large" }
         anchors.centerIn: parent
         visible: gDocumentService.busy
         running:  visible
         z: 1
    }

    ListView {
        id: docsListView
        height: parent.height
        width: parent.width
        anchors.fill: parent
        spacing: 8
        snapMode: ListView.SnapToItem

        header: AppHeader { width: parent.width; height: UiConstants.HeaderDefaultHeightPortrait }

        delegate: DocumentListDelegate { width: parent ? parent.width : undefined; height: UiConstants.ListItemHeightSmall }

        section {
            property: "type"
            criteria: ViewSection.FullString
            delegate: Rectangle {
                width: parent.width; height: UiConstants.ListItemHeightSmall/2
                color: "orange"
                Label {
                    anchors.fill: parent
                    text: section + 's'
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font: UiConstants.GroupHeaderFont
                }
            }
        }
    }

    FastScroll { listView: docsListView }
    ScrollDecorator { flickableItem: docsListView }

    function login(username, password)
    {
        gDocumentService.loginFailed.connect(mainPage.loginFailed)
        gDocumentService.documentListFetched.connect(mainPage.showDocuments)
        gDocumentService.clientLogin(username, password)
    }

    function showDocuments()
    {
        docsListView.model = documentModel
    }
}
