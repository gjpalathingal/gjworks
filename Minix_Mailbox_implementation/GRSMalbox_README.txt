							README
                                                    **************
The included minix source supports 5 new Mailbox IPC system calls
1. do_openMailBox
2. do_closeMailBox
3. do_deposit
4. do_retrieveMessage
5. do_getReceiverList

NEW SOURCE FILE and RELATED HEADER FILES
****************************************
The system calls are added as part of Process Management(PM) server process. The source code for these system calls have been added to the file
maibox.c in /usr/src/server/pm/ . The prototype definitions are added to the header file grsMailbox.h in the same location. All new datatypes 
and structures for the new system calls are included here. In order to use the new system calls the header file must be included in the programs. 

LIBRARY FUNCTIONS
*****************
The wrapper library functions for these system calls are also defined in /usr/src/lib/libc/sys-minix/mailbox.c. The users can call these functions
in their programs to utilize the new mailbox IPC functionalitiies. This library functions will internally call the concerned system calls. The library 
functions defined are as follows

1. openMailBox(char * mailboxname, int permission_flags)
2. closeMailBox(int mailboxid)
3. deposit(int mailboxid, char *message, pid_t receiverlist[])
4. retrieveMessage(int mailboxid,char *message)
5. getReceiverList(int mailboxid, pid_t receiverlist[])

For more details regarding functionality and usage of these library functions refer to the design document.

OTHER CHANGED MINIX SOURCE FILES 
********************************
/usr/src/include/minix/callnr.h has be updated with new system call numbers.
/usr/src/server/pm/table.c has been updated with new system call numbers as well.
/usr/src/server/pm/proto.h has been updated with the new system call prototypes.
/usr/src/server/pm/Makefile has been updated to include the new file mailbox.c for source compilation.

TESTING THE NEW SYSTEM CALLS
****************************
A menu based testing program by name TestMailBox.c has been included with this source.
Once you execute this program it will show you the options as in which system call you want to invoke and test among 5 new implemented ones.

Also includeded is program by name grsMailBoxDemo.c which when executed will show a demo of the new message passing mechanism implemented.
The demo depicts two processes opening the same mailbox and sending and receiving messages through it.




