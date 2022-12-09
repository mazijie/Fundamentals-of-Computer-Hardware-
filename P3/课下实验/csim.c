#include "cachelab.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define m (sizeof(void*)*8)
struct block{ //块结构（不带数据）
    int valid;
    int tag;
    int timestamp;
};
int detailed=0;
int s=0,E=0,b=0;
int S,B,t;
int C;
int timestamp=0;
int hits=0,misses=0,evictions=0;
struct block ***cache;
char command[256];
FILE *traceFile;
void getOptions(int argc,char* argv[]);
void initCache();
void freeCache();
void callCache(unsigned long long address);
void handleInput();
int main(int argc,char* argv[])
{
    getOptions(argc,argv);//设置参数
    initCache();//缓存器初始化
    handleInput();//处理输入
    fclose(traceFile);
    freeCache();//清理缓存
    printSummary(hits, misses, evictions);//打印数据
    return 0;
}
void getOptions(int argc,char* argv[])
{
    int result;
    while((result=getopt(argc,argv,"vs:E:b:t:"))!=-1)
    {
        
        switch (result)
        {
        case 'v':
            detailed=1;
            break;
        case 's':
            s=atoi(optarg);
            S=1<<s;
            //printf("%d\n",setBits);
            break;
        case 'E':
            E=atoi(optarg);
            //printf("%d\n",linesBits);
            break;
        case 'b':
            b=atoi(optarg);
            B=1<<b;
            //printf("%d\n",blockBits);
            break;
        case 't':
            traceFile=fopen(optarg,"r");
            if(traceFile==NULL)
            {
                printf("File Open Failed.\n");
                exit(-1);
            }
            break;
        default:
            break;
        }
    }
    t=m-(s+b);
    C=B*E*S;
    //if(detailed==1) hits=0;
    //if(traceFile==NULL) hits=0;
    return;
}
void initCache()
{
    cache=malloc(sizeof(void*)*S);
    for(int i=0;i<S;i++)
    {
        cache[i]=malloc(sizeof(void*)*E);
        for(int j=0;j<E;j++)
        {
            struct block *pBlock=malloc(sizeof(struct block));
            pBlock->valid=0;
            pBlock->tag=0;
            pBlock->timestamp=0;
            cache[i][j]=pBlock;
        }
    }
}
void freeCache()
{
    for(int i=0;i<S;i++)
    {
        for(int j=0;j<E;j++)
        {
            free(cache[i][j]);
        }
        free(cache[i]);
    }
    free(cache);
    return;
}
void callCache(unsigned long long address)
{
    int setAddress=address<<t>>(t+b);
    int tagAddress=address>>(s+b);
    struct block **pSet=cache[setAddress];
    struct block *pHitBlock=NULL;//命中的Block
    struct block *pEmptyBlock=NULL;//组set里首个空的block
    struct block *pLRUBlock=NULL;//LRU策略下准备替换的block
    for(int i=0;i<E;i++)
    {
        struct block *pBlock=pSet[i];
        if(pHitBlock==NULL&&pBlock->valid&&pBlock->tag==tagAddress)
        {
            pHitBlock=pBlock;
        }
        if(pEmptyBlock==NULL&&!pBlock->valid)
        {
            pEmptyBlock=pBlock;
        }
        if(pBlock->valid&&(pLRUBlock==NULL||pLRUBlock->timestamp>pBlock->timestamp))
        {
            pLRUBlock=pBlock;
        }
    }
    if(pHitBlock!=NULL)
    {
        hits++;
        if(detailed)
        {
            printf(" hit");
        }
        pHitBlock->timestamp=timestamp;
    }
    else
    {
        misses++;
        if(detailed)
        {
            printf(" miss");
        }
        if(pEmptyBlock!=NULL)
        {
            pEmptyBlock->valid=1;
            pEmptyBlock->tag=tagAddress;
            pEmptyBlock->timestamp=timestamp;
        }
        else
        {
            if(pLRUBlock==NULL)
            {
                printf("LRU Error!\n");
                exit(-1);
            }
            evictions++;
            if(detailed)
            {
                printf(" eviction");
            }
            pLRUBlock->tag=tagAddress;
            pLRUBlock->timestamp=timestamp;
        }
    }
}
void handleOperation()
{
    timestamp++;
    char opType;
    unsigned long long address;
    int size;
    int count=sscanf(command," %c %llx,%d",&opType,&address,&size);
    if(detailed)
    {
        printf("%c %llx,%d",opType,address,size);
        //printf("%s",command);
    }
    if(count==0) return;//I
    switch(opType)
    {
        case 'L':
            callCache(address);
            break;
        case 'S':
            callCache(address);
            break;
        case 'M':
            callCache(address);
            callCache(address);
            break;
        default:
            break;
    }
    if(detailed) printf("\n");
}
void handleInput()
{
    while(fgets(command,256,traceFile))
    {
        while(command[strlen(command)-1]=='\n'||command[strlen(command)-1]=='\r') command[strlen(command)-1]='\0';
        handleOperation();
    }
}