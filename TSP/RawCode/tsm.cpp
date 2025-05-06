#include<tsm.h>

int ans[21];
bool visited[20];
int  dem=0;
int Greedy=0;
int best_cost = 999999;
int allmin=999999;

int firstMin(int Arr[20][20], int numVertex, int a)
{
    bool found = false;
    int min = 999999;
    for (int k = 0; k < numVertex; k++)
    {
        if (a != k)
        {
            if (!found || Arr[a][k] < min)
            {
                min = Arr[a][k];
                found = true;
            }
        }
    }
    return min;
}

int secondMin(int Arr[20][20], int numVertex, int a)
{
    bool firstFound = false, secondFound = false;
    int first = 999999, second = 999999;
    for (int j = 0; j < numVertex; j++)
    {
        if (a == j)
            continue;

        if (!firstFound || Arr[a][j] < first)
        {
            second = first;
            secondFound = firstFound;
            first = Arr[a][j];
            firstFound = true;
        }
        else if ((!secondFound || Arr[a][j] < second) && Arr[a][j] != first)
        {
            second = Arr[a][j];
            secondFound = true;
        }
    }
    return second;
}

void TSPRecursive(int Arr[20][20], int bound, int total_cost,int level, int path[],int numVertex)
{
	dem++;
    if (level == numVertex)
    {
        if (Arr[path[level-1]][path[0]] != 0)
        {
            int cost = total_cost +
                           Arr[path[level-1]][path[0]];

            if (cost < best_cost)
            {
                for (int i=0; i<numVertex; i++){
                    ans[i] = path[i];
                }
				ans[numVertex] = path[0];
                best_cost = cost;
                // for (int i=0; i<numVertex; i++){
                //     cout<<ans[i]<<" ";
                // }
                // cout<<best_cost<<endl;
            }
        }
        return;
    }

    for (int i = 0; i < numVertex; i++)
    {
        if (Arr[path[level-1]][i] != 0 && visited[i] == false)
        {
            int temp = bound;
            total_cost += Arr[path[level-1]][i];
            if (level == 1)
                bound -= ((firstMin(Arr,numVertex, i-1)+firstMin(Arr,numVertex, i)) / 2);
            else if (level > 1)
                bound -= ((secondMin(Arr,numVertex, i-1)+firstMin(Arr,numVertex, i)) / 2);
            if(numVertex<16) bound=0;

            if (bound + total_cost < best_cost
			&&total_cost+allmin*(numVertex-i-1)<best_cost
			)
            {
                path[level] = i;
                visited[i] = true;
                TSPRecursive(Arr, bound, total_cost, level+1, path,numVertex);
            }
            total_cost -= Arr[path[level-1]][i];
            bound = temp;
            for (int i=0; i<numVertex; i++){
				visited[i]=0;
			}
			for (int i=0; i<=level-1; i++){
                visited[path[i]] =1;
            }
        }
    }
}

string Traveling(int Arr[20][20],int numVertex,char Start)
{	
	int st=int(Start)-65;
	int path[21];
	int bound = 0;
	for (int i=0; i<numVertex; i++){
		path[i]=-1;
		visited[i]=0;
	}
	
	
	int check[20]={0	};
	int row =0;
	check[row] = 1;
	int count = 1;
	while (count < numVertex) {
	    int min = 999999;
	    int nextRow = -1;
	    for (int i = 0; i < numVertex; i++) {
	        if (Arr[row][i] < min && Arr[row][i] != 0 && check[i] == 0) {
	            min = Arr[row][i];
	            nextRow = i;
	        }
	    }
	    if (nextRow != -1) {
	        row = nextRow;
	        Greedy += min;
	        check[row] = 1;
	    } else {
	        break;
	    }
	}
	
	Greedy += Arr[row][0];
	best_cost=Greedy;
	
	for(int i=0;i<numVertex;i++){
		for(int j=0;j<numVertex;j++){
			if(allmin>Arr[i][j]&&Arr[i][j]!=0) allmin=Arr[i][j];
		}
	}

	for (int i=0; i<numVertex; i++){
		bound += (firstMin(Arr,numVertex, i) +secondMin(Arr,numVertex, i));
	}
	bound/=2;
	visited[0] = true;
	path[0] = 0;
	TSPRecursive(Arr, bound, 0, 1, path,numVertex);
	
	string answer;
    for (int i = 0; i <= numVertex; i++) {
        answer += char(ans[i] + 65);
        if (i < numVertex) {
            answer += " ";
        }
    }
    
    if(st!=0){
    string temp;
	int pos=0;
    for (int i = 0; i < numVertex; ++i) {
        if (ans[i] == st) {
            pos = i;
            break;
        }
    }
    for (int i = pos; i < numVertex; ++i) {
        temp+= char(ans[i] + 65);
        if (i < numVertex) {
            temp += " ";
        }
    }

    for (int i = 0; i < pos; ++i) {
        temp+= char(ans[i] + 65); ;
        if (i < numVertex) {
            temp += " ";
        }
    }
    temp+=Start;
    	return temp;
	}
//	cout<<endl;
 //   cout<<best_cost;
 //cout<<dem;
    return answer;
}