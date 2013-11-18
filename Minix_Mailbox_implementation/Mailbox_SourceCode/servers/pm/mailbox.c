#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "pm.h"
#include "mproc.h"
#include "param.h"

#define TRUE  1
#define FALSE 0
#define OK    0
#define ERROR -1
#define DEBUG TRUE

#define MAX_MAILBOX_COUNT          5
#define MAX_MAILBOX_NAME_LENGTH    10
#define MAX_MESSAGE_IN_MAILBOX     5
#define MAX_MESSAGE_LENGTH         256
#define MAX_NODES                5

struct node
{
    pid_t _pid;
    struct node *next;
};
typedef struct node NODE;

struct message
{
    NODE _sender;
    NODE *_receiverList;
    char _text[MAX_MESSAGE_LENGTH];
    struct message *next;
};
typedef struct message MESSAGE;

struct mailbox
{
    int _id;
    char _name[MAX_MAILBOX_NAME_LENGTH];
    NODE *_owner;
    NODE *_senderList;
    NODE *_receiverList;
    MESSAGE *_messageList;
    int _messageCount;
    struct mailbox *next;
};
typedef struct mailbox MAILBOX;

static MAILBOX *g_mailbox = NULL;
static int g_last_mailbox_id;

PUBLIC int isValidSender(int _mailboxId, int _senderId);
PUBLIC int getMailboxCount();
PUBLIC int fetchMessage(char *_message, int _len, char *_dest);
PUBLIC void saveContext(message _message, int _who_p, char *_msg, int _pid);
PUBLIC MAILBOX *getMailbox(int _mailboxId);
PUBLIC NODE *makeNode(pid_t _pid);
PUBLIC int isSenderBlocked(int _who_p);
PUBLIC int getContext(message *_message, int _who_p, char *_msg);
PUBLIC int deleteSender(int _who_p);
PUBLIC int getMessageCount(int _mailboxId);
PUBLIC MESSAGE *retrieveMessage(MAILBOX *_mailbox, pid_t _pid);
PUBLIC void unblockSender(int _mailboxId);
PUBLIC int removeProcess(MAILBOX *_mailbox, pid_t _pid);
PUBLIC int updateMailbox(MAILBOX *_mailbox);
PUBLIC int deleteProcess(NODE *_node_head, pid_t _pid);
PUBLIC int validateReceiver(MAILBOX *_mailbox, pid_t _pid);

/*================================================================*
 *                         getMailboxByName                       *
 *================================================================*/
/* fetch the mailbox for the given name */
 PUBLIC MAILBOX *getMailboxByName(char *_mailboxName){

    MAILBOX *_mailbox = g_mailbox;
    while (_mailbox != NULL)
    {
        if (strcmp(_mailbox->_name, _mailboxName) == 0)
            return _mailbox;
        
        _mailbox = _mailbox->next;
    }
    
    return NULL;
}

/*================================================================*
 *                         listAppend                             *
 *================================================================*/
/* Append a user process to the sender or receiver list */
PUBLIC void listAppend(NODE *_node_head, pid_t _pid)
{
    if (_node_head == NULL)
        _node_head = makeNode(_pid);
    else
    {
        while (_node_head->next != NULL)
            _node_head = _node_head->next;
            
        _node_head->next = makeNode(_pid);
    }
}    

/*================================================================*
 *                         do_openMailbox                         *
 *================================================================*/
