#!/bin/bash


start()
{
sudo service syslog-ng start
sudo depmod #need first time
sudo modprobe hook_mem

}

stop()
{
sudo service syslog-ng stop
sudo modprobe -r hook_mem
}

diagnosis()
{
dmesg | less
}


usage () {
cat <<EOF
Usage: sudo $0 [option]

options:
	start: Loads the module and starts logging events
	
	stop: Unload the module and stop logging the events
        
	Diagnosis: Check if module loaded

EOF
}



function main() {
    case ${1:-} in
	start) start ;;
	stop) stop ;;
        diagnosis) diagnosis ;;
	*) usage ;;
    esac
}

main $*
