configure_file(tree.png tree.png COPYONLY)

add_executable(gx-example
    main.cpp
)
target_link_libraries(gx-example PRIVATE gx)
