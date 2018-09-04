#-------------------------------------------------
#
# Project created by QtCreator 2018-08-03T13:19:11
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QQ_Client
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    login_frame.cpp \
    main_frame.cpp \
    client_socket.cpp \
    chat_frame.cpp \
    face_frame.cpp \
    face_qlabel.cpp \
    recv_file_frame.cpp \
    register_frame.cpp \
    register_result_frame.cpp \
    add_friends_frame.cpp

HEADERS += \
    login_frame.h \
    main_frame.h \
    client_socket.h \
    chat_frame.h \
    face_frame.h \
    face_qlabel.h \
    recv_file_frame.h \
    register_frame.h \
    register_result_frame.h \
    add_friends_frame.h

FORMS += \
    login_frame.ui \
    main_frame.ui \
    chat_frame.ui \
    face_frame.ui \
    recv_file_frame.ui \
    register_frame.ui \
    register_result_frame.ui \
    add_friends_frame.ui

RESOURCES += \
    resources.qrc

CONFIG +=C++11
