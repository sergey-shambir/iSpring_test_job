QT += quick widgets
CONFIG += c++11

INCLUDEPATH += $$PWD

*linux-*{
    INCLUDEPATH += $$PWD/GDIPlus_fake
    HEADERS += \
        GDIPlus_fake/gdiplus.h

    SOURCES += \
        GDIPlus_fake/gdiplus.cpp
}

SOURCES += \
    main.cpp \
    src/json11.cpp \
    src/trianglenode.cpp \
    src/rectanglenode.cpp \
    src/ellipsenode.cpp \
    src/abstractnode.cpp \
    src/shapesscene.cpp \
    src/shapecommand.cpp \
    shapessceneview.cpp

RESOURCES += \
    iSpringTestPrototype.qrc

HEADERS += \
    math/geometry.h \
    math/mat3.h \
    math/mat4.h \
    math/math \
    math/quat.h \
    math/rectangle.h \
    math/vec2.h \
    math/vec3.h \
    math/vec4.h \
    src/json11.hpp \
    src/trianglenode.h \
    src/rectanglenode.h \
    src/ellipsenode.h \
    src/abstractnode.h \
    src/shapesscene.h \
    src/shapecommand.h \
    shapessceneview.h
