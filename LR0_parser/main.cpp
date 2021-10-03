#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<bits/stdc++.h>
using namespace std;

char items[30][100][100];
char augmented[100][100], terminals[10], nonterminals[10];
int numprod = 0, numstates = 0, numitems[30], numterm = 0, numnonterm = 0;
int state_index = 0, goto_state_index = 0, closure_item_index = 0;

struct Parsing_Table {
	char ACTION[30][100][100];
	int GOTO[30][100];
}t;

int check(char c) {
	int i;
	for(i = 0; i < numterm; i++)
		if(terminals[i] == c)
			return 1;
	return 0;
}

void generate_terminals() {
	int i, j;
	int index = 0;
	for(i = 0; i < numprod; i++) {
		for(j = 0; augmented[i][j] != '>'; j++);
		j++;
		for(; augmented[i][j] != '\0'; j++) {
			if(augmented[i][j] < 65 || augmented[i][j] > 90) {
				if(!check(augmented[i][j])) {
					terminals[index] = augmented[i][j];
					numterm++;
					index++;
				}
			}
		}
	}
	terminals[index] = '\0';
    cout<<"The terminals in the grammar  are :"<<endl;
    for(int i = 0;i<numterm-1;i++){
        cout<<terminals[i];
    }
    cout<<endl;
}

int check2(char c, int index) {
	int i;
	for(i = 0; i < index; i++)
		if(nonterminals[i] == c)
			return 1;
	return 0;
}

void generate_nonterminals() {
	int i, index = 0;
	for(i = 0; i < numprod; i++)
		if(!check2(augmented[i][0], index)) {
			nonterminals[index] = augmented[i][0];
			index++;
		}
	numnonterm = index;
	nonterminals[index] = '\0';
	cout<<"The nonterminals in the grammar are :"<<endl;
    for(int i = 0;i<numnonterm;i++){
        cout<<nonterminals[i];
    }
    cout<<endl;
}

void initialize_items() {
	generate_terminals();
	generate_nonterminals();
	int i;
	for(i = 0; i < 30; i++)
		numitems[i] = 0;
}

void generate_item(char *s, char *t) {
	int i;
	for(i = 0; i < 3; i++)
		t[i] = s[i];
	t[i] = '.';
	if(s[i] != '@')
		for(; i < strlen(s); i++)
			t[i+1] = s[i];
	t[i+1] = '\0';
}

int item_found(char *s) {
	int i;
	for(i = 0; i < closure_item_index; i++) {
		if(!strcmp(s, items[state_index][i]))
			return 1;
	}
	return 0;
}

int isterminal(char s) {
	int i;
	for(i = 0; i < numterm; i++)
		if(s == terminals[i])
			return 1;
	return 0;
}

void closure(char *s) {
	int i, j;
	for(i = 0; s[i] != '.'; i++);
	i++;
	if(!item_found(s)) {
		strcpy(items[state_index][closure_item_index], s);
		closure_item_index++;
    }
	if(s[i] == s[0] && s[i-2] == '>')
		return;
	if(isterminal(s[i]))
		return;
	else 	{
		for(j = 0; j < numprod; j++) {
			char temp[100];
			if(augmented[j][0] == s[i]) {
				generate_item(augmented[j], temp);
				closure(temp);
			}
		}
	}
}

int Goto1(char s, char temp[][100]) {
	int i, j;
	int n = 0;
	char t, temp2[100];
	if(s == '\0') {
		return n;
	}
	for(i = 0; i < numitems[goto_state_index]; i++) {
		strcpy(temp2, items[goto_state_index][i]);
		for(j = 0; temp2[j] != '.'; j++);
		if(temp2[j+1] == '\0')
			continue;
		if(temp2[j+1] == s) {
			t = temp2[j];
			temp2[j] = temp2[j+1];
			temp2[j+1] = t;

			strcpy(temp[n], temp2);
			n++;
		}
	}
	return n;
}

int state_found(char *s) {	//Checks for existance of same state.
	int i;
	for(i = 0; i < state_index; i++) {
		if(!strcmp(s, items[i][0]))	//Compare with the first item of each state.
			return 1;
	}
	return 0;
}

int transition_item_found(char * t_items, char s, int t_index) {
	int i;
	for(i = 0; i < t_index; i++)
		if(s == t_items[i])
			return 1;
	return 0;
}

void compute_closure_goto() {
	char temp[100][100], transition_items[100];
	int i, no_of_goto_items,j, transition_index = 0;
	generate_item(augmented[0], temp[0]);
	closure(temp[0]);
    numitems[state_index] = closure_item_index;
    closure_item_index = 0;
	state_index++;
	while(goto_state_index < 30) {
		transition_index = 0;
		transition_items[transition_index] = '\0';
		for(i = 0; i < numitems[goto_state_index]; i++) {
			for(j = 0; items[goto_state_index][i][j] != '.'; j++);
			j++;
			if(!transition_item_found(transition_items, items[goto_state_index][i][j], transition_index)) {
				transition_items[transition_index] = items[goto_state_index][i][j];
				transition_index++;
			}
		}
		transition_items[transition_index] = '\0';
		for(i = 0; i < transition_index; i++) {
			int add_flag = 0;
			no_of_goto_items = Goto1(transition_items[i], temp);
			for(j = 0; j < no_of_goto_items; j++) {
				if(!state_found(temp[j])) {
					add_flag = 1;
					closure(temp[j]);
				}
				else
					break;
			}
			if(add_flag) {
				numitems[state_index] = closure_item_index;
				closure_item_index = 0;
				state_index++;
			}
		}
		goto_state_index++;
	}
	numstates = state_index;
}


