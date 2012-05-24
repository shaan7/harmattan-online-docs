#include "accountsmodel.h"

#include <QDebug>

AccountsModel::AccountsModel(QObject *parent) :
    QAbstractListModel(parent),
    m_accountsManager(new Accounts::Manager())
{
    foreach(Accounts::AccountId id, m_accountsManager.accountListEnabled("e-mail")) {
        Accounts::Account *account = m_accountsManager.account(id);
        if (account->providerName() == "google") {
            m_accountsList.append(account);
        }
    }
}

QVariant AccountsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() >= rowCount())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
        return m_accountsList.at(index.row())->displayName();
    }
}

int AccountsModel::rowCount(const QModelIndex &parent) const
{
    return m_accountsList.count();
}

void AccountsModel::gotInfo (const SignOn::IdentityInfo &info)
{
    foreach(SignOn::MethodName name, info.methods()) {
        qDebug() << info.mechanisms(name);
    }
}

void AccountsModel::response (const SignOn::SessionData &sessionData)
{
    emit loggedIn(sessionData.getProperty("Auth").toByteArray());
}

void AccountsModel::performLogin(int accountIndex)
{
    if (accountIndex >= rowCount())
        return;
    Accounts::Account *account = m_accountsList.at(accountIndex);
    SignOn::Identity *identity = SignOn::Identity::existingIdentity(account->valueAsInt("CredentialsId"));
    SignOn::AuthSessionP session = identity->createSession("google");
    connect(session, SIGNAL(response(SignOn::SessionData)), SLOT(response (SignOn::SessionData)));
    QVariantMap parameters;
    parameters["service"] = "writely";
    parameters["source"] = "Calligrav2";
    qDebug() << "Logging in " << parameters;
    m_lastUsedIndex = accountIndex;
    session->request(SignOn::SessionData(parameters), "ClientLogin");
}

void AccountsModel::performLoginAgain(const QString &service)
{
    Accounts::Account *account = m_accountsList.at(m_lastUsedIndex);
    SignOn::Identity *identity = SignOn::Identity::existingIdentity(account->valueAsInt("CredentialsId"));
    SignOn::AuthSessionP session = identity->createSession("google");
    connect(session, SIGNAL(response(SignOn::SessionData)), SLOT(response (SignOn::SessionData)));

    QVariantMap parameters;
    parameters["service"] = service;
    parameters["source"] = "Calligrav2";
    qDebug() << "Logging in " << parameters;
    session->request(SignOn::SessionData(parameters), "ClientLogin");
}
