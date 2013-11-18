#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <mailbox.h>

int main()
{
	int mailbox_id =0, i=0;
//	struct responsemessage recv_msg;
    char message[MAX_MESSAGE_LENGTH] = {0};
	pid_t grsPid;
	char msg[MAX_MESSAGE_LENGTH] = "This is a test message";
	pid_t receivers[1];
	grsPid = fork();
	if(grsPid)
	{
		mailbox_id = openMailbox("MailBox1", O_CREAT | O_WRONLY);
		printf("mailbox id: %d\n", mailbox_id);
        int receivers[MAX_NODES];
        getReceiverList(mailbox_id, receivers);
		// receivers[1] = grsPid;
		depositMessage(mailbox_id,msg,receivers);
	}
	else
	{
		mailbox_id = openMailbox("MailBox1", O_CREAT | O_RDONLY);
		for(i=0;i<5;i++){
		printf("mailbox id: %d\n", mailbox_id);
		retrieveMessage(mailbox_id, message);
		if (strcmp(message,"") != 0)
		{
        printf("received message is : %s\n", message);
        break;
        }
        /*
		if(recv_msg != NULL)
			{
			printf("The received message is :  %s\n",recv_msg.message);
			break;
			}	
            */
		}
	}
	return 0;
}	
