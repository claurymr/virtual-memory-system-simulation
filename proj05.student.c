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
#include <utility>
#include <algorithm>
#include <new>
#include <vector>
#include "/user/cse410/McCullen/Projects/project05.lib.h"

using namespace std;

class TableEntry 
{
    
public:
    int v;
    int r;
    int m;
    int frame;

 private:
    int time;

 public:
    int getTime(){return time;}
    void setTime(int t){time = t;}
    
    TableEntry();
    ~TableEntry();
};

TableEntry::TableEntry(){
  
}

TableEntry::~TableEntry(){
    
}

/*---------------------------------------------------------------------------
    Name:  printLogFile

    Purpose:  Print references made bu the udser process to proj04.log
    Receive:  A vector of unsigned integers, which are the references
    Return:   Nothing, just print to an output file
  ---------------------------------------------------------------------------*/
void printLogFile(vector<unsigned>,vector<char>, vector<string>, vector<TableEntry*>);

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

/*---------------------------------------------------------------------------
    Name:  frameAvailable

    Purpose:  Get availability of frames
    Receive:  A vector of table entries pointers and an integer for number
               page frames.
    Return:   An bool for whether there are any frames available or not
  ---------------------------------------------------------------------------*/
bool frameAvailable(vector<TableEntry*>, int);

/*---------------------------------------------------------------------------
    Name:  updateTableEntries

    Purpose:  Update Time varible for each page table entry
    Receive:  A vector of table entries pointers, an unsigned integer
               for page number and a string to indicate algorithm
    Return:   Nothing
  ---------------------------------------------------------------------------*/
void updateTableEntries(vector<TableEntry*>, unsigned int, string);

/*---------------------------------------------------------------------------
    Name:  replacePage

    Purpose:  Finds the page to be replaced in RAM 
    Receive:  A vector of table entries pointers, an unsigned integer
               for page number and a string to indicate algorithm
    Return:   The page to be replaced
  ---------------------------------------------------------------------------*/
int replacePage(vector<TableEntry*>, unsigned int, string);

int main(int argc, char *argv[])
{
  vector<unsigned int> memoryReferences;
  vector<TableEntry*> pageTable;
  
  int* sizeOfProcess = new int[1];
  int* numberMemoryReferences = new int[1];
  int* numberPageFrames = new int[1];
  char* algorithm = new char[20];
  string strAlgorithm = "";
  
  vector<char> pageFaults;
  vector<string> writeBacks;
  vector<int> availableFrames;

  int pagesDiskToRAM = 0;
  int pagesRAMToDisk = 0;
  int totalPageFaults = 0;
        
  setup(sizeOfProcess, numberMemoryReferences, numberPageFrames, algorithm);

  //Convert char* algorithm to string
  strAlgorithm = string(algorithm);
  //transform(strAlgorithm.begin(), strAlgorithm.end(), std::back_inserter(strAlgorithm), ::toupper);

  //cout << strAlgorithm;

  //Run time error correction
  if(sizeOfProcess < 0)
    {
      cout << "Size of Process (in page) input invalid" << endl;
      exit(2);
    }
  if(*numberMemoryReferences < 0)
    {
      cout << "Number of memory references input invalid" << endl;
      exit(2);
    }
  if(*numberPageFrames < 0)
    {
      cout << "Number of page frames input invalid" << endl;
      exit(2);
    }

  /* if(strAlgorithm != "LRU" || strAlgorithm != "FIFO" || strAlgorithm != "Clock")
    {
      cout << "Algorithm input invalid" << endl;
      exit(2);
      }*/
  if(strAlgorithm == "LRU" || strAlgorithm == "FIFO" || strAlgorithm == "Clock")
    {

    

  //Create Table Entries  
  for(int i = 0; i < *sizeOfProcess; i++)
    {
      //TableEntry{v,r,m,frame number,time since last referenced, time in RAM}
      pageTable.push_back(new TableEntry());
      pageTable[i]->v = 0;
      pageTable[i]->r = 0;
      pageTable[i]->m = 0;
      pageTable[i]->frame = 0;
      pageTable[i]->setTime(0);
    }

  /*Push the references obtained from calling reference() to a
    vector of unsigned int*/
  
  for(int i = 0; i < *numberMemoryReferences; i++)
    {
      memoryReferences.push_back(reference());
      pageFaults.push_back(' ');
      writeBacks.push_back(" ");
    }
  
  int countFrames = 0;
  
  for(unsigned int i = 0; i < memoryReferences.size(); i++)
    {
      int pageNumber = getPageNumber(memoryReferences[i]);
      char operation = getOperation(memoryReferences[i]);

      //countFrames helps assign the right number of frame to the page
      if(countFrames == *numberPageFrames)
	{
	  countFrames = 0;
	}

      if(frameAvailable(pageTable, *numberPageFrames))
	{
	  //Before all frames are taken
	  if(pageTable[pageNumber]->v == 0)
	    {
	      pageTable[pageNumber]->v = 1;
	      pageTable[pageNumber]->setTime(0);
	      pageTable[pageNumber]->frame = countFrames;
	      pageFaults[i] = 'F';
	      totalPageFaults++;
	      pagesDiskToRAM++;
	    }
	  
	  if(pageTable[pageNumber]->r == 0)
	    {
	      pageTable[pageNumber]->r = 1;
	    }
	      
	  if(operation == 'W')
	    {
	      if(pageTable[pageNumber]->m == 0)
		{
		  pageTable[pageNumber]->m = 1;
		}
	    }
	}
      else
	{
	  /*cout << "Do I even come here?" << endl;
	    cout << "after all frames are occupied: v-" << pageTable[pageNumber]->v << endl;*/

	  int pageToReplace = 0;
	  
	  //changes the victim for the new page to occupy a page frame
	  if(pageTable[pageNumber]->v == 0)
	    {
	      pageToReplace = replacePage(pageTable, pageNumber, strAlgorithm);
	      pageTable[pageToReplace]->v = 0;
	      pageTable[pageToReplace]->setTime(0);
	      pageTable[pageNumber]->v = 1;
	      if(pageTable[pageToReplace]->m == 1)
		{
		  writeBacks[i] = "WB";
		}
	      pageTable[pageNumber]->frame = pageTable[pageToReplace]->frame;
	      pageFaults[i] = 'F';
	      pagesDiskToRAM++;
	      pagesRAMToDisk++;
	      totalPageFaults++;
	    }

	  if(pageTable[pageNumber]->r == 0)
	    {
	      pageTable[pageNumber]->r = 1;
	      if(strAlgorithm == "Clock")
		{
		  pageTable[pageNumber]->setTime(0);
		}
	    }
	      
	  if(operation == 'W')
	    {
	      if(pageTable[pageNumber]->m == 0)
		{
		  pageTable[pageNumber]->m = 1;
		}
	    }
	  
	}
      countFrames++;

      updateTableEntries(pageTable, pageNumber, strAlgorithm);
	  
    }

  /*Print simulation summary*/
  cout<< "--------Simulation Summary--------\n"
      << "Total Number of Page Faults: " << totalPageFaults << "\n"
      << "Pages from Disk to RAM: " << pagesDiskToRAM << "\n"
      << "Pages from RAM to Disk: " << pagesRAMToDisk << endl;

  /*Print to the Log File*/
  printLogFile(memoryReferences, pageFaults, writeBacks, pageTable); 
  
  delete [] sizeOfProcess;
  delete [] numberMemoryReferences;
  delete [] numberPageFrames;
  delete [] algorithm;
    }
  else
    {
      cout << "Algorithm input invalid" << endl;
      exit(2);
    }  
}

