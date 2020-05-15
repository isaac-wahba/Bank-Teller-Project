#include <stdio.h>
#include <stdlib.h>

typedef struct Client Client;
typedef struct Teller Teller;
typedef struct Line Line;

Line noLine;


struct Client
{
    int arrivalTime;
    int transTime;
    int waitTime;
    int startService;
    int endService;
    Client *prev;
    Client *next;
};
struct Line
{
    Client* first;
    Client* last;
};
struct Teller
{
    int totalTrans;
    int clientCount;
    Line line;

};
Client* createClient (int arrival, int trans)
{
    Client *newClient = (Client*)malloc(sizeof(Client));
    newClient->arrivalTime=arrival;
    newClient->transTime=trans;
    newClient->next=newClient->prev=NULL;
    return newClient;
}
void addClient(Client *newClient, Line* someLine)
{
    if(someLine->first==NULL)
    {
        someLine->first=someLine->last=newClient;
    }
    else
    {
        newClient->prev=someLine->last;
        someLine->last->next=newClient;
        someLine->last=newClient;
    }
}
void insertBefore(Client* newClient, Client* nextClient, Line* someLine)
{
    if(nextClient != someLine->first)
    {
        newClient -> next = nextClient;
        newClient -> prev = nextClient -> prev;
        (nextClient -> prev) -> next = newClient;
        nextClient -> prev = newClient;
    }
    else
    {
        newClient -> next = someLine->first;
        someLine->first = newClient;
        (newClient -> next) -> prev = newClient;
    }

}
void removee(Client* pRemove, Line* someLine)
{
    if(pRemove == someLine->first)
    {
        if(pRemove == someLine->last)
            someLine->first = someLine->last = NULL;
        else
        {
            someLine->first = pRemove -> next;
            someLine->first -> prev = NULL;
        }
    }
    else if(pRemove == someLine->last)
    {
        someLine->last = pRemove -> prev;
        someLine->last -> next = NULL;
    }
    else
    {
        (pRemove -> prev) -> next = pRemove -> next;
        (pRemove -> next) -> prev = pRemove -> prev;
    }
    pRemove -> next = pRemove -> prev = NULL;
}
void listSelectionSort(Line* someLine)
{
    Client *i = someLine->first, *j, *min;
    while(i != NULL)
    {
        min = i;
        j = i->next;
        while(j != NULL)
        {
            if(j ->arrivalTime < min ->arrivalTime)
                min = j;
            j = j -> next;
        }

        if(min != i)
        {
            removee(min, someLine);
            insertBefore(min, i, someLine);
        }
        else
            i = i -> next;
    }

}
void requestCustomer (Line *someLine)
{
    char c;
    int arrival;
    int transaction;
    printf("Do you want to add a new client? \n\t If YES >> press y \n\t If NO >> any character else \n");
    c=getche();
    int count =1;
    while (c=='y')
    {
        c=getche();
        printf("\n\nClient number%d", count);
        printf("\nArrival time =  ");
        scanf("%d", &arrival);
        printf("Transaction time =  ");
        scanf("%d", &transaction);
        Client* newClient = createClient(arrival,transaction);
        addClient(newClient, someLine);
        printf("\nDo you want to add a new client?  ");
        c=getche();
        count++;
    }
}
void initializeLine(Line *someLine)
{
    someLine->first=NULL;
    someLine->last=NULL;
}
void initializeTeller(Teller *anyTeller)
{
    anyTeller->totalTrans=0;
    anyTeller->clientCount=0;

}
int emptyTeller (Teller* T)
{
    for(int i=0; i<3; i++)
    {
        if( T[i].clientCount== 0)
        {
            return i;
        }
    }
    return -1;
}

void assignClient (Teller* T)
{
    Client* currentClient ;
    currentClient = noLine.first;

    while(currentClient != NULL )
    {
        int empty = emptyTeller(T);
        if(empty > -1 ) //for empty tellers.
        {
            addClient(currentClient, &T[empty].line);
            T[empty].clientCount++;
            T[empty].totalTrans += currentClient->transTime;
            currentClient->waitTime = 0;
            currentClient->endService = currentClient->arrivalTime + currentClient->transTime;
        }
        else
        {

            int min=0;
            for(int i=0; i<3; i++)
            {
                if( T[i].line.last->endService < T[min].line.last->endService )
                {
                    min=i;
                }
            }
            addClient(currentClient, &T[min].line);
            T[min].clientCount++;
            T[min].totalTrans += currentClient->transTime;
            currentClient->endService = currentClient->prev->endService + currentClient->transTime;
            currentClient->waitTime = currentClient->prev->endService - currentClient->arrivalTime;

            if( currentClient->waitTime < 0 )
            {
                currentClient->waitTime=0;
            }
        }
        currentClient=currentClient->next;
    }
}

int calcTotalWaitingPerTeller(Teller t){
    int totalWaiting =0 ;
    Client* ptr;
    ptr = t.line.first;
    for(int i=0; i<t.clientCount;i++){
        totalWaiting += ptr->waitTime;
        ptr = ptr->next;
    }
    return totalWaiting;
}

int main()
{

    Teller T[3];
    initializeLine(&noLine);
    for(int i=0; i<3; i++){
        initializeLine(&T[i].line);
    }
    for(int i=0; i<3; i++){
        initializeTeller(&T[i]);
    }

    requestCustomer (&noLine);
    listSelectionSort(&noLine);
    assignClient(T);

    int bankTotalTransaction =0;
    int bankTotalWaiting =0;
    int bankTotalClients = 0;
    for(int i=0 ; i<3 ; i++){
        bankTotalTransaction += T[i].totalTrans;
        bankTotalClients += T[i].clientCount;
        bankTotalWaiting += calcTotalWaitingPerTeller(T[i]);
        printf("\n\n");
        printf("\n++++++++++ for Teller %d ++++++++++++\n", i+1);
        printf("\nTotal number of clients = %d" , T[i].clientCount );
        printf("\nTotal Transaction Time = %d" , T[i].totalTrans );
        printf("\nTotal Waiting Time = %d" , calcTotalWaitingPerTeller(T[i]) );

        if(T[i].clientCount != 0){
            printf("\nAverage Transaction Time per teller %d = %d" ,i+1, T[i].totalTrans / T[i].clientCount );
            printf("\nAverage Waiting Time per teller %d = %d" ,i+1, calcTotalWaitingPerTeller(T[i]) / T[i].clientCount );
        }
        else{
            printf("\nAverage Transaction Time teller %d = 0" ,i+1);
            printf("\nAverage Waiting Time per teller %d = 0" ,i+1 );
        }
    }

    printf("\n\n++++++++++ in The Bank ++++++++++++++");
    printf("\nTotal number of clients = %d" , bankTotalClients );
    printf("\nTotal Transaction Time = %d" , bankTotalTransaction );
    printf("\nTotal Waiting Time = %d" , bankTotalWaiting );

    if(bankTotalClients !=0){
    printf("\nAverage Transaction Time = %d" , bankTotalTransaction /bankTotalClients );
    printf("\nAverage Waiting Time = %d" , bankTotalWaiting /bankTotalClients );
    }
    else {
    printf("\nAverage Transaction Time = 0" );
    printf("\nAverage Waiting Time = 0" );
    }

    return 0;
}


