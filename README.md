# CPE-2600 Lab 9: System Calls

Four programs demonstrate Linux system calls for system information, process
control, and file information.

Build the programs with:

```bash
make
```

Run them with:

```bash
./info              # Display system information
./pinfo [processID] # Display a process's priority and scheduling method
./pmod              # Lower this process's priority and sleep
./finfo <file>      # Display information about a file
```

If no process ID is provided, `pinfo` displays information about itself.
