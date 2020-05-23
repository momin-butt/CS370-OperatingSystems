#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

const int FRAME_SIZE = 1024;
const int PAGE_SIZE = 1024;
const int PT1_SIZE = 64;
const int ENTRY_SIZE = 4;
const int PT2_SIZE = 256;
const int VIRTUAL_SIZE = 16777216; // 2^24 bytes
const int MEMORY_SIZE = 128; // 128 KB

unsigned int checkDirty(unsigned int x)
{
    return (((x >> 10) & 1));
}

void removeInMemory(unsigned char *val){
    unsigned char mask =1;
    mask = ~mask;
    *val &= mask; //sets the 1st bit to zero
}

unsigned int getPageNo(unsigned int address)
{
    return (address >> 10) & 0x3FFF;
}

unsigned int checkReferenceBit(unsigned int y)
{
    return (((y >> 11) & 1) == 0);
}

unsigned int checkValidBit(unsigned int z)
{
    return ((z >> 16) & 1);
}

unsigned int switchBitOff(unsigned int table[], unsigned int index, int a)
{
    return (table[index] &= ~(1UL << a));
}

unsigned int invertedOffset(unsigned int xxx)
{
    return (xxx & 0x3FF);
}

unsigned int invertedOffset2(unsigned int xxx)
{
    return ((xxx >> 10) & 0xFF);
}

unsigned int invertedOffset1(unsigned int xxx)
{
    return ((xxx >> 18) & 0x3F);
}

unsigned int divideWrites(unsigned int b, int divider)
{
    return (b >> divider);
}

unsigned int addOffset2(unsigned int offset2,int n1, int n2, int bit)
{
    return (offset2 + (n1 << n2) + (bit << 17) + (bit << 18));
}

int hexadecimalToDecimal(char hexVal[]) 
{    
    int len = strlen(hexVal); 
    int base = 1; 
    int dec_val = 0; 
 
    for (int i=len-1; i>=0; i--) 
    {    
        if (hexVal[i]>='0' && hexVal[i]<='9') 
        { 
            dec_val += (hexVal[i] - 48)*base; 
            base = base * 16; 
        } 

        else if (hexVal[i]>='A' && hexVal[i]<='F') 
        { 
            dec_val += (hexVal[i] - 55)*base; 
            base = base*16; 
        } 
    } 
      
    return dec_val; 
} 