PUBLIC int do_openMailbox()
{
    register struct mproc *_rmp;
    pid_t _pid;
    char _mailboxName[MAX_MAILBOX_NAME_LENGTH];
    int _flags;
    int retval;

    // printf("======================================================\n");
    printf(" System Call Invoked : do_openMailbox \n");

    _rmp = &mproc[who_p];
    _pid = _rmp->mp_pid;
    _flags = m_in.m3_i1;
    
    printf(" Calling Process: %d \n", _pid);

    retval = fetchMessage(m_in.m3_p1, m_in.m3_i2, _mailboxName);
    
    printf(" Mailbox Name: %s \n", _mailboxName);

    MAILBOX *_mailbox = g_mailbox;
    printf(" _mailbox %x g_mailbox %x\n", _mailbox, g_mailbox);
    if (_flags & O_CREAT)
    {
        printf(" In create loop\n");
        if (_mailbox == NULL)
        {
            printf(" allocating new memory\n");
            _mailbox = (MAILBOX *) malloc(sizeof(MAILBOX));
            g_mailbox = _mailbox; // Fixme
        }
        else
        {
            if (getMailboxCount() >= MAX_MAILBOX_COUNT)
            {
                err_code = EMAILBOXFULL;
                return ERROR;
            }
            
            printf(" Calling getMailboxByName with %s\n", _mailboxName);
            MAILBOX *_tmp = getMailboxByName(_mailboxName);
            if (_tmp != NULL)
            {
                if (_flags & O_WRONLY)
                    listAppend(_tmp->_senderList, _pid);
                else
                    listAppend(_tmp->_receiverList, _pid);
            
                return _tmp->_id;
            }
            
            while (_mailbox->next != NULL)
                _mailbox = _mailbox->next;
            
            _mailbox->next = (MAILBOX *) malloc(sizeof(MAILBOX));
            _mailbox = _mailbox->next;
        }
        
        _mailbox->next = NULL;
        strcpy(_mailbox->_name, _mailboxName);
        listAppend(_mailbox->_owner, _pid);
        
        if (_flags & O_WRONLY)
            listAppend(_mailbox->_senderList, _pid);
        else
            listAppend(_mailbox->_receiverList, _pid);
            
        _mailbox->_id = g_last_mailbox_id;
        
        printf(" _mailbox %x g_mailbox %x\n", _mailbox, g_mailbox);
        g_last_mailbox_id++;
        
        return _mailbox->_id;
    }
    else
    {
        printf(" Calling getMailboxByName with %s\n", _mailboxName);
        MAILBOX *_tmp = getMailboxByName(_mailboxName);
        if (_tmp != NULL)
        {
            if (_flags & O_WRONLY)
                listAppend(_tmp->_senderList, _pid);
            else
                listAppend(_tmp->_receiverList, _pid);
            
            return _tmp->_id;
        }
        else
        {
            err_code = EMAILBOXNOTFOUND;
            return ERROR;
        }
    }
  
    // printf("======================================================\n");
}

/*================================================================*
 *                         do_closeMailbox                        *
 *================================================================*/
PUBLIC int do_closeMailbox()
{
    register struct mproc *_rmp;
    pid_t _pid;
    int _mailboxId;
    int retval;
    
    // printf("======================================================\n");
    printf(" System Call Invoked : do_closeMailbox \n");
    
    _rmp = &mproc[who_p];
    _pid = _rmp->mp_pid;
    _mailboxId = m_in.mailboxId;
    
    MAILBOX *_mailbox = getMailbox(_mailboxId);
    if (_mailbox == NULL)
    {
        err_code = EMAILBOXNOTFOUND;
        return ERROR;
    }
    
    retval = removeProcess(_mailbox, _pid);
    retval = updateMailbox(_mailbox);
    
    // printf("======================================================\n");
    
    return OK;
}

/*================================================================*
 *                         updateMailbox                          *
 *================================================================*/
PUBLIC int updateMailbox(MAILBOX *_mailbox)
{
	int retval;
    
	if ((_mailbox->_receiverList == NULL) && (_mailbox->_senderList == NULL))
    {
        retval = _mailbox->_id;
        free(_mailbox);
        return retval;
	}
    
	return ERROR;
}
 
/*================================================================*
 *                         removeProcess                          *
 *================================================================*/
/* Deletes the mailbox if the owner has called it or passed the node heads to the delete api */
PUBLIC int removeProcess(MAILBOX *_mailbox, pid_t _pid)
{
    NODE *_tmpNode = NULL;

    if ((_mailbox->_owner != NULL) && (_mailbox->_owner->_pid == _pid))
    {
        if (DEBUG) printf(" Owner has called \n");
        _tmpNode = _mailbox->_owner;
        _mailbox->_owner = NULL;
        free(_tmpNode);
    }
            
    deleteProcess(_mailbox->_receiverList, _pid);
    deleteProcess(_mailbox->_senderList, _pid);
}

