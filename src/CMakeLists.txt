AUX_SOURCE_DIRECTORY(./ SRC_LIST)
ADD_EXECUTABLE(a ${SRC_LIST})
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include/3rd-party)

#pcl start
FIND_PACKAGE(VTK REQUIRED)
include(${VTK_USE_FILE})

if(VTK_LIBRARIES)
	target_link_libraries(a ${VTK_LIBRARIES})
else()
	target_link_libraries(a vtkHybrid vtkWidgets)
endif()
#pcl end

