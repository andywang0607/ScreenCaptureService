
set(targets
    screencapture
)
foreach(target ${targets})
    add_subdirectory(${target})
endforeach(target)