/*================================================================*
 *                         deleteProcess                          *
 *================================================================*/
/* Delete the corresponding user process from the access list */
PUBLIC int deleteProcess(NODE *_node_head, pid_t _pid)
{
	NODE *_node = _node_head;
	NODE *_tmp = NULL;
    
    if (_node->_pid == _pid)
    {
        _tmp = _node;
        _node = _node->next;
        free(_tmp);
    }
    else
    {
        while (_node->next != NULL)
        {
            if (_node->next->_pid == _pid)
            {
                _tmp = _node->next;
                break;
            }

            _node = _node->next;
        }

        if (_tmp != NULL)
        {
            _node->next = _tmp->next;
            free(_tmp);
        }
    }
}

/*================================================================*
 *                         do_depositMessage                      *
 *================================================================*/
PUBLIC int do_depositMessage()
{
    register struct mproc *_rmp;
    pid_t _pid;
    char _msg[MAX_MESSAGE_LENGTH];
    int retval = TRUE;
    int _mailboxId;
    int _rcvList[MAX_NODES];

    // printf("======================================================\n");
    printf(" System Call Invoked : do_depositMessage \n");

    _rmp = &mproc[who_p];
    _pid = _rmp->mp_pid;
    _mailboxId = m_in.m3_i1;

    if (DEBUG) printf(" Calling Process: %d \n", _pid);
    printf("Mailbox Id %d\n", _mailboxId);

    if (!isSenderBlocked(who_p))
    {
        printf(" fetching message \n");
        fetchMessage(m_in.m3_p1, m_in.m3_i2, _msg);
        printf(" fetched message %s\n", _msg);
    }
    else
    {
        printf(" getting context \n");
        getContext(&m_in, who_p, _msg);
        deleteSender(who_p);
    }
    
    printf(" Received message %s \n", _msg);

    if (isValidSender(_mailboxId, _pid))
    {
        retval = sys_datacopy(who_e, (vir_bytes) m_in.m1_p2, PM_PROC_NR, (vir_bytes) _rcvList, (phys_bytes) m_in.m1_i3);

        MAILBOX *_mb = g_mailbox;
        while (_mb != NULL)
        {
            if (_mb->_id == _mailboxId)
            {
                int _count;
                int _messageCount = getMessageCount(_mailboxId);

                if (_messageCount < MAX_MESSAGE_IN_MAILBOX)
                {
                    MESSAGE *_messageList = _mb->_messageList;
                    printf(" Empty messageList\n");
                    printf(" _messageList %x _mb->_messageList %x \n", _messageList, _mb->_messageList);
                    if (_messageList == NULL)
                    {
                        _messageList = (MESSAGE *) malloc(sizeof(MESSAGE));
                        _mb->_messageList = _messageList; // Fixme
                        printf(" _messageList %x _mb->_messageList %x \n", _messageList, _mb->_messageList);
                    }
                    else
                    {
                        while (_messageList->next != NULL)
                            _messageList = _messageList->next;

                        _messageList->next = (MESSAGE *) malloc(sizeof(MESSAGE));
                        _messageList = _messageList->next;
                    }

                    strcpy(_messageList->_text, _msg);

                    _messageList->_sender._pid = _pid;

                    NODE *_receiver = _messageList->_receiverList;
                    for (_count = 0; _count < MAX_NODES; ++_count)
                    {
                        if (_rcvList[_count] > 0)
                        {
                            if (_receiver == NULL)
                            {
                                _receiver = (NODE *) malloc(sizeof(NODE));
                                _messageList->_receiverList = _receiver; // Fixme
                            }
                            else
                            {
                                _receiver->next = (NODE *) malloc(sizeof(NODE));
                                _receiver = _receiver->next;
                            }

                            _receiver->_pid = _rcvList[_count];
                            _receiver->next = NULL;
                        }
                        else break;
                    }

                    _messageList->next = NULL;
                }
                else
                {
                    if (DEBUG) printf(" Mailbox buffer is full \n");
                    saveContext(m_in, who_p, _msg, _pid);
                    _rmp->mp_flags |= SIGSUSPENDED;

                    return SUSPEND;
                }

                break;
            }
        }
    }
    else
    {
        err_code = EINVALIDSENDER;
        return EGENERIC;
    }

    // printf("======================================================\n");
}

