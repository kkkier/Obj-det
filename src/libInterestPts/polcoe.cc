#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#define NR_END 1
#define FREE_ARG char*

void nr_error(char error_text[])
/* Numerical Recipes standard error handler */
{
	fprintf(stderr,"Numerical Recipes run-time error...\n");
	fprintf(stderr,"%s\n",error_text);
	fprintf(stderr,"...now exiting to system...\n");
	exit(1);
}

float *fvector(long nl, long nh)
/* allocate a float vector with subscript range v[nl..nh] */
{
	float *v;

	v=(float *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(float)));
	if (!v) nr_error("allocation failure in vector()");
	return v-nl+NR_END;
}

void free_fvector(float *v, long nl, long nh)
/* free a float vector allocated with vector() */
{
	free((FREE_ARG) (v+nl-NR_END));
}

void polcoe(float x[], float y[], int n, float cof[])
{
	int k,j,i;
	float phi,ff,b,*s;

	s=fvector(0,n);
	for (i=0;i<=n;i++) s[i]=cof[i]=0.0;
	s[n] = -x[0];
	for (i=1;i<=n;i++) {
		for (j=n-i;j<=n-1;j++)
			s[j] -= x[i]*s[j+1];
		s[n] -= x[i];
	}
	for (j=0;j<=n;j++) {
		phi=n+1;
		for (k=n;k>=1;k--)
			phi=k*s[k]+x[j]*phi;
		ff=y[j]/phi;
		b=1.0;
		for (k=n;k>=0;k--) {
			cof[k] += b*ff;
			b=s[k]+x[j]*b;
		}
	}
	free_fvector(s,0,n);
}
#undef NRANSI
