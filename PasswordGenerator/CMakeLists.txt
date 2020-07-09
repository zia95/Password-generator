cmake_minimum_required(VERSION 3.0)

#lib
#project(calc LANGUAGES C)
#add_library(calc calc.c)





#exe
project(passgen LANGUAGES C)

add_executable(passgen main.c)
set_property(TARGET passgen PROPERTY C_STANDARD 11)


#target_link_libraries(cmakhw calc)

#install(TARGETS cmakhw RUNTIME DESTINATION bin)
