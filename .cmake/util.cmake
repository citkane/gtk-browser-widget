set(BROWSERS MSWEBVIEW2 WEBKIT CHROMIUM)

## Defines the embedded browser to use. One of [MSWEBVIEW2, WEBKIT, CHROMIUM] is allowed.
##
## Defines the embedded browser to use. One of [MSWEBVIEW2, WEBKIT, CHROMIUM] is allowed.
## Based on value of $BROWSER.

function (set_browser_definition)
    if("${BROWSER}" STREQUAL "chromium")
        target_compile_definitions(${PROJECT_NAME} PRIVATE GBW_CHROMIUM=ON)
    elseif("${BROWSER}" STREQUAL "mswebview2")
        if(NOT WIN32)
            message(FATAL_ERROR "MSWEBVIEW2 is only available on the Windows operating system.")
        endif()
        target_compile_definitions(${PROJECT_NAME} PRIVATE GBW_MSWEBVIEW2=ON)
    elseif("${BROWSER}" STREQUAL "webkit")
        target_compile_definitions(${PROJECT_NAME} PRIVATE GBW_WEBKIT=ON)
    endif()
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
