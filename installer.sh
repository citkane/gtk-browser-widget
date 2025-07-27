#!/bin/bash
# shellcheck disable=SC1091
# shellcheck disable=SC2034
# shellcheck disable=SC2120

## Note:
## Bash ≥ 4.0 required
## We should implement a check, especially for older Macs < 2019

BUILD_TYPE=Debug
BUILD_GENERATOR="Ninja Multi-Config"
PROJECT_NAME=gbw

BUILD_DIR=$(pwd)/.build
BUILD_TARGET=""
INSTALL_DIR=$(pwd)/.dist
PACKAGES_DIR=$(pwd)/.packages

BROWSER_OPTIONS="<chromium|webkit|mswebview2>"
BROWSER_DEFAULT="chromium"

WIN_COMPILER_DEFAULT="UCRT"

source ./.scripts/util.sh
source ./.scripts/packages.sh
source ./.scripts/init.sh

SELECT_BROWSER_HELP="\
Opens a dialogue to select the browser engine

Options:
    chromium                                Use Chromium Embedded Framework (CEF)
    webkit                                                             Use Webkit
    mswebview2                                     Use MS WebView2 (Windows only)
Default:
    $BROWSER_DEFAULT
Usage:
    select_browser"
select_browser() {
    if is_help "$@"; then
        print_help "$SELECT_BROWSER_HELP"
        return 0
    fi

    prompt_browser
}

PACKAGE_INSTALL_HELP="\
Installs required browser library packages.
ALPHA stage: MsWebview2 on Windows OS is functional.

Usage:
 packages_install"
packages_install() {
    if is_help "$@"; then
        print_help "$PACKAGE_INSTALL_HELP"
        return 0
    fi
    if [ -z "$BROWSER" ]; then 
        select_browser
    fi
    
    if [ "$BROWSER" = "chromium" ]; then
        install_cef
    elif [ "$BROWSER" = "mswebview2" ]; then
        install_mswebview2
    elif [ "$BROWSER" = "webkit" ]; then
        install_webkit
    fi
}

SET_TARGET_HELP="\
Opens a dialogue to sets the path to the compilation target

Options:
    <path>  # The relative path to the compilation target (required)
Default:
    examples/$BROWSER_OPTIONS/main.cc
Usage:
    set_target"
set_target(){
    if is_help "$@"; then
        print_help "$SET_TARGET_HELP"
        return 0
    fi
        
    if [ -z "$BROWSER" ]; then 
        select_browser
    fi    
    prompt_build_target
}

GENERATE_HELP="\
Generates a clean build definition

Usage:
    generate"
generate() {
    if is_help "$@"; then
        print_help "$GENERATE_HELP"
        return 0
    fi
    if [ -z "$BROWSER" ]; then 
        select_browser
    fi
    if [ -z "$BUILD_TARGET" ]; then 
        set_target 
    fi
    rm -rf "$BUILD_DIR"
    set_sys_opts

    # shellcheck disable=SC2086
    cmake . -G "$BUILD_GENERATOR" -B "$BUILD_DIR" -S . \
        -DPROJECT_NAME=$PROJECT_NAME \
        -DBUILD_TARGET="$BUILD_TARGET" \
        -DBROWSER="$BROWSER" \
        $SYS_OPTS

    mv "$BUILD_DIR/compile_commands.json" ./
}

BUILD_HELP="\
Builds the target binary

Options:
    --clean                         # Generate a clean definition before building (optional)
    <Debug|Release|RelWithDebInfo>  # The build type (optional)
Default:
    Debug
Usage:
    build
    build --clean
    build <Debug|Release|RelWithDebInfo>
    build <Debug|Release|RelWithDebInfo> --clean"
build() {
    if is_help "$@"; then
        print_help "$BUILD_HELP"
        return 0
    fi

    rm -rf "$INSTALL_DIR"

    if is_build_clean "$@"  ||  ! is_generated; then
        generate "$@"
    fi
    set_build_type "$@"
    cmake --build "$BUILD_DIR" --config "$BUILD_TYPE"
}

INSTALL_HELP="\
Installs the built binary and required libs

Options:
    <dir>   # The installation directory. (optional) 
Default:
    <project_root>/.dist
Usage:
    install
    install <dir>"
install() {
    if is_help "$@"; then
        print_help "$INSTALL_HELP"
        return 0
    fi

    set_install_dir "$1"
    rm -rf "$INSTALL_DIR"
    cmake --install "$BUILD_DIR" --config "$BUILD_TYPE" --prefix "$INSTALL_DIR"

    echo "Installed to: $INSTALL_DIR"
}

RUN_HELP="\
Runs the installed binary

Options:
    <dir>   # The installation directory (optional) 
Default:
    <project_root>/.dist
Usage:
    run
    run <dir>"
run() {
    if is_help "$@"; then
        print_help "$RUN_HELP"
        return 0
    fi

    set_install_dir "$1"
    local executable=$INSTALL_DIR/bin/$PROJECT_NAME$(exe_ext)

    if [ ! -f "$executable" ]; then
        install "$@"
    fi

    $executable
}

print_gbw_header() {
    cat <<EOF
####################################################################################
#           Welcome to the Gtk Browser Widget installer script!                    #
####################################################################################

EOF
}

help() {
    cat <<EOF
Basic usage:
------------------------------------------------------------------------------------
$ source installer.sh   # Creates the installer session environment       (required)

$ --help                # Prints help (this)
$ select_browser        # Select a browser engine
$ packages_install      # Install the required packages
$ set_target            # Set the compilation target file
$ generate              # Generate a clean build definition
$ build                 # Build the executable binary
$ install               # Install the built binary
$ run                   # Run the installed binary

$ <command> <-h|--help> # Prints detailed help for the command

EOF
}



print_gbw_sourced() {
    cat <<EOF
####################################################################################
#               The install script has been sourced.                               #
#               Run instructions directly from your terminal                       #
####################################################################################

EOF
}

is_interactive() {
    if [[ "${BASH_SOURCE[0]}" != "${0}" ]]; then
        return 0
    else
        return 1
    fi
}

if is_interactive; then
    print_gbw_header
    if ! source ./.scripts/os.sh; then
        return 1
    fi
    init_prompts
    print_gbw_sourced
    help
else
    help
fi
