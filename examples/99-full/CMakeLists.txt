configure_file(tree.png tree.png COPYONLY)
configure_file(grass.png grass.png COPYONLY)

add_executable(gx-example
    main.cpp
)
target_link_libraries(gx-example PRIVATE gx)
