#include <stdio.h>
#include <string.h>

int main()
{
   int t,z;
   scanf("%d",&t);
   for(z=0;z<t;z++)
   {
       int n,flag=0,i;
       int a[1000];
       scanf("%d",&n);
       a[0]=0;
       a[1]=1;
       i=2;
       while(1)
       {
           a[i]=a[i-1]+a[i-2];
           if(a[i]==n)
           {
               flag=1;
               break;
           }
           else if(a[i]>n)
           {
               flag=2;
               break;
           }
           i++;
       }
       if(flag==1)
       {
           printf("Yes\n");
       }
       else
       {
           printf("No\n");
       }
   }
   return 0;
}