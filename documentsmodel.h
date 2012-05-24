#ifndef DOCUMENTSMODEL_H
#define DOCUMENTSMODEL_H

#include <QAbstractListModel>

#include "googledocument.h"

class DocumentsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count)
public:
    enum Roles {
        UrlRole = Qt::UserRole + 1,
        TypeRole,
        AuthorRole
    };

    explicit DocumentsModel(QObject *parent = 0);
    void setDocumentsList(QList<GoogleDocument*> *docList);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;

    int count() const;
    Q_INVOKABLE QVariantMap get(int index) const;
    
signals:
    
public slots:
private:
    QList<GoogleDocument*> *m_docList;
};

#endif // DOCUMENTSMODEL_H
