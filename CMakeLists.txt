cmake_minimum_required(VERSION 3.8)
project(rdda_interface)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclpy REQUIRED )
find_package(std_msgs REQUIRED  )
find_package(sensor_msgs REQUIRED )
find_package(std_srvs REQUIRED  )
find_package(trajectory_msgs REQUIRED  )

find_package(rosidl_default_generators REQUIRED)

find_package(Boost COMPONENTS system python thread program_options REQUIRED)



rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/JointCommands.msg"
  "msg/JointStates.msg"
  "msg/ControlState.msg"
  "msg/RDDAPacket.msg"
  DEPENDENCIES std_msgs sensor_msgs # Add packages that above messages depend on, in this case geometry_msgs for Sphere.msg
)

#required to use messages in the project they are generated in 
rosidl_get_typesupport_target(cpp_typesupport_target
  ${PROJECT_NAME} rosidl_typesupport_cpp)











## Declare a C++/C library
add_library(${PROJECT_NAME}_src
    src/rdda_interface/shm.c
)

include_directories(
    include/rdda_interface
    ${Boost_INCLUDE_DIRS}
)


add_executable(test_node src/test_node.cpp)
target_include_directories(test_node PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
$<INSTALL_INTERFACE:include>)
target_compile_features(test_node PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17


add_executable(${PROJECT_NAME}_node src/rdda_interface/rdda_interface.cpp)
ament_target_dependencies(${PROJECT_NAME}_node
  rclcpp
  sensor_msgs
  std_msgs
  std_srvs
  trajectory_msgs
  )

target_link_libraries(${PROJECT_NAME}_node 
    ${Boost_LIBRARIES}
    ${PROJECT_NAME}_src
    ${cpp_typesupport_target} #required to use custom messages
    -lrt
)



install(TARGETS test_node ${PROJECT_NAME}_node
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_export_dependencies(rosidl_default_runtime rclcpp rclpy std_msgs)
ament_export_include_directories(include)

ament_package()
