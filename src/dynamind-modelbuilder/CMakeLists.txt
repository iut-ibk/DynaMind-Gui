INCLUDE_DIRECTORIES(../libdynamind-gui/)
INCLUDE_DIRECTORIES(.)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../libdynamind-gui)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/../libdynamind-gui)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/../dynamind-gui/)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../dynamind-gui)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

INCLUDE_DIRECTORIES(muparser/include ../3dparty/qwt-5.2.0/src)
FILE(GLOB DynaMind *.cpp)

IF(USEQT5)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
    QT5_ADD_RESOURCES(DynaMind icons.qrc)
ELSE()
    QT4_ADD_RESOURCES(DynaMind icons.qrc)
ENDIF()


ADD_EXECUTABLE( dynamind-gui ${DynaMind} )

IF(USEQT5)
    qt5_use_modules(dynamind-gui Widgets)
	if(WITH_VIEWER)
		add_definitions( -DVIEWER )
		TARGET_LINK_LIBRARIES(dynamind-gui dynamindgui dynamind-viewer ${DYNAMIND_CORE_LIBRARY} ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES})
	ELSE()
		TARGET_LINK_LIBRARIES(dynamind-gui dynamindgui ${DYNAMIND_CORE_LIBRARY} ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES})
	ENDIF()
ELSE()
    TARGET_LINK_LIBRARIES(dynamind-gui dynamindgui dynamind-viewer ${DYNAMIND_CORE_LIBRARY} ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES})
ENDIF()

INSTALL(TARGETS dynamind-gui
		DESTINATION "bin"
		PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
