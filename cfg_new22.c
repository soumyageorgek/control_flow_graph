#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define SIZE 40

struct arith_exp {
  char operation;
  union a_operand {
    enum kind {constant, adr_var, adr_ae} k;
    int x;
    int* ad;
    struct arith_exp* a_exp;
  }al_operand, ar_operand;
};

struct rel_exp {
  char operation;
  union rl_operand {
    int x;
    int* ad;
  }l_operand, r_operand;
};

struct statement
{
  char raw[20];
  char opcode;
  char lbool;
  char label[5];
  union operand {
    enum kindst {arith, conditional, goto_stm} kst;
    union l_value {
      enum kindlv {lv_adr, lv_r_adr, lv_label} klv;
      int* adr;
      struct rel_exp* r_ad;
      char label[6];
    } left;
    union r_value {
      enum kindrv {rv_constant, rv_adr, rv_label} krv;
      int constant;
      int* adr;
      char label[6];
      struct statement* rgoto;
    } right;
  } opr;
  struct statement* next;
};

struct block {
  struct statement* start;
  struct statement* end;
}bl[10];

int parse(struct statement *st, char *sinput){
  char *index;
  int end;
  struct statement *temp;
  
 switch(*sinput) {
      case 'g': 
	if((index=strstr(sinput, "goto")) != 0) {
	    st->opcode = 'g';
	    if((index=strstr(index+3, " ")) != 0) {
	      strcpy(st->opr.left.label, index+1);
	    }
	    return 1;
	}
      case 'i': 
	if(strstr(sinput, "if")) {
	  st->opcode = 'i';
	  if((index=strstr(sinput, "goto")) != 0) {
	    temp = malloc(sizeof(struct statement));
	    (st)->opr.right.rgoto = temp;
	    temp->opcode = 'g';
	    if((index=strstr(index+3, " ")) != 0) {
	      strcpy(temp->opr.left.label, index+1);
	      end = strlen(temp->opr.left.label);
	      (temp->opr.left.label)[end] = '\0';
	    }
	  }
	  return 1;
	}
 }
        return 0;               
}

void display(blockn) {
  int i;
  struct statement *temp;
  
  for(i=0; i<=blockn; i++) {
    printf("BLOCK %d\n", i);
    temp = bl[i].start;
    while(temp != bl[i].end) {
      printf("%s\n", temp->raw);
      temp = temp->next;
    }
    printf("%s\n\n\n", bl[i].end->raw, i); 
  }
}

void warshal(int (*adj)[10], int (*path)[10], int blockn) {
  int i, j, k;
  
 for(i = 0; i <= blockn; i++) {
  for(j = 0; j <= blockn; j++) {
      path[i][j] = adj[i][j];
  }
}

 for(i = 0;i <= blockn; i++)
  for(j = 0;j <= blockn; j++)
   if(path[i][j] == 1)
    for(k = 0; k <= blockn; k++)
      if(path[j][k] == 1) {
        path[i][k] = 1;
      }
printf("PATH MATRIX\n");
printf("-----------\n");
 for(i=0; i<=blockn; i++) {
   for(j=0; j<=blockn; j++)
     printf("%d ", path[i][j]);
   printf("\n");
 } 
printf("\n"); 
}

int main(int argc, char *argv[])
{
  int count=0, i=0, blockn=0, j=0, flag=1, end=0, k=0, flg2=0;
  int adj[10][10], path[10][10];
  char *index;
  char buf[SIZE],input[SIZE][SIZE];
  char *templ=0;
  FILE *fp;
  struct statement *st=0, *prev=0, *temp, *temp2, *temp3;
  struct node *tempn;
  
  fp=fopen(argv[1], "r");
  while(!(feof(fp))) {
    fgets(buf,SIZE,fp);
    end = strlen(buf) - 2;
    buf[end] = 0;
    strcpy(input[count],buf);
    count++;
  }
  fclose(fp);
  count--;
  while(i < count) {
    prev = st;
    st = malloc(sizeof(struct statement));
    st->lbool = 0;
    st->opcode = '\0';
    
    strcpy(st->raw,  &(input[i][0]));

    if (flag == 1){
        bl[blockn].start = st;
    }	  
   
    if(prev != 0) prev->next = st;
    	
	if((index=strstr(&(input[i][0]), ":")) != 0) {
	  if(prev != 0) {
	    bl[blockn].end = prev;
	  }
	  if(flag == 1) blockn--;
	  blockn++;
	  st->lbool = 1;
	  bl[blockn].start=st;
	  strncpy(st->label, &(input[i][0]), index-&(input[i][0]));
	  st->label[index-&(input[i][0])]='\0'; 
	  flag = 0;
	  flag = parse(st, (char *)index+1);
        }
        else {
	    flag = 0;
            flag = parse(st, (char *)&(input[i][0]));
        }
        if(flag==1) {
	  bl[blockn].end = st;
	  blockn++;
	  if((i+1) == count)
	    flg2 = 1;
	}
    i++;
    
  }
  if(flg2 == 1)
    blockn--;
bl[blockn].end = st;

printf("PROGRAM IN BLOCKS\n");
printf("------------------\n");

display(blockn);

for(i=0; i<=blockn; i++)
  for(j=0; j<=blockn; j++)
      adj[i][j] = 0;
    

  for(i=0; i<=blockn; i++) {
  switch(bl[i].end->opcode) {
    case 'g':
      templ = (char *)bl[i].end->opr.left.label;
      break;
    case 'i':
      temp = bl[i].end->opr.right.rgoto;
      templ = (char *)temp->opr.left.label;
    default:
      if(i+1<=blockn)
	adj[i][i+1]=1;
  }
  if(templ) {
      for(j=0; j<=blockn; j++){
	if(bl[j].start->lbool == 1){
	  if(!(strcmp(templ, bl[j].start->label))) {
	    adj[i][j]=1;
	  }
	}
      }
  }
  templ = 0;
}

printf("ONE HOP ADJACENCY MATRIX\n");
printf("------------------------\n");
for(i=0; i<=blockn; i++) {
  for(j=0; j<=blockn; j++)
    printf("%d ", adj[i][j]);
  printf("\n");
}
printf("\n");
warshal(adj, path, blockn);

flag = 0;
count = 0;
 for(j=0; j<=blockn; j++){
  if((path[0][j] == 0)&&(j!=0)) {
    printf("BLOCK %d ", j);
    flag = 1;
    count++;
    temp=bl[j].start;
    while((temp<= bl[j].end)&&(temp!=0)){
	temp2 = temp->next;
        free(temp);
	temp = temp2;
	}
    temp3->next = temp2;
   bl[j].start = bl[j].end = 0;		
  } else {
    temp3 = bl[j].end;
  }
}  
if (flag == 1)
  printf("ARE UNREACHABLE\n");
else
  printf("ALL BLOCKS REACHABLE FROM BLOCK 0\n");
for(k=0, j=0; k<=blockn; k++) {
  if(bl[k].start == 0) {
    j = k+1;
    while(j<=blockn) {
	  if(bl[j].start != 0) {
		  bl[k].start = bl[j].start;
		  bl[k].end = bl[j].end;
		  bl[j].start = 0;
		  bl[j].end = 0;
		  break;
	  }
	  else {
	    j++;
	  }
    }
  }
  if(j==blockn)
      break;
}
  blockn-=count;
  printf("\nBLOCKS AFTER REMOVING UNREACHABLE BLOCKS FROM BLOCK 0\n");
  printf("-------------------------------------------------------\n");
  display(blockn); 
}
