/*! \file pagefaultmanager.cc

Routines for the page fault managerPage Fault Manager

*/

//

//  Copyright (c) 1999-2000 INSA de Rennes.

//  All rights reserved.  

//  See copyright_insa.h for copyright notice and limitation 

//  of liability and disclaimer of warranty provisions.

//



#include "kernel/thread.h"

#include "vm/swapManager.h"

#include "vm/physMem.h"

#include "vm/pagefaultmanager.h"



PageFaultManager::PageFaultManager() {

}



// PageFaultManager::~PageFaultManager()

/*! Nothing for now

*/

PageFaultManager::~PageFaultManager() {

}



// ExceptionType PageFault(uint32_t virtualPage)

/*! 	

//	This method is called by the Memory Management Unit when there is a 

//      page fault. This method loads the page from :

//      - read-only sections (text,rodata) $\Rightarrow$ executive

//        file

//      - read/write sections (data,...) $\Rightarrow$ executive

//        file (1st time only), or swap file

//      - anonymous mappings (stack/bss) $\Rightarrow$ new

//        page from the MemoryManager (1st time only), or swap file

//

//	\param virtualPage the virtual page subject to the page fault

//	  (supposed to be between 0 and the

//        size of the address space, and supposed to correspond to a

//        page mapped to something [code/data/bss/...])

//	\return the exception (generally the NO_EXCEPTION constant)

*/  

#ifndef ETUDIANTS_TP
ExceptionType PageFaultManager::PageFault(uint32_t virtualPage) 

{

  printf("**** Warning: page fault manager is not implemented yet\n");

    exit(-1);

    return ((ExceptionType)0);

}
#endif

#ifdef ETUDIANTS_TP

ExceptionType PageFaultManager::PageFault(uint32_t virtualPage) 

{
    // chercher la page sur le disk



    addPhysicaltoVirtualMapping(A
    int addr_disk = g_machine->translationtable->getAddrDisk(virtualPage);
    bool bit_swap = g_machine->translationtable->getBitSwap(virtualPage);
    Addrspace* addrspace = process->addrspace;
    translationTable *tradtable = addrspace->translationTable;

    addPhysicaltoVirtualMapping(addrspace,
    
    int IO_bit = tradtable->getBitIO;
    if(IO_bit)
    {

    }
    //la page est dans la zone de swap
    if(bit_swap == 1)
    {
	//addr disk contient le numéro de la page à récupérer dans le swap
	//(numéro du secteur sur disque)
	if(addr_disk == -1)
	{
	    //voleur de page, il faut attendre que addr_disk soit positionné
	    //à une autre valeur
	}
	else
	{
	    
        }
    }

    }
    else
    {
	if(addr_disk == -1)
	{
	    //page anonyme (pile d'un contexte utilisateur d'un thread ou non
	    //initialisé)
	    //
	}
	else
	    {
	    //page est à charger dans l'executable, addr_disk contient
	    //l'addresse de la page à charger depuis le fichier

	}
    }

}
#endif








