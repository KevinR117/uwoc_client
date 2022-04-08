cmake_minimum_required(VERSION 3.16)
project(uwoc_client)

set(CMAKE_CXX_STANDARD 14)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED)
find_package(OpenCV REQUIRED)

#add_executable(test_client main.cpp client.cpp buffer.cpp client_1.h client_1.cpp client_2.h client_2.cpp)
#target_include_directories(test_client PUBLIC ${Boost_INCLUDE_DIRS})
#target_include_directories(test_client PUBLIC ${OpenCV_INCLUDE_DIRS})
#target_link_libraries(test_client PUBLIC ${Boost_LIBRARIES})
#target_link_libraries(test_client PUBLIC ${CMAKE_THREAD_LIBS_INIT})
#target_link_libraries(test_client PUBLIC ${OpenCV_LIBS})

#add_executable(test_client_1 main.cpp buffer.cpp client_1.cpp)
#target_include_directories(test_client_1 PUBLIC ${Boost_INCLUDE_DIRS})
#target_include_directories(test_client_1 PUBLIC ${OpenCV_INCLUDE_DIRS})
#target_link_libraries(test_client_1 PUBLIC ${Boost_LIBRARIES})
#target_link_libraries(test_client_1 PUBLIC ${CMAKE_THREAD_LIBS_INIT})
#target_link_libraries(test_client_1 PUBLIC ${OpenCV_LIBS})

add_executable(uwoc_client main.cpp buffer.cpp client.cpp)
target_include_directories(uwoc_client PUBLIC ${Boost_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})
target_link_libraries(uwoc_client PUBLIC ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${OpenCV_LIBS})

#add_executable(test main_test.cpp buffer.cpp)