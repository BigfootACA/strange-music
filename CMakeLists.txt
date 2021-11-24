cmake_minimum_required(VERSION 3.0)
project(strange-music)
set(CMAKE_INSTALL_PREFIX /usr)
add_executable(
	strange-music
	strange-music.c
)
install(
	TARGETS strange-music
	RUNTIME DESTINATION bin
)
install(
	FILES strange-music.service strange-music.timer
	DESTINATION lib/systemd/system
)
