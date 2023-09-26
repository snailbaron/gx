configure_file(cursor.png cursor.png COPYONLY)
configure_file(grass.png grass.png COPYONLY)
configure_file(hero.png hero.png COPYONLY)
configure_file(stone.png stone.png COPYONLY)
configure_file(tree.png tree.png COPYONLY)

add_executable(gx-example
    main.cpp
)
target_link_libraries(gx-example PRIVATE gx)
