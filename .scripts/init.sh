#!/bin/bash
# shellcheck disable=SC1091

init_prompts() {
    prompt_browser
    prompt_build_target
}

prompt_browser() {
    local options
    if [ "$OS" = "windows" ]; then
        options=("chromium" "webkit" "mswebview2")
    else
        options=("chromium" "webkit")
    fi
    local BROWSER_DEFAULT="chromium"
    local input
    local opt

    while true; do
        echo "Available browser options are:"
        for i in "${!options[@]}"; do
            printf "%d) %s\n" $((i+1)) "${options[i]}"
        done
        read -r -p "Please select a number or press 'enter' for the default <$BROWSER_DEFAULT>: " input

        if [ -z "$input" ]; then
            opt="$BROWSER_DEFAULT"
            break
        elif [[ "$input" =~ ^[1-9][0-9]*$ ]] && [ "$input" -ge 1 ]  && [ "$input" -le "${#options[@]}" ]; then
            opt="${options[$((input-1))]}"
            break
        else
            echo "Invalid selection: '$input'. Please try again."
        fi
    done

    select_browser_imp "$opt"                             
}

prompt_build_target() {
    local build_target
    
    if [ -z "$BUILD_TARGET" ]; then
        build_target="examples/$BROWSER/main.cc"
        echo "The default build target is: $build_target"
    else
        build_target="$BUILD_TARGET"
        echo "The current build target is: $BUILD_TARGET"
    fi
    echo "Enter the path to your build target or press 'enter' to continue:"
    read -r -e -p "Relative path: " -i "" user_target

    if [ -z "$user_target" ]; then
        set_target_imp "$build_target"
        return 0;
    fi
    set_target_imp "$user_target"
}

select_browser_imp() {

    if [ -z "$1" ]; then 
        BROWSER=$BROWSER_DEFAULT
        echo -e "Using the default browser '$BROWSER'\n"
        return 0
    fi

    if [ "$1" = "mswebview2" ]; then
        BROWSER="$1"
    elif [ "$1" = "chromium" ]; then
        BROWSER="$1"
    elif [ "$1" = "webkit" ]; then
        BROWSER="$1"
    fi

    echo -e "'$BROWSER' browser selected\n"
}

set_target_imp(){
    local target_file
    local resolved_target_file
           
    if [[ -z "$1" ]]; then
        target_file=$(default_target "$BROWSER")
    else
        target_file="$1"        
    fi
    resolved_target_file=$(resolve_path "$1")
    
    if [ ! -e "$resolved_target_file" ]; then
        print_error "Target file '$target_file' not found"
        prompt_build_target
    else
        BUILD_TARGET=$resolved_target_file
        echo -e "The build target was set to:\n$BUILD_TARGET\n"
    fi
}




