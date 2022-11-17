#include <stdio.h>
#include <stdlib.h>

//for LinkedList
struct node {
    unsigned char value: 4;
    struct node *nxtNode;
    struct node *prvNode;
};
typedef struct node node;

//All LinkedList's pointers

//Points left most digit
node *multiplicand = NULL;
//Points left most digit
node *multiplier = NULL;
//Points right most digit
node *multiplicandEnd = NULL;
//Points right most digit
node *multiplierEnd = NULL;
//Points left most digit
node *result = NULL;
//Points right most digit
node *resultEnd =NULL;
//Points left most digit of decimal form
node *multiplicandDec = NULL;
//Points left most digit of decimal form
node *multiplierDec = NULL;
//Points right most digit of decimal form
node *multiplicandDecEnd = NULL;
//Points right most digit of decimal form
node *multiplierDecEnd = NULL;
//Points left most digit of decimal form
node *resultDec = NULL;
//Points right most digit of decimal form
node *resultDecEnd = NULL;

void addNodeToEnd(char, node**, node**);
void addNodeToStart(char, node**, node**);
void toDecimal(char, node*, node**, node**);
void add(node*, node**, unsigned long);
void printLinkedList(node *first);
void multiplication(node*, node*, node**, char);
unsigned long divi(unsigned long, char, char*);

int main(int argc, char* argv[]) {
    FILE* input;
    if(argc>=2){
        input= fopen(argv[1], "r");
    }
    else{
        printf("ERROR");
        return -1;
    }
    char k;

    //Check if input is empty
    if (!(input)) {
        printf("input can't be opened \n");
    }

    //Reads multiplicand
    char ch;
    do {
        ch = fgetc(input);

        if (ch >= '0'){
            addNodeToEnd((ch-'0'), &multiplicandEnd, &multiplicand);
        }
    } while (!(ch == EOF || ch == '\n' || ch == ' '));


    //Reads multiplier
    do {
        ch = fgetc(input);

        if (ch >= '0'){
            addNodeToEnd((ch-'0'), &multiplierEnd, &multiplier);

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

    //Adds digits to result
    multiplication(multiplicandEnd, multiplierEnd, &result, k);

    //If base is not 10 turns them to decimal
    if(k!=10) {
        toDecimal(k, multiplicandEnd, &multiplicandDec, &multiplicandDecEnd);
        toDecimal(k, multiplierEnd, &multiplierDec, &multiplierDecEnd);
        toDecimal(k, resultEnd, &resultDec, &resultDecEnd);
    }




    ch=0;
    while(ch<2) {

        //Print to console
        printLinkedList(multiplicand);

        printf("\n");

        printLinkedList(multiplier);

        printf("\n");

        printLinkedList(result);

        printf("\n");

        ch++;
        if(k!=10) {

            //Print to console
            printLinkedList(multiplicandDec);

            printf("\n");

            printLinkedList(multiplierDec);

            printf("\n");

            printLinkedList(resultDec);

            printf("\n");

            ch++;
        }
    }
return 0;
}

void printLinkedList(node *first){

    if (first){
        node *tmp = first;
        while (tmp->nxtNode){
            printf("%d", tmp->value);
            tmp = tmp->nxtNode;
        }
        printf("%d", tmp->value);

    } else {
        printf("List is empty\n");
        return;
    }
}

//adds node to given pointer to end
void addNodeToEnd(char val,node** prv, node** first){

    //if given pointer is null creates node and assigns as first node of LL
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
    //If it is last node adds to its next
    if (!((*prv)->nxtNode)){
        node *newNode;
        newNode = (node*)malloc(sizeof(node));
        newNode->value = val;
        newNode->nxtNode = NULL;
        newNode->prvNode = *prv;
        (*prv)->nxtNode = newNode;
        (*prv)=newNode;

    }
    //If given pointer is not last node searches for last node
    else {
        addNodeToEnd(val,(&(*prv)->nxtNode), first);
    }
}

//adds node to given pointer to end
void addNodeToStart(char val,node** nxt, node** last){

    //if given pointer is null creates node and assigns as last node of LL
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
    //If it is first node adds to its previous
    if (!((*nxt)->prvNode)){
        node *newNode;
        newNode = (node*)malloc(sizeof(node));
        newNode->value = val;
        newNode->nxtNode = *nxt;
        newNode->prvNode = NULL;
        (*nxt)->prvNode = newNode;
        (*nxt)=newNode;
    }
    //If given pointer is not first node searches for first node
    else {
        addNodeToStart(val,(&(*nxt)->prvNode), last);
    }
}

//I go in depth about how the function works in the documentation
void multiplication(node* originalm, node* originaln, node** res, char k){
    //copies original values for further operations
    node* m = originalm;
    node* n = originaln;
    //for divi function's remainder part
    char a =0;
    unsigned long sum = 0;
    unsigned long carry = 0;
    //while both originals points other than left most digit
    while((originalm->prvNode) || (originaln->prvNode)){
        while((m->nxtNode)&&(n->prvNode)){
            sum+= (n->value)*(m->value);
            m=m->nxtNode;
            n=n->prvNode;
        }
        sum+= (n->value)*(m->value) + carry;
        if(originalm->prvNode) {
            originalm = originalm->prvNode;
        }
        else{
            originaln = originaln->prvNode;
        }
        m = originalm;
        n = originaln;
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

//adds given number to LinkedList
void add(node* decEnd, node** decStart, unsigned long carry) {
    //for divi function's remainder part
    char a=0;
    do{
        //if it has a node it adds the value to carry then finds new carry and value for that node
        if (decEnd) {
            if(!(decEnd->prvNode)){
                (*decStart)=decEnd;
            }
            carry = divi((decEnd->value + carry), 10, &a);
            decEnd->value=a;
            decEnd = decEnd->prvNode;
        }
        //if it has not got a node finds carry then adds node to LinkedList
        else {
            carry = divi(carry, 10, &a);
            addNodeToStart(a, decStart, &decEnd);
        }
    }while(carry!=0);
}

void toDecimal(char k, node* res, node** newDec, node** newDecEnd){
    //for k**(number of digit)
    unsigned long exp =1;
    char i=0;
    //To avoid errors I created a 1digit newDec
    addNodeToStart(0, newDec, newDecEnd);
    while (res){
        if(i!=0){
            exp*=k;
        }
        add((*newDecEnd), newDec, (exp * (res->value)));
        res=res->prvNode;
        if(i==0){
            i++;
        }
    }
}

//It is a division function to get remainder and quotient at the same time(shorter time for q1)
unsigned long divi(unsigned long q1, char q2, char* rem){
    unsigned long ans=0;
    while(q1>=q2){
        q1-=q2;
        ans++;
    }
    (*rem)=q1;
    return ans;
}
