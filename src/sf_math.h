#ifndef sf_math_h
#define sf_math_h

// Matricies are in column-major order

typedef int* SF_Matrix;

// Intializes a matrix of size MxN or NxN
void sf_init_mat(SF_Matrix* matrix, int n);

// 4x4 Zeroed matrix
void sf_mat_4x4(SF_Matrix* matrix);

// Creates an identity of specified size
void sf_mat_identity(SF_Matrix* matrix, int n);

// Creates a 4x4 projection matrix
void sf_mat_projection(SF_Matrix* matrix);



/*
Access
*/

// Replace a value in  the given matrix
void sf_mat_replace(SF_Matrix* matrix, int row, int column, int value);

#endif sf_math_h