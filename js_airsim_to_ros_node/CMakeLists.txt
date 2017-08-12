cmake_minimum_required(VERSION 2.8.3)
project(js_airsim_to_ros_node)

find_package(catkin REQUIRED COMPONENTS
  roscpp
)

find_package(js_airsim_to_ros_library REQUIRED)

###################################
## catkin specific configuration ##
###################################

catkin_package(
  CATKIN_DEPENDS js_airsim_to_ros_library
)

###########
## Build ##
###########

include_directories(${js_airsim_to_ros_library_INCLUDE_DIRS})

add_executable(airsim_to_ros_node src/airsim_to_ros_node.cpp)

target_link_libraries(airsim_to_ros_node
  ${js_airsim_to_ros_library_LIBRARIES}
  ${catkin_LIBRARIES}
)

#############
## Install ##
#############

install(TARGETS airsim_to_ros_node
   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

#############
## Testing ##
#############

if(CATKIN_ENABLE_TESTING)
  find_package(rostest REQUIRED)
  add_rostest_gtest(test_airsim_to_ros_node test/test_airsim_to_ros_node.test test/test_airsim_to_ros_node.cpp)
  target_link_libraries(test_airsim_to_ros_node ${catkin_LIBRARIES} ${js_airsim_to_ros_node_LIBRARIES})
endif()