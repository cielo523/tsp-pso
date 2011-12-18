

//============================================================== BB_UPDATE
void BB_update(struct particle p)
{
 /*
	Update the blackboard. Must be used each time a particle modify its  position
	BB is a global variable.
	G is a global variable.
	BB_option is a global variable
	------------------------
	BB_option=0:
	For each arc (n1,n2) of x, the value f is added to BB[n1][n2][0]
	and 1 is added to BB[n1][n2][1].
	BB[n1][n2][0]/BB[n1][n2][1] is seen as the probable "cost" (penalty) of
	using the arc (n1,n2) in a tour.
	----------------------  
	BB_option=1:
	For each arc (n1,n2) of x, the value f replace the previous one if it is smaller,
	and 1 is added to BB[n1][n2][1].
	*/
 if (move[4]<3) return; // The blackboard won't be used. No need to update it!
 if (trace>1)
	printf("\n BB_update");
 int  i;
 int n1,n2;
 
 for (i=0;i<G.N;i++) // Warning. This suppose the last node is equal to the first one
 {
	n1=p.x.s[i];
	n2=p.x.s[i+1];
	if (BB_option==0)
	{
	 BB[0].v[n1][n2]=BB[0].v[n1][n2]+p.f;
	 BB[1].v[n1][n2]=BB[1].v[n1][n2]+1;
	}
	else
	{
	 if ( BB[1].v[n1][n2]>0 && p.f<BB[0].v[n1][n2])
	 {
		BB[1].v[n1][n2]=BB[1].v[n1][n2]+1;
		BB[0].v[n1][n2]=p.f;
	 }
	}
 }
 
}

//============================================================== BB_USE
struct particle BB_use(struct particle p)
{  /*
	Build a tour, using the blackboard
	For each arc (n,*) to add, make a probabilistic choice,
	according to the line n of BB
	*/
 
 double   big_value;
 int   candidate[Nmax];
 double   candidate_penalty[Nmax];
 int         candidate_nb;
 double   c_max;
 int      i;
 //int      j;
 //int      k;
 //int      m;
 int      node1,node2;
 double   penalty;
 struct particle pt;
 double  r;
 int  rank;
 int     used[Nmax]={0};
 
 big_value=G.N*G.l_max;
 
 pt=p;
 node1=0; // The new tour begins in 0
 rank=1;
 used[node1]=1;
 pt.x.s[node1]=0;
 
 //printf("\n");
 next_node:
	candidate_nb=0;
	for (node2=0;node2<G.N;node2++) // For each possible new node ...
 {
	if (used[node2]>0) continue;
	if(rank==G.N-1) goto set_node; // Last node => no choice, anyway
	
	// Ask the blackboard
	if (BB[1].v[node1][node2]==0)  penalty=big_value;
	else
	{
	 if (BB_option==0)
		penalty= BB[0].v[node1][node2]/BB[1].v[node1][node2] ;
	 else
		penalty= BB[0].v[node1][node2];
	}
	
	candidate_penalty[candidate_nb]=penalty;
	candidate[candidate_nb]=node2;
	candidate_nb=candidate_nb+1;
 }   // next node2
	
	if (candidate_nb==1) goto set_node;  // Fi there is just one possible node, add it
	
	// Choose the new node at random, according to penalties
	c_max=0;
	for (i=0;i<candidate_nb;i++) c_max=c_max+ candidate_penalty[i];
	for (i=0;i<candidate_nb;i++) candidate_penalty[i]=1-candidate_penalty[i]/c_max;//Proba
	for (i=1;i<candidate_nb;i++) candidate_penalty[i]=candidate_penalty[i-1]+candidate_penalty[i];  // Cumulate
	r=alea_float(0,candidate_nb-1); // random choice
	
	//printf("\n %i: ",candidate_nb);for (i=0;i<candidate_nb;i++) printf(" %f",  candidate_penalty[i]);
	
	for (i=0;i<candidate_nb; i++)
 {
	if(r<candidate_penalty[i])
	{
	 node2=candidate[i];
	 goto set_node;
	}
 }
 set_node:
	//printf("\n r %f, i %i node2 %i",r,i,node2);
	//printf(" %i",node2);
	pt.x.s[rank]=node2;  // Add the node
	used[node2]=1;   //  remember it is already used
	rank=rank+1;
	if (rank<G.N) goto next_node;
	
	pt.x.s[G.N]=pt.x.s[0]; // Complete the tour
	pt.f=f(pt,-1,-1); // evaluate
	if (pt.f<pt.best_f)  // eventually update the best previous position
 {
	pt.best_x=pt.x;
	pt.best_f=pt.f;
	BB_update(pt); // Update the blackboard
 }
	if (trace>1)
 {
	display_position(pt,1);
	display_position(pt,2);
 }
	BB_update(pt); // Update the blackboard
	return pt;
}