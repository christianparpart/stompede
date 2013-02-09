add_definitions(
	-Dlinux=1
	-pthread
	-std=c++0x
	-DPACKAGE_NAME="${PACKAGE_NAME}"
	-DPACKAGE_VERSION="${PACKAGE_VERSION}"
	-DPACKAGE_HOMEPAGE_URL="${PACKAGE_HOMEPAGE_URL}"
	-DSYSCONFDIR="${SYSCONFDIR}"
	-DLOGDIR="${LOGDIR}"
	-DPLUGINDIR="${PLUGINDIR}"
	-DVERSION="${PACKAGE_VERSION}")

add_executable(stompeded main.cpp)
target_link_libraries(stompeded stompede)

install(TARGETS stompeded RUNTIME DESTINATION bin)
install(FILES stompeded.conf-dist DESTINATION ${SYSCONFDIR})
