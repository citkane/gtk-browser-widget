#!/bin/bash

OS=""
LINUX_FLAVOUR=""


## Returns the OS type, one of "linux", "darwin", "windows"
get_os() {

    case "$(uname | tr '[:upper:]' '[:lower:]')" in
    linux*)
        OS="linux"
        ;;
    darwin*)
        OS="darwin"
        echo -e "Installing on a $OS operating system\n"
        ;;
    msys* | mingw*)
        OS="windows"
        echo -e "Installing on a $OS operating system\n"
        ;;
    cygwin*)
        OS="windows"
        echo -e "Installing on a $OS operating system\n"
        PATH=$(cygpath -w /ucrt64/bin):$PATH
        export PATH
        ;;
    *)
        print_error "Unsupported OS"
        return 1
        ;;
    esac
}

## Returns the Linux distribution flavour
get_linux_distro() {
    local linux_flavour
    linux_flavour=$(awk -F= '$1=="ID" {print $2}' /etc/os-release | tr -d '"')

    case "$linux_flavour" in
    ubuntu | debian | linuxmint | elementary | pop | raspbian | kali | parrot | deepin | mx | zorin)
        LINUX_FLAVOUR="debian"
        ;;
    fedora | centos | rhel | rocky | almalinux | ol | scientific | oracle)
        LINUX_FLAVOUR="redhat"
        ;;
    arch | manjaro | arcolinux | endeavouros | artix)
        LINUX_FLAVOUR="arch"
        ;;
    opensuse* | sles | sled | suse | leap | tumbleweed)
        LINUX_FLAVOUR="suse"
        ;;
    gentoo | calculate)
        LINUX_FLAVOUR="gentoo"
        ;;
    alpine)
        LINUX_FLAVOUR="alpine"
        ;;
    void)
        LINUX_FLAVOUR="void"
        ;;
    clear-linux*)
        LINUX_FLAVOUR="clearlinux"
        ;;
    *)
        print_error "Unsupported Linux distribution '$linux_flavour'"
        return 1
        ;;
    esac
    echo -e "Installing on a $LINUX_FLAVOUR $OS operating system\n" 
}

exe_ext(){
    if [ "$OS" = "windows" ]; then
        echo ".exe"
    else
        echo ""
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

if ! get_os; then
    return 1;
fi

if [ "$OS" = "linux" ]; then
    if ! get_linux_distro; then
        return 1
    fi
fi

# Check if we are running in a MSYS2 UCRT environment for Windows
if [[ "$OS" = "windows" && ! $(check_win_env) ]]; then
    return 1;
fi
