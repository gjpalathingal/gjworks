/*
 * TestMailbox.c
 *
 *      Author: shilpa rajappa
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include "grsMailbox.h"

int  main()
{
    int i;
    int mailboxid;
    char mailboxname[256];
    char message[256];
    int d;
    while(1)
    {
    printf("Mailbox Operations\n");
    printf("===============\n");
    printf("1.Create Mailbox\n");
    printf("2.Close Mailbox\n");
    printf("3.Deposit message\n");
    printf("4.Retrieve \n");
    printf("5.Exit\n");
    printf("Enter your choice : ");
    scanf("%d",&i);
    if(i<=0){
        printf("Enter only an Integer\n");
        break;
    } else {
        switch(i)
        {
        case 1: printf("\n ==== Invoke create mailbox system call ====== \n");
				printf("Enter mailbox name to create : ");
				scanf("%s",&mailboxname);
				d=openMailbox(mailboxname, O_CREAT | O_WRONLY);
				if(d<0)	 printf("\n Mailbox creation failed with error code %d\n",d);
				else  printf("\n Mailbox created with id %d\n",d);
                 break;
        case 2: printf("\n ===== Invoke close mailbox system call ===== \n");
				printf("Enter mailbox name to close : ");
				scanf("%s",&mailboxname);
				mailboxid = openMailbox(mailboxname, O_CREAT | O_RDONLY);
				d=closeMailBox(mailboxid);
                break;
        case 3:   printf("\n ====== Invoke deposit message system call ===== \n");
				printf("Enter mailbox name to send : ");
        		scanf("%s",&mailboxname);
        		printf("\n Enter message to send : ");
        		scanf("%s",&message);
        		mailboxid = openMailbox(mailboxname, O_CREAT | O_WRONLY);
        		printf("\n mailbox id: %d\n", mailboxid);
                int receivers[MAX_NODES];
                getReceiverList(mailboxid, receivers);
        		// receivers[1] = grsPid;
        		depositMessage(mailboxid,message,receivers);
                break;
        case 4: printf("\n ==== Retrieve system call ===== \n");
				printf("Enter mailbox name to recieve from : ");
                scanf("%s",&mailboxname);
                mailboxid = openMailbox(mailboxname, O_CREAT | O_RDONLY);
                d = retrieveMessage(mailboxid, message);
				if(d<0){
					printf("\n Message read error : %d",d);
				}else{
					printf("\n Message received : %s",message);
				}

                break;
        case 5:     return 0;
        default:    printf("Invalid option\n");
        }
    }
    }
    return 0;
}

