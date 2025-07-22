#!/bin/bash
# shellcheck disable=SC2153
# shellcheck disable=SC2034

BROWSER=""
BROWSER_INCLUDE_DIR=""
BROWSER_LIB_DIR=""

set_build_type() {
    for arg in "$@"; do
        if [[ "$arg" == "Debug" || "$arg" == "Release" || "$arg" == "RelWithDebInfo" ]]; then
            BUILD_TYPE=$arg
        fi
    done
}

set_target_file() {
    if [[ -z "$1" ]]; then
        throw_error "A compilation target file MUST be provided"
        return 1
    fi
    local target_file
    target_file=$(resolve_path "$1")
    if [ ! -e "$target_file" ]; then
        throw_error "Target file $target_file not found"
        return 1
    fi
    BUILD_TARGET=$target_file
    return 0
}

is_target_file_set() {
    if [[ -z "$BUILD_TARGET" ]]; then
        throw_error "\
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

select_browser() {
    local browser_engine="$1"

    if [$browser_engine="Chromium"]; then
        BROWSER="Chromium"
        BROWSER_INCLUDE_DIR="$CHROMIUM_INCLUDE_DIR"
        BROWSER_LIB_DIR="$CHROMIUM_LIB_DIR"
        SYS_OPTS="-DCHROMIUM_INCLUDE_DIR=$BROWSER_INCLUDE_DIR \
    elif [$browser_engine="MSWebView2"]; then
        BROWSER="MSWebView"
        BROWSER_INCLUDE_DIR="$MSWEBVIEW_INCLUDE_DIR"
        BROWSER_LIB_DIR="$MSWEBVIEW_LIB_DIR"
        SYS_OPTS="-DMSWEBVIEW_INCLUDE_DIR=$BROWSER_INCLUDE_DIR \
    else
        throw_error "Unsupported browser engine option: $browser_engine. Program will terminate."
        return 1
    fi
}

is_browser_selected() {
    if [-z "$BROWSER"]; then
        throw_error "A browser must be selected. Program will terminate."
        return 1
    fi
    return 0
}
install_cef() {
    local nuget=$PACKAGE_DIR/bin/nuget.exe
    mkdir -p "$PACKAGE_DIR/bin"

    if [ ! -f "$nuget" ]; then
        curl -o "$nuget" https://dist.nuget.org/win-x86-commandline/latest/nuget.exe
    fi
    
    $nuget install chromiumembeddedframework.runtime -Version $CEF_VERSION -OutputDirectory "$PACKAGE_DIR"
    echo "CEF installed successfully"
}

install_mswebview2() {
    local nuget=$PACKAGE_DIR/bin/nuget.exe
    mkdir -p "$PACKAGE_DIR/bin"

    if [ ! -f "$nuget" ]; then
        curl -o "$nuget" https://dist.nuget.org/win-x86-commandline/latest/nuget.exe
    fi
    
    $nuget install Microsoft.Web.WebView2 -Version $MSWEBVIEW_VERSION -OutputDirectory "$PACKAGE_DIR"
    echo "MS WebView2 installed successfully"
}

check_env() {
    case "$(uname | tr '[:upper:]' '[:lower:]')" in
    msys* | cygwin* | mingw*)
        # Now check if the environment is MSYS2 UCRT
        if [ "$MSYSTEM" != "UCRT64" ]; then
            throw_error "\
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

throw_error() {
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
