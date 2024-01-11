target_sources( ${PROJECT_NAME}
    PRIVATE
        Example.h
        Example.cpp
        GDExtensionTemplate.h
        GDExtensionTemplate.cpp
        RegisterExtension.cpp
)

target_include_directories( ${PROJECT_NAME}
    PRIVATE
        "src"
)

# Add dclass parser.
add_subdirectory(dclass)
include_directories(dclass)