int replacePage(vector<TableEntry*> pageTable, unsigned int pageNumber, string algorithm)
{
  int max = 0;
  int pageToReplace = 0;

  if (algorithm == "LRU")
    {
      for(unsigned int i = 0; i < pageTable.size(); i++)
	{
	  int tempTime = pageTable[i]->getTime();
	  if (tempTime > max)
	    {
	      max = tempTime;
	      pageToReplace = i;
	    }
    
	}
    }
  else if(algorithm == "FIFO")
    {
      for(unsigned int i = 0; i < pageTable.size(); i++)
	{
	  int tempTime = pageTable[i]->getTime();
	  if (tempTime > max)
	    {
	      max = tempTime;
	      pageToReplace = i;
	    }
    
	} 
    }
  else if(algorithm == "Clock")
    {
      bool arePagesCleaned = false;
      
      for(auto page: pageTable)
	{
	  if(page->r == 0 && page->v == 1)
	    {
	      arePagesCleaned = true;
	    }
	}
      
      for(unsigned int i = 0; i < pageTable.size(); i++)
	{
	  if(!arePagesCleaned)
	    {
	      if(pageTable[i]->r != 0)
		{
		  pageTable[i]->r = 0;
		}
	    }
	  
	  int tempTime = pageTable[i]->getTime();
	  if (tempTime > max)
	    {
	      max = tempTime;
	      pageToReplace = i;
	    }
	  
	}
      
    }
  
  return pageToReplace;
  
}

bool frameAvailable(vector<TableEntry*> pageTable, int numberPageFrames){
    
    int pageFrames = 0;
    for(auto table: pageTable){
        if (table->v == 1) pageFrames++;
    }
    
    return (pageFrames < numberPageFrames);
}

void updateTableEntries(vector<TableEntry*> pt, unsigned int pageNumber, string algorithm){
    
  int tempTime = 0;
  
  if(algorithm == "LRU")
    {
      for(unsigned int i = 0; i < pt.size(); i++){
	  
	  if(i == pageNumber && pt[i]->v == 1)
	    {
	      pt[i]->setTime(0);
	    }
	  else if(i != pageNumber && pt[i]->v == 1)
	    {
	      tempTime = pt[i]->getTime() + 1;
	      pt[i]->setTime(tempTime);
	    }
      }
      
    }
  else if(algorithm == "FIFO")
    {
      for(unsigned int i = 0; i < pt.size(); i++){
	  
	if(pt[i]->v == 1)
	{
	    tempTime = pt[i]->getTime() + 1;
	    pt[i]->setTime(tempTime);
	}
      }
    }
  else if(algorithm == "Clock")
    {
      for(unsigned int i = 0; i < pt.size(); i++){
	
	if(pt[i]->v == 1)
	{
	    tempTime = pt[i]->getTime() + 1;
	    pt[i]->setTime(tempTime);
	}
      }
    }
  
}

void printLogFile(vector<unsigned> references,vector<char> pageFaults, vector<string> writeBacks, vector<TableEntry*> pt)
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
      for(unsigned int i = 0; i < references.size(); i++)
	{
	  int pageNumber = getPageNumber(references[i]);
	  
	  fprintf(out, "REF %08x %05x %02x %03x %c %c %s %02x%03x\n\n%d %d %d %02x\n\n", references[i],
		  getLogicalAddress(references[i]),
		  pageNumber, getPageOffset(references[i]),
		  getOperation(references[i]), pageFaults[i], writeBacks[i].c_str(), pt[pageNumber]->frame, getPageOffset(references[i]),
		  pt[pageNumber]->v, pt[pageNumber]->r, pt[pageNumber]->m, pt[pageNumber]->frame);
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


