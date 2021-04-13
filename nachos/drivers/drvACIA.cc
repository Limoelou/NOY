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
    send_sema = new Semaphore((char*)"Send_Sema",1);

   //BUSY_WAITING_MODE
   if(g_cfg->ACIA == ACIA_BUSY_WAITING)
   {
	DEBUG('d', (char*)"On entre en attente active");
	receive_sema = new Semaphore((char*)"Rec_sema",1);
	printf("%p", receive_sema);
	g_machine->acia->SetWorkingMode(BUSY_WAITING);

   }

   // INTERRUPT_MODE 
   if(g_cfg->ACIA == ACIA_INTERRUPT)
   {
	DEBUG('d', (char*)"On entre en interruption");
	receive_sema = new Semaphore((char*)"Rec_sema",0);
	ind_rec = 0;
	g_machine->acia->SetWorkingMode(REC_INTERRUPT);
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

#ifndef ETUDIANTS_TP
int DriverACIA::TtySend(char* buff)

{ 
    DEBUG('d',(char*)"On souhaite envoyer une chaine !");
#endif

#ifdef ETUDIANTS_TP
int DriverACIA::TtySend(char* buff)
{ 
    DEBUG('d',(char*)"On souhaite envoyer une chaine !");

    int i;

    send_sema->P();

    i = 0;
    if(g_cfg->ACIA == ACIA_BUSY_WAITING)
    {
	do{
	    while(g_machine->acia->GetOutputStateReg() != EMPTY)
	    DEBUG('d',(char*)"en attente active ...");	
	    g_machine->acia->PutChar(buff[i]);
	    i++;

	}while(buff[i - 1] != '\0' && i < BUFFER_SIZE);

	send_sema->V();
    }

    else if(g_cfg->ACIA == ACIA_INTERRUPT)
    {
	DEBUG('d',(char*)"en attente passive ...");
	ind_send = 0;
	g_machine->acia->SetWorkingMode(SEND_INTERRUPT|g_machine->acia->GetWorkingMode());
	do{
	    send_buffer[i] = buff[i];
	    i++;
	}while(buff[i - 1] != '\0' && i < BUFFER_SIZE);

	send_buffer[i - 1] = '\0';
	
	g_machine->acia->PutChar(send_buffer[ind_send]);
	ind_send++;
    }	
    return i;
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
  if(lg <= 0){                                                          

    return -1;                                                              
  }
  char current;
  printf("Rec semaphore addr: %p", receive_sema);
  DEBUG('d',(char*)"rec sema addr: %p\n", receive_sema);	
  DEBUG('d',(char*)"send sema addr: %p\n", send_sema);	
  receive_sema->P();
  //Check mode
  int i = 0;
  if(g_machine->acia->GetWorkingMode() == BUSY_WAITING)
  {
    do {
      while(g_machine->acia->GetInputStateReg() == EMPTY);
      current = g_machine->acia->GetChar();
      DEBUG('d', (char*)"char received : %c\n", current);
      buff[i] = current;
      i++;
    }
    while(current != '\0' && (i < lg - 1));
    return i;
  }
  else{
    DEBUG('d',(char*)"en mode interruption...");	
    i = 0;
    do{
      buff[i] = receive_buffer[i];
      i++;
    }while(buff[ind_rec] != '\0' && ind_rec < lg);

    ind_rec = 0;
    return i;
  }

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
	g_machine->acia->SetWorkingMode(SEND_INTERRUPT|g_machine->acia->GetWorkingMode());
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
    if(rec_char == '\0' || ind_rec == (BUFFER_SIZE - 1))
    {
	DEBUG('d',(char*)"le message à envoyer est bien formé");
	//on ajoute le caractère à la fin du buffer de réception
	receive_buffer[ind_rec] = '\0';
	g_machine->acia->SetWorkingMode(g_machine->acia->GetWorkingMode()^REC_INTERRUPT);
	//on relâche le sémaphore de réception
	receive_sema->V();
    }
    //sinon, on continue de recevoir les caractères de la chaine
    else
    {
	receive_buffer[ind_rec] = rec_char;
	ind_rec++;
    }
}
#endif
