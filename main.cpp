#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"

#include "googledocsservice.h"
#include "accountsmodel.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QmlApplicationViewer viewer;

    AccountsModel accountsModel;
    viewer.rootContext()->setContextProperty("accountsModel", &accountsModel);

    GoogleDocsService gDocumentService;
    gDocumentService.setAccountsModel(&accountsModel);
    gDocumentService.setRootContext(viewer.rootContext());
    viewer.rootContext()->setContextProperty("gDocumentService", &gDocumentService);

    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/harmattangoogledrive/main.qml"));
    viewer.showExpanded();

    return app->exec();
}