/*================================================================*
 *                         saveContext                            *
 *================================================================*/
PUBLIC void saveContext(message _message, int _who_p, char *_msg, int _pid)
{
    BLOCKED_SENDERS *_blockedSenders = g_blockedSenders;

    if (_blockedSenders == NULL)
    {
        _blockedSenders = (BLOCKED_SENDERS *) malloc(sizeof(BLOCKED_SENDERS));
        g_blockedSenders = _blockedSenders; // Fixme
    }
    else
    {
        while (_blockedSenders->next != NULL)
            _blockedSenders = _blockedSenders->next;

        _blockedSenders->next = (BLOCKED_SENDERS *) malloc(sizeof(BLOCKED_SENDERS));
        _blockedSenders = _blockedSenders->next;
    }

    _blockedSenders->_mailboxId = _message.mailboxId;
    _blockedSenders->_who_p = _who_p;
    strcpy(_blockedSenders->_msg, _msg);
    memcpy(&_blockedSenders->_messageSender, &_message, sizeof(message));
    _blockedSenders->_unblocked = FALSE;

    _blockedSenders->next = NULL;
}

/*================================================================*
 *                         getContext                             *
 *================================================================*/
PUBLIC int getContext(message *_message, int _who_p, char *_msg)
{
    BLOCKED_SENDERS *_blockedSenders = g_blockedSenders;
    while (_blockedSenders != NULL)
    {
        if (_blockedSenders->_who_p == _who_p)
        {
            memcpy(_message, &_blockedSenders->_messageSender, sizeof(message));
            strcpy(_msg, _blockedSenders->_msg);

            return TRUE;
        }

        _blockedSenders = _blockedSenders->next;
    }

    return FALSE;
}

/*================================================================*
 *                         isSenderBlocked                        *
 *================================================================*/
PUBLIC int isSenderBlocked(int _who_p)
{
    printf(" In isSenderBlocked \n");
    BLOCKED_SENDERS *_blockedSenders = g_blockedSenders;
    while (_blockedSenders != NULL)
    {
        if (_blockedSenders->_who_p == _who_p)
            return TRUE;

        _blockedSenders = _blockedSenders->next;
    }

    return FALSE;
}

/*================================================================*
 *                         do_retrieveMessage                     *
 *================================================================*/
PUBLIC int do_retrieveMessage()
{
    register struct mproc *_rmp;
    pid_t _pid;
    int _mailboxId;
    char _msg_text[MAX_MESSAGE_LENGTH] = {0};
    
    // printf("======================================================\n");
    printf(" System Call Invoked : do_retrieveMessage \n");
    
    
    _rmp = &mproc[who_p];
    _pid = _rmp->mp_pid;

    _mailboxId = m_in.mailboxId;
    MAILBOX *_mailbox = getMailbox(_mailboxId);

    if (_mailbox != NULL)
    {
        int valid = validateReceiver(_mailbox, _pid);
        if (valid == TRUE)
        {
            MESSAGE *_tmp = retrieveMessage(_mailbox, _pid);
            strcpy(_msg_text, _tmp->_text);

            sys_datacopy(PM_PROC_NR, _msg_text, m_in.m_source, m_in.m1_p1, sizeof(_msg_text));
        }
        else
        {
            err_code = EPERM;
            return ERROR;
        }
    }
    else
    {
        err_code = EMAILBOXNOTFOUND;
        return ERROR;
    }

    // printf("======================================================\n");

    return OK;
}

/*================================================================*
 *                         retrieveMessage                        *
 *================================================================*/
