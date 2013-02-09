add_definitions(
	-Dlinux=1
	-Wall -Wno-deprecated
	-pthread
	-std=c++0x
	-DVERSION="${PACKAGE_VERSION}")

add_definitions(-fvisibility=hidden -fvisibility-inlines-hidden -DBUILD_STOMPEDE=1)

file(GLOB stompede_stomp_SRC "stomp/*.cpp")
file(GLOB stompede_base_SRC "*.cpp")
add_library(stompede SHARED ${stompede_base_SRC} ${stompede_stomp_SRC})
target_link_libraries(stompede pthread ${EV_LIBRARIES})
set_target_properties(stompede PROPERTIES VERSION ${PACKAGE_VERSION})
install(TARGETS stompede DESTINATION ${LIBDIR})
