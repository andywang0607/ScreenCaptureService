
set(targets
    screencapture
    messagehelper
    threadsafequeue
)
foreach(target ${targets})
    add_subdirectory(${target})
endforeach(target)