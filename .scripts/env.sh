WEBKIT_REPO_DIR=$(resolve_rel_path "$ROOT_DIR/../../clone/Webkit")

function set_msvc() {
    local vs_path=$(get_msvc_root) || return 1
    vs_path=$(resolve_rel_path "$vs_path")


    echo "$vs_path"
    echo "$WEBKIT_REPO_DIR"
    echo "$(get_msvc_root)"
    echo "$(get_win_temp)"

  #cmd.exe //c "\"$vs_path\\Common7\\Tools\\VsDevCmd.bat\" && bash"
}


function set_win_webkit_env(){
    local webkit_libraries="$WEBKIT_REP_DIR/WebKitLibraries/win"

    PATH="/c/xampp/apache/bin:$PATH"
    PATH="/c/xampp/perl/bin:$PATH"
    PATH="$(resolve_rel_path \"$PROGRAMFILES\")/Cmake/bin:$PATH"
    PATH="$webkit_libraries:$PATH"

    export PATH
    export WEBKIT_TESTFONTS="$WEBKIT_REP_DIR/Tools\WebKitTestRunner\fonts"
    export DUMPRENDERTREE_TEMP="$(get_win_temp)"

    
}

function get_msvc_root() {
    local vswhere="C:\\Program Files (x86)\\Microsoft Visual Studio\\Installer\\vswhere.exe"
    local vs_path=$(powershell "& \"$vswhere\" -latest -property installationPath")
    if [[ -z "$vs_path" ]]; then
        print_error "Visual Studio installation not found"
        return 1
    fi
    echo $(win_to_unixpath "$vs_path")
}

function get_win_temp() {
    local temp_path=$(powershell "echo \${Env:TEMP}")
    echo $(win_to_unixpath "$temp_path")
}



#params
# $1 The powershell command to run
function powershell(){
    if ! powershell.exe -NoProfile -Command "$1" | tr -d '\r\n' || [ -z "$1" ];then
        print_error "Powershell command \"$1\" failed"
        return 1
    fi
}