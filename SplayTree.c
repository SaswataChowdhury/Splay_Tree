/*Program for "Top-Down" Splay Tree*/

#include<stdio.h>
#include<stdlib.h>

/*definition of Node type*/
struct Node
{
   int data;
   struct Node *left,*right;
};
typedef struct Node splaytree;
/*g-head pointer pointing to the subtree with values larger than key
  s-head pointer pointing to the subtree with values smaller than key*/   
splaytree *g,*s;

/*initialisation of newnode*/
splaytree* initialise(int key)
{
    splaytree *s;
    s=(splaytree*)malloc(sizeof(splaytree));
    s->data=key;
    s->left=NULL;
    s->right=NULL;
     return s;
}

/*Splay operations*/
splaytree* L_insert(splaytree *node,splaytree *x)
{
   x->left=node;
   x=x->left;
    return x;
}
splaytree* R_insert(splaytree *node,splaytree *y)
{
   y->right=node;
   y=y->right;
    return y;
}
splaytree* LL_insert(splaytree *node,splaytree *x)
{
    x->left=node->left;
    x=x->left;
    node->left=x->right;
    x->right=node;
     return x;
}
splaytree* RR_insert(splaytree *node,splaytree *y)
{
    y->right=node->right;
    y=y->right;
    node->right=y->left;
    y->left=node;
     return y;
}


/*Making newnode/(most recently visited node) as the root*/
splaytree* update(splaytree *newnode)
{
    /*Setting right subtree of key to the left of the header node with values greater than key
        and left subtree of key to the right of the header node with values lesser than key*/ 
        newnode->right=g->left;   
        newnode->left=s->right;
        free(g); free(s);
       return newnode;
}

/*Update larger subtree by removing the most recently visited node*/
splaytree* update_greater(splaytree *node)
{
   splaytree *p=g,*t;
    while(p!=NULL)
    {
       if(p->left->data==node->data)
       {
          t=p->left; 
          p->left=p->left->right;
           return t;
       } 
       p=p->left; 
    }
}
/*Update smaller subtree by removing the most recently visited node*/
splaytree* update_smaller(splaytree *node)
{
   splaytree *p=s,*t;
    while(p!=NULL)
    {
       if(p->right->data==node->data)
       { 
          t=p->right;
          p->right=p->right->left;
            return t;
       } 
       p=p->right; 
    }
}

/*Searching for a given key*/
splaytree* search(splaytree *root,int key)
{
    splaytree *newnode,*x,*y;
    newnode=initialise(key);
    x=root;    /*Set x with root node*/
    
    g=(splaytree*)malloc(sizeof(splaytree));
    s=(splaytree*)malloc(sizeof(splaytree));
    
    /*greater-for storing nodes of the larger subtree
     smaller-for storing nodes of the smaller subtree*/
    splaytree *greater,*smaller;
    greater=g; smaller=s;
    
      while(x!=NULL)
      {
         if(x->data==key)
         {
            newnode=x;
            break;
         }
         if(x->data>key)    /*key should be in left subtree*/
         {
            if(x->left==NULL)   /*Key is the left child of parent*/
            {
               greater=L_insert(x,greater);
               y=x;
               x=x->left;
            }
            else
            {
               if(x->left->data>key)   /*Key is present in the left subtree of the left child of parent */
               {
                  greater=LL_insert(x,greater);
                  y=greater;
                  x=greater->left;
               }
               else if(x->left->data<key)  /*Key is present in the right subtree of the left child of parent*/
               {
                  greater=L_insert(x,greater);
                  x=x->left;
                  smaller=R_insert(x,smaller);
                  y=x;
                  x=x->right;
               }
               else      /*Key found*/
               {
 		   greater=L_insert(x,greater);
                  newnode=x->left;
                  break;
               }
            }
         }
         else     /*Key should be in the right subtree*/
         {
            if(x->right==NULL)     /*Key is the right child of parent*/
            {
               smaller=R_insert(x,smaller);
               y=x;
               x=x->right;
            }
            else
            {
               if(x->right->data<key)    /*Key is present in the right subtree of the right child of parent */
               {
                  smaller=RR_insert(x,smaller);
                  y=smaller;
                  x=smaller->right;
               }
               else if(x->right->data>key)    /*Key is present in the left subtree of the right child of parent */
               {
                  smaller=R_insert(x,smaller);
                  x=x->right;
                  greater=L_insert(x,greater);
                  y=x;
                  x=x->left;
               }
               else        /*Key found*/
               {
                  smaller=R_insert(x,smaller);
                  newnode=x->right;
                  break;
               }
            }
         }
      }
      greater->left=newnode->right;    
      smaller->right=newnode->left;
     
      if(x!=NULL)    
      {
        newnode=update(newnode);
         return newnode;       
      }
      
     return y;      /*Return the most recently visited node*/
}

