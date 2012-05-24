/***************************************************************************
 *  Copyright (c) 2010 Mani Chandrasekar <maninc@gmail.com>                *
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

#include "googledocsservice.h"
#include "googledocumentlist.h"
#include "googledocument.h"
#include "documentsmodel.h"

#include <QNetworkRequest>
#include <QDebug>
#include <QMessageBox>
#include <QXmlInputSource>
#include <QDir>
#include <QDesktopServices>

#include <Accounts/Manager>

bool compareGoogleDocumentByType(const GoogleDocument *d1, const GoogleDocument *d2)
{
    return d1->documentType().compare(d2->documentType()) <= 0;
}

GoogleDocsService::GoogleDocsService(QObject *parent) :
    QObject(parent)
  , m_waitingForDoc(false)
  , m_haveDocAuthToken(false)
  , m_loggedin(false)
  , m_newInformation(true)
  , m_progress(0)
  , m_busy(false)
{
    connect(&m_networkManager, SIGNAL(finished(QNetworkReply*)),
               this, SLOT(handleNetworkData(QNetworkReply*)));
    m_gHandler = new GoogleContentHandler();
    m_xmlReader.setContentHandler(m_gHandler);
}

void GoogleDocsService::clientLogin(const QString & username, const QString & password)
{
    setBusy(true);
    QByteArray data;
    data.append(QString("accountType=HOSTED_OR_GOOGLE&Email=" + username + "&Passwd=" + password).toUtf8());

    if(!m_haveDocAuthToken) {
        data.append(QString("&service=writely&source=Calligrav2").toUtf8());
        m_username = username;
        m_password = password;
    } else {
        data.append(QString("&service=wise&source=Calligrav2").toUtf8());
    }

    QNetworkRequest req(QUrl("https://www.google.com/accounts/ClientLogin"));
    req.setRawHeader("Host", "www.google.com");
    req.setRawHeader("GData-Version", "3.0");
    req.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    req.setHeader(QNetworkRequest::ContentLengthHeader, data.length());

    m_networkManager.post(req, data);
}

void GoogleDocsService::handleNetworkData(QNetworkReply *networkReply)
{
    setBusy(false);
    if (!networkReply->error()) {
        if (!m_loggedin) {
            QString text(networkReply->readAll());
            text = text.right(text.length() - text.indexOf("Auth=") - 5);
            m_authToken = QString("GoogleLogin auth=") + text.left(text.indexOf("\n"));
            if(m_authToken.length() > 20) {
                if(!m_haveDocAuthToken) {
                    m_docAuthToken = m_authToken;
                    m_haveDocAuthToken = true;
                    clientLogin(m_username, m_password);
                    return;
                }
                m_spreadAuthToken = m_authToken;
                m_authToken = "";
                m_loggedin = true;
                listDocuments();
                emit loginSuccessful();
            }
    } else if (m_waitingForDoc) {
            QByteArray data = networkReply->readAll();
            if (m_type == "document") {
                m_extension = ".odt";
            } else if(m_type == "spreadsheet") {
                m_extension = ".ods";
            } else if (m_type == "presentation") {
                m_extension = ".ppt";
            }
            QFile file(QDir::tempPath() + "/doc" + m_extension);
            file.open(QIODevice::ReadWrite);
            file.write(data);
            file.waitForBytesWritten(60000);
            file.close();
            m_progress = 0;
            emit progressChanged();
            QDesktopServices::openUrl(file.fileName()); //open file with required application
            qDebug() << "opening filename " << file.fileName();
            m_waitingForDoc = false;
    } else {
        QByteArray bytAry = networkReply->readAll();
        m_xmlInput.setData(bytAry);

        if (m_newInformation) {
           m_newInformation = m_xmlReader.parse(&m_xmlInput, true);
           QList<GoogleDocument*> docList = m_gHandler->documentList()->entries();
           qSort(docList.begin(), docList.end(), compareGoogleDocumentByType);
           foreach(GoogleDocument *d , docList) {
               m_docList.append(d);
           }
           qDebug() << "SETTING CONTEXT";
           DocumentsModel *docsModel = new DocumentsModel(this);
           docsModel->setDocumentsList(&m_docList);
           m_ctxt->setContextProperty("documentModel", docsModel);
           emit documentListFetched();
        }
    }
} else {
        QString errorString(networkReply->readAll());
        qDebug() << "Error occured !!!!  " << errorString;
        errorString = errorString.right(errorString.length() - errorString.indexOf("Error=") - 6);
        if (!m_loggedin) {
            emit loginFailed();
        } else {
            qDebug() << "Online Document Services Error occured !!!!  " << errorString;
        }
    }
}

void GoogleDocsService::listDocuments()
{
    setBusy(true);
    m_authToken = m_docAuthToken;
    //QString url = "https://docs.google.com/feeds/default/private/full/-/" + m_type;
    QString url = "https://docs.google.com/feeds/default/private/full";
    QNetworkRequest requestHeader(QUrl(url.toUtf8()));
    requestHeader.setRawHeader("Host", "docs.google.com");
    requestHeader.setRawHeader("User-Agent", "Calligra");
    requestHeader.setRawHeader("GData-Version", "3.0");
    requestHeader.setRawHeader("Content-Type", "application/atom+xml");
    requestHeader.setRawHeader("Authorization", m_authToken.toUtf8());
    m_networkManager.get(requestHeader);
    qDebug() << "Successfully authenticated!!! Retreiving document list...";
}

void GoogleDocsService::downloadDocument(const QString & _url, const QString & _type)
{
    setBusy(true);
    m_authToken = m_docAuthToken;
    QString url = _url;
    QString type = _type;
    url.replace("docId", "docID", Qt::CaseInsensitive);
    QString exportFormat = "";
    m_type = _type;

    if(QString::compare(type, "spreadsheet", Qt::CaseInsensitive) == 0) {
        exportFormat = "&exportFormat=ods&format=ods";
        m_authToken = m_spreadAuthToken;
    } else if(QString::compare(type, "presentation", Qt::CaseInsensitive) == 0) {
        exportFormat = "&exportFormat=ppt&format=ppt";
    }
    else if(QString::compare(type, "document", Qt::CaseInsensitive) == 0) {
        exportFormat = "&exportFormat=odt&format=odt";
    }

    QUrl documentUrl(url + exportFormat);

    QNetworkRequest requestHeader(documentUrl);
    requestHeader.setRawHeader("User-Agent", "Calligra");
    requestHeader.setRawHeader("GData-Version", "3.0");
    requestHeader.setRawHeader("Authorization", m_authToken.toUtf8());

    QList<QByteArray> headerlist = requestHeader.rawHeaderList();
    foreach (QByteArray element, headerlist)
        qDebug() << element << requestHeader.rawHeader(element);

    qDebug() << documentUrl;
    QNetworkReply * reply = m_networkManager.get(requestHeader);
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), SLOT(downloadProgress(qint64,qint64)));

    m_waitingForDoc = true;

}

void GoogleDocsService::setRootContext(QDeclarativeContext *ctxt)
{
    m_ctxt = ctxt;
}

void GoogleDocsService::downloadProgress(qint64 received, qint64 total)
{
    m_progress = received;
    emit progressChanged();
}

int GoogleDocsService::progress() const
{
    return m_progress;
}

void GoogleDocsService::setAccountsModel(AccountsModel *model)
{
    connect(model, SIGNAL(loggedIn(QByteArray)), SLOT(processLogin(QByteArray)));
}

void GoogleDocsService::processLogin(const QByteArray &authToken)
{
    m_authToken = QString("GoogleLogin auth=") + authToken;
    listDocuments();
}

bool GoogleDocsService::busy() const
{
    return m_busy;
}

void GoogleDocsService::setBusy(bool busy)
{
    m_busy = busy;
    emit busyChanged();
}
