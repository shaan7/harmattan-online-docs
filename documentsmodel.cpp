#include "documentsmodel.h"

DocumentsModel::DocumentsModel(QObject *parent) :
    QAbstractListModel(parent),
    m_docList(0)
{
    QHash<int, QByteArray> roles = roleNames();
    roles[UrlRole] = "url";
    roles[TypeRole] = "type";
    roles[AuthorRole] = "author";
    setRoleNames(roles);
}

void DocumentsModel::setDocumentsList(QList<GoogleDocument *> *docList)
{
    beginResetModel();
    m_docList = docList;
    endResetModel();
}

int DocumentsModel::rowCount(const QModelIndex &parent) const
{
    if (m_docList)
        return m_docList->count();
    else
        return 0;
}

QVariant DocumentsModel::data(const QModelIndex &index, int role) const
{
    if (!m_docList || index.row() >= rowCount())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
        return m_docList->at(index.row())->title();
    case UrlRole:
        return m_docList->at(index.row())->documentUrl();
    case TypeRole:
        return m_docList->at(index.row())->documentType();
    case AuthorRole:
        return m_docList->at(index.row())->author();
    }

    return QVariant();
}

int DocumentsModel::count() const
{
    return m_docList ? m_docList->count() : 0;
}

QVariantMap DocumentsModel::get(int index) const
{
    QVariantMap listElement;

    if (index < 0 || !m_docList || index > m_docList->count())
        return listElement;

    listElement["type"] = m_docList->at(index)->documentType();

    return listElement;
}
