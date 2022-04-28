# memfini (Version 0.1 alpha)
A Systemwide memory related events monitoring interface for linux.<br/>
Memfini is capable of monitoring memory allocations on User space, Kernel space as well as some under looked allocations like PCI device mapping etc. It provides a command line interface with multiple filters, allowing a user to interact with the logs generated & get the required data. Currently, the user will be able to filter the events by individual process, type of access etc.<br/>


Read More about usage on wiki here: [wiki link](https://github.com/anoixesecurity/memfini/wiki)

---

## System Requirement
Current version is supported on Linux kernel Version > 5.2.0.<br/>
Tested on Ubuntu 20.04 LTS and Debian Buster 
## Usage
- Run `./install.sh install` for installation
- Start Memfini by running `memfini --start`

Upon starting, Memfini will start logging all the supported events & will be logged in `/var/log/memfini.log`. Memfini provides basic filters, which can be used to extract required information from the log file. 

**Note** - Kernel memory monitoring is disabled by default. To enable it, change the value of `DKERNEL_MONITOR=1` in Makefile <br/><br/>
**Supported filters**  - 
- PID
- Process Name
- Shared Memory
- Kernel Memory
- Foreign Process

Example - <br> 
`memfini --pid 123`<br>
`memfini --pname procname`


**NOTE -** Remember to stop memfini (`memfini --stop`),  to avoid excessive disk space usage.


### Uninstall - 
- Run `./install.sh uninstall` for removing Memfini. 

**Warning -** This will remove all the logs, recommended to backup. 