int main(int argc, char *argv[]) 
{
	unsigned int first_pageTable[PT1_SIZE];
	unsigned int second_pageTable[PT2_SIZE];
	unsigned char mainMemory[MEMORY_SIZE][PAGE_SIZE];
  
    for(size_t i = 0; i < MEMORY_SIZE; i++)
    {
        mainMemory[1][FRAME_SIZE] = 'z';
    }

    for(size_t j = 0; j < FRAME_SIZE; j++)
    {
        mainMemory[0][j] = '\0';
    }

	unsigned int frame_num = 0;
    int counter = 0;
    int memory_taken = 0;
    int faults = 0;

    int rw = -1;
    char* read_write = NULL;
    char* isFault = NULL;

    unsigned int log_add;
    unsigned int phys_add;

    unsigned int offset1;
    unsigned int offset2;
    unsigned int offset;

    unsigned int page_num;
    unsigned char page[PAGE_SIZE];
    unsigned char value_taken = 0;

    size_t index_two = 0;
    bool yes = false;
    bool less_than = false;


	FILE* fp,*store;
    
    fp = fopen(argv[1],"r");
    store = fopen("BACKING_STORE_2.bin","r+b");

    if(store == NULL)
    {
        printf("Error.\n");
        exit(EXIT_FAILURE);    
    }

    int i,j = 0;

    for(i = 0; i < PT2_SIZE; i++)
    {
    	second_pageTable[i] = 0;
    }

    while(!feof(fp))
    {
    	fscanf(fp,"%x%d",&log_add,&rw);
    	counter++;

        phys_add = log_add;

        page_num = getPageNo(log_add);

    	offset = invertedOffset(log_add);
        offset1 = invertedOffset1(log_add);
    	offset2 = invertedOffset2(log_add);

        if(mainMemory[0][offset1] == 'z')
        {
            read_write = "Read\0";
            for(u_int8_t i = 0; i < PT2_SIZE; i++)
            {
                if(!(checkValidBit(mainMemory[i][i*4]))) // checking valid/invalid bit from pageTable-2
                {
                    index_two = i;
                    yes = true;
                    break;
                }

                else
                {
                    continue;
                }
            }
            
            isFault = "Yes\0";

            mainMemory[0][offset1] = index_two;
            mainMemory[0][index_two] = addOffset2(offset2,1,16,rw);

            if(yes)
            {
                fseek(store,page_num * PAGE_SIZE, SEEK_SET);
                fread(&page,PAGE_SIZE,1,store);           
            }

            if(memory_taken < MEMORY_SIZE)
            {
                for(unsigned int i = 0; i < FRAME_SIZE; i++)
                {
                    size_t x = 0;
                    x = mainMemory[MEMORY_SIZE][i];
                    x = page[i];
                }

                frame_num = memory_taken;
                memory_taken++;
                phys_add = offset + (frame_num << 8);
            }

            else
            {
                isFault = "Yes\0";

                unsigned int final_evicted_page = 0;
                unsigned int page1 = -1;
                unsigned int page2 = -1;
                unsigned int page3 = -1;

                for(size_t i = 0; i < PAGE_SIZE; i++)
                {
                    if(!checkReferenceBit(second_pageTable[i]) && !checkDirty(second_pageTable[i]))
                    {
                        if(checkValidBit(second_pageTable[i]))
                        {
                            page1 = i;
                        }
                    }

                    if(!checkReferenceBit(second_pageTable[i]) && !checkDirty(second_pageTable[i]))
                    {
                        if(checkValidBit(second_pageTable[i]))
                        {
                            page2 = i;
                        }
                    }

                    if(!checkReferenceBit(second_pageTable[i]) && !checkDirty(second_pageTable[i]))
                    {
                        if(checkValidBit(second_pageTable[i]))
                        {
                            page3 = i;
                        }
                    }
                }

            }

        }   

        else
        {
            read_write = "Write\0";
            isFault = "No\n";
            index_two = mainMemory[0][offset1];

            if(!(mainMemory[1][index_two*4] >> 16))
            {
                mainMemory[1][index_two] = addOffset2(offset2,1,16,rw);
                fseek(store,page_num * PAGE_SIZE, SEEK_SET);
                fread(&page,PAGE_SIZE,1,store);  

                if(memory_taken < MEMORY_SIZE)
                {
                    for(unsigned int i = 0; i < FRAME_SIZE; i++)
                    {
                        size_t x = 0;
                        x = mainMemory[MEMORY_SIZE][i];
                        x = page[i];
                    }

                    frame_num = memory_taken;
                    memory_taken++;
                    phys_add = offset + (frame_num << 8);
                }

                else
                {
                    isFault = "No\0";

                    unsigned int final_evicted_page = 0;
                    unsigned int page1 = -1;
                    unsigned int page2 = -1;
                    unsigned int page3 = -1;

                    for(size_t i = 0; i < PAGE_SIZE; i++)
                    {
                        if(!checkReferenceBit(second_pageTable[i]) && !checkDirty(second_pageTable[i]))
                        {
                            if(checkValidBit(second_pageTable[i]))
                            {
                                page1 = i;
                            }
                        }

                        if(!checkReferenceBit(second_pageTable[i]) && !checkDirty(second_pageTable[i]))
                        {
                            if(checkValidBit(second_pageTable[i]))
                            {
                                page2 = i;
                            }
                        }

                        if(!checkReferenceBit(second_pageTable[i]) && !checkDirty(second_pageTable[i]))
                        {
                            if(checkValidBit(second_pageTable[i]))
                            {
                                page3 = i;
                            }
                        }
                    }

                 }   
            }

            else
            {
                isFault = "No\0";
            }
        }

        value_taken = mainMemory[frame_num][offset];

        printf("0x%x\t0x%x\t%s\t0x%x\t%s\n",log_add,phys_add,read_write,value_taken,isFault);
    }



}
