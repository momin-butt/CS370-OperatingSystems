#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const int PAGE_SIZE = 256;
const int MEMSIZE = 64;
const int BIT_OFF = -1;

unsigned int checkDirty(unsigned int x)
{
    return (((x >> 10) & 1));
}

unsigned int getPageNo(unsigned int address)
{
    return (address >> 8) &  0xFF;
}

unsigned int checkReferenceBit(unsigned int y)
{
    return (((y >> 11) & 1) == 0);
}

unsigned int checkValidBit(unsigned int z)
{
    return ((z >> 8) & 1);
}

unsigned int switchBitOff(unsigned int table[], unsigned int index, int a)
{
    return (table[index] &= ~(1UL << a));
}

unsigned int inverted(unsigned int xxx)
{
    return (xxx & 0xFF);
}

int evictPage(unsigned int table[],int PAGE_SIZE)
{
    unsigned int final_evicted_page = 0;
    unsigned int page1 = BIT_OFF;
    unsigned int page2 = BIT_OFF;
    unsigned int page3 = BIT_OFF;

    for(size_t i = 0; i < PAGE_SIZE; i++)
    {
        if(!checkReferenceBit(table[i]) && !checkDirty(table[i]))
        {
            if(checkValidBit(table[i]))
            {
                page1 = i;
            }
        }

        if(!checkReferenceBit(table[i]) && !checkDirty(table[i]))
        {
            if(checkValidBit(table[i]))
            {
                page2 = i;
            }
        }

        if(!checkReferenceBit(table[i]) && !checkDirty(table[i]))
        {
            if(checkValidBit(table[i]))
            {
                page3 = i;
            }
        }
    }

    if(page1 != BIT_OFF)
    {
        final_evicted_page = page1;
    }

    else if (page2 != BIT_OFF)
    {
        final_evicted_page = page2;
    }

    else
    {
        final_evicted_page = page3;
    }

    return final_evicted_page;
}

unsigned int switchBitOn(unsigned int num)
{
    return (num | 100100000000);
    // return (num ^= 1UL << index);
    // return number & ~(1 << (bit - 1));
}

unsigned int divideWrites(unsigned int b, int divider)
{
    return (b >> divider);
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
    FILE* fp,*store;
    
    fp = fopen(argv[1],"r");
    store = fopen("BACKING_STORE_1.bin","r+b");

    if(store == NULL)
    {
        printf("Error.\n");
        exit(EXIT_FAILURE);    
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
    unsigned int offset;
    unsigned int page_num;
    unsigned char page[PAGE_SIZE];
    unsigned char value_taken = 0;

    unsigned char mainMemory[MEMSIZE][PAGE_SIZE];
    unsigned int pageTable[PAGE_SIZE];

    for(int i = 0; i < PAGE_SIZE; i++)
    {
        pageTable[i] = 0;
    }

    while(!feof(fp))
    {
        fscanf(fp,"%x%d",&log_add,&rw);
        log_add = log_add & 0xFFFF;

        counter++;

        offset = inverted(log_add);
        page_num = getPageNo(log_add);

        int valid_bit = checkValidBit(pageTable[page_num]);

        if(!valid_bit) //isFault
        {
            isFault = "Yes";
            faults++;

            fseek(store,page_num * PAGE_SIZE, SEEK_SET);
            fread(&page,PAGE_SIZE,1,store);
            
            if(memory_taken != MEMSIZE) // memory not full yet
            {
                frame_num = memory_taken;
                memcpy(mainMemory[frame_num],page,PAGE_SIZE);
                memory_taken++;
            }

            else // memory full, evict and replace page
            {
                int evicted_page = 0;
                evicted_page = evictPage(pageTable,PAGE_SIZE);

                frame_num = inverted(pageTable[evicted_page]);
                memcpy(mainMemory[frame_num],page,PAGE_SIZE);

                switchBitOff(pageTable,evicted_page,11); // reference bit off
                switchBitOff(pageTable,evicted_page,8); // valid bit off
                switchBitOff(pageTable,evicted_page,9); // dirty bit off
            }
            

            pageTable[page_num] = frame_num & 0x00;
            pageTable[page_num] = switchBitOn(frame_num);

        }
        else //not a fault
        {
            frame_num = inverted(pageTable[page_num]);
            isFault = "No";
        }

        phys_add = (frame_num << 8) + offset;

        if(rw) //write
        {
            read_write = "Write";
            unsigned int indexer = page_num * PAGE_SIZE;

            unsigned int halved = divideWrites(mainMemory[frame_num][offset],1);

            mainMemory[frame_num][offset] = halved;
            fseek(store,indexer, SEEK_SET); 
            fwrite(mainMemory[frame_num],PAGE_SIZE,1,store);            
        }
        else //read
        {
            read_write = "Read";
        }

        value_taken = mainMemory[frame_num][offset];
        
        printf("0x%x\t0x%x\t%s\t0x%x\t%s\n",log_add,phys_add,read_write,value_taken,isFault);
    }

    printf("\n%s%d\n","Number of page Faults: ",faults);
    printf("%s%d\n","Total Addresses Read: ",counter-1);
    printf("%s%f%c\n","Page fault percentage: ",((faults*1.000)/counter) * 100,'%');
    return 0;
}