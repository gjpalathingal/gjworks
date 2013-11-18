/* If there were any type definitions local to the Process Manager, they would
 * be here.  This file is included only for symmetry with the kernel and File
 * System, which do have some local type definitions.
 */

#define MAX_MESSAGE_LENGTH      256

struct blockedSender
{
    int _pid;
    int _mailboxId;
    int _who_p;
    char _msg[MAX_MESSAGE_LENGTH];
    message _messageSender;
    int _unblocked;
    struct blockedSender *next;
};
typedef struct blockedSender BLOCKED_SENDERS;
