#!/bin/bash
# shellcheck disable=SC2153
# shellcheck disable=SC2034
# shellcheck disable=SC2120

BROWSER=""
BROWSER_INCLUDE_DIR=""
BROWSER_LIB_DIR=""
NUGET=$PACKAGES_DIR/bin/nuget.exe


set_build_type() {
    for arg in "$@"; do
        if [[ "$arg" == "Debug" || "$arg" == "Release" || "$arg" == "RelWithDebInfo" ]]; then
            BUILD_TYPE=$arg
        fi
    done
}

set_target_file() {
    if [[ -z "$1" ]]; then
        print_error "A compilation target file MUST be provided"
        return 1
    fi
    local target_file
    target_file=$(resolve_path "$1")
    if [ ! -e "$target_file" ]; then
        print_error "Target file $target_file not found"
        return 1
    fi
    BUILD_TARGET=$target_file
    return 0
}

is_target_file_set() {
    if [[ -z "$BUILD_TARGET" ]]; then
        print_error "\
A build target file MUST be set.
Did you forget to call 'set_target'?"
    return 1
    fi
    return 0
}

set_install_dir() {
    if [[ -n "$1" ]]; then
        INSTALL_DIR=$1
    fi
}

is_build_clean() {
    local clean=false
    for arg in "$@"; do
        if [ "$arg" = "--clean" ]; then
            clean=true
        fi
    done
    echo $clean
}

is_generated() {
    if [ -f "$BUILD_DIR/CMakeCache.txt" ]; then
        echo true
    else
        echo false
    fi
}

set_sys_opts() {
    if [ "$BROWSER" = "chromium" ]; then
        SYS_OPTS="\
        -DBROWSER_INCLUDE_DIR=$PACKAGES_DIR/chromiumembeddedframework.runtime.$CHROMIUM_NUGET_V/build/native/include \
        -DBROWSER_LIB_DIR=$PACKAGES_DIR/chromiumembeddedframework.runtime.$CHROMIUM_NUGET_V/build/native/x64"
    elif [ "$BROWSER" = "mswebview2" ]; then
        SYS_OPTS="\
        -DBROWSER_INCLUDE_DIR=$PACKAGES_DIR/Microsoft.Web.WebView2.$MSWEBVIEW_NUGET_V/build/native/include \
        -DBROWSER_LIB_DIR=$PACKAGES_DIR/Microsoft.Web.WebView2.$MSWEBVIEW_NUGET_V/build/native/x64"
    elif [ "$BROWSER" = "webkit" ]; then
        SYS_OPTS="\
        -DBROWSER_INCLUDE_DIR=$PACKAGES_DIR/webkit-$WEBKIT_WPE_V/include \
        -DBROWSER_LIB_DIR=$PACKAGES_DIR/webkit-$WEBKIT_WPE_V/lib"
    fi
}

check_win_env() {
    case "$(uname | tr '[:upper:]' '[:lower:]')" in
    msys* | cygwin* | mingw*)

        if [ "$MSYSTEM" != "UCRT64" ]; then
            print_error "\
On Windows, you must use the MSYS2 UCRT64 terminal environment for this script.\n\
For download and installation, see: https://www.msys2.org"
            return 1
        fi
        ;;
    esac

    return 0;
}


resolve_path() {
  echo "$(cd -- "$(dirname -- "$1")" && pwd)/$(basename -- "$1")"
}

print_error() {
    printf 'ERROR: %s\n' "$1" >&2
    return 1
}

is_help() {
    local res="false"
    while [[ $# -gt 0 ]]; do
        case "$1" in
        -h | --help)
            res="true"
            ;;
        *) ;;
        esac
        shift
    done
    echo $res
}

print_help() {
    local message
    message=$(
        cat <<EOF
$1
EOF
    )
    echo "$message"
}
