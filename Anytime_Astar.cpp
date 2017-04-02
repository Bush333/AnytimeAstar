#include<iostream>
#include<cmath>
#include<time.h>
#define GRID_X 100
#define GRID_Y 100
using namespace std;
class Node
{
public:
    int position;
    int Hvalue;
    int Gvalue;
    int Fvalue;
    int block;
    char ListName;		//'n' stands for none,'o' for open,'c' for closed list
    Node * parent;
};

//GLOBALS
long sleep_time;
bool Success=false;
Node GRID[GRID_X*GRID_Y];
clock_t Stopper=0;
clock_t begining;

Node * CurrentArrivalPoint;
void CalclulateHvalue(Node A[],int size,Node GoalNode)
{
    int i;
    int Xpos1,Xpos2;
    int Ypos1,Ypos2;
    Xpos1=(GoalNode.position)%GRID_X;
    Xpos1=(Xpos1!=0)?Xpos1:(GRID_X);
    Ypos1=ceil(float(GoalNode.position)/GRID_X);
    for(i=0; i<size; i++)
    {
        if (A[i].block!=1)
        {
            Xpos2=(A[i].position)%GRID_X;
            Xpos2=(Xpos2!=0)?Xpos2:(GRID_X);
            Ypos2=ceil(float(A[i].position)/GRID_X);
            A[i].Hvalue=abs(Xpos2-Xpos1)+abs(Ypos2-Ypos1);
        }
    }
}
void Check(Node * a)
{
    //cout<<"Stopper:"<<Stopper<<endl;

    Stopper=clock();

  //  cout<<"Begnig::"<<begining<<endl;
    /*cout<<"Sleep time"<<sleep_time;
    cout<<'\n'<<"Stopper:"<<Stopper;
    */
    if (Stopper>sleep_time+begining)////////////////////////////////////////////////////////////////
  {

        //cout<<Stopper<<endl;
        Success=true;
        CurrentArrivalPoint=a;//fake destination point
       // cout<<"Success is true"<<endl;
   }
}

void UpdateGvalueAndParent(Node* from,Node* to)
{
   // Stopper++;
    Check(from);
    int Xpos1,Xpos2;
    int Ypos1,Ypos2;
    Xpos1=((*from).position)%GRID_X;
    Xpos1=(Xpos1!=0)?Xpos1:(GRID_X);
    Ypos1=ceil(float((*from).position)/GRID_X);
    Xpos2=((*to).position)%GRID_X;
    Xpos2=(Xpos2!=0)?Xpos2:(GRID_X);
    Ypos2=ceil(float((*to).position)/GRID_X);
    int MovementCost;       //can also be defined by the user.
    if ( (Xpos1==Xpos2)||(Ypos1==Ypos2))
        MovementCost=10;
    else
        MovementCost=14;
    if ((*from).Gvalue+MovementCost<(*to).Gvalue)
    {
        (*to).Gvalue=(*from).Gvalue+MovementCost;
        (*to).Fvalue=(*to).Gvalue+(*to).Hvalue; //Fvalue also gets updated
        (*to).parent=from;
      //  Stopper++;
        Check(from);
    }
}

Node NeighborList[9];
void ExpandNode(Node* MainNode)
{
    int Xpos1,Xpos2;
    int Ypos1,Ypos2;
    Xpos1=((*MainNode).position)%GRID_X;
    Xpos1=(Xpos1!=0)?Xpos1:(GRID_X);
    Ypos1=ceil(float((*MainNode).position)/GRID_X);
    int k=0;
   // Stopper++;
    Check(MainNode);
    //cout<<"Main node:"<<(*MainNode).position<<endl;
    for(int i=-1; i<=1; i++)
        for(int j=-1; j<=1; j++)
        {
            if ( Xpos1+i > 0  &&  Xpos1+i <= GRID_X  &&  Ypos1+j > 0  &&  Ypos1+j <= GRID_Y )
            {
              //  Stopper++;
                Check(MainNode);
                NeighborList[k]=GRID[((*MainNode).position+i+j*GRID_X)-1];
                if (NeighborList[k].block!=1)
                    UpdateGvalueAndParent(MainNode,&NeighborList[k]);
            }
            else
            {
        //        Stopper++;
                Check(MainNode);
                NeighborList[k]=GRID[((*MainNode).position+j+i*GRID_X)-1];
                NeighborList[k].block=1;
            }
            //cout<<"here"<<((*MainNode).position+j+i*GRID_X)<<endl;
            k++;
      //      Stopper++;
            Check(MainNode);
        }
    NeighborList[4].block=1;     //replacing the MainNode with an invalid Node
    /*cout<<"\nIn Expand Node"<<endl;
    for(int i =0; i<9; i++)
        cout<<NeighborList[i].position<<"     "<<NeighborList[i].block<<"    ";
    cout<<'\n';*/
}

