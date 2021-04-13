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


    Process *process = g_current_thread->GetProcessOwner();
    AddrSpace *as = process->addrspace;
    TranslationTable * tt = as->translationTable;
    // addr de chargement de page en ram
    int pp;
    int addr_disk = tt->getAddrDisk(virtualPage);
    bool bit_swap = tt->getBitSwap(virtualPage);

    int page_size = g_cfg->PageSize;
    char page_temp[page_size];
    //la page est dans la zone de swap
    //
    DEBUG('d',(char*)"on demande la page numéro %i à l'adresse virtuelle %x.\n");

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
	    g_swap_manager->GetPageSwap(addr_disk, page_temp);
	}

    }
    else
    {
	if(addr_disk == -1)
	{
	    //page anonyme (pile d'un contexte utilisateur d'un thread ou non
	    //initialisé)
	    memset(page_temp, 0x0, page_size);
	}
	else
	{
	    //page est à charger dans l'executable, addr_disk contient
	    //l'addresse de la page à charger depuis le fichier
	    int check_size =process->exec_file->ReadAt(page_temp, page_size, addr_disk);
	    if(check_size != page_size)
	    {
		DEBUG('d',(char*)"les tailles des pages ne correspondent pas, erreur dans la lecture\n");
		return(PAGEFAULT_EXCEPTION);
	    }
	    else
	    {
		DEBUG('d',(char*)"on lit une page dans l'addresse disque %x qui est de taille %d\n", addr_disk, page_size);
	    }
	}
    }

    //recupération d'une page libre
    pp = g_physical_mem_manager->AddPhysicalToVirtualMapping(as, virtualPage);

    if(pp == -1) { 
	printf("Not enough free space to load program %s\n", process->exec_file->GetName());

	g_machine->interrupt->Halt(-1);
    }

    //recopie de la page
    memcpy(&(g_machine->mainMemory[pp*page_size]), page_temp, page_size);

    //récupération de la page physique et de la page virtuelle valide située en ram
    tt->setPhysicalPage(virtualPage, pp);
    
    tt->setAddrDisk(virtualPage,-1);

    tt->setBitValid(virtualPage);

    tt->clearBitSwap(virtualPage);

    g_physical_mem_manager->UnlockPage(pp);

    tt->setBitReadAllowed(virtualPage);

    tt->setBitWriteAllowed(virtualPage);

    tt->clearBitIo(virtualPage);
    
    return (NO_EXCEPTION);
    
}

#endif