/*Insert new node with a given key in the tree*/
splaytree* insert(splaytree *root,int key)
{
      splaytree *t=search(root,key);   /*Search for the given key in the tree*/
      
       if(t->data==key)   /*Key already present,i.e, failed insertion*/
       {
          printf("%d\n",key);
          printf("Key already present: %d\n",key);
          return t; 
       }
       
       /*Insertion of new node*/
       splaytree *newnode=initialise(key);
       newnode=update(newnode); 
         return newnode;   
}

/*Joining 2 trees*/
splaytree* join(splaytree *small,splaytree *big)
{
    s=(splaytree*)malloc(sizeof(splaytree));
    splaytree *smaller=s,*p=small;

     while(p->right!=NULL)
     {
        smaller=L_insert(p,smaller);
        p=p->right;  
     }
     smaller->right=p->left;
     p->left=s->right;
     free(s);
     p->right=big;
      return p;
}
/*Deleting a node with a given key from the key*/
splaytree* delete(splaytree *root,int key)
{
    splaytree *t=search(root,key);   /*Search for the given key in the tree*/
    
    if(t->data!=key)	   /*Key not present,i.e,Failed deletion*/
    {
        printf("Key not found\n");
        if(t->data>key)
         t=update_greater(t);
        else
         t=update_smaller(t);
       root=update(t);  
    }
    else
    {
       printf("Element deleted :%d\n",t->data);
       if(t->left==NULL && t->right==NULL)    /*Empty left and right child of deleted node*/
        root=NULL;
       else
       {
          if(t->left==NULL)	/*Empty left child of deleted node*/
           root=t->right;
          else
           root=join(t->left,t->right); 
       } 
       free(t);
    }
    return root;
}

/*Inorder traversal of the tree*/
void inorder(splaytree *root)
{
   if(root!=NULL)
   {
      inorder(root->left);
      printf("%d ",root->data);
      inorder(root->right);
   }
}

/*Splitting of the tree at a given node into smaller and bigger subtrees(Logical Splitting)*/
splaytree* split(splaytree *root,int key)
{
   splaytree *t=search(root,key);
    if(t->data!=key)       /*Key not found,i.e, unsuccessful splitting*/
    {
       printf("Key not found\n");
        if(t->data>key)
         t=update_greater(t);
        else
         t=update_smaller(t);
       root=update(t);   
    }
    else
    {
       printf("Key: %d\n",t->data);
       printf("Smaller Subtree:\n");
       inorder(t->left);     /*Tree with values lesser than key*/
       printf("\n\n");
       printf("Bigger Subtree:\n");
       inorder(t->right);     /*Tree with values greater than key*/
       printf("\n");
        root=t;
    }
    return root;
}

int main()
{
    splaytree *root=NULL;
    int c,key;
    splaytree *t;
    FILE *fp;
    fp=fopen("data.txt","r");
     if(fp==NULL)
     {
         printf("File doesn't exist\n");
         exit(0);
     }
     while(!feof(fp))
     {
        fscanf(fp,"%d",&key);
        root=insert(root,key);
     } 
     fclose(fp);
      for(;;)
      {
         printf("\n1:Search\n2:Delete\n3:Split\n4:Most recently visited/accessed node(key)\n5:Inorder display\n6:Exit\n\n");
         scanf("%d",&c);
          switch(c)
          {
             case 1: if(root==NULL)
              		printf("Tree is empty\n");
              	      else	
              	      {
              	         printf("Enter key to be searched: ");
              	         scanf("%d",&key);
              	         t=search(root,key);
              	          if(t->data!=key)	/*Failed search*/
              	          {
              	            printf("Key not found\n");
              	            if(t->data>key)
		              t=update_greater(t);         
              	            else
              	              t=update_smaller(t);
              	            root=update(t);
              	          } 
              	          else		/*Successful search*/
              	          {
              	             root=t;
              	             printf("Key found: %d\n",root->data);
              	          } 
              	      }
              	      break;
             case 2: if(root==NULL)
              		printf("Tree is Empty\n");
              	      else
              	      {
              	        printf("Enter the key to be deleted: ");
              	        scanf("%d",&key);
              	        root=delete(root,key);
              	      }
              	      break;
              	      
             case 3: if(root==NULL)
              		printf("Tree is Empty\n");
              	      else
              	      {
              	         printf("Enter the key at which splitting is to be done: ");
              	         scanf("%d",&key);
              	         root=split(root,key);
              	      }	
              	      break;
              	             	       
             case 4:  if(root==NULL)
            	       printf("Tree is empty\n\n");
            	     else  
                      printf("Key: %d\n",root->data);
            	     break;
             case 5: inorder(root);
            	     printf("\n");
            	     break;	     
             case 6: exit(0);
             default: printf("Try again\n");	      	       
          }
      }   
    
    
    return 0;
}
