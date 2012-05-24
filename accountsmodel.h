#ifndef ACCOUNTSMODEL_H
#define ACCOUNTSMODEL_H

#include <QtCore/QAbstractListModel>
#include <QtCore/QList>

#include <Accounts/Manager>
#include <SignOn/Identity>

class AccountsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit AccountsModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    
signals:
    void loggedIn(const QByteArray &authToken);

public slots:
    void performLogin(int accountIndex);
    void performLoginAgain(const QString &service);

private slots:
    void gotInfo (const SignOn::IdentityInfo &info);
    void response (const SignOn::SessionData &sessionData);

private:
    Accounts::Manager m_accountsManager;
    QList<Accounts::Account*> m_accountsList;
    int m_lastUsedIndex;
};

#endif // ACCOUNTSMODEL_H
