#include <fstream>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nifti1.h"

#include "functions.h"

#define MIN_HEADER_SIZE 348
#define NII_HEADER_SIZE 352

int main()
{
	nifti_1_header hdr;

	FILE* header_file = fopen("C:/ETH/Neuro/GlobalTracking/1159_tensor.nii", "r");
	if (header_file == NULL)
	{
		fprintf(stderr, "\nError opening header file\n");
		exit(1);
	}

	long ret = fread(&hdr, MIN_HEADER_SIZE, 1, header_file);
	if (ret != 1)
	{
		fprintf(stderr, "\nError reading header file %s\n", header_file);
		exit(1);
	}

	fclose(header_file);

	//******** Open data file ********
	FILE* data_file = fopen("C:/ETH/Neuro/GlobalTracking/1159_tensor.nii", "rb");
	if (data_file == NULL)
	{
		fprintf(stderr, "\nError opening data file\n");
	}
	//******** Set read head ********
	ret = fseek(data_file, (long)(hdr.vox_offset), SEEK_SET);
	if (ret != 0)
	{
		fprintf(stderr, "\nError doing fseek() to data file\n");
	}
	//******** Allocate data memory ********
	float* data = (float *)malloc(sizeof(float)* hdr.dim[1] * hdr.dim[2] * hdr.dim[3] * hdr.dim[4]);
	if (data == NULL)
	{
		fprintf(stderr, "\nError allocating data buffer\n");
	}
	//******** Read data from file ********
	ret = fread(data, sizeof(float), hdr.dim[1] * hdr.dim[2] * hdr.dim[3] * hdr.dim[4], data_file);
	if (ret != hdr.dim[1] * hdr.dim[2] * hdr.dim[3] * hdr.dim[4])
	{
		fprintf(stderr, "\nError reading data\n");
	}

	fclose(data_file);

	vertex*** ensemble = new vertex**;

	int coo;
	int center [4] = {0, 48, 64, 64 };
	int size = 9;
	int dim[4] = {0, hdr.dim[1], hdr.dim[2], hdr.dim[3] };
	float ten [6] = { 0, 0, 0, 0, 0, 0 };

	for (int i = 0; i < size; i++)
	{
		ensemble[i] = new vertex*;

		for (int j = 0; j < size; j++)
		{
			ensemble[i][j] = new vertex [size];

			for (int k = 0; k < size; k++)
			{
				//******** Tensor ********
				ensemble[i][j][k].T = new float[6];

				for (int t = 0; t < 6; t++)
				{
					coo =	t*dim[1] * dim[2] * dim[3]
							+ (dim[3] - (center[3] + k - size/2) ) * dim[2] * dim[1]
							+ (dim[2] - (center[2] + j - size/2) ) * dim[1]
							+ (dim[1] - (center[1] + i - size/2) );

					ten[t] = data[coo];
				}

				float norm = 1/ ( ten[1] * ten[1] + ten[2] * ten[2] + ten[4] * ten[4] - ten[0] * ten[3] - ten[5] * (ten[0] + ten[3]) );

				ensemble[i][j][k].T[0] = (ten[4] * ten[4] - ten[3] * ten[5])* norm;
				ensemble[i][j][k].T[1] = (ten[1] * ten[5] - ten[2] * ten[4])* norm;
				ensemble[i][j][k].T[2] = (ten[2] * ten[3] - ten[1] * ten[4])* norm;
				ensemble[i][j][k].T[3] = (ten[2] * ten[2] - ten[0] * ten[5])* norm;
				ensemble[i][j][k].T[4] = (ten[0] * ten[4] - ten[1] * ten[2])* norm;
				ensemble[i][j][k].T[5] = (ten[1] * ten[1] - ten[0] * ten[3])* norm;

				//******** Position ********
				ensemble[i][j][k].x = new float[3];
				ensemble[i][j][k].x[0] = i;
				ensemble[i][j][k].x[1] = j;
				ensemble[i][j][k].x[2] = k;

				//******** Surface ********
				if (i == 0 || j == 0 || k == 0 || i == size - 1 || j == size - 1 || k == size - 1)
					ensemble[i][j][k].sig = 1;
				else
					ensemble[i][j][k].sig = 0;
			}
		}
	}

	int nn;
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			for (int k = 0; k < size; k++)
			{
				//******** Neighbours ********
				nn = (fmin(size, i + 1) - fmax(0, i - 1) + 1)
					*(fmin(size, j + 1) - fmax(0, j - 1) + 1)
					*(fmin(size, k + 1) - fmax(0, k - 1) + 1)
					- 1;

				ensemble[i][j][k].nn = nn;
				ensemble[i][j][k].n = new vertex* [nn];

				int m = 0;
				for (int ii = fmax(0, i - 1); ii < fmin(size, i + 2); ii++)
				{
					for (int jj = fmax(0, j - 1); jj < fmin(size, j + 2); jj++)
					{
						for (int kk = fmax(0, k - 1); kk < fmin(size, k + 2); kk++)
						{
							if (ii != i || jj != j || kk != k)
							{
								ensemble[i][j][k].n[m] = &ensemble[ii][jj][kk];
								m++;
							}
						}
					}
				}

				//******** Connected ********

				ensemble[i][j][k].c = new vertex* [nn];
				for (int n = 0; n < nn; n++)
					ensemble[i][j][k].c[n] = &null;
			}

	free(data);

	
}

