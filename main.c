#include <stdio.h>
#include <stdlib.h>

//for multiplicand and result and multiplier
struct node {
    unsigned int value: 4;
    struct node *nxtNode;
    struct node *prvNode;
};
typedef struct node node;


node *mltpcand = NULL;
node *mltplier = NULL;
node *mltpcandEnd = NULL;
node *mltplierEnd = NULL;
node *result = NULL;
node *resultEnd =NULL;
node *resultDec = NULL;
node *resultDecEnd = NULL;
short k = 0;

void addNodeToEnd(short, node**, node**);
void addNodeToStart(short, node**, node**);
void toDecimal(short, node*, node**, node**);
void add(node*, node**, unsigned long);
void printList(node*);
void multiplication(node*, node*, node**, short);
unsigned long divi(unsigned long, short, short*);

int main() {
    //Opens input
    FILE* input;
    input = fopen("/home/pc/Desktop/integer-multiplication-master/input.txt", "r");

    //Check if input is empty
    if (!(input)) {
        printf("input can't be opened \n");
    }

    //Reads multiplicand
    char ch;
    short num;
    do {
        ch = fgetc(input);

        num = ch - '0';
        if (num >= 0){
            addNodeToEnd(num,&mltpcandEnd, &mltpcand);
        }
    } while (!(ch == EOF || ch == '\n' || ch == ' '));

    mltpcandEnd=mltpcand;
    while(mltpcandEnd->nxtNode){
        mltpcandEnd = mltpcandEnd->nxtNode;
    }

    //Reads multiplier
    do {
        ch = fgetc(input);

        num = ch - '0';
        if (num >= 0){
            addNodeToEnd(num,&mltplierEnd, &mltplier);

        }
    } while (!(ch == EOF || ch == '\n' || ch == ' '));

    //Reads base system constant (k)
    ch = fgetc(input);
    k = ch - '0';
    if (k == 1){
        k = 10;
    }


    //Closes input
    fclose(input);

    //Adds values to result (DELETE THIS BEFORE SUBMISSION)
    multiplication(mltpcandEnd, mltplierEnd, &result, k);

    if(k!=10) {
        toDecimal(k, resultEnd, &resultDec, &resultDecEnd);
    }


    //Prints lists
    printList(mltpcand);
    printf("\n");
    printList(mltplier);
    printf("\n");
    printList(result);
    printf("\n");
    printf("%d", k);

    //Opens output
    FILE* output;
    output = fopen("/home/pc/Desktop/integer-multiplication-master/output.txt", "w");

    //Prints multiplicand
    node *temp = mltpcand;
    while (temp){
        fprintf(output, "%d", temp->value);
        temp = temp->nxtNode;
    }
    fputc('\n',output);

    //Prints multiplier
    temp = mltplier;
    while (temp != NULL){
        fprintf(output, "%d", temp->value);
        temp = temp->nxtNode;
    }
    fputc('\n',output);

    //Prints result
    temp=result;
    while (temp){
        fprintf(output, "%d", temp->value);
        temp = temp->nxtNode;
    }
    fputc('\n',output);



    if(k==10){
        return 0;
    }
    temp=resultDec;
    while (temp){
        fprintf(output, "%d", temp->value);
        temp = temp->nxtNode;
    }
    fputc('\n',output);
    return 0;
}

void printList(node *head){

    if (head){
        node *tempNode = head;
        while (tempNode->nxtNode){
            printf("%d -> ",tempNode->value);
            tempNode = tempNode->nxtNode;
        }
        printf("%d",tempNode->value);

    } else {
        printf("List is empty\n");
        return;
    }
}

void addNodeToEnd(short val,node** prv, node** first){

    if(!(*prv)){
        node *newNode;
        newNode = (node*)malloc(sizeof(node));
        newNode->value = val;
        newNode->nxtNode = NULL;
        newNode->prvNode = NULL;
        (*prv)=newNode;
        (*first)=newNode;
        return ;
    }
    //Checks if it is last node
    if (!((*prv)->nxtNode)){
        node *newNode;
        newNode = (node*)malloc(sizeof(node));
        newNode->value = val;
        newNode->nxtNode = NULL;
        newNode->prvNode = *prv;
        (*prv)->nxtNode = newNode;
        (*prv)=newNode;

    } else {
        //Searches for last node
        addNodeToEnd(val,(&(*prv)->nxtNode), first);
    }
}

void addNodeToStart(short val,node** nxt, node** last){

    if(!(*nxt)){
        node *newNode;
        newNode = (node*)malloc(sizeof(node));
        newNode->value = val;
        newNode->nxtNode = NULL;
        newNode->prvNode = NULL;
        (*nxt)=newNode;
        (*last)=newNode;
        return ;
    }
    //Checks if it is last node
    if (!((*nxt)->prvNode)){
        node *newNode;
        newNode = (node*)malloc(sizeof(node));
        newNode->value = val;
        newNode->nxtNode = *nxt;
        newNode->prvNode = NULL;
        (*nxt)->prvNode = newNode;
        (*nxt)=newNode;
    } else {
        //Searches for last node
        addNodeToStart(val,(&(*nxt)->prvNode), last);
    }
}

void multiplication(node* orgm, node* orgn,node** res, short k){
    node* m = orgm;
    node* n = orgn;
    //for divi function's remainder part
    short a =0;
    unsigned long sum = 0;
    unsigned long carry = 0;
    while((orgm->prvNode)||(orgn->prvNode)){
        while((m->nxtNode)&&(n->prvNode)){
            sum+= (n->value)*(m->value);
            m=m->nxtNode;
            n=n->prvNode;
        }
        sum+= (n->value)*(m->value) + carry;
        if(orgm->prvNode) {
            orgm = orgm->prvNode;
        }
        else{
            orgn = orgn->prvNode;
        }
        m = orgm;
        n = orgn;
        carry = divi(sum, k, &a);
        addNodeToStart(a, res, &resultEnd);
        sum = 0;
    }
    sum+=(n->value)*(m->value) + carry;
    carry = divi(sum, k, &a);
    addNodeToStart(a, res, &resultEnd);
    if(carry){
        addNodeToStart(carry,res, &resultEnd);
    }
}

void add(node* decEnd, node** decStart, unsigned long carry) {
    //for divi function's remainder part
    short a=0;
    do{
        if (decEnd) {
            if(!(decEnd->prvNode)){
                (*decStart)=decEnd;
            }
            carry = divi((decEnd->value + carry), 10, &a);
            decEnd->value=a;
            decEnd = decEnd->prvNode;
        } else {
            carry = divi(carry, 10, &a);
            addNodeToStart(a, decStart, &decEnd);
        }
    }while(carry!=0);
}

void toDecimal(short k, node* res, node** newDec, node** newDecEnd){
    unsigned long e =1;
    unsigned long i=0;
    //To avoid errors I created a 1digit newDec
    addNodeToStart(0, newDec, newDecEnd);
    while (res){
        if(i!=0){
            e*=k;
        }
        add((*newDecEnd), newDec, (e*(res->value)));
        res=res->prvNode;
        i++;
    }
}

unsigned long divi(unsigned long q1, short q2, short* rem){
    unsigned long ans=0;
    while(q1>=q2){
        q1-=q2;
        ans++;
    }
    (*rem)=q1;
    return ans;
}
