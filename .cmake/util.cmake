set(BROWSERS MSWEBVIEW2 WEBKIT CHROMIUM)

## Defines the embedded browser to use. One of [MSWEBVIEW2, WEBKIT, CHROMIUM] is allowed.
##
## If none is given, the default OS embedded browser will be defined:
## - Windows: MSWEBVIEW2
## - Linux: WEBKIT
## - Mac: WEBKIT
function (set_browser_definition)
    set(count 0)
    # Get a count of how many embedded browsers have been defined
    foreach(browser IN LISTS BROWSERS)
        if(DEFINED "${browser}")
            math(EXPR count "${count} + 1")
        endif()
    endforeach()
    # Only one embedded browser type is allowed, so throw error if more are defined.
    if("${count}" GREATER 1)
        string(JOIN "," csv_list ${BROWSERS})
        message(FATAL_ERROR "More than one embbeded browser was defined. Define ONE of: [${csv_list}]")
    endif()
    # Proceed if only one embedded browser has been defined
    if("${count}" GREATER 0)
        # Throw fatal error if MSWEBVIEW2 is defined for a non Windows OS
        if(MSWEBVIEW2 AND NOT WIN32)
            message(FATAL_ERROR "MSWEBVIEW2 is only available on the Windows operating system")
        endif()
        # Compile the embedded browser type definition
        if(MSWEBVIEW2)
            target_compile_definitions(${PROJECT_NAME} PRIVATE EBW_MSWEBVIEW2=ON)
        endif()
        if(WEBKIT)
            target_compile_definitions(${PROJECT_NAME} PRIVATE EBW_WEBKIT=ON)
        endif()
        if(CHROMIUM)
            target_compile_definitions(${PROJECT_NAME} PRIVATE EBW_CHROMIUM=ON)
        endif()
    # Set the default embedded browser if none has been defined
    elseif(WIN32)
        target_compile_definitions(${PROJECT_NAME} PRIVATE EBW_MSWEBVIEW2=ON)
    else()
        target_compile_definitions(${PROJECT_NAME} PRIVATE EBW_WEBKIT=ON)
    endif()
endfunction()

function (set_browser_linked_lib)
    # Determine which browser is active
    set(active_browser "")
    foreach(browser IN LISTS BROWSERS)
        if(DEFINED "${browser}")
            set(active_browser "${browser}")
            break()
        endif()
    endforeach()
    
    # Set default browser to Chromium if none defined
    if("${active_browser}" STREQUAL "")
        set(active_browser "chromium")
    endif()
    if(WIN32)
        if("${active_browser}" STREQUAL "MSWEBVIEW2")
            set(BROWSER_LINKED_LIB "WebView2Loader.dll" PARENT_SCOPE)
        elseif("${active_browser}" STREQUAL "CHROMIUM")
            set(BROWSER_LINKED_LIB "chromium.dll" PARENT_SCOPE)
        endif()
    elseif(APPLE)
        pass
    else() # Linux
        elseif("${active_browser}" STREQUAL "CHROMIUM")
            set(BROWSER_LINKED_LIB "libchromium.so" PARENT_SCOPE)
        endif()
    endif()
endfunction()