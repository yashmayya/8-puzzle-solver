#include<bits/stdc++.h>
using namespace std;

void printBoard(vector<int> currentState)
{
	for(int i=0; i<3; i++)
	{
		for(int j=0; j<3; j++)
		{
			cout<<currentState[3*i+j]<<" ";
		}

		cout<<endl;
	}
}

bool isInverted(vector<int> currentState, int i, int j)		//returns true if j comes before i
{
	for(int k=0; k<9; k++)
	{
		if(currentState[k] == i)
			return false;

		else if(currentState[k] == j)
			return true;
	}

}

int numInversions(vector<int> currentState, vector<int> goalState)
{
	int num = 0;

	for(int i=0; i<9; i++)
	{
		for(int j=i+1; j<9; j++)
		{
			if(currentState[i]!=0 && currentState[j]!=0)
			{
				if(isInverted(goalState, currentState[i], currentState[j]))
				{
					num++;
				}
			}
		}
	}

	return num;
}

bool isSolvable(vector<int> currentState, vector<int> goalState)
{
	return (numInversions(currentState, goalState)%2==0);
}

bool isGoalState(vector<int> currentState, vector<int> goalState)
{
	return (currentState == goalState);
}

int h1(vector<int> currentState, vector<int> goalState)
{
	int numMisplaced = 0;

	for(int i=0; i<9; i++)
	{
		if(currentState[i]!=0)	//Check if not a blank tile
		{
			if(currentState[i] != goalState[i])
				numMisplaced++;
		}
	}

	return numMisplaced;
}

int h2(vector<int> currentState, vector<int> goalState)
{
	int totalDistance = 0;

	for(int i=0; i<9; i++)
	{
		if(currentState[i] != 0)
		{
			for(int j=0; j<9; j++)
			{
				if(currentState[i] == goalState[j])
				{
					int x1 = i/3, y1 = i%3, x2 = j/3, y2 = j%3;
					totalDistance += (abs(x2-x1) + abs(y2-y1));
				}
			}
		}
	}

	return totalDistance;
}

int blankTilePosition(vector<int> currentState)
{
	for(int i=0; i<9; i++)
	{
		if(currentState[i]==0)
			return i;
	}
}

vector<vector<int> > nextStates(vector<int> currentState)	//Move the blank tile up, down, left, right if possible
{
	vector<vector<int> > next;

	int blankpos = blankTilePosition(currentState);

	//Check if left move possible
	if(blankpos%3!=0)
	{
		vector<int> copyCurrentState(currentState.begin(), currentState.end());
		swap(copyCurrentState[blankpos], copyCurrentState[blankpos-1]);
		next.push_back(copyCurrentState);
	}

	//Check if up move possible
	if(blankpos>=3)
	{
		vector<int> copyCurrentState(currentState.begin(), currentState.end());
		swap(copyCurrentState[blankpos], copyCurrentState[blankpos-3]);
		next.push_back(copyCurrentState);
	}

	//Check if right move possible
	if(blankpos%3!=2)
	{
		vector<int> copyCurrentState(currentState.begin(), currentState.end());
		swap(copyCurrentState[blankpos], copyCurrentState[blankpos+1]);
		next.push_back(copyCurrentState);
	}

	//Check if down move possible
	if(blankpos<6)
	{
		vector<int> copyCurrentState(currentState.begin(), currentState.end());
		swap(copyCurrentState[blankpos], copyCurrentState[blankpos+3]);
		next.push_back(copyCurrentState);
	}

	return next;
}

vector<pair<vector<int>, int> > generateNextStates(vector<int> currentState, int depth)
{
	queue<vector<int> > q;
	map<vector<int>, int> dep;
	vector<pair<vector<int>, int> > tmp;	//pair of state and distance from initial state node
	
	q.push(currentState);
	dep.insert({currentState, 0});

	while(!q.empty())
	{
		vector<int> fr = q.front();
		q.pop();
		if(dep[fr] == depth)
			break;

		vector<vector<int> > next = nextStates(fr);

		for(int i=0; i<next.size(); i++)
		{
			vector<int> currentNext = next[i];
			if(dep.find(currentNext) == dep.end())
			{
				dep.insert({currentNext, dep[fr]+1});
				q.push(currentNext);
				tmp.push_back({currentNext, dep[currentNext]});
			}
		}
	}

	return tmp;
}

int AStar(vector<int> initialState, vector<int> goalState, int heuristic, int depth, int &totalNodes, int &maxFringe)	//TODO: Add depth
{
	function<int(vector<int>, vector<int>)> heuristicFunction;

	if(heuristic==1)
		heuristicFunction = h1;
	else
		heuristicFunction = h2;

	set<pair<int, vector<int> > > fringe;
	set<vector<int> > explored;
	
	map<vector<int>, int> fvalue;

	int hinit = heuristicFunction(initialState, goalState);
	fringe.insert(make_pair(hinit, initialState));	//f = g+h and g(initialState) = 0
	fvalue.insert({initialState, hinit});

	while(!fringe.empty())
	{
		int currentf = fringe.begin()->first;
		vector<int> currentState = fringe.begin()->second;
		fringe.erase(fringe.begin());
		
		if(isGoalState(currentState, goalState))
		{
			return currentf;
		}

		explored.insert(currentState);

		vector<pair<vector<int>, int> > next = generateNextStates(currentState, depth);

		for(int i=0; i<next.size(); i++)
		{
			vector<int> currentNext = next[i].first;
			int dgval = next[i].second;
			int fval = heuristicFunction(currentNext, goalState) + currentf - heuristicFunction(currentState, goalState) + dgval;

			if(explored.find(currentNext)==explored.end() && fringe.find({fvalue[currentNext], currentNext})==fringe.end())
			{
				fringe.insert({fval, currentNext});
				fvalue.insert({currentNext, fval});
				totalNodes++;
				maxFringe = max(maxFringe, (int)fringe.size());
			}

			else if(fringe.find({fvalue[currentNext], currentNext})!=fringe.end())
			{
				if(fvalue[currentNext] > fval)
				{
					auto it = fringe.find({fvalue[currentNext], currentNext});
					fringe.erase(it);
					fringe.insert({fval, currentNext});
					fvalue[currentNext] = fval;
				}
			}
		}
	}
}

