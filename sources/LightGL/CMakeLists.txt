addModule()

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${ModuleName} PUBLIC glfw)

find_package(Vulkan REQUIRED)
target_link_libraries(${ModuleName} PUBLIC Vulkan::Vulkan)