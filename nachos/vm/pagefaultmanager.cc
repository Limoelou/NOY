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


    TranslationTable * tt = g_current_thread->GetProcessOwner()->addrspace->translationTable;
    int addr_phys;
    int addr_disk = tt->getAddrDisk(virtualPage);
    bool bit_swap = tt->getBitSwap(virtualPage);

    int page_size = g_cfg->PageSize;
    char page_temp[page_size];
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
	    g_current_thread->GetProcessOwner()->exec_file->ReadAt(page_temp, page_size, addr_disk);
	    //page est à charger dans l'executable, addr_disk contient
	    //l'addresse de la page à charger depuis le fichier

	}
    }

    int pp = g_physical_mem_manager->AddPhysicalToVirtualMapping(g_current_thread->GetProcessOwner()->addrspace, virtualPage);

    if(pp == -1) { 
	printf("Not enough free space to load program %s\n", g_current_thread->GetProcessOwner()->exec_file->GetName());

	g_machine->interrupt->Halt(-1);
    }

	
    tt->setAddrDisk(virtualPage,-1);

    tt->setBitValid(virtualPage);

    tt->clearBitSwap(virtualPage);

    tt->setBitReadAllowed(virtualPage);

    tt->setBitWriteAllowed(virtualPage);

    tt->clearBitIo(virtualPage);
}

#endif






