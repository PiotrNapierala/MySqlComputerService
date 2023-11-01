QT       += core gui sql charts printsupport svg network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Dialogs/About/aboutdialog.cpp \
    Dialogs/AddClient/addclientdialog.cpp \
    Dialogs/AddComputer/addcomputerdialog.cpp \
    Dialogs/AddImage/addimagedialog.cpp \
    Dialogs/AddMenu/addmenudialog.cpp \
    Dialogs/AddOrder/addorderdialog.cpp \
    Dialogs/AddUser/adduserdialog.cpp \
    Dialogs/Ask/askdialog.cpp \
    Dialogs/ChangePassword/changepassworddialog.cpp \
    Dialogs/ClientInfo/clientinfodialog.cpp \
    Dialogs/ComputerInfo/computerinfodialog.cpp \
    Dialogs/FinishOrder/finishorderdialog.cpp \
    Dialogs/FirstRun/firstrundialog.cpp \
    Dialogs/GetClient/getclientdialog.cpp \
    Dialogs/GetComputer/getcomputerdialog.cpp \
    Dialogs/Info/infodialog.cpp \
    Dialogs/Install/installdialog.cpp \
    Dialogs/Language/language.cpp \
    Dialogs/Login/logindialog.cpp \
    Dialogs/MoveToCompleted/movetocompleteddialog.cpp \
    Dialogs/MoveToRepair/movetorepairdialog.cpp \
    Dialogs/OrderInfo/orderinfodialog.cpp \
    Dialogs/OrderMenu/ordermenudialog.cpp \
    Dialogs/PhotoPreview/photopreviewdialog.cpp \
    Dialogs/Search/searchdialog.cpp \
    Dialogs/Settings/settingsdialog.cpp \
    Dialogs/Statistic/statisticsdialog.cpp \
    Dialogs/String/stringdialog.cpp \
    Dialogs/UserAdmin/useramindialog.cpp \
    Libraries/ApplicationLog/applicationlog.cpp \
    Libraries/DatabaseConnector/databaseconnector.cpp \
    Libraries/EmailConnector/emailconnector.cpp \
    Libraries/GetUrlValue/httpserverget.cpp \
    Libraries/ImageDownloader/imagedownloader.cpp \
    Libraries/MyCrypto/mycrypto.cpp \
    Libraries/QRConnector/qrconnector.cpp \
    Libraries/ServiceCore/servicecore.cpp \
    Libraries/StatisticConnector/statisticconnector.cpp \
    Libraries/Totp/totp.cpp \
    QR-Code-generator/qrcodegen.cpp \
    main.cpp \
    Dialogs\MainWindow\mainwindow.cpp

HEADERS += \
    Classes/Client/client.h \
    Classes/Computer/computer.h \
    Classes/Order/order.h \
    Classes/User/user.h \
    Dialogs/About/aboutdialog.h \
    Dialogs/AddClient/addclientdialog.h \
    Dialogs/AddComputer/addcomputerdialog.h \
    Dialogs/AddImage/addimagedialog.h \
    Dialogs/AddMenu/addmenudialog.h \
    Dialogs/AddOrder/addorderdialog.h \
    Dialogs/AddUser/adduserdialog.h \
    Dialogs/Ask/askdialog.h \
    Dialogs/ChangePassword/changepassworddialog.h \
    Dialogs/ClientInfo/clientinfodialog.h \
    Dialogs/ComputerInfo/computerinfodialog.h \
    Dialogs/FinishOrder/finishorderdialog.h \
    Dialogs/FirstRun/firstrundialog.h \
    Dialogs/GetClient/getclientdialog.h \
    Dialogs/GetComputer/getcomputerdialog.h \
    Dialogs/Info/infodialog.h \
    Dialogs/Install/installdialog.h \
    Dialogs/Language/language.h \
    Dialogs/Login/logindialog.h \
    Dialogs/MoveToCompleted/movetocompleteddialog.h \
    Dialogs/MoveToRepair/movetorepairdialog.h \
    Dialogs/OrderInfo/orderinfodialog.h \
    Dialogs/OrderMenu/ordermenudialog.h \
    Dialogs/PhotoPreview/photopreviewdialog.h \
    Dialogs/Search/searchdialog.h \
    Dialogs/Settings/settingsdialog.h \
    Dialogs/Statistic/statisticsdialog.h \
    Dialogs/String/stringdialog.h \
    Dialogs/UserAdmin/useramindialog.h \
    Dialogs\MainWindow\mainwindow.h \
    Libraries/ApplicationLog/applicationlog.h \
    Libraries/DatabaseConnector/databaseconnector.h \
    Libraries/EmailConnector/emailconnector.h \
    Libraries/GetUrlValue/httpserverget.h \
    Libraries/ImageDownloader/imagedownloader.h \
    Libraries/MyCrypto/mycrypto.h \
    Libraries/QRConnector/qrconnector.h \
    Libraries/ServiceCore/servicecore.h \
    Libraries/StatisticConnector/statisticconnector.h \
    Libraries/Totp/totp.h \
    QR-Code-generator/qrcodegen.hpp

FORMS += \
    Dialogs/About/aboutdialog.ui \
    Dialogs/AddClient/addclientdialog.ui \
    Dialogs/AddComputer/addcomputerdialog.ui \
    Dialogs/AddImage/addimagedialog.ui \
    Dialogs/AddMenu/addmenudialog.ui \
    Dialogs/AddOrder/addorderdialog.ui \
    Dialogs/AddUser/adduserdialog.ui \
    Dialogs/Ask/askdialog.ui \
    Dialogs/ChangePassword/changepassworddialog.ui \
    Dialogs/ClientInfo/clientinfodialog.ui \
    Dialogs/ComputerInfo/computerinfodialog.ui \
    Dialogs/FinishOrder/finishorderdialog.ui \
    Dialogs/FirstRun/firstrundialog.ui \
    Dialogs/GetClient/getclientdialog.ui \
    Dialogs/GetComputer/getcomputerdialog.ui \
    Dialogs/Info/infodialog.ui \
    Dialogs/Install/installdialog.ui \
    Dialogs/Language/language.ui \
    Dialogs/Login/logindialog.ui \
    Dialogs/MoveToCompleted/movetocompleteddialog.ui \
    Dialogs/MoveToRepair/movetorepairdialog.ui \
    Dialogs/OrderInfo/orderinfodialog.ui \
    Dialogs/OrderMenu/ordermenudialog.ui \
    Dialogs/PhotoPreview/photopreviewdialog.ui \
    Dialogs/Search/searchdialog.ui \
    Dialogs/Settings/settingsdialog.ui \
    Dialogs/Statistic/statisticsdialog.ui \
    Dialogs/String/stringdialog.ui \
    Dialogs/UserAdmin/useramindialog.ui \
    Dialogs\MainWindow\mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/SmtpClient/build-SMTPEmail-Desktop_Qt_6_4_1_MinGW_64_bit-Debug/debug/ -lSmtpMime2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/SmtpClient/build-SMTPEmail-Desktop_Qt_6_4_1_MinGW_64_bit-Debug/debug/ -lSmtpMime2
else:unix: LIBS += -L$$PWD/SmtpClient/build-SMTPEmail-Desktop_Qt_6_4_1_MinGW_64_bit-Debug/ -lSmtpMime2

INCLUDEPATH += $$PWD/SmtpClient/build-SMTPEmail-Desktop_Qt_6_4_1_MinGW_64_bit-Debug/debug
DEPENDPATH += $$PWD/SmtpClient/build-SMTPEmail-Desktop_Qt_6_4_1_MinGW_64_bit-Debug/debug
