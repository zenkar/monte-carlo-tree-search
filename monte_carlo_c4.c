#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include "mc.h"

int* legal_moves(grid* state)
{
	int* legal = (int*)malloc(sizeof(int)*7);
	for(int i = 0; i<7; i++)
	{
		if(state->top[i] < 5)
			legal[i] = 1;
		else
			legal[i] = 0;
	}
	return legal;
}

int nodes_created = 0;
int sim_count = 0;

Node* create_node(Node* parent, grid* state)
{
	nodes_created += 1;
	Node *node = (Node*)malloc(sizeof(Node));
	node->reward = 0.0;
	node->visits = 1;
	node->parent = parent;
	node->len_child = 0;
	node->chmov = 0;
	node->children = NULL;
	node->children_move = NULL;
	node->state = state;
	/*node->state = (grid*)malloc(sizeof(grid));
	node->state->status = 0;
	for(int i = 0; i<7 ; i++)
	{
		node->state->top[i] = state->top[i];
		for(int j = 0; j<6 ;j++)
			node->state->gr[i][j] = state->gr[i][j];
	}*/

	node->children_move = NULL;
	node->children = NULL;
	return node;
}

void append_child(Node* parent, Node* child)
{
	int l = parent->len_child;
	parent->children = (Node**)realloc(parent->children, sizeof(Node*)*(l+1));
	parent->children[l] = child;
	parent->len_child += 1;
}

void append_child_move(Node* parent, int move)
{
	int l = parent->chmov;
	parent->children_move = (int*)realloc(parent->children_move, sizeof(int)*(l+1));
	parent->children_move[l] = move;
	parent->chmov += 1;
}

void addChild(Node* node, grid* child_state, int move)
{
	Node* child = create_node(node,child_state);
	append_child(node,child);
	append_child_move(node,move);
}

void update(Node* node, float reward)
{
	node->reward += reward;
	node->visits += 1;
}

int fully_explored(Node* node)
{
	int nlm = 0;
	int* lm = legal_moves(node->state);
	for(int i = 0; i<7 ; i++)
		nlm += lm[i];
	if(node->len_child == nlm)
		return 1;
	return -1;
}

Node* MCTS(int mx_itr, grid* root_state, float factor)
{
	nodes_created = 0;
	sim_count = 0;
	grid* rsc = copy_state(root_state);
	Node* root = create_node(NULL,rsc);
	Node* front;
	int turn,winner;
	for(int i = 0; i < mx_itr; i++)
	{
		//printf("iteration %d\n",i+1);
		FT *ft = treePolicy(root,1,factor);
		front = ft->node;
		turn = ft->turn;
		winner = defaultPolicy(front->state,turn);
		backup(front,winner,turn);
	}
	Node* ans = bestChild(root,0);
	/*printf("\n MCTS stats \n");
	printf("# of iterations : %d\n",mx_itr);
	printf("# of nodes created : %d\n",nodes_created);
	printf("# of simulations : %d\n",sim_count);*/
	return ans;
}


FT* treePolicy(Node* node, int turn, float factor)
{
	//printf("tree policy\n");
	FT* ret = (FT*)malloc(sizeof(FT));
	while(node->state->status == 0)
	{
		if(fully_explored(node) == -1)
		{
			ret->node = expand(node,turn);
			ret->turn = -turn;
			//printf("exit tree policy\n");
			return ret;
		}
		else
		{
			node = bestChild(node,factor);
			turn *= -1;
		}
	}
	//printf("exit tree policy\n");
	ret->node = node;
	ret->turn = turn;
	return ret;
}

grid* copy_state(grid* org)
{
	/*Node *dup = (Node*)malloc(sizeof(Node));
	dup->reward = org->reward;
	dup->visits = org->visits;
	dup->parent = org->parent;
	dup->len_child = org->len_child;
	dup->chmov = org->chmov;

	dup->children_move = (int*)malloc(sizeof(int)*dup->chmov);
	for(int i = 0; i < dup->chmov ; i++)
		dup->children_move[i] = org->children_move[i];

	dup->children = (Node*)malloc(sizeof(Node)*dup->len_child);
	for(int i = 0; i < dup->len_child ; i++)
		dup->children[i] = org->children[i];*/
	grid* new_grid = (grid*)malloc(sizeof(grid));
	new_grid->status = org->status;
	for(int i = 0; i<7 ; i++)
	{
		new_grid->top[i] = org->top[i];
		for(int j = 0; j<6 ;j++)
			new_grid->gr[i][j] = org->gr[i][j];
	}
	return new_grid;
}

Node* expand(Node* node, int turn)
{
	//printf("expansion begins\n");
	int move;
	int *legal = legal_moves(node->state);
	for(int i = 0;i<node->chmov;i++)
		legal[node->children_move[i]] = 0;
	grid* new_state;
	for(int i = 0; i<7 ; i++)
	{
		if(legal[i] == 1)
		{
			move = i;
			new_state = copy_state(node->state);
			new_state->top[i] += 1;
			new_state->gr[i][new_state->top[i]] = turn;
			check_winner(new_state,move);
			check_fill(new_state);
			break;
		}
	}
	addChild(node,new_state,move);
	//printf("expasion ends\n");
	return node->children[node->len_child-1];
}

Node* bestChild(Node* node, float factor)
{
	//printf("best child\n");
	float bestscore = -10000000.0;
	float exploit,explore,score;
	Node** bestChildren = (Node**)malloc(sizeof(Node*)*node->len_child);
	int l = 0;
	/*if(factor < 2)
		{
			printf("move   score\n");
		}*/
	for(int i = 0; i<node->len_child; i++)
	{
		Node* c = node->children[i];
		exploit = c->reward/c->visits;
		explore = sqrt(log(2.0*node->visits)/(float)(c->visits));
		score = exploit + factor*explore;
		/*if(factor < 2)
		{
			printf("%d     %f \n",node->children_move[i]+1,score);
		}*/
		if(score == bestscore)
		{
			bestChildren[l] = c;
			l += 1;
		}
		else if(score > bestscore)
		{
			bestChildren[l] = c;
			l += 1;
			bestscore = score;
		}
	}
	if(l == 0)
	{
		printf("div by zero\n");
	}
	if(factor<2)
		return bestChildren[l-1];
	return bestChildren[rand()%l];
}

grid* next_state(grid* state,int turn)
{
	int nlm = 0;
	int* lm = legal_moves(state);
	for(int i = 0; i<7 ; i++)
		nlm += lm[i];
	int r;
	grid* aux = copy_state(state);
	while(nlm>0)
	{
		r = rand()%7;
		if(lm[r] == 1)
		{
			aux->top[r] += 1;
			aux->gr[r][aux->top[r]] = turn;
			check_winner(aux,r);
			check_fill(aux);
			break;
		}

	}
	return aux;
}
int defaultPolicy(grid* state,int turn)
{
	//printf("enter default policy\n");
	while(state->status == 0)
	{
		state = next_state(state,turn);
		turn *= -1;
		sim_count += 1;
		//printf("how many lol\n");
	}
	//printf("exit default policy\n");
	if(state->status == 3)
	{	
		return 0;
	}
	return state->status;
}

void backup(Node* node, int reward, int turn)
{
	//printf("backing up\n");
	while(node != NULL)
	{
		node->visits += 1;
		node->reward -= turn*reward;
		node = node->parent;
		turn *= -1;
	}
}
