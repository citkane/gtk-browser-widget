set(BROWSERS MSWEBVIEW2 WEBKIT CHROMIUM)

## Defines the embedded browser to use. One of [MSWEBVIEW2, WEBKIT, CHROMIUM] is allowed.
##
## If none is given, the default OS embedded browser will be defined:
## - Windows: MSWEBVIEW2
## - Linux: WEBKIT
## - Mac: WEBKIT
function (set_browser_definition)
    if(BROWSER=="mswebview2" AND NOT WIN32)
            message(FATAL_ERROR "MSWEBVIEW2 is only available on the Windows operating system")
        endif()
    # Compile the embedded browser type definition
    if(BROWSER=="mswebview2")
        target_compile_definitions(${PROJECT_NAME} PRIVATE GBW_MSWEBVIEW2=ON)
    endif()
    if(BROWSER=="webkit")
        target_compile_definitions(${PROJECT_NAME} PRIVATE GBW_WEBKIT=ON)
    endif()
    if(BROWSER=="chromium")
        target_compile_definitions(${PROJECT_NAME} PRIVATE GBW_CHROMIUM=ON)
    endif()
    # Set the default embedded browser if none has been defined
endfunction()

function (set_browser_linked_lib)
    if(WIN32)
        if("${BROWSER}" STREQUAL "mswebview2")
            set(BROWSER_LINKED_LIB "WebView2Loader.dll" PARENT_SCOPE)
        elseif("${BROWSER}" STREQUAL "chromium")
            set(BROWSER_LINKED_LIB "chromium.dll" PARENT_SCOPE)
        endif()
    elseif(APPLE)
        # Placeholder for Mac logic
    else() # Linux
        # Placeholder for Linux logic
    endif()
endfunction()
