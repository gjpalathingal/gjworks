#include <sys/cdefs.h>
#include <lib.h>
#include <minix/callnr.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

#define MAX_NODES 5

PUBLIC int openMailbox(char *mailboxName, int flags);
PUBLIC int closeMailbox(int mailboxId);
PUBLIC int depositMessage(int mailboxId, char *msg_text, pid_t receiverList[]);
PUBLIC int retrieveMessage(int mailboxId, char *msg_text);
PUBLIC int getReceiverList(int mailboxId, int receiverList[]);

PUBLIC int openMailbox(char *mailboxName, int flags)
{
    message m;

    m.m3_p1 = mailboxName;
    m.m3_i1 = flags;
    m.m3_i2 = strlen(mailboxName);

    return (_syscall(PM_PROC_NR, OPENMAILBOX, &m));
}

PUBLIC int closeMailbox(int mailboxId)
{
    message m;

    m.m1_i1 = mailboxId;

    return (_syscall(PM_PROC_NR, CLOSEMAILBOX, &m));
}

PUBLIC int depositMessage(int mailboxId, char *msg_text, pid_t receiverList[])
{
    message m;

    m.m3_i1 = mailboxId;
    m.m3_i2 = strlen(msg_text);
    m.m3_p1 = msg_text;

//     m.m7_p2 = receiverList;
//    m.m7_i3 = sizeof(receiverList);

    return (_syscall(PM_PROC_NR, DEPOSITMESSAGE , &m));
}

PUBLIC int retrieveMessage(int mailboxId, char *msg_text)
{
    message m;
    m.m1_i1 = mailboxId;
    m.m1_p1 = msg_text;

    return (_syscall(PM_PROC_NR, RETRIEVEMESSAGE, &m));
}

PUBLIC int getReceiverList(int mailboxId, int receiverList[])
{
    message m;

    m.m1_i1 = mailboxId;
    m.m1_p2 = receiverList;

    return (_syscall(PM_PROC_NR, GETRECEIVERLIST, &m));
}
