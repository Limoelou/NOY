/* \file drvACIA.cc

   \brief Routines of the ACIA device driver

//

//      The ACIA is an asynchronous device (requests return 

//      immediately, and an interrupt happens later on).  

//      This is a layer on top of the ACIA.

//      Two working modes are to be implemented in assignment 2:

//      a Busy Waiting mode and an Interrupt mode. The Busy Waiting

//      mode implements a synchronous IO whereas IOs are asynchronous

//      IOs are implemented in the Interrupt mode (see the Nachos

//      roadmap for further details).

//

//  Copyright (c) 1999-2000 INSA de Rennes.

//  All rights reserved.  

//  See copyright_insa.h for copyright notice and limitation 

//  of liability and disclaimer of warranty provisions.

//

*/



/* Includes */



#include "kernel/system.h"         // for the ACIA object

#include "kernel/synch.h"

#include "machine/ACIA.h"

#include "drivers/drvACIA.h"



//-------------------------------------------------------------------------

// DriverACIA::DriverACIA()

/*! Constructor.

  Initialize the ACIA driver. 

  In the ACIA Interrupt mode, 

  initialize the reception index and semaphores and allow 

  reception interrupts. 

  In the ACIA Busy Waiting mode, simply inittialize the ACIA 

  working mode and create the semaphore.

  */

//-------------------------------------------------------------------------


#ifndef ETUDIANTS_TP
DriverACIA::DriverACIA()

{

  printf("**** Warning: contructor of the ACIA driver not implemented yet\n");

  exit(-1);

}
#endif

#ifdef ETUDIANTS_TP

DriverACIA::DriverACIA()

{
    Semaphore *send_sema = new Semaphore((char*)"Send_Sema",1);

   //BUSY_WAITING_MODE
   if(g_cfg->ACIA == BUSY_WAITING)
   {
	DEBUG('d', (char*)"On entre en attente active");
	receive_sema = new Semaphore((char*)"Rec_sema",1);
	g_machine->acia->SetWorkingMode(BUSY_WAITING);

   }

   // INTERRUPT_MODE 
   if(g_cfg->ACIA == ACIA_INTERRUPT)
   {
	DEBUG('d', (char*)"On entre en interruption");
	receive_sema = new Semaphore((char*)"Rec_sema",0);
	ind_rec = 0;
   }

}
#endif


//-------------------------------------------------------------------------

// DriverACIA::TtySend(char* buff)

/*! Routine to send a message through the ACIA (Busy Waiting or Interrupt mode)

  */

//-------------------------------------------------------------------------


#ifndef ETUDIANTS_TP
int DriverACIA::TtySend(char* buff)
{ 

  printf("**** Warning: method Tty_Send of the ACIA driver not implemented yet\n");

  exit(-1);

  return 0;

}
#endif

#ifdef ETUDIANTS_TP
int DriverACIA::TtySend(char* buff)

{ 
    DEBUG('d',(char*)"On souhaite envoyer une chaine !");

#ifdef ETUDIANTS_TP

// J'ai changé le type de retour de int vers void comme dans le td
int DriverACIA::TtySend(char* buff)
{ 
    DEBUG('d',(char*)"On souhaite envoyer une chaine !");

    int i;

    send_sema->P();

    i = -1;
    if(g_machine->acia->GetWorkingMode() == BUSY_WAITING)
    {
	do{
	    while(g_machine->acia->getOutputStateReg != EMPTY)
	    {
		DEBUG('d',(char*)"en attente active ...");	
	    }

	    g_machine->acia->PutChar(buff[i]);
	    i++;

	}while(buff[i] != '\0' && i < BUFFER_SIZE);

    send_sema->V();
    return i;

    else if(g_machine->acia->GetWorkingMode() == SEND_INTERRUPT || g_machine->acia->GetWorkingMode() == REC_INTERRUPT)
    {
	DEBUG('d',(char*)"en attente passive ...");
	ind_send = 0;

	do{
	    send_buffer[i] = buff[i];
	    i++;
	}while(buff[i] != '\0' && i < BUFFER_SIZE);

	send_buffer[i] == '\0';
	
	g_machine->acia->PutChar(send_buffer[ind_send]);
	ind_send++;

	return i;
    }	
    
}
#endif

  return 0;

}
#endif

//-------------------------------------------------------------------------

// DriverACIA::TtyReceive(char* buff,int length)

/*! Routine to reveive a message through the ACIA 

//  (Busy Waiting and Interrupt mode).

  */

//-------------------------------------------------------------------------


#ifndef ETUDIANTS_TP
int DriverACIA::TtyReceive(char* buff,int lg)

{

   printf("**** Warning: method Tty_Receive of the ACIA driver not implemented yet\n");

  exit(-1);

  return 0;

}
#endif

#ifdef ETUDIANTS_TP
int DriverACIA::TtyReceive(char* buff,int lg)

{
  char EOL = 0;
  char current;

  for(int i = 0; i < lg; i++) {
    while(!receive_sema); // attente active
    current = GetChar();
    buff[i] = current;
    if(current == '\0')
      break;
  }
  return i;
}

#endif


//-------------------------------------------------------------------------

// DriverACIA::InterruptSend()

/*! Emission interrupt handler.

  Used in the ACIA Interrupt mode only. 

  Detects when it's the end of the message (if so, releases the send_sema semaphore), else sends the next character according to index ind_send.

  */

//-------------------------------------------------------------------------


#ifndef ETUDIANTS_TP
void DriverACIA::InterruptSend()

{

  printf("**** Warning: send interrupt handler not implemented yet\n");

  exit(-1);

}
#endif

#ifdef ETUDIANTS_TP
void DriverACIA::InterruptSend()

{
    //si l'envoi est terminé
    if(send_buffer[ind_send - 1] == '\0')
    {
	DEBUG('d',(char*)"on est à la fin du message à envoyer");
	//on relâche le sémaphore d'émission
	send_sema->V();
    }
    //sinon, on continue d'envoyer notre chaine
    else
    {
	g_machine->acia->PutChar(send_buffer[ind_send]);
	ind_send++;
    }
}
#endif
//-------------------------------------------------------------------------

// DriverACIA::Interrupt_receive()

/*
  Used in the ACIA Interrupt mode only. Reveices a character through the ACIA. 

  Releases the receive_sema semaphore and disables reception 

  interrupts when the last character of the message is received 

  (character '\0').
*/

//-------------------------------------------------------------------------


#ifndef ETUDIANTS_TP
void DriverACIA::InterruptReceive()

{

  printf("**** Warning: receive interrupt handler not implemented yet\n");

  exit(-1);

}
#endif

#ifdef ETUDIANTS_TP
void DriverACIA::InterruptReceive()

{
    //on récupère le prochain caractère à être reçu
    char rec_char = g_machine->acia->GetChar();

    //Si on arrive sur un caractère de fin ou à la taille max du buffer
    if(send_buffer[ind_send - 1] == '\0' || ind_rec == (BUFFER_SIZE - 1))
    {
	DEBUG('d',(char*)"le message à envoyer est bien formé");
	//on ajoute le caractère à la fin du buffer de réception
	receive_buffer[ind_rec] = '\0';
	//on relâche le sémaphore de réception
	receive_sema->V();
    }
    //sinon, on continue de recevoir les caractères de la chaine
    else
    {
	receive_buffer[ind_rec] = send_buffer[ind_send];
	ind_rec++;
    }

}
#endif
