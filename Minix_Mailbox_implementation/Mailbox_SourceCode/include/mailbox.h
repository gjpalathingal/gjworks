#ifndef _MAILBOX_H_
#define _MAILBOX_H_

#include <sys/types.h>

#define MAX_MAILBOX_COUNT          5
#define MAX_MAILBOX_NAME_LENGTH    10
#define MAX_MESSAGE_IN_MAILBOX     5
#define MAX_MESSAGE_LENGTH         256
#define MAX_NODES                  5

int openMailbox(char *mailboxName, int flags);
int closeMailbox(int mailboxId);
int depositMessage(int mailboxId, char *msg_text, pid_t receiverList[]);
int retrieveMessage(int mailboxId, char *msg_text);
int getReceiverList(int mailboxId, int receiverList[]);

#endif
