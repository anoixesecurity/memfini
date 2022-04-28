# memfini
A Systemwide memory related events monitoring interface for linux. Memfini is capable of monitoring memory allocations on User space, Kernel space as well as some under looked allocations like PCI device mapping etc. It provides a command line interface with multiple filters, allowing a user to interact with the logs generated & get the required data. Currently, the user will be able to filter the events by individual process, type of access etc.

### Usage

- Run `./install.sh install` for installation
- Start Memfini by running `memfini --start`

Upon starting, Memfini will start logging all the supported events & will be logged in `/var/log/memfini.log`. Memfini provides basic filters, which can be used to extract required information from the log file. 

**Supported filters**  - 
- PID
- Process Name
- Shared Memory
- Kernel Memory
- Foreign Process

Example - <br> 
`memfini --pid 123`<br>
`memfini --pname procname`


**NOTE -** Remember to stop memfini (`memfini --stop`),  to avoid full disk space usage.


### Uninstall - 
- Run `./install.sh uninstall` for removing Memfini. 

**Warning -** This will remove all the logs, recommended to backup. 


