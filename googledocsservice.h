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

#ifndef GOOGLEDOCSSERVICE_H
#define GOOGLEDOCSSERVICE_H

#include "accountsmodel.h"
#include "googlecontenthandler.h"
#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>
#include <QXmlInputSource>
#include <QDeclarativeContext>

#include <Accounts/Account>

class GoogleDocsService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
public:
    explicit GoogleDocsService(QObject *parent = 0);
    Q_INVOKABLE void clientLogin(const QString & username, const QString & password);
    Q_INVOKABLE void downloadDocument (const QString & url, const QString & type);
    void setRootContext(QDeclarativeContext *ctxt);
    int progress() const;
    void setAccountsModel(AccountsModel *model);
    bool busy() const;
signals:
    void progressChanged();
protected:
    void setBusy(bool busy);
private slots:
    void handleNetworkData(QNetworkReply *networkReply);
    void downloadProgress(qint64 received, qint64 total);
    void processLogin(const QByteArray &authToken);
signals:
    void loginSuccessful();
    void loginFailed();
    void busyChanged();
    void documentListFetched();
private:
    void listDocuments();
    QXmlSimpleReader m_xmlReader;
    QXmlInputSource  m_xmlInput;
    QString m_username;
    bool m_waitingForDoc;
    QString m_password;
    GoogleContentHandler *m_gHandler;
    QString m_authToken;
    QString m_docAuthToken;
    bool m_loggedin;
    bool m_haveDocAuthToken;
    QString m_spreadAuthToken;
    QNetworkAccessManager m_networkManager;
    bool m_newInformation;
    QList<GoogleDocument*> m_docList;
    QDeclarativeContext *m_ctxt;
    QString m_type;
    int m_progress;
    QString m_extension;
    AccountsModel *m_accountsModel;
    bool m_busy;
};

#endif // GOOGLEDOCSSERVICE_H
