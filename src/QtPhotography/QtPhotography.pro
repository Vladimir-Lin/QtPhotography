NAME         = QtPhotography
TARGET       = $${NAME}

QT           = core
QT          += gui
QT          += network
QT          += sql
QT          += script
QT          += positioning
QT          += widgets
QT          += opengl
QT          += printsupport
QT          += multimedia
QT          += multimediawidgets
QT          += opengl
QT          += QtCUDA
QT          += QtOpenCV
QT          += Essentials
QT          += QtCalendar
QT          += QtGMP
QT          += QtGSL
QT          += QtAlgebra
QT          += QtDiscrete
QT          += QtFFT
QT          += Mathematics
QT          += QtFuzzy
QT          += QtFLP
QT          += QtFoundation
QT          += QtGeometry
QT          += QtGadgets
QT          += QtComponents
QT          += QtManagers
QT          += QtVCF

load(qt_build_config)
load(qt_module)

INCLUDEPATH += $${PWD}/../../include/$${NAME}

HEADERS     += $${PWD}/../../include/$${NAME}/qtphotography.h

include ($${PWD}/Galleries/Galleries.pri)
include ($${PWD}/Models/Models.pri)
include ($${PWD}/Pictures/Pictures.pri)
include ($${PWD}/Scene/Scene.pri)

OTHER_FILES += $${PWD}/../../include/$${NAME}/headers.pri

include ($${PWD}/../../doc/Qt/Qt.pri)

TRNAME       = $${NAME}

include ($${PWD}/../../Translations.pri)