PUBLIC MESSAGE *retrieveMessage(MAILBOX *_mailbox, pid_t _pid)
{
    MESSAGE *_tmp;
    MESSAGE *_previousMessage = NULL;
    MESSAGE *_message = _mailbox->_messageList;
    int _mailboxId = _mailbox->_id;
    while (_message != NULL)
    {
        NODE *_previousReceiver = NULL;
        NODE *_receiverList = _message->_receiverList;
        while (_receiverList != NULL )
        {
            if (_receiverList->_pid == _pid)
            {
                if (_previousReceiver == NULL)
                {
                    _previousReceiver = _receiverList;
                    _receiverList = _receiverList->next;
                    free(_previousReceiver);
                }
                else
                {
                    _previousReceiver->next = _receiverList->next;
                    free(_receiverList);
                }
                  
                _tmp = (MESSAGE *) malloc(sizeof(MESSAGE));
                strcpy(_tmp->_text, _message->_text);

                if(_message->_receiverList == NULL)
                {
                    if (_previousMessage == NULL)
                    {
                        _previousMessage = _message;
                        _message = _message->next;
                        free(_previousMessage);
                    }
                    else
                    {
                        _previousMessage->next = _message->next;
                        free(_message);
                    }
                    
                    unblockSender(_mailboxId);
                }

                break;
            }
            
            _previousReceiver = _receiverList;
            _receiverList = _receiverList->next;
        }
        
        if (_tmp != NULL) break;
        
        _previousMessage = _message;
        _message = _message->next;
    }

    return _tmp;
}

/*================================================================*
 *                         validateReceiver                       *
 *================================================================*/
PUBLIC int validateReceiver(MAILBOX *_mailbox, pid_t _pid)
{
    NODE *_receiverList = _mailbox->_receiverList;

    while (_receiverList != NULL)
    {
        if (_receiverList->_pid == _pid)
            return TRUE;
            
        _receiverList = _receiverList->next;
    }
    
    return FALSE;
}

/*================================================================*
 *                         do_getReceiverList                     *
 *================================================================*/
PUBLIC int do_getReceiverList()
{
    register struct mproc *_rmp;
    pid_t _pid;
    MAILBOX *_mb;
    int _receiverList[MAX_NODES + 1];
    int _count = 0;
    int _mailboxId;

    // printf("======================================================\n");
    printf(" System Call Invoked : do_getReceiverList \n");

    _rmp = &mproc[who_p];
    _pid = _rmp->mp_pid;
    _mailboxId = m_in.mailboxId;
    _mb = g_mailbox;

    while (_mb != NULL)
    {
        if (_mb->_id == _mailboxId)
        {
            NODE *_receiver = _mb->_receiverList;

            while (_receiver != NULL)
                _receiverList[_count++] = _receiver->_pid;

            break;
        }

        _mb = _mb->next;
    }

    sys_datacopy(PM_PROC_NR, _receiverList, m_in.m_source, m_in.m1_p2, sizeof(_receiverList));
    m_in.m1_i3 = _count;

    // printf("======================================================\n");

    return OK;
}

/*================================================================*
 *                         fetchMessage                           *
 *================================================================*/
PUBLIC int fetchMessage(char *_message, int _len, char *_dest)
{
    int retval = OK;

    if (_len > MAX_MESSAGE_LENGTH)
    {
        err_code = EMSGSIZE;
        return EGENERIC;
    }

    if (_len <= 0)
    {
        err_code = EINVAL;
        return EGENERIC;
    }

    retval = sys_datacopy(m_in.m_source, (vir_bytes) _message, PM_PROC_NR, (vir_bytes) _dest, (phys_bytes) _len);

    if (_dest[_len - 1] != '\0')
    {
        err_code = EMSGSIZE;
        return EGENERIC;
    }

    if (_dest[0] == '\0')
    {
        err_code = EMSGEMPTY;
        return EGENERIC;
    }

    return retval;
}

/*================================================================*
 *                         getMailboxCount                        *
 *================================================================*/
PUBLIC int getMailboxCount()
{
    int count = 0;
    MAILBOX *_mb = g_mailbox;

    while (_mb != NULL)
    {
        ++count;
        _mb = _mb->next;
    }

    return count;
}