void print() {
	int i, j;
	cout<<"\nNumber of states are = "<<numstates<<endl;
	for(i = 0; i < numstates; i++) {
		cout<<"\n\nItems in State "<<i<<": \n";
		for(j = 0; j < numitems[i]; j++)
			cout<<items[i][j]<<endl;
	}
}

void start() {
	char str[100];
	cout<<"Enter number of productions:";
	cin>>numprod;
	cout<<"Enter the productions: \n";
	int i;
	for(i = 1; i <= numprod; i++)
		cin>>augmented[i];
	cout<<"\n\nThe augmented grammar of the given grammar is...\n\n";
	strcpy(augmented[0], "Z->");
	str[0] = augmented[1][0];
	str[1] = '\0';
	strcat(augmented[0], str);
	numprod++;
	for(i = 0; i < numprod; i++)
		cout<<augmented[i]<<endl;
	initialize_items();
	compute_closure_goto();
	print();
}

void print_table() {
	int i, j;
	cout<<"\nThe Parsing Table for the given grammar is...\n\n";
	printf("%10s   ", "");
	for(i = 0; i < numterm; i++)
		printf("%10c", terminals[i]);
	for(i = 1; i < numnonterm; i++)
		printf("%10c", nonterminals[i]);
	printf("\n\n");
	for(i = 0; i < numstates; i++) {
		printf("%10d  ", i);
		for(j = 0; j < numterm; j++) {
			if(!strcmp(t.ACTION[i][j], "."))
				printf("%10s", ".");
			else
				printf("%10s", t.ACTION[i][j]);
		}
		for(j = 1; j < numnonterm; j++) {
			if(t.GOTO[i][j] == -1)
				printf("%10s", ".");
			else
				printf("%10d", t.GOTO[i][j]);
		}
		cout<<endl;
	}
}

void Goto(int i, int item, char *temp) {	//Computes goto for 'item'th item of 'i'th state.
	char t;
	strcpy(temp, items[i][item]);
	for(i = 0; temp[i] != '\0'; i++){
		if(temp[i] == '.') {
			t = temp[i];
			temp[i] = temp[i+1];
			temp[i+1] = t;
			break;
		}
	}
}

int get_state(char *t, int state) {	//Returns the state of a given item.
	int i, j;
	for(i = state; i < (numstates + state); i++) {	//Start searching from current state and then wrap around.
		for(j = 0; j < numitems[i % numstates]; j++) {
			if(!strcmp(t, items[i % numstates][j]))
				return i % numstates;
		}
	}
	printf("No match for the string! (%s)\n", t);
}

int get_pos(int flag, char symbol) {
	int i;
	if(flag == 0)
		for(i = 0; i < numterm; i++) {
			if(terminals[i] == symbol)
				return i;
		}
	else
		for(i = 0; i < numnonterm; i++) {
			if(nonterminals[i] == symbol)
				return i;
		}
	if(flag == 0)
		printf("Terminal not found! (%c)\n", symbol);
	else
		printf("Non-terminal not found! (%c)\n", symbol);
}

int get_production_no(char * item) {
	int i, j;
	char production[20];
	for(i = 0, j = 0; item[i] != '\0'; i++)
		if(item[i] != '.') {
			production[j] = item[i];
			j++;
		}
	if(j == 3) {		// an epsilon production, the production won't have a body.
		production[j] = '@';
		j++;
	}
	production[j] = '\0';
	for(i = 0; i < numprod; i++) {
		if(!strcmp(production, augmented[i]))
			return i;
	}
	printf("The production is not found! (%s)\n", production);
}

void compute() {
	int i, item, j;
	char temp[100], symbol;
	for(i = 0; i < numstates; i++) {
		for(item = 0; item < numitems[i]; item++) {
			char *s = strchr(items[i][item], '.');	//Returns a substring starting with '.'
			if(!s) {	//error.
				printf("Item not found! State = %d, Item = %d\n", i, item);
				exit(-1);
			}
			if(strlen(s) > 1) {	//SHIFT ACTION!!
				if(isterminal(s[1])) {		//for terminals.
					char state[3];
					Goto(i, item, temp);
					j = get_state(temp, i);
					sprintf(state, "%d", j);
					strcpy(temp, "S:");
					strcat(temp, state);
					strcpy(t.ACTION[i][get_pos(0, s[1])], temp);
				}
				else {		//for non-terminals.
					Goto(i, item, temp);
					j = get_state(temp, i);
					if(t.GOTO[i][get_pos(1, s[1])] == -1)
						t.GOTO[i][get_pos(1, s[1])] = j;
				}
			}
			else {	//REDUCE ACTION!!
				char  production_no[3];
				int k, n;
				n = get_production_no(items[i][item]);
				sprintf(production_no, "%d", n);
				strcpy(temp, "R:");
				strcat(temp, production_no);
				for(k = 0; k<numterm ; k++) {
					strcat(t.ACTION[i][get_pos(0, terminals[k])], temp);
				}
			}
		}
	}
	strcpy(t.ACTION[1][get_pos(0, '$')], "acc");	//Accept-entry for item [S'->S.]
}

void create_table() {
	int i, j;
	for(i = 0; i < numstates; i++) {
		for(j = 0; j < numterm; j++)
			strcpy(t.ACTION[i][j], ".");
		for(j = 0; j < numnonterm; j++)
			t.GOTO[i][j] = -1;
	}
	compute();
	print_table();
}

int main(){
    start();
    create_table();
    return 0;
}