void Astar(Node* Root,Node* Goal)
{
    static int counter=0;
    //Stopper++;
    Check(Root);
    counter+=1;
    if ((*Root).position==(*Goal).position)
    {
        Success=true;
        //cout<<"The shortest path is: ";
        return;
    }
    else
    {
        (*Root).ListName='c';
        ExpandNode(Root);
        //Stopper++;
        Check(Root);
       /* cout<<"\nin A*"<<endl;
        cout<<"Root node:"<<(*Root).position<<endl;
        cout<<"Neighbors:"<<endl;
        for(int i =0; i<9; i++)
        {
            //cout<<NeighborList[i].position<<"   "<<" block:"<<NeighborList[i].block<<" ";
           // if (NeighborList[i].block==0)
             //   cout<<"parent"<<(*NeighborList[i].parent).position<<endl;
        }*/
        for(int i=0; i<9; i++)
            if (NeighborList[i].block==0)
            {
          //      cout<<"Neighbor"<<NeighborList[i].position<<endl;
            //    cout<<"In the Grid"<<GRID[NeighborList[i].position-1].position<<endl;
                //Stopper++;
                Check(Root);
                GRID[NeighborList[i].position-1]=NeighborList[i];
      //          cout<<"this should be parent:"<<(*NeighborList[i].parent).position<<endl;
        //        cout<<"parent of"<<GRID[NeighborList[i].position-1].position<<" is:"<<(*GRID[NeighborList[i].position-1].parent).position<<endl;
                if (GRID[NeighborList[i].position-1].ListName=='n')
                    if (GRID[NeighborList[i].position-1].block==0)
                        GRID[NeighborList[i].position-1].ListName='o';
            }
        Node Min=GRID[15];       //MAKE SURE TO ASSIGN IT TO SOME IMPASSABLE BLOCK INITIALLY!!!!!!
        //for(int i=0;i<42;i++)
        //  cout<<"Position:"<<GRID[i].position<<" Listname:"<<GRID[i].ListName<<" Fvalue:"<<GRID[i].Fvalue<<"\n";
        //for(int i=0;i<42;i++)
        // cout<<GRID[i].position<<" "<<GRID[i].Fvalue<<" list name"<<GRID[i].ListName<<endl;
        for(int i=0; i<GRID_X*GRID_Y; i++)
            if ((GRID[i].ListName=='o')&&(GRID[i].Fvalue<Min.Fvalue))
            {
                //Stopper++;
                Check(Root);
                Min=GRID[i];
                //cout<<GRID[i].position<<" ";
                //cout<<GRID[i].Fvalue<<" ";
            }
    //    cout<<"Min position: "<<Min.position<<" "<<Min.Fvalue;
        Root=&GRID[Min.position-1];

        Check(Root);
        if (Success!=true)
        {
            CurrentArrivalPoint=Root;
            Astar(Root,Goal);		//this point may cause error.Put a while loop here!
        }
        else return;
    }
}
void printPath(Node* ToReach,Node* startingFrom)
{
    if ((*ToReach).position!=(*startingFrom).position)
        printPath(ToReach,(*startingFrom).parent);
    cout<<(*startingFrom).position<<" ";
}
bool is_First=true;
void   initialize(Node *,Node *);  // a is start point and b is goal.
Node* new_Solution;
class ctr_manager
{
public:
    void thread_AA( long SleepTime, Node* rN, Node* gN)
    {
        sleep_time=SleepTime;
        if(is_First)
        {
            new_Solution= rN;
            Astar( rN, gN);
            is_First=false;
        }
        else if( rN == new_Solution)
            {
            //    cout<<"rN:"<<(*rN).position<<endl;
            Astar( rN, gN);
          //  cout<<"22222";
            }
        else if( rN != new_Solution)
        {
        //    cout<<"rN:"<<(*rN).position<<endl;
            initialize(rN,gN);
      //       cout<<"3333333333333333333333333333333333333333";
            Astar( rN, gN);
    //cout<<"Comes back here";
    }
        }

};
void   initialize(Node *a,Node *b)  // a is start point and b is goal.
{
    for(int i=0; i<GRID_X*GRID_Y; i++)
    {
        GRID[i].Gvalue=100000;          // ideally must be infinite
        GRID[i].Fvalue=100000;          // ideally must be infinite

        GRID[i].ListName='n';
    }
    //cout<<"comes here";
    GRID[(*a).position-1].Gvalue=0;
}
int main()
{
    //cout<<"main"<<endl;
    int x=0;
    while(x!=-1)
    {
        for(int i=0; i<GRID_X*GRID_Y; i++)
        {
            GRID[i].position=i+1;
            GRID[i].Gvalue=100000;          // ideally must be infinite
            GRID[i].Fvalue=100000;          // ideally must be infinite
            GRID[i].Hvalue=100000;
            GRID[i].block=0;
            GRID[i].ListName='n';
        }
        Node* StartNode,*GoalNode;
        int StartPoint;
        int EndPoint;
        cout<<"\nEnter Start Point and End point:";
        cin>>StartPoint;
        cin>>EndPoint;
        StartPoint-=1;
        EndPoint-=1;
        //Define blocks here..
        //Note->Array index+1=Block position in the grid
        GRID[15].block=1;
        GRID[25].block=1;
        GRID[27].block=1;
        GRID[33].block=1;
        GoalNode=&GRID[EndPoint];
        CalclulateHvalue(GRID,GRID_X*GRID_Y,*GoalNode);
        GRID[StartPoint].Gvalue=0;
        StartNode=&GRID[StartPoint];
        (*StartNode).parent=&GRID[StartPoint];
        cout<<endl;
        ctr_manager A;
        long sleeptime;
        CurrentArrivalPoint=StartNode;
        Node *last=StartNode;

        while ((*CurrentArrivalPoint).position!=(*GoalNode).position)
        {
            cout<<"Enter sleep time:";
                        cin>>sleeptime;
            Success=false;
            last=CurrentArrivalPoint;
            begining=clock();
            //cout<<begining<<endl;
            A.thread_AA(sleeptime,CurrentArrivalPoint,GoalNode);
            printPath(last,CurrentArrivalPoint);
        }
    }
}
