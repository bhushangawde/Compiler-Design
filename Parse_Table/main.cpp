#include<stdio.h>
#include<ctype.h>
#include<cstring>
#include <map>
#include<bits/stdc++.h>
using namespace std;

void FIRST(char[],char);
void FOL(char);
void find_first(char);
int i = 0;
int j = 0;
void addToResult(char);
void addToResultSet(char[],char);
int numprod;
char prodset[10][10];
char parser[10][10];
char followResult[20];

int main()
{

    map<char,string> m1,m2;
    map<char,int>m3;
    int k=0;
    char choice;
    int num=0;
    char nont[10];
    char alp[25]={0};
    printf("Enter the number of productions ? :");
    scanf(" %d",&numprod);
    for(int i=0;i<numprod;i++)
    {
        printf("Enter productions Number %d : ",i+1);
        scanf(" %s",prodset[i]);
    }
    for(int j = 0; j<numprod;j++){
        char x = prodset[j][0];
        if(alp[x-65]==0){
            alp[x-65]=1;
            nont[num++]=x;
            m1[x]="";
            m2[x]="";
        }
    }
    m3['+']=1;
    m3['*']=2;
    m3['a']=6;
    m3['(']=3;
    m3[')']=4;
    m3['$']=5;
    printf("The nonterminals found are\n");
    for(int i = 0;i<num;i++){
        printf("%c\n" , nont[i]);
    }
    //string str[num][2];

    for(int z = 0;z<num;z++){
        char result[20];
        FIRST(result,nont[z]);
        printf("\n FIRST(%c)= { ",nont[z]);
        string s = m1[nont[z]];
        for(int i=0; result[i]!='\0';i++){
            printf(" %c ",result[i]);
            s=s+result[i];
        }
        printf("}\n");
        m1[nont[z]]=s;
    }
    for(int j=0; j<num ; j++){
        FOL(nont[j]);
        printf("\n FOL(%c)= { ",nont[j]);
        string s = m2[nont[j]];

        for(int i=0; followResult[i]!='\0';i++){
            printf(" %c ",followResult[i]);
            s+=followResult[i];
        }
        printf("}\n");
        m2[nont[j]]=s;
    }

    map<char,string>::iterator it;
/*    for(it=m1.begin();it!=m1.end();++it){
        cout<<it->first<<"\t"<<it->second<<endl;
    }
*/
    cout<<setw(35)<<"Parsing table\n";
    cout<<setw(10)<<"+"<<setw(10)<<"*"<<setw(10)<<"("<<setw(10)<<")"<<setw(10)<<"$"<<setw(10)<<"a"<<setw(10)<<"\n";
    for(int i=0;i<numprod;i++){
            int f=0;
            string x=m1[prodset[i][0]];
            string y=m2[prodset[i][0]];
            /*  for(int j=0;j<x.length();j++){
                if(x[j]=='@'){
                    f=1;
                }
            }*/
            for(int j=0;j<x.length();j++){
                if(x[j]=='+' && prodset[i][2]!='@'){
                    cout<<setw(10)<<prodset[i];
                }
                if(x[j]=='*' && prodset[i][2]!='@'){
                    cout<<setw(22)<<prodset[i];
                }
                if(x[j]=='(' && !islower(prodset[i][2]) && prodset[i][2]!='@'){
                    cout<<setw(30)<<prodset[i];
                }
                if(x[j]==')' && prodset[i][2]!='@'){
                    cout<<setw(40)<<prodset[i];
                }
                if(x[j]=='a' && (isupper(prodset[i][2])) && prodset[i][2]!='@'){
                    cout<<setw(30)<<prodset[i];
                }
                if(x[j]=='a' && (islower(prodset[i][2])) && prodset[i][2]!='@'){
                    cout<<setw(62)<<prodset[i];
                }
            }
            if(prodset[i][2]=='@'){
                for(int j=0;j<y.length();j++){
                    if(y[j]=='+'){
                        cout<<endl<<prodset[i];
                    }
                    if(y[j]==')'){
                        cout<<setw(7)<<prodset[i];
                    }
                    if(y[j]=='$'){
                        cout<<"\t\t\t\t\t"<<prodset[i]<<"\t";
                    }
                }
        }
        cout<<endl;
    }
    return 0;
}

void FIRST(char* Result,char c){
    int i,j,k;
    char subres[20];
    int eps;
    subres[0]='\0';
    Result[0]='\0';
    if(!(isupper(c)))
    {
        addToResultSet(Result,c);
        return ;
    }
    for(i=0;i<numprod;i++)
    {
        if(prodset[i][0]==c)
        {
            if(prodset[i][2]=='@'){
                addToResultSet(Result,'@');
            }
            else
            {
                j=2;
                while(prodset[i][j]!='\0')
                {
                    eps=0;
                    FIRST(subres,prodset[i][j]);
                    for(k=0;subres[k]!='\0';k++){
                        addToResultSet(Result,subres[k]);
                    }
                    for(k=0;subres[k]!='\0';k++){
                         if(subres[k]=='@'){
                             eps=1;
                             break;
                         }
                    }
                    if(!eps)
                         break;
                    j++;
                }
            }
        }
    }
    return ;
}

void FOL(char ch){
    if(prodset[0][0]==ch){
        addToResult('$');
    }
    for(i = 0;i<numprod;i++){
        for(j=2;j<strlen(prodset[i]);j++){
            if(prodset[i][j]==ch){
                if(prodset[i][j+1]!='\0'){
                    find_first(prodset[i][j+1]);
                                        }
                if(prodset[i][j+1]=='\0' && ch!=prodset[i][0]){
                    FOL(prodset[i][0]);
                }
            }
        }
    }
}

void find_first(char c){
    int k;
    if(!(isupper(c)))
    {
        addToResult(c);
    }
    for (k=0;k<numprod;k++){
        if(prodset[k][0]==c){
            if(prodset[k][2]=='@'){
                FOL(prodset[i][0]);
            }
            else if(islower(prodset[k][2])){
                addToResult(prodset[k][2]);
            }
            else {
                find_first(prodset[k][2]);
            }
        }
    }
}

void addToResultSet(char Result[],char val)
{
    int k;
    for(k=0 ;Result[k]!='\0';k++)
        if(Result[k]==val)
            return;
    Result[k]=val;
    Result[k+1]='\0';
}

void addToResult(char val)
{
    int k;
    for(k=0 ;followResult[k]!='\0';k++)
        if(followResult[k]==val)
            return;
    followResult[k]=val;
    followResult[k+1]='\0';
}
