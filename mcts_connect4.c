#include<stdio.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include "mc.h"
//status : 0 game on, -1 human win, 1 bot win, 3 Draw

#define ITER 1000

grid* create_grid()
{
    grid* cg = (grid*)malloc(sizeof(grid));
    cg->status = 0;
    for(int i = 0; i<7; i++)
    {
        cg->top[i] = -1;
        for(int j = 0;j<6;j++)
        {
            cg->gr[i][j] = 0;
        }
    }
    return cg;
}
void print_grid(grid* g)
{
	printf("\n-----------------------\n      \033[1;36m CONNECT 4 \033[0m \n-----------------------\n");
    printf("\nPlayer(X) vs AI Bot(O)\n\n");
    printf("\n     1 2 3 4 5 6 7\n\n");
    for(int i = 5; i >= 0; i--)
    {
        printf("     ");
        for(int j = 0;j<=6;j++)
        {
            if(g->gr[j][i] == -1)
                printf("\033[1;32mX \033[0m");
            else if(g->gr[j][i] == 1)
                printf("\033[1;31mO \033[0m");
            else
                printf("- ");
        }
        printf("\n");
    }
    /*printf("     ");
    for(int i = 0; i<7; i++)
        printf("%d ",g->top[i]);
    printf("\n");*/
}
int make_move(int p, grid* curr_grid, int st)
{
    if(st>6 || curr_grid->top[st] == 5)
        {
            printf("\n-----INVALID MOVE!!!-----\n");
            return 0;
        }
    curr_grid->top[st] += 1;
    curr_grid->gr[st][curr_grid->top[st]] = p;
    return 1;   
}

int check_winner(grid* g, int cst)
{
    int x = cst;
    int y = g->top[cst];
    if(y < 0)
    {
        g->status = 0;
        return 0;
    }
    int ru = 0,rd = 0,r = 0,u = 0,p = 0,d = 0,l = 0,lu = 0,ld = 0;
    int c = g->gr[x][y];
    if(c == 0)
    {
        g->status = 0;
        return 0;
    }
    //printf("checking %d,%d\n",cst, y);
    while(0<=--x && x<=6 && 0<=--y && y<=5 && g->gr[x][y] == c)
        ld += 1; 
    x = cst;
    y = g->top[cst];
    while(0<=--x && x<=6 && 0<=++y && y<=5 && g->gr[x][y] == c)
        lu += 1; 
    x = cst;
    y = g->top[cst];
    while(0<=++x && x<=6 && 0<=--y && y<=5 && g->gr[x][y] == c)
        rd += 1; 
    x = cst;
    y = g->top[cst];
    while(0<=++x && x<=6 && 0<=++y && y<=5 && g->gr[x][y] == c)
        ru += 1; 
    x = cst;
    y = g->top[cst];
    while(0<=--y && y<=5 && g->gr[x][y] == c)
        d += 1; 
    x = cst;
    y = g->top[cst];
    while(0<=++y && y<=5 && g->gr[x][y] == c)
        u += 1; 
    x = cst;
    y = g->top[cst];
    while(0<=--x && x<=6  && g->gr[x][y] == c)
        l += 1; 
    x = cst;
    y = g->top[cst];
    while(0<=++x && x<=6  && g->gr[x][y] == c)
        r += 1; 
    if(u+d >= 3 || l+r >= 3 || lu+rd >= 3 || ru+ld >= 3)
    {
        if(c==-1)
        {
            g->status = -1;
            return -1;
        }
        g->status = 1;
        return 1;
    }
    g->status = 0;
    return 0;
}

int check_fill(grid* g)
{
	for(int i = 0; i<7; i++)
		if( g->top[i] < 5 )
			return -1;
    g->status = 3;
	return 1;
}

