#!/bin/bash

install()
{
#apt-get update  # To get the latest package lists
make
if ! [ -x "$(command -v syslog-ng)" ]; then
    echo "Installing missing dependency" >&2

	if command -v apt-get >/dev/null; then
	  sudo apt-get install syslog-ng -y
	elif command -v yum >/dev/null; then
	  sudo yum install syslog-ng syslog-ng-libdbi
	else
	  echo "Sorry, not supported!"
	fi
fi

sudo cp memfini.conf /etc/syslog-ng/conf.d/memfini.conf

MODPATH="/lib/modules/"

if [ ! -d $MODPATH$(uname -r)/kernel/drivers/memfini ]; then
	 sudo mkdir $MODPATH$(uname -r)/kernel/drivers/memfini/
		if [ ! -d $MODPATH$(uname -r)/kernel/drivers/memfini/memfini.ko ]; then
			sudo cp memfini.ko $MODPATH$(uname -r)/kernel/drivers/memfini/
		fi
fi
sudo chmod +x memfini_run
sudo cp memfini_run /usr/local/bin
sudo $BASH_SOURCE cron_install
echo "Installation done.."
exit
}

uninstall()
{
MODPATH="/lib/modules/"
sudo rm -rf /var/log/memfini.log
sudo rm -rf /usr/local/bin/memfini_run
sudo rm -rf $MODPATH$(uname -r)/kernel/drivers/memfini/
make clean
echo "See you soon!"
exit
}

cron_install()
{
aa="$(sudo crontab -l | grep 'logrotate_cmd'| wc -l)"
if [ "$aa" == '0' ] ; then
	sudo crontab -l | { cat; echo "* 1 * * * logrotate $BASH_SOURCE logrotate_cmd"; } | crontab -
	echo "Cron Added for Log Rotation.."
fi
}

logrotate_cmd()
{
sudo logrotate /etc/logrotate.conf
}

usage () {
cat <<EOF
Usage: sudo $0 [option]

options:
	install: Install Memfini
	uninstall: Remove Memfini (WARNING: All logs will be deleted)
EOF
}


function main() {
	if [ `id -u` -eq 0 ]
	then
		echo "Running Installer..."
	else
		echo "Please run as root"
		exit 1
	fi

    case ${1:-} in
    	install) install;;
        uninstall) uninstall;;
        cron_install) cron_install;;
	*) usage ;;
    esac
}

main $*
