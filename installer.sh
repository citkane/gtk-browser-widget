#!/bin/bash
# shellcheck disable=SC1091
# shellcheck disable=SC2034

BUILD_TYPE=Debug
BUILD_GENERATOR="Ninja Multi-Config"
PROJECT_NAME=gbw

BUILD_DIR=$(pwd)/.build
BUILD_TARGET=""
INSTALL_DIR=$(pwd)/.dist
PACKAGE_DIR=$(pwd)/.packages

MSWEBVIEW_NUGET_V=1.0.3351.48
CHROMIUM_NUGET_V=138.0.17
BROWSER_OPTIONS="<chromium|mswebview2>"
SYS_OPTS=""

PATH=$(cygpath -w /ucrt64/bin):$PATH
export PATH
source ./.scripts/util.sh
# For Windows, check that we are running in a MSYS2 UCRT environment
check_env


SELECT_BROWSER_HELP="\
#  Selects the browser engine to use
## Options:
#      chromium                                                 Use Chromium Embedded Framework (CEF)
#      mswebview2                                               Use Microsoft WebView2 (Windows only)
## Default:
#      chromium
## Usage:
#      select_browser $BROWSER_OPTIONS                          
"
select_browser() {
    if [ "$(is_help "$@")" = true ]; then
        print_help "$SELECT_BROWSER_HELP"
        [ $PS1 ] && return 0 || exit 0
    fi

    select_browser_caller "$1"

}

PACKAGE_INSTALL_HELP="\
#  Installs required library packages.
#  ALPHA stage: Supports MsWebview2 and Chromium on Windows OS.

## Usage:
#   packages_install
"

packages_install() {
    if [ "$(is_help "$@")" = true ]; then
        print_help "$PACKAGE_INSTALL_HELP"
        [[ $PS1 ]] && return 0 || exit 0
    fi
    select_browser
    verify_nuget
    if [ "$BROWSER" = "chromium" ]; then
        install_cef
    elif [ "$BROWSER" = "mswebview2" ]; then
        install_mswebview2
    fi
}


SET_TARGET_HELP="\
#  Sets the path to the compilation target
## Usage:
#      set_target <rel/path/to/main.cc>                                   (required)
"
set_target(){
    set_target_file "$1"
}

GENERATE_HELP="\
#  Generates a clean build definition
## Usage:
#      generate\
"
generate() {
    if [ "$(is_help "$@")" = true ]; then
        print_help "$GENERATE_HELP"
        [[ $PS1 ]] && return 0 || exit 0
    fi
    if ! is_target_file_set; then 
        return 1 
    fi
    if [ -z $BROWSER ]; then 
        select_browser
    fi


    rm -rf "$BUILD_DIR"
    # shellcheck disable=SC2086
    cmake . -G "$BUILD_GENERATOR" -B "$BUILD_DIR" -S . \
        -DPROJECT_NAME=$PROJECT_NAME \
        -DBUILD_TARGET="$BUILD_TARGET" \
        $SYS_OPTS

    mv "$BUILD_DIR/compile_commands.json" ./
}

BUILD_HELP="\
#  Builds the target binary
## Options:
#      <--clean> Generate a clean definition before building              (optional)
#      <[Debug, Release, RelWithDebInfo]> The build type.                 (optional)
#          Default: Debug
#
## Usage:
#      build <[Debug, Release, RelWithDebInfo]> <--clean>\
"
build() {
    if [ "$(is_help "$@")" = true ]; then
        print_help "$BUILD_HELP"
        [[ $PS1 ]] && return 0 || exit 0
    fi

    rm -rf "$INSTALL_DIR"

    if [ "$(is_build_clean "$@")" = true ] || [ "$(is_generated)" = false ]; then
        generate "$@"
    fi
    set_build_type "$@"
    cmake --build "$BUILD_DIR" --config "$BUILD_TYPE"
}

INSTALL_HELP="\
#  Installs the built binary and required libs
## Options:
#      <path/to/install/dir> The installation path.                       (optional) 
#          Default: <project_root>/.dist
#
## Usage:
#      install <path/to/install/dir>\
"
install() {
    if [ "$(is_help "$@")" = true ]; then
        print_help "$INSTALL_HELP"
        [[ $PS1 ]] && return 0 || exit 0
    fi

    set_install_dir "$1"
    rm -rf "$INSTALL_DIR"
    cmake --install "$BUILD_DIR" --config "$BUILD_TYPE" --prefix "$INSTALL_DIR"

    echo "Installed to: $INSTALL_DIR"
}

RUN_HELP="\
#  Runs the installed binary
## Options:
#      <path/to/install/dir> The installation path.                       (optional)
#          Default: <project_root>/.dist
#
## Usage:
#      run <path/to/install/dir>\
"
run() {
    if [ "$(is_help "$@")" = true ]; then
        print_help "$RUN_HELP"
        [[ $PS1 ]] && return 0 || exit 0
    fi

    set_install_dir "$1"
    local executable=$INSTALL_DIR/bin/$PROJECT_NAME.exe
    if [ ! -f "$executable" ]; then
        install "$@"
    fi
    $executable
}

HELP=$(
    cat <<EOF
####################################################################################
#           Welcome to the Gtk Browser Widget installer script!                    #
####################################################################################

Basic usage:
------------------------------------------------------------------------------------
$ source installer.sh   # Creates the installer session environment       (required)

$ --help                # Prints help (this)
$ select_browser        # Select browser engine
$ packages_install      # Installs required packages
$ set_target            # Set compilation target file
$ generate              # Generate a clean build definition
$ build                 # Builds the binary
$ install               # Installs the built binary
$ run                   # Runs the installed binary

$ <command> <-h --help> # Prints help for the command

Advanced usage:
------------------------------------------------------------------------------------
## select_browser: ################################################################
$SELECT_BROWSER_HELP
####################################################################################

## set_target: ####################################################################
$SET_TARGET_HELP
####################################################################################

## packages_install: ###############################################################
$PACKAGE_INSTALL_HELP
####################################################################################

## generate: #######################################################################
$GENERATE_HELP
####################################################################################

## build: ##########################################################################
$BUILD_HELP
####################################################################################

## install: ########################################################################
$INSTALL_HELP
####################################################################################

## run: ############################################################################
$RUN_HELP
####################################################################################
EOF
)

help() {
    echo "$HELP"
}

help

if [[ "${BASH_SOURCE[0]}" != "${0}" ]]; then
    cat <<EOF

####################################################################################
#               The install script has been sourced.                               #
#               Run the commands directly from your terminal                       #
####################################################################################

EOF
fi

if [ "$(is_help "$@")" = true ]; then
    help
fi
