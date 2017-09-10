set(SOURCES FGPropertyManager.cpp            
            FGXMLElement.cpp          
            FGPropertyReader.cpp)

set(HEADERS FGPropertyManager.h
            FGXMLElement.h
            string_utilities.h)

add_full_path_name(INPUT_OUTPUT_SRC "${SOURCES}")
add_full_path_name(INPUT_OUTPUT_HDR "${HEADERS}")

#install(FILES ${HEADERS} DESTINATION include/JSBSim/input_output)

