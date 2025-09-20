# ROS2-Tutorial

## Cấu trúc của file CMakeList :
1. Cấu hình chung
2. Khai báo phụ thuộc ( dependencies )
3. Tạo thư viện 
5. Tạo node ROS 2
6. Cài đặt (install)
7. ament_package() để ROS 2 build và export

### 1. Cấu hình cơ bản 
```cmake
cmake_minimum_required(VERSION 3.8)
project(gamepad_interface)
```
Cmake cần phải có phiên bản từ 3.8 trở lên mới có thể chạy được file này.\
`project(gamepad_interface)` để khai báo tên project là package trong ROS.\

```cmake
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()
```
Cấu hình compiler để giúp code sạch cảnh báo.

### 2. Khai báo các dependencies 
```cmake
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(robot_interfaces REQUIRED)

include_directories(include)
```
* `find_package` → tìm các package ROS 2 cần thiết:\
  `ament_cmake` (build system ROS 2).
  `rclcpp` (thư viện C++ client API cho ROS 2).
  `robot_interfaces` (package custom, chứa msg/srv cho robot).
* `include_directories(include)` → thêm thư mục include/ để các file .hpp có thể được tìm thấy bởi #include.

  ### 3. Thêm thư viện driver
```cmake
add_library(gamepad_driver
  src/gamepad.cpp
)
target_compile_features(gamepad_driver PUBLIC cxx_std_11)
```
`add_library(gamepad_driver src/gamepad.cpp)` → tạo thư viện gamepad_driver từ file gamepad.cpp.

`target_compile_features(... cxx_std_11)` → dùng chuẩn C++11.

📌 Đây là thư viện xử lý mức thấp, đọc tín hiệu từ /dev/input/eventNN.
```cmake
add_library(robot_input_mapper
  src/mapper.cpp
)
target_link_libraries(robot_input_mapper
  gamepad_driver
)
ament_target_dependencies(robot_input_mapper
  robot_interfaces
)
target_compile_features(robot_input_mapper PUBLIC cxx_std_11)

```


`target_link_libraries` vì thư viện robot_input_mapper phụ thuộc vào thư viện gamepad_driver .\
Khi khai báo thư viện phụ thuộc như vậy cần đảm bảo đã khai báo thư viện riêng trước.\
`ament_target_dependencies(... robot_interfaces)` → dùng message/interface từ package robot_interfaces.

### Node ROS2
```cmake
add_executable(gamepad_node
  src/gamepad_node.cpp
)
target_link_libraries(gamepad_node
  robot_input_mapper
  gamepad_driver
)
ament_target_dependencies(gamepad_node
  rclcpp
  robot_interfaces
)
target_compile_features(gamepad_node PUBLIC cxx_std_11)
```
`add_executable( )` để tạo node từ file cpp.\
Sau đó `target_link_libraries` để khai báo thư viện mà node phụ thuộc.\
Khai báo node phụ thuộc: `ament_target_dependencies` → node phụ thuộc vào rclcpp (API ROS2) và robot_interfaces.\
Cài đặt node :
```cmake
install(DIRECTORY include/
        DESTINATION include)

install(TARGETS
  gamepad_driver
  robot_input_mapper
  gamepad_node
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)
```


