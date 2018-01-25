
#include <stdio.h>  
#include <stdlib.h>  
#include <time.h>  
#define	MAX_PAGE 9
#define random(x) (rand()%x)
typedef struct __frame {  //pages queue in frame, time order
    int *buf;       // buffer
    int n;          // max buffer size
    int head;       // frame pages head
    int tail;       // frame pages tail
} frame;
 

int main(int argc,char **argv){  
  
	int refLen,* refStr,framePage, invalidNum = 1, * validBit, pageFault = 0, swapOut,i,cur,* historyBits,least,j;
	double faultRate;
	frame queue_frame;//frame的循环数组队列	
    srand((int)time(0));//时间种子 真随机
    
    
	printf("Length of reference string:");
	scanf("%d",&refLen);
	while (invalidNum){//非法重新输入
		printf("The number of page frames(1-7):");
		scanf("%d",&framePage);
		if (framePage<8 && framePage>0)//判断输入合法
			invalidNum = 0;
	}

    refStr = (int *)malloc(refLen*sizeof(int));  
	printf("Generated reference string: ");    
    for( i=0; i<refLen; i++){
    	*(refStr+i) = random(10);
    	//*(refStr+i) = 3;
    	printf("%d ",refStr[i]);
    } //random page-reference string where page numbers range from 0 to 9
    printf("\n");
    
    validBit = (int *)malloc(MAX_PAGE*sizeof(int));  
    for( i=0; i<MAX_PAGE; i++){//set valid-invalid bit
    	*(validBit+i)=0;
    }//previous error: i<framePage

 	queue_frame.n = framePage;
	queue_frame.buf = (int *)malloc(queue_frame.n*sizeof(int));
    queue_frame.head = -1;  
    queue_frame.tail = -1;  
    cur = 0;
	printf("FIFO\n");
    for ( i=0; i<refLen; i++){
    	if( validBit[refStr[i]] == 0){
    		pageFault ++;
    		if (cur == queue_frame.n){
    			swapOut = queue_frame.buf[(++queue_frame.head)%(queue_frame.n)];
    			validBit[swapOut] = 0;
    			printf("%d out\t",swapOut);  
    		}
    		else {
    			cur ++;
    			printf("---\t"); 
    		}
    		validBit[refStr[i]] = 1;
    		queue_frame.buf[(++queue_frame.tail)%(queue_frame.n)] = refStr[i];
    		printf("%d in\n",refStr[i]);
    	}
    	else
    		printf("No page fault.\n");
    } //random page-reference string where page numbers range from 0 to 9
	faultRate = refLen;
	faultRate = pageFault/faultRate * 100;
	printf("Page faults: %d\nPage fault rate: %.2lf%%\n\n",pageFault,faultRate);


	historyBits = (int *)malloc(MAX_PAGE*sizeof(int));
	
	  
    for( i=0; i<MAX_PAGE; i++){//set valid-invalid bit
    	*(validBit+i)=0;
    	*(historyBits+i)=0;
    }//previous error: i<framePage
    queue_frame.head = 0;  
    queue_frame.tail = -1;  
    cur = 0;
	pageFault = 0;

	printf("LRU\n");
	int queueStep = 0;
    for ( i=0; i<refLen; i++){
    	if ( validBit[refStr[i]] == 0){
    		pageFault ++;
    		if (cur == queue_frame.n){
    			least = 255;//8bit 1 byte
    			swapOut=-1;//replace的内容 
				//printf("History bits:\t");
				j = (queue_frame.head)%(queue_frame.n);;
				int record_j;
				queueStep = 0;
				while (queueStep < cur){//找出最小的 总从第一个开始
				//	int j_ = (j+1)%(queue_frame.n);
					if (historyBits[queue_frame.buf[j]]<least){
	//					swapOut = queue_frame.buf[j_];
						record_j = j;
						least = historyBits[queue_frame.buf[j]];
					}
    				printf("%d - %d\t",queue_frame.buf[j],historyBits[queue_frame.buf[j]]);
					historyBits[queue_frame.buf[j]]=historyBits[queue_frame.buf[j]]/2;
					//j = j_;
					j = (j+1)%(queue_frame.n);
					queueStep++;
				}//遍历队列
				//printf("\n");
				queue_frame.head = (record_j)%(queue_frame.n);//防止低位的总被换出
				queue_frame.tail = (record_j-1)%(queue_frame.n);//为后面统一++tail 准备
    			swapOut = queue_frame.buf[(queue_frame.head++)%(queue_frame.n)];
    			validBit[swapOut] = 0;
    			historyBits[swapOut] =0;

    			printf("%d out\t",swapOut);  
    			
    		}
    		else {

				j = (queue_frame.head)%(queue_frame.n);;

				queueStep = 0;
				while (queueStep < cur){//维护history bit
					printf("%d - %d\t",queue_frame.buf[j],historyBits[queue_frame.buf[j]]);
		
					historyBits[queue_frame.buf[j]]=historyBits[queue_frame.buf[j]]/2;
					//j = j_;
					j = (j+1)%(queue_frame.n);
					queueStep++;
				}//遍历队列
    		
    			cur ++;
    			printf("---\t");


    		}
			historyBits[queue_frame.buf[j]]=historyBits[queue_frame.buf[j]]/2;
    		queue_frame.buf[(++queue_frame.tail)%(queue_frame.n)] = refStr[i];     		
    		validBit[refStr[i]] = 1;
    		printf("%d in\n",refStr[i]);

    	}
    	else{//hit
			j = (queue_frame.head)%(queue_frame.n);;

			queueStep = 0;
			while (queueStep < cur){//找出最小的 总从第一个开始
			
					printf("%d - %d\t",queue_frame.buf[j],historyBits[queue_frame.buf[j]]);
								
					historyBits[queue_frame.buf[j]]=historyBits[queue_frame.buf[j]]/2;
					//j = j_;
					j = (j+1)%(queue_frame.n);
					queueStep++;
			}//遍历队列
    		printf("---\t%d hit\n",refStr[i]);
    		
    	}
		historyBits[refStr[i]] += 128;   		

    } //random page-reference string where page numbers range from 0 to 9
	faultRate = refLen;
	faultRate = pageFault/faultRate * 100;
	printf("Page faults: %d\nPage fault rate: %.2lf%%\n\n",pageFault,faultRate);

	free(queue_frame.buf);
	free(historyBits);
  	free(validBit);
	free(refStr);



}  
