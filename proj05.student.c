/****************************************************************************
    Claury Mejia
    Section 2
    Computer Project #5
  ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <new>
#include <vector>
#include "/user/cse410/McCullen/Projects/project05.lib.h"

using namespace std;

struct TableEntry 
{
  int v;
  int r;
  int m;
  int frame;
  
  int timeReferenced;
  int timeInRAM;
  
};

/*---------------------------------------------------------------------------
    Name:  printLogFile

    Purpose:  Print references made bu the udser process to proj04.log
    Receive:  A vector of unsigned integers, which are the references
    Return:   Nothing, just print to an output file
  ---------------------------------------------------------------------------*/
void printLogFile(vector<unsigned>);

/*---------------------------------------------------------------------------
    Name:  getOperation

    Purpose:  Get the operation of the reference by masking and shifting
    Receive:  An unsigned integer which is a reference
    Return:   An unsigned integer after masking and shifting the reference
  ---------------------------------------------------------------------------*/
char getOperation(unsigned int);

/*---------------------------------------------------------------------------
    Name:  getLogicalAddress

    Purpose:  Get the logical address of the reference by masking and shifting
    Receive:  An unsigned integer which is a reference
    Return:   An unsigned integer after masking and shifting the reference
  ---------------------------------------------------------------------------*/
unsigned int getLogicalAddress(unsigned int);

/*---------------------------------------------------------------------------
    Name:  getPageNumber

    Purpose:  Get the page number of the reference by masking and shifting
    Receive:  An unsigned integer which is a reference
    Return:   An unsigned integer after masking and shifting the reference
  ---------------------------------------------------------------------------*/
unsigned int getPageNumber(unsigned int);

/*---------------------------------------------------------------------------
    Name:  getPageOffset

    Purpose:  Get the page offset of the reference by masking and shifting
    Receive:  An unsigned integer which is a reference
    Return:   An unsigned integer after masking and shifting the reference
  ---------------------------------------------------------------------------*/
unsigned int getPageOffset(unsigned int);

int main(int argc, char *argv[])
{
  vector<unsigned int> memoryReferences;
  vector<TableEntry> pageTable;
  
  int* sizeOfProcess = new int[1];
  int* numberMemoryReferences = new int[1];
  int* numberPageFrames = new int[1];
  char* algorithm = new char[20];

  int pagesDiskToRAM = 0;
  int pagesRAMToDisk = 0;
  int pageFaults = 0;
        
  setup(sizeOfProcess, numberMemoryReferences, numberPageFrames, algorithm);

  /*Create Table Entries*/
  cout << "Page Table" << endl;
  
  for(int i = 0; i < *sizeOfProcess; i++)
    {
      //TableEntry{v,r,m,frame number,time since last referenced, time in RAM}
      pageTable.push_back(TableEntry{0,0,0,i,0,0});
      cout << "Table Entry: " << pageTable[i].v << " "
	   << pageTable[i].r << " " << pageTable[i].m << " ";
      
      printf("%02x\n", pageTable[i].frame);//(2 hexadecimal digits, with leading zeroes)
      
    }

  /*Push the references obtained from calling reference() to a
    vector of unsigned int*/
  
  for(int i = 0; i < *numberMemoryReferences; i++)
    {
      memoryReferences.push_back(reference());
    }

  /*for(auto reference: memoryReferences)
    {
      int pageFrames = 0;

      //Replace with function frameAvailable
      for(auto table: pageTable)
	{
	  if (table.v == 1) pageFrames++;
	}

      updateTableEntries(pageTable, pageNumber);

      if(pageFrames < *numberPageFrames || pageTable[pageNumber].v == 1 )
	{
	  int pageNumber = getPageNumber(reference);
	  char operation = getOperation(reference);
	  
	  if(pageTable[pageNumber].v == 0)
	    {
	      pageTable[pageNumber].v = 1;
	      pagesDiskToRAM++;
	    }
	  
	      
	  if(pageTable[pageNumber].r != 1)
	    {
	      pageTable[pageNumber].r = 1;
	    }
	      
	  if(operation == 'W')
	    {
	      if(pageTable[pageNumber].m != 1)
		{
		  pageTable[pageNumber].m = 1;
		} 
	    }
	  else
	    {
	      if(pageTable[pageNumber].m != 0)
		{
		  pageTable[pageNumber].m = 0;
		}
	    }
	      
	}
      else
	{
	  //Returns the index of the table entry to be the victim
	  int victim = getPageToReplace(*algorithm, pageTable);
	  
	}
      
	  
	}*/
      
}
  

  printLogFile(memoryReferences); 
  
  delete [] sizeOfProcess;
  delete [] numberMemoryReferences;
  delete [] numberPageFrames;
  delete [] algorithm;
}

void printLogFile(vector<unsigned> references)
{
  FILE *out;
  
  out = fopen("proj05.log", "w");

  if(out == NULL)
    {
      fprintf(stderr, "Unable to open output file\n");
      exit(2);
    }
  else
    {
      for(auto reference: references)
	{
	  fprintf(out, "%08x %05x %02x %03x %c \n", reference,
		  getLogicalAddress(reference),
		  getPageNumber(reference), getPageOffset(reference),
		  getOperation(reference));
	}
    }
  fclose(out);
}

char getOperation(unsigned int reference)
{
  unsigned int operation = (reference & 0x00000FFF);
  if(operation == 0)
    {
      return 'R';
    }
  else
    {
      return 'W';
    }
}

unsigned int getLogicalAddress(unsigned int reference)
{
  return ((reference & 0xFFFFF000) >> 12);
}

unsigned int getPageNumber(unsigned int reference)
{
  return ((reference & 0xFF000000) >> 24);
}

unsigned int getPageOffset(unsigned int reference)
{
  return ((reference & 0x00FFF000) >> 12);
}

/*void updateTableEntries()*/


