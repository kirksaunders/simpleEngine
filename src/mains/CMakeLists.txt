add_library(MAINS_SRC INTERFACE IMPORTED)
target_sources(MAINS_SRC
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/main1.cpp
        ${CMAKE_CURRENT_LIST_DIR}/main2.cpp
        ${CMAKE_CURRENT_LIST_DIR}/main3.cpp
        ${CMAKE_CURRENT_LIST_DIR}/main4.cpp
        ${CMAKE_CURRENT_LIST_DIR}/main5.cpp
        ${CMAKE_CURRENT_LIST_DIR}/main6.cpp
        ${CMAKE_CURRENT_LIST_DIR}/main7.cpp
)