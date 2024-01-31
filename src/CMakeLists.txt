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
        RegisterExtension.cpp
)

target_include_directories( ${PROJECT_NAME}
    PRIVATE
        "src"
)

# Add dclass parser.
add_subdirectory(dclass)
