add_library(rapidxml rapidxml.hpp rapidxml_utils.hpp rapidxml_print.hpp rapidxml_iterators.hpp)
set_target_properties(rapidxml PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(rapidxml PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