/*int check_winner2(grid* g, int cst)
{
    int x = cst;
    int y = g->top[cst];
    if(y < 0)
    {
        g->status = 0;
        return 0;
    }
    int ru = 0,rd = 0,r = 0,u = 0,p = 0,d = 0,l = 0,lu = 0,ld = 0;
    int c = g->gr[x][y];
    if(c == 0)
    {
        g->status = 0;
        return 0;
    }
    //printf("checking %d,%d\n",cst, y);
    while(0<=--x && x<=6 && 0<=--y && y<=5 && g->gr[x][y] == c)
        ld += 1; 
    x = cst;
    y = g->top[cst];
    while(0<=--x && x<=6 && 0<=++y && y<=5 && g->gr[x][y] == c)
        lu += 1; 
    x = cst;
    y = g->top[cst];
    while(0<=++x && x<=6 && 0<=--y && y<=5 && g->gr[x][y] == c)
        rd += 1; 
    x = cst;
    y = g->top[cst];
    while(0<=++x && x<=6 && 0<=++y && y<=5 && g->gr[x][y] == c)
        ru += 1; 
    x = cst;
    y = g->top[cst];
    while(0<=--y && y<=5 && g->gr[x][y] == c)
        d += 1; 
    x = cst;
    y = g->top[cst];
    while(0<=++y && y<=5 && g->gr[x][y] == c)
        u += 1; 
    x = cst;
    y = g->top[cst];
    while(0<=--x && x<=6  && g->gr[x][y] == c)
        l += 1; 
    x = cst;
    y = g->top[cst];
    while(0<=++x && x<=6  && g->gr[x][y] == c)
        r += 1; 
    printf("u %d,d %d,r %d,l %d,lu %d,ld %d,ru %d,rd %d\n",u,d,r,l,lu,ld,ru,rd);
    if(u+d == 3 || l+r == 3 || lu+rd == 3 || ru+ld == 3)
    {
        if(c==-1)
        {
            g->status = -1;
            return -1;
        }
        g->status = 1;
        return 1;
    }
    g->status = 0;
    return 0;
}*/
    
int main()
{
    grid *curr_grid = create_grid();
    system("clear");
    print_grid(curr_grid);
    int x_move,o_move,fl;
    double t;
    struct timespec start, end ;
    while(curr_grid->status == 0)
    {
        fl = 0;
        while(fl == 0)
        {
        	printf("\n\nYour(X) move : ");
        	scanf("%d",&x_move);
        	x_move -= 1;
            fl = make_move(-1,curr_grid,x_move);

        }
        system("clear");
        if(check_winner(curr_grid,x_move) == -1)
        {
        	print_grid(curr_grid);
        	break;
        }
        if(check_fill(curr_grid) == 1)
        {
        	curr_grid->status = 3;
        	break;
        }
        fl = 0;
        //o_move = rand()%7;
        //printf("entering mcts\n");

        clock_gettime(CLOCK_REALTIME, &start);
        Node* root_state = MCTS(3000,curr_grid,2.0);
        clock_gettime(CLOCK_REALTIME, &end);
        t = (end.tv_sec - start.tv_sec);
        t += (end.tv_nsec - start.tv_nsec)*0.000000001;
        //printf("\ntime taken : %f sec\n", t);
        curr_grid = copy_state(root_state->state);
        //printf("exiting mcts\n");
        //print_grid(curr_grid);
        //printf("in connect4\n");
        for(int j = 0; j<7; j++)
        {        
            //printf("check_winner %d\n",j);
            if(curr_grid->top[j]>=0 && check_winner(curr_grid,j) == 1)
            {
                fl = 1;
                print_grid(curr_grid);
                break;
            }
        }
        if(fl == 1)
            break;
        print_grid(curr_grid);
        if(check_fill(curr_grid) == 1)
        {
        	curr_grid->status = 3;
        	break;
        }
        //printf("reached end of loop\n");    
    }
    if(curr_grid->status == -1)
        printf("\033[1;32m\n~ ~ ~ ~ YOU WIN ~ ~ ~ ~\n\n \033[0m");
    else if(curr_grid->status == 1)
        printf("\033[1;31m\n~ ~ ~ ~ YOU LOST ~ ~ ~ ~\n\n \033[0m");
    else
    	printf("\n~ ~ ~ ~ GAME DRAW ~ ~ ~ ~\n\n");
    
}
    
