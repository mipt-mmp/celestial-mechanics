
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()

qt_add_executable(celestial
    main.cpp
    mainwindow.cpp
    mainwindow.hpp
)

target_link_libraries(celestial PRIVATE phys Qt6::Core Qt6::Widgets)
