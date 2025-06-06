#include<bellman.h>

void BF(int Arr[][20],int numVertex,char Start,int ValueArray[], int PreviousArray[]){
    for(int i=0;i<numVertex;i++){
	    if(ValueArray[i]==0){
	        Start=char('A'+i);
	    }
	}
	int dis0=int(Start-'A');
	ValueArray[dis0]=0;
	int Temp[1000];
	int Temp1[1000];
    for(int a=0;a<numVertex;a++){
        Temp[a]=ValueArray[a];
    }
		 for (int j = 0; j < numVertex; ++j) {
            for (int k = 0; k < numVertex; ++k) {
                if ((Arr[j][k] != 0  && (Temp[j] + Arr[j][k] < ValueArray[k]||ValueArray[k]==-1))&&Temp[j]!=-1) {
                     ValueArray[k]=Temp[j] + Arr[j][k];
                    PreviousArray[k] = j;
                }
            }
        }	
}
string BF_Path(int Arr[][20],int numVertex,char Start,char Goal){
	int dis0=int(Start)-65;
	int ValueArray[1000];
	int PrevArray[1000];
	for(int i=0;i<numVertex;i++){
	    PrevArray[i]=-1;
	    ValueArray[i]=-1;
	}
		ValueArray[dis0]=0;
	int Temp[1000];
	int ans[1000];
	string path;
	for(int i=0;i<numVertex-1;i++){
	    BF(Arr,numVertex,Start,ValueArray,PrevArray);
	}
//	BF(Arr,numVertex,Start,ValueArray,PrevArray)
	int end=int(Goal)-65;
	int start=dis0;
	int temp=end;
	int pos=end;
	int k=0;
	while(temp!=-1){
		temp=PrevArray[pos];
		pos=temp;
		if(temp!=-1){
		ans[k]=temp;
		k++;
		}
	}
	for(int i=k-1;i>=0;i--){
		char ch=char(ans[i]+65);
		path+=ch;
		path+=' ';
	}
	path+=Goal;
	return path;
	 
}

