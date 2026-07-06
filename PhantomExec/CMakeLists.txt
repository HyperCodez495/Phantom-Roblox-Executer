cmake_minimum_required(VERSION 3.20)
project(PhantomExecutor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

# ============ Main DLL ============
add_library(Phantom SHARED
    PhantomCore.cpp
    LuauHooker.cpp
    UNCValidator.cpp
    ByfronBypass.cpp
    IPCServer.cpp
    dllmain.cpp
)

# Include directories
target_include_directories(Phantom PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link libraries
target_link_libraries(Phantom PRIVATE
    kernel32
    user32
    ws2_32
    ntdll
    advapi32
)

# Compiler flags for MSVC
if(MSVC)
    target_compile_options(Phantom PRIVATE
        /W4                 # Warning level 4
        /WX                 # Warnings as errors
        /O2                 # Optimize for speed
        /Oy-                # Disable frame pointer omission
        /fp:precise         # Precise floating point
        /Zc:inline          # Remove unreferenced functions
    )
    
    # Linker flags
    set_target_properties(Phantom PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:DllMain"
    )
endif()

# Set output directory
set_target_properties(Phantom PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Release"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Release"
)

# ============ Injector Executable ============
add_executable(PhantomInjector
    Injector/main.cpp
)

target_include_directories(PhantomInjector PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(PhantomInjector PRIVATE
    kernel32
    user32
)

set_target_properties(PhantomInjector PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Release"
)

# ============ Unit Tests ============
enable_testing()

add_executable(PhantomTests
    Tests/test_luau_hooker.cpp
    Tests/test_unc_validator.cpp
    Tests/test_byfron_bypass.cpp
    Tests/test_ipc_server.cpp
    LuauHooker.cpp
    UNCValidator.cpp
    ByfronBypass.cpp
    IPCServer.cpp
)

target_include_directories(PhantomTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(PhantomTests PRIVATE
    kernel32
    user32
    ws2_32
)

add_test(NAME LuauHookerTests COMMAND PhantomTests)
add_test(NAME UNCValidatorTests COMMAND PhantomTests)
add_test(NAME ByfronBypassTests COMMAND PhantomTests)
add_test(NAME IPCServerTests COMMAND PhantomTests)

# ============ Installation ============
install(TARGETS Phantom PhantomInjector
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES
    PhantomCore.h
    LuauHooker.h
    UNCValidator.h
    ByfronBypass.h
    IPCServer.h
    DESTINATION include
)

# ============ Summary ============
message(STATUS "")
message(STATUS "Phantom Executor Build Configuration")
message(STATUS "====================================")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "")
message(STATUS "Targets:")
message(STATUS "  - Phantom (DLL)")
message(STATUS "  - PhantomInjector (EXE)")
message(STATUS "  - PhantomTests (Tests)")
message(STATUS "")
message(STATUS "Output Directory: ${CMAKE_BINARY_DIR}/Release")
message(STATUS "")