/*================================================================*
 *                         isValidSender                          *
 *================================================================*/
PUBLIC int isValidSender(int _mailboxId, int _senderId)
{
    printf(" In isValidSender \n");
    
    int retval = FALSE;
    MAILBOX *_mb = g_mailbox;

    while (_mb != NULL)
    {
        if (_mb->_id == _mailboxId)
        {
            NODE *_sender = _mb->_senderList;

            while (_sender != NULL)
            {
                if (_sender->_pid == _senderId)
                {
                    retval = TRUE;

                    break;
                }
            }

            break;
        }

        _mb = _mb->next;
    }

    printf(" isValidSender returning %d\n", retval);
    return retval;
}

/*================================================================*
 *                         getMessageCount                        *
 *================================================================*/
PUBLIC int getMessageCount(int _mailboxId)
{
    MAILBOX *_mb = g_mailbox;
    int count = 0;

    while (_mb != NULL)
    {
        if (_mb->_id == _mailboxId)
        {
            MESSAGE *_message = _mb->_messageList;

            while (_message != NULL)
            {
                ++count;
                _message = _message->next;
            }

            break;
        }

        _mb = _mb->next;
    }

    return count;
}

/*================================================================*
 *                         unblockSender                          *
 *================================================================*/
PUBLIC void unblockSender(int _mailboxId)
{
    BLOCKED_SENDERS *_blockedSenders = g_blockedSenders;
    while (_blockedSenders != NULL)
    {
        if (_blockedSenders->_mailboxId == _mailboxId)
        {
            _blockedSenders->_unblocked = TRUE;
        }

        _blockedSenders = _blockedSenders->next;
    }
}

/*================================================================*
 *                         deleteSender                           *
 *================================================================*/
PUBLIC int deleteSender(int _who_p)
{
    BLOCKED_SENDERS *_tmp = NULL;
    BLOCKED_SENDERS *_blockedSenders = g_blockedSenders;
    if (_blockedSenders == NULL)
        return FALSE;

    if (_blockedSenders->_who_p == _who_p)
    {
        _tmp = _blockedSenders;
        _blockedSenders = _blockedSenders->next;
        free(_tmp);
    }
    else
    {
        while (_blockedSenders->next != NULL)
        {
            if (_blockedSenders->next->_who_p == _who_p)
            {
                _tmp = _blockedSenders->next;
                break;
            }

            _blockedSenders = _blockedSenders->next;
        }

        if (_tmp != NULL)
        {
            _blockedSenders->next = _tmp->next;
            free(_tmp);
        }
        else
            return FALSE;
    }

    return TRUE;
}

/*================================================================*
 *                         makeNode                               *
 *================================================================*/
/* Make a node out of given PID */
PUBLIC NODE *makeNode(pid_t _pid)
{
    NODE *_tmp = (NODE *) malloc(sizeof(NODE));
    _tmp->_pid = _pid;
    _tmp->next = NULL;
    return _tmp;
}

/*================================================================*
 *                         getMailbox                             *
 *================================================================*/
/* To acquire the mailbox pointer from the given mailbox id */
PUBLIC MAILBOX *getMailbox(int _mailboxId)
{
    MAILBOX *_mailbox = g_mailbox;
    if (_mailbox == NULL)
    {
        printf(" Mailbox does not exist \n");
        return NULL;
    }
    else
    {
        do
        {
            if (_mailbox->_id == _mailboxId)
                return _mailbox;

            _mailbox = _mailbox->next;
        } while (_mailbox->next != NULL);
    }    

    printf(" Mailbox does not exist \n");
    return NULL;
}

/*================================================================*
 *                         isAnySenderUnblocked                   *
 *================================================================*/
int isAnySenderUnblocked()
{
    BLOCKED_SENDERS *_blockedSenders = g_blockedSenders;
    while (_blockedSenders != NULL)
    {
        if (_blockedSenders->_unblocked == TRUE)
            return TRUE;
            
        _blockedSenders = _blockedSenders->next;
    }
    
    return FALSE;
}


