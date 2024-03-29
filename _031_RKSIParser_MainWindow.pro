TEMPLATE = app
TARGET = _031_RKSIParser_MainWindow
QT += core \
    gui \
    xml \
    sql \
    network
HEADERS += PARSING/XML/XML_WRITER/RnavAid.h \
    PARSING/XML/XML_WRITER/Holding.h \
    PARSING/XML/XML_WRITER/XMLReadWriter.h \
    PARSING/XML/XML_IMPORT/XMLImport.h \
    PARSING/PARSER/MyLineEdit.h \
    PARSING/PARSER/DAFIF_PARSER.h \
    DRAWING/DRAWING04/Infoline.h \
    lib/Sql/AirspaceSqlGen.h \
    lib/Sql/SidStarSqlGen.h \
    lib/Sql/CoastSqlGen.h \
    lib/Sql/SqlGen.h \
    lib/DbManager.h \
    lib/LayerManager.h \
    DRAWING/DRAWING04/NodeInfo.h \
    Net/QtServer.h \
    THREAD/MyThread.h \
    DRAWING/DRAWING04/Node.h \
    DRAWING/DRAWING04/DrawingScene.h \
    DRAWING/DRAWING04/Window04.h \
    DRAWING/DRAWING04/DrawingExample_04.h \
    DRAWING/DRAWING03/RenderArea03.h \
    DRAWING/DRAWING03/Window03.h \
    DRAWING/DRAWING02/Window02.h \
    DRAWING/DRAWING02/RenderArea02.h \
    DRAWING/RenderArea.h \
    DRAWING/WINDOW.h \
    XMLParser/XMLParser.h \
    XMLParser/Aoi.h \
    XMLParser/AoiPolygon.h \
    XMLParser/AoiSector.h \
    XMLParser/Area.h \
    XMLParser/Ba.h \
    XMLParser/BaPolygon.h \
    XMLParser/BaSector.h \
    XMLParser/Msaw.h \
    XMLParser/MsawPolygon.h \
    XMLParser/MsawSector.h \
    XMLParser/Qnh.h \
    XMLParser/QnhPolygon.h \
    XMLParser/QnhSector.h \
    XMLParser/Stca.h \
    XMLParser/StcaPolygon.h \
    XMLParser/StcaSector.h \
    XMLParser/xml_parser.h \
    lib/UserInfo.h \
    DAFIF/SqlGen_Dafif.h \
    DAFIF/tables.h \
    DAFIF/Dafif.h \
    lib/Lon.h \
    lib/ConvertDataInfo.h \
    lib/Lat.h \
    lib/LatLon.h \
    lib/FolderManager.h \
    RightLayout.h \
    LeftLayout.h \
    lib/Function.h \
    lib/Style.h \
    lib/StyleManager.h \
    mainwindow.h
SOURCES += PARSING/XML/XML_WRITER/RnavAid.cpp \
    PARSING/XML/XML_WRITER/Holding.cpp \
    PARSING/XML/XML_WRITER/XMLReadWriter.cpp \
    PARSING/XML/XML_IMPORT/XMLImport.cpp \
    PARSING/PARSER/MyLineEdit.cpp \
    PARSING/PARSER/DAFIF_PARSER.cpp \
    lib/Sql/AirspaceSqlGen.cpp \
    lib/Sql/SidStarSqlGen.cpp \
    lib/Sql/CoastSqlGen.cpp \
    lib/Sql/SqlGen.cpp \
    lib/DbManager.cpp \
    lib/LayerManager.cpp \
    DRAWING/DRAWING04/NodeInfo.cpp \
    Net/QtServer.cpp \
    THREAD/MyThread.cpp \
    DRAWING/DRAWING04/Node.cpp \
    DRAWING/DRAWING04/DrawingScene.cpp \
    DRAWING/DRAWING04/Window04.cpp \
    DRAWING/DRAWING04/DrawingExample_04.cpp \
    DRAWING/DRAWING03/RenderArea03.cpp \
    DRAWING/DRAWING03/Window03.cpp \
    DRAWING/DRAWING02/Window02.cpp \
    DRAWING/DRAWING02/RenderArea02.cpp \
    DRAWING/RenderArea.cpp \
    DRAWING/WINDOW.cpp \
    XMLParser/XMLParser.cpp \
    XMLParser/Aoi.cpp \
    XMLParser/AoiPolygon.cpp \
    XMLParser/AoiSector.cpp \
    XMLParser/Area.cpp \
    XMLParser/Ba.cpp \
    XMLParser/BaPolygon.cpp \
    XMLParser/BaSector.cpp \
    XMLParser/Msaw.cpp \
    XMLParser/MsawPolygon.cpp \
    XMLParser/MsawSector.cpp \
    XMLParser/Qnh.cpp \
    XMLParser/QnhPolygon.cpp \
    XMLParser/QnhSector.cpp \
    XMLParser/Stca.cpp \
    XMLParser/StcaPolygon.cpp \
    XMLParser/StcaSector.cpp \
    XMLParser/xml_parser.cpp \
    lib/UserInfo.cpp \
    DAFIF/SqlGen_Dafif.cpp \
    DAFIF/Dafif.cpp \
    lib/Lon.cpp \
    lib/ConvertDataInfo.cpp \
    lib/Lat.cpp \
    lib/LatLon.cpp \
    lib/FolderManager.cpp \
    RightLayout.cpp \
    LeftLayout.cpp \
    lib/Style.cpp \
    lib/StyleManager.cpp \
    main.cpp \
    mainwindow.cpp
FORMS += PARSING/PARSER/DAFIF_PARSER.ui \
    DRAWING/DRAWING04/NodeInfo.ui \
    XMLParser/XMLParser.ui \
    XMLParser/xml_parser.ui \
    rksiparser.ui
RESOURCES += images/rksiparser.qrc
CONFIG += thread
