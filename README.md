# memfini
A Systemwide memory monitoring interface for linux

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


