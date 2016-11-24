TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../CommandLineParser/release/ -lCommandLineParser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../CommandLineParser/debug/ -lCommandLineParser
else:unix: LIBS += -L$$OUT_PWD/../CommandLineParser/ -lCommandLineParser

INCLUDEPATH += $$PWD/../CommandLineParser
DEPENDPATH += $$PWD/../CommandLineParser

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../CommandLineParser/release/libCommandLineParser.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../CommandLineParser/debug/libCommandLineParser.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../CommandLineParser/release/CommandLineParser.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../CommandLineParser/debug/CommandLineParser.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../CommandLineParser/libCommandLineParser.a