vector<vector<int> > generateRandom()
{
	srand(time(0));
	vector<int> initialState = {0,1,2,3,4,5,6,7,8};

	vector<vector<int> > ans;

	for(int i=0; i<10; i++)	//generate 10 random states
	{
		random_shuffle(initialState.begin(), initialState.end());
		ans.push_back(initialState);
	}

	return ans;
}


// A state is represented by a vector of 9 values named board. 	[0 1 2]	So for example, board[1] will contain the number (1-9) present in the spot labeled 1. A blank spot is denoted by 0
// 																[3 4 5]	
// 																[6 7 8]

int main()
{
	vector<int> goalState = {0,1,2,3,4,5,6,7,8};	//Assuming goal state always the same as it isn't explicitly mentioned in the assignment doc. 
	
	int depth;
	cout<<"Input depth parameter: ";
	cin>>depth;
	cout<<endl;
	
	cout<<"Enter initial state (0 for blank tile). For example: "<<endl;
	cout<<"7 2 4"<<endl;
	cout<<"5 0 6"<<endl;
	cout<<"8 3 1"<<endl;
	cout<<endl;

	vector<int> initialState;

	for(int i=0; i<9; i++)
	{
		int t;
		cin>>t;
		initialState.push_back(t);
	}

	cout<<endl;

	cout<<"Default goal state is:"<<endl;
	printBoard(goalState);

	cout<<endl<<"Type y if you want to use the default goal state and n if you want to input goal state manually"<<endl;
	char yorn;
	cin>>yorn;
	cout<<endl;

	if(yorn=='n')
	{
		for(int i=0; i<9; i++)
		{
			cin>>goalState[i];
		}
	}

	int totalNodes=0, maxFringe=0;

	if(!isSolvable(initialState, goalState))
	{
		cout<<"The goal state is not reachable from the initial state"<<endl;
	}

	else
	{
		cout<<endl;
		cout<<"Using tile mismatch heuristic and input depth parameter ("<<depth<<")"<<". The number of moves needed is "<<AStar(initialState, goalState, 1, depth, totalNodes, maxFringe)<<endl;
		totalNodes=0, maxFringe=0;
		cout<<"Using Manhattan distance heuristic and input depth parameter ("<<depth<<")"<<". The number of moves needed is "<<AStar(initialState, goalState, 2, depth, totalNodes, maxFringe)<<endl;
		totalNodes=0, maxFringe=0;
		cout<<"Using tile mismatch heuristic and depth parameter 1. The number of moves needed is "<<AStar(initialState, goalState, 1, 1, totalNodes, maxFringe)<<endl;
		totalNodes=0, maxFringe=0;
		cout<<"Using Manhattan distance heuristic and depth parameter 1. The number of moves needed is "<<AStar(initialState, goalState, 2, 1, totalNodes, maxFringe)<<endl;
		cout<<endl;
	}

	cout<<"------------------------------------------------------------------------------------------------------";
	cout<<endl;
	
	vector<vector<int> > randomStates = generateRandom();
	cout<<"Checking on randomly generated initial states now"<<endl<<endl;


	for(int i=0; i<10; i++)
	{
		totalNodes=0, maxFringe=0;
		vector<int> initState = randomStates[i];
		cout<<"Random state #"<<i+1<<endl<<endl;

		if(!isSolvable(initState, goalState))
		{
			cout<<"Random state #"<<i+1<<" is not solvable (odd number of inversions)"<<endl;
			cout<<"------------------------------------------------------------------------------------------------------";
			cout<<endl;
			continue;
		}

		cout<<"Using tile mismatch heuristic and input depth parameter ("<<depth<<")."<<endl;
		cout<<"Number of moves is "<<AStar(initState, goalState, 1, depth, totalNodes, maxFringe)<<endl;
		cout<<"Number of nodes generates is "<<totalNodes<<" and max fringe length is "<<maxFringe<<endl;
		cout<<endl;

		totalNodes=0, maxFringe=0;
		cout<<"Using Manhattan distance heuristic and input depth parameter ("<<depth<<")."<<endl;
		cout<<"Number of moves is "<<AStar(initState, goalState, 2, depth, totalNodes, maxFringe)<<endl;
		cout<<"Number of nodes generates is "<<totalNodes<<" and max fringe length is "<<maxFringe<<endl;
		cout<<endl;

		totalNodes=0, maxFringe=0;
		cout<<"Using tile mismatch heuristic and depth parameter 1"<<endl;
		cout<<"Number of moves is "<<AStar(initState, goalState, 1, 1, totalNodes, maxFringe)<<endl;
		cout<<"Number of nodes generates is "<<totalNodes<<" and max fringe length is "<<maxFringe<<endl;
		cout<<endl;

		totalNodes=0, maxFringe=0;
		cout<<"Using Manhattan distance heuristic and depth parameter 1"<<endl;
		cout<<"Number of moves is "<<AStar(initState, goalState, 2, 1, totalNodes, maxFringe)<<endl;
		cout<<"Number of nodes generates is "<<totalNodes<<" and max fringe length is "<<maxFringe<<endl;
		cout<<endl;
		cout<<"------------------------------------------------------------------------------------------------------";
		cout<<endl;
	}
}
