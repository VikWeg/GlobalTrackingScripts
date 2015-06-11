#include <fstream>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nifti1.h"

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

	//***************************************************

	FILE* data_file = fopen("C:/ETH/Neuro/GlobalTracking/1159_tensor.nii", "r");
	if (data_file == NULL)
	{
		fprintf(stderr, "\nError opening data file\n");
	}

	ret = fseek(data_file, (long)(hdr.vox_offset), SEEK_SET);
	if (ret != 0)
	{
		fprintf(stderr, "\nError doing fseek() to data file\n");
	}

	float* data = (float *)malloc(sizeof(float)* hdr.dim[1] * hdr.dim[2] * hdr.dim[3] * hdr.dim[4]);
	if (data == NULL)
	{
		fprintf(stderr, "\nError allocating data buffer\n");
	}

	ret = fread(data, sizeof(float), hdr.dim[1] * hdr.dim[2] * hdr.dim[3] * hdr.dim[4], data_file);
	if (ret != hdr.dim[1] * hdr.dim[2] * hdr.dim[3] * hdr.dim[4])
	{
		fprintf(stderr, "\nError reading data\n");
	}

	int err = ferror(data_file);
	int eof = feof(data_file);
	int pos = ftell(data_file);

	fclose(data_file);

	int x = 48;
	int y = 64;
	int z = 64;
	int t = 1;

	int coo = (t - 1) * 128 * 128 * 72 + (72 - z) * 128 * 128 + (128 - y) * 128 + (128 - x);

	//for(i = 0; i < hdr.dim[1]; i++)
	printf("\ndata[%d] = %f\n", coo, data[coo]);

}

