
set(targets
    screencapture
    messagehelper
)
foreach(target ${targets})
    add_subdirectory(${target})
endforeach(target)