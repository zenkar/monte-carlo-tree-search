typedef struct store_grid
{
    int status;
    int gr[7][6];
    int top[7];
    int move;
}grid;

typedef struct node{
	int visits;
	float reward;
	grid* state;
	struct node** children;
	int len_child;
	int* children_move;
	int chmov;
	struct node* parent;
}Node;

typedef struct front_turn{
	Node* node;
	int turn;
}FT;
void print_grid(grid* g);
int* legal_moves(grid* state);
Node* create_node(Node* parent, grid* state);
void append_child(Node* parent, Node* child);
void append_child_move(Node* parent, int move);
void addChild(Node* node, grid* child_state, int move);
void update(Node* node,float reward);
int fully_explored(Node* node);
Node* MCTS(int mx_itr, grid* root, float factor);
FT* treePolicy(Node* node, int turn, float factor);
grid* copy_state(grid* org);
Node* expand(Node* node, int turn);
Node* bestChild(Node* node, float factor);
int defaultPolicy(grid* state,int turn);
void backup(Node* node, int reward, int turn);
int check_fill(grid* g);
int check_winner(grid* g, int cst);

