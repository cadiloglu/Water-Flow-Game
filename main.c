#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void form_matrix(int size, int **ground){
	int i;
	for(i=-1; i<=size; i++){
		ground[i]=(int *)calloc(size, sizeof(int));
	}
}
void flow(int **ground , int line , int column){
	if(ground[line-1][column]==1){
		ground[line-1][column]=2;
		flow(ground , line-1 , column);
	}
	if(ground[line+1][column]==1){
		ground[line+1][column]=2;
		flow(ground , line+1 , column);
	}
	if(ground[line][column-1]==1){
		ground[line][column-1]=2;
		flow(ground , line , column-1);
	}
	if(ground[line][column+1]==1){
		ground[line][column+1]=2;
		flow(ground , line , column+1);
	}
}
void move_number(int **ground, int size , float *count , int move){
	int i, j;
	for(i=0;i<size;i++){
		for(j=0;j<size;j++){
			if(ground[i][j]==1 || ground[i][j]==2)
				count[move]++;
		}
	}
}

int main(int argc, char *argv[]){
	FILE *output=fopen("output.txt", "w"); // Creating an output file as required.
	int size, number, i, j, k, move=1, line, column, check; // "size" is the magnitude of one side of the matrix (N). "number" is the number of independent experiment (T).
	float sum=0, mean=0, std_sq, minus=0, std=1; // This variants are used to calculate "mean" and "stddev".
	size=atoi(argv[1]);
	number=atoi(argv[2]);
	float *count=(float *)calloc(number, sizeof(float)); // This array used to count and keep the number of moves in every independent experiment.
	int **ground=(int **)calloc(size, sizeof(int));
	srand(time(NULL));
	form_matrix(size, ground);

for(i=0;i<number;i++){
	check=0; // "check" used to check if water reaches the bottom line.
	
	for(j=0;j<size;j++){  //This part is making all the cells 0 before starting to open randomly.
		for(k=0;k<size;k++){
			ground[j][k]=0;
		}
	}

	while(check==0){
		line=rand()%size;
		column=rand()%size;	
	
		ground[line][column]=1;
		
		if(ground[0][column]==1){
			ground[0][column]=2;
		}

		if(ground[line][column]==2 || ground[line+1][column]==2 || ground[line-1][column]==2 || ground[line][column+1]==2 || ground[line][column-1]==2){
			ground[line][column]=2; //If any cell near to the opened cell is 2, makes the opened cell 2.
			flow(ground , line , column);

		}

		for(j=0;j<size;j++){
			if(ground[size-1][j]==2)
				check++; // Check becomes non-zero if one of the cells on the bottom becomes 2.		
		}	
	}	
	
	move_number(ground, size, count, move);
	move++; //Passing to the next move.
}
	
	for(i=0;i<number;i++){
		sum+=count[i]/(size*size);
	}
		mean=sum/number; // Calculated "mean".
	
	for(i=0;i<number;i++){
		minus+=(mean-(count[i]/(size*size)))*(mean-(count[i]/(size*size)));
	}
		std_sq=minus/(number-1);//Calculated "stddev" square.
	
	for(i=0;i<1000;i++){ // Here i used "babylonian method" to calculate square root without "sqrt" function.
		std=0.5*(std+(std_sq/std));
	}
	
	for(i=0;i<size;i++){
		for(j=0;j<size;j++){
			fprintf(output,"%d ",ground[i][j]); // Printing last matrix to the "output.txt" file.
		}
		fprintf(output,"\n");
	}
	fprintf(output,"\nmean()  = %f\n",mean); // Printing "mean()"
	fprintf(output,"stddev()= %f\n",std); // Printing "stddev()"
	fclose(output);
	
	return 0;
}
