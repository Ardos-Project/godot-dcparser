target_sources( ${PROJECT_NAME}
    PRIVATE
        GDDCParser.h
        GDDCParser.cpp
        DCFileResource.h
        DCFileResource.cpp
        DCFileLoader.h
        DCFileLoader.cpp
        GDDCFile.h
        GDDCFile.cpp
        GDDCClass.h
        GDDCClass.cpp
        GDDCField.h
        GDDCField.cpp
        GDDCUtils.h
        GDDCUtils.cpp
        Datagram.h
        Datagram.cpp
        DatagramIterator.h
        DatagramIterator.cpp
        UniqueIdAllocator.h
        UniqueIdAllocator.cpp
        RegisterExtension.cpp
)

target_include_directories( ${PROJECT_NAME}
    PRIVATE
        "src"
)

# Add dclass parser.
add_subdirectory(dclass)
