#include<iostream>
#include<bits/stdc++.h>
#include<string.h>

using namespace std;

int main(){
	int i=0, token = 0;
	char str[20];
	cout<<"Enter the expression:"<<endl;
	cin.getline(str,256);
	while(i<=strlen(str)-1){
		if(str[i]>=97 && str[i]<=122){
			string s="";
			while(str[i]>=97 && str[i]<=122){
                s = s+str[i];
				i++;
			}
			token++;
		cout<<"Token "<<token<<": "<<s<<endl;
		}
		if(str[i]==32){
            while(str[i]==32){
                i++;
            }
		}
		if(str[i]== '+' || str[i]== '/' || str[i]== '*' || str[i]== '=' || str[i]== '-'){
			token++;
			cout<<"Token "<<token<<": "<<str[i]<<endl;
			i++;
		}
	}
	return 0;
}
