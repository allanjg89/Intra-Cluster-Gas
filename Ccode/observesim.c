/*
===========================================================================
Contributions made by Allan Gamboa
===========================================================================
*/
//strucutre to store rad values and corresponding index
//for later sorting
struct rad{
	float radius;
	int index;
};


	
	
//==================================================================

//The comapre function for qsort
int cmpfunc (const void  *p, const void * q)
{
	float a = ((struct rad *)p)->radius;
    float b = ((struct rad *)q)->radius; 
	if (a-b>0) {
		return 1;
	} else if (a-b<0) {
		return -1;
	}else {
		return 0;
	}
}


//==================================================================
/*
The following method computes the mass and density profile for a given simulation 
then writes the informatuon to 4 txt file, one for every particle type and one for
the combination of all particle types,  containing 
5 columns [radius, mass enclosed, shell, density for particular shell, mass 
computed from fit to spectrum, density computed from the fit to the spectrum].
*/

void produceMassProf(struct jaco_state *js){
	
	
	
	 struct particle *p;
	 int shellNum = 100;//The number of particles per shell
	 int shellCount = 0;//initializing the current shell
	 float mass,prevR,prevM;
	 float volShell, massShell, rdiff, D2ShellRho;//keep track of previous radius and mass when computing the density prof.
	 double rho=0;
	 double rhoFromSpec, massFromSpec;//Desnity computed from fitting to the bremstrahlung spectrum
	 double md = js->Md;
     double xcen,ycen,zcen,x,y,z;
     int k1, k2, k3, t, i,  ind;
	 const char *type;
	 int count = 0;//This variable keeps track of the proper index for the arrays 
					//rAll and massAll which contain the values for all the particle types.
	 //long ARCMINTORAD = 1; 
	//creating a unique filename 
	char ending[50], tok[50], simfile[50], jacofilename[50],*filename;
     k1 = js->axis; k2 = (js->axis+1)%3; k3 = (js->axis+2)%3;
	const char fileHeader[500] = "File created within produceMasProf Method contained in the observesim.c file within the jaco package.\n[ radius | mass enclosed [10^14 M_{not}] | shell | density | mass from spectrum | density from spectrum ]\n\n";

     //mpctopix = 60./(ARCMINTORAD*js->angdist*js->pixscale);
	
     


     //iterating over the different particle types
	//FILE *file = fopen("rvals.txt", "w+");//file for rvals
	
	
	struct rad *rAll = (struct rad *)malloc(sizeof(struct rad)*(js->np[0]+js->np[1]+js->np[2]));
	float *massAll = (float *)malloc(sizeof(float)*(js->np[0]+js->np[1]+js->np[2]));//[js->np[0]+js->np[1]+js->np[2]];
							
		
	 for (t = 0; t < 3; ++t) {

		/*printf("%i\n",js->np[t]);
		fflush(stdout);*/
		struct rad *r = (struct rad *)malloc(sizeof(struct rad)*js->np[t]);
		//The particles are held in the following structure, where t ranges
		//from 0 to 2 to indicate the different particle types.
		//1)Gas 2)Dark 3)Stars
		if (t==0) {
			type = "Dark";
		}else if (t==1) {
			type = "Gas";
		}else {
			type = "Stars";
		}
		
		//creating a unique filename 
		//char ending[50], tok[50], simfile[50], jacofilename[50];

	    strcpy(simfile,  js->simfile);
		strcpy(jacofilename,  js->jacofilename);
	    strcpy(ending, "DensityProf.txt");
	    strcpy(tok,".");

	   
	    filename = strcat(strtok(simfile,tok),type);
		filename = strcat(strtok(jacofilename,tok),filename);
	    filename = strcat(filename,ending);
		//printf("%s\n",filename);
		//------------------------------------------

		
		 p = js->ptype[t];
		 xcen = 0;ycen = 0; zcen = 0;
		//taking a wighted average to find the center of 
		//each coordinate.
		for (i = 0; i < js->np[t]; ++i) {
			xcen += (double)p[i].pos[k2];
			ycen += (double)p[i].pos[k3];
			zcen += (double)p[i].pos[k1];
			
	     }
	
		xcen /= (js->np[t]);
		ycen /= (js->np[t]);
		zcen /= (js->np[t]);
	
			//computing the radii for each particle and storing the radii struct array.
			
			for (i = 0; i < js->np[t]; ++i) {

			  z = p[i].pos[k1]-zcen;
			  x = p[i].pos[k2]-xcen;
			  y = p[i].pos[k3]-ycen;
			  //y = ycen+(p[i].pos[k3]-js->center[t][k3]);
			  r[i].radius = (float)sqrt(x*x+y*y+z*z);
			  r[i].index = i;
			  rAll[count].radius = (float)sqrt(x*x+y*y+z*z);
			  rAll[count].index = count;
			  massAll[count] = p[i].mass;
			  count++;
			  }			
	
		FILE *file = fopen(filename, "w+");
		
		//sorting the rad array
		qsort(r, js->np[t], sizeof(long), cmpfunc);

		mass = 0;
		
		prevR = 0;
		prevM = 0;
		if (file) {
			fprintf(file, fileHeader);
			for (i = 0;i < js->np[t]; ++i) {
				
				ind = r[i].index;
			    mass += p[ind].mass; 
					
				//computing the density of a particular shell
				if (i%shellNum == 0 && i!=0) {
					
					volShell = (float)(4.0/3)*3.14*(pow((double)r[i].radius,3)-pow((double)prevR,3));
					massShell = (mass-prevM);
					rho = massShell/volShell;
					//rdiff = r[i].radius-prevR;
					//the following parameter, D2ShellRho, has no physical significance;
					//it is mereley there or testing purposes
					//D2ShellRho = massShell/rdiff;
					prevR = r[i].radius;
					prevM = mass;
					shellCount += 1;
					//Writting the information to the file
					rhoFromSpec = md*js->darkdens((double)r[i].radius,js);//Compunting the density from the
												//spectrum using the built in function: darkdens
					massFromSpec = md*js->darkmass((double)r[i].radius,js);
					fprintf(file, "%f\t %f\t %d\t %f\t %f\t %f\n", r[i].radius,mass,shellCount,rho, massFromSpec, rhoFromSpec);

				}
								
						
			}
			
		}else {
			printf("Unable to open mass density file.");
		}


	   fclose(file);
	   shellCount = 0;
	
	}
	
	
	//The following is to create the parameters for a combination of all the particles
	
	//char ending[50], tok[50], simfile[50];

	strcpy(simfile,  js->simfile);
	strcpy(jacofilename,  js->jacofilename);
    strcpy(ending, "DensityProf.txt");
    strcpy(tok,".");

   
    filename = strcat(strtok(simfile,tok),"All");
	filename = strcat(strtok(jacofilename,tok),filename);
    filename = strcat(filename,ending);
    
	
	
	FILE *file = fopen(filename, "w+");
			
	//sorting the rad array
	qsort(rAll, js->np[0]+js->np[1]+js->np[2], sizeof(long), cmpfunc);
	
	prevR = 0;
	prevM = 0;
	mass = 0;
	
	if (file) {
		fprintf(file, fileHeader);
		for (i = 0;i < js->np[0]+js->np[1]+js->np[2]; ++i) {
			
			ind = rAll[i].index;
		    mass += massAll[ind]; 
				
			//computong the density of a particular shell
			if (i%shellNum == 0 && i!=0) {
				
				volShell = (float)(4.0/3)*3.14*(pow((double)rAll[i].radius,3)-pow((double)prevR,3));
				massShell = (mass-prevM);
				rho = massShell/volShell;
				//rdiff = r[i].radius-prevR;
				//the following parameter, D2ShellRho, has no physical significance;
				//it is mereley there or testing purposes
				//D2ShellRho = massShell/rdiff;
				prevR = rAll[i].radius;
				prevM = mass;
				shellCount += 1;
				//---------------------------------------------
				//Writting the information to the file
				rhoFromSpec = md*js->darkdens((double)rAll[i].radius,js);//Compunting the density from the
											//spectrum using the built in function: darkdens
				massFromSpec = md*js->darkmass((double)rAll[i].radius,js);							
				fprintf(file, "%f\t %f\t %d\t %f\t %f\t %f\n", rAll[i].radius,mass,shellCount,rho, massFromSpec, rhoFromSpec);
			}
			
			
					
		}
		
	}else {
		printf("Unable to open mass density file for the combined particle types.");
	}


   fclose(file);
   shellCount = 0;
   //printf(filename);
	//fflush(stdout);

			
}


//==================================================================
