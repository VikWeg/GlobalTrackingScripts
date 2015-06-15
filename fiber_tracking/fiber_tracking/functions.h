<<<<<<< HEAD
#include <fstream>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iomanip>
#include <functional>
#include <random>
#include "nifti1.h"

#define MIN_HEADER_SIZE 348
#define NII_HEADER_SIZE 352

std::default_random_engine generate;

struct vertex
{
	float* x;
	int* c;
	float* T;
	int* pos;
	int sig;
	int nn;
	vertex** n;
};

float Edata(vertex* vi, vertex* vj)
=======

struct vertex
{
	float* x; //
	vertex** c; //
	float* T; //
	//float Emin;
	//float Emax;
	int sig; //
	int nn; //
	vertex** n; //
};

vertex null = { 0, 0, 0, 0, 0, 0 };

float Edata(vertex vi, vertex vj)
>>>>>>> origin/master
{
	float xij [3];
	
	for (int n = 0; n < 3; n++)
<<<<<<< HEAD
		xij[n] = (*vi).x[n] - (*vj).x[n];

	float norm = xij[0] * xij[0] + xij[1] * xij[1] + xij[2] * xij[2];

	return	(	(*vi).T[0] * xij[0] * xij[0]
		+ 2 *	(*vi).T[1] * xij[0] * xij[1]
		+ 2 *	(*vi).T[2] * xij[0] * xij[2]
		+		(*vi).T[3] * xij[1] * xij[1]
		+ 2 *	(*vi).T[4] * xij[1] * xij[2]
		+		(*vi).T[5] * xij[2] * xij[2]

		+		(*vj).T[0] * xij[0] * xij[0]
		+ 2 *	(*vj).T[1] * xij[0] * xij[1]
		+ 2 *	(*vj).T[2] * xij[0] * xij[2]
		+		(*vj).T[3] * xij[1] * xij[1]
		+ 2 *	(*vj).T[4] * xij[1] * xij[2]
		+		(*vj).T[5] * xij[2] * xij[2]
			)/norm;
}

float Eint(vertex* vj, vertex* vi, vertex* vk)
{
	float xij[3];
	for (int n = 0; n < 3; n++)
		xij[n] = (*vj).x[n] - (*vi).x[n];

	float xik[3];
	for (int n = 0; n < 3; n++)
		xik[n] = (*vk).x[n] - (*vi).x[n];
=======
		xij[n] = vi.x[n] - vj.x[n];

	float norm = xij[0] * xij[0] + xij[1] * xij[1] + xij[2] * xij[2];

	return	(     vi.T[0] * xij[0] * xij[0]
		    + 2 * vi.T[1] * xij[0] * xij[1]
		    + 2 * vi.T[2] * xij[0] * xij[2]
		    +     vi.T[3] * xij[1] * xij[1]
	      	+ 2 * vi.T[4] * xij[1] * xij[2]
		    +     vi.T[5] * xij[2] * xij[2]

			+     vj.T[0] * xij[0] * xij[0]
			+ 2 * vj.T[1] * xij[0] * xij[1]
			+ 2 * vj.T[2] * xij[0] * xij[2]
			+     vj.T[3] * xij[1] * xij[1]
			+ 2 * vj.T[4] * xij[1] * xij[2]
			+     vj.T[5] * xij[2] * xij[2]
			)/norm;
}

float Eint(vertex vj, vertex vi, vertex vk)
{
	float xij[3];
	for (int n = 0; n < 3; n++)
		xij[n] = vj.x[n] - vi.x[n];

	float xik[3];
	for (int n = 0; n < 3; n++)
		xik[n] = vk.x[n] - vi.x[n];
>>>>>>> origin/master

	float norm_ij = sqrtf(xij[0] * xij[0] + xij[1] * xij[1] + xij[2] * xij[2]);
	float norm_ik = sqrtf(xik[0] * xik[0] + xik[1] * xik[1] + xik[2] * xik[2]);

	return (xij[0] * xik[0] + xij[1] * xik[1] + xij[2] * xik[2]) / norm_ij / norm_ik;

}

<<<<<<< HEAD
float Ei_x(vertex* vi)
{
	float E = 0;

	for (int j = 0; j < (*vi).nn; j++)
		E += (*vi).c[j] * Edata(vi, (*vi).n[j]);

	for (int j = 0; j < (*vi).nn; j++)
		for (int k = j + 1; k < (*vi).nn; k++)
			E += (*vi).c[j] * (*vi).c[k] * Eint((*vi).n[j], vi, (*vi).n[k]);

	for (int j = 0; j < (*vi).nn; j++)
		for (int k = 0; k < (*(*vi).n[j]).nn; k++)
			if ((*(*vi).n[j]).n[k] != vi)
				E += (*vi).c[j] * (*(*vi).n[j]).c[k] * Eint(vi, (*vi).n[j], (*(*vi).n[j]).n[k]);

	return E;
}

float Ei_c(vertex* vi)
{
	int c = 0;
	for (int i = 0; i < (*vi).nn; i++)
		c += (*vi).c[i];

	float E = 0;

	E += (1 - (*vi).sig)*fabsf(c - 2);
	E += (*vi).sig*fabsf(c - 1);

	int cj;
	for (int j = 0; j < (*vi).nn; j++)
	{
		cj = 0;
		for (int k = 0; k < (*(*vi).n[j]).nn; k++)
			cj += (*(*vi).n[j]).c[k];

		E += (1 - (*(*vi).n[j]).sig)*fabsf(cj - 2);
		E += (*(*vi).n[j]).sig*fabsf(cj - 1);
	}

	return E;
}

float Ei(vertex* vi)
{
	float E = 0;

	for (int j = 0; j < (*vi).nn; j++)
		E += (*vi).c[j] * Edata(vi, (*vi).n[j]);

	E = 0.5 *E;

	for (int j = 0; j < (*vi).nn; j++)
		for (int k = j + 1; k < (*vi).nn; k++)
			E += (*vi).c[j] * (*vi).c[k] * Eint((*vi).n[j], vi, (*vi).n[k]);

	int c = 0;
	for (int i = 0; i < (*vi).nn; i++)
		c += (*vi).c[i];

	E += (1 - (*vi).sig)*fabsf(c - 2);
	E += (*vi).sig*fabsf(c - 1);

	return E;
}

void mc_c(vertex* vi)
{
	int n = 100;
	int d;
	float E0, E1, p;

	for (float T = 1.; T > 10e-2*T; T *= 0.9*T)
		for (int i = 1; i < n; i++)
		{

			E0 = Ei_c(vi) + Ei_x(vi);

			std::uniform_int_distribution<int> u_c(0, (*vi).nn-1);

			d = u_c(generate);
			(*vi).c[d] = 1 - (*vi).c[d];

			int j = 0;
			for (; j < (*(*vi).n[d]).nn; j++)
				if (vi == (*(*vi).n[d]).n[j])
				{
					(*(*vi).n[d]).c[j] = 1 - (*(*vi).n[d]).c[j];
					break;
				}

			E1 = Ei_c(vi) + Ei_x(vi);

			p = fminf(1., expf((E0 - E1) / T));
			std::bernoulli_distribution acceptQ(p);

			if (acceptQ(generate))
				;
			else
			{
				(*vi).c[d] = 1 - (*vi).c[d];
				(*(*vi).n[d]).c[j] = 1 - (*(*vi).n[d]).c[j];
			}
		}
}

void mc_x(vertex* vi)
{
	int n = 100;

	float x0,y0,z0,x1,y1,z1;
	float E0,E1,p;

	for (float T = 1.; T > 10e-2*T; T *= 0.9*T)
		for (int i = 1; i < n; i++)
		{
			E0 = Ei_x(vi);

			x0 = (*vi).x[0];
			y0 = (*vi).x[1];
			z0 = (*vi).x[2];

			std::uniform_real_distribution<float> u_x(x0 - (x0 - (*vi).pos[0] + 0.5) / 10., x0 + ((*vi).pos[0] + 0.5 - x0) / 10);
			(*vi).x[0] = u_x(generate);
			std::uniform_real_distribution<float> u_y(y0 - (y0 - (*vi).pos[1] + 0.5) / 10., y0 + ((*vi).pos[1] + 0.5 - y0) / 10);
			(*vi).x[1] = u_y(generate);
			std::uniform_real_distribution<float> u_z(z0 - (z0 - (*vi).pos[2] + 0.5) / 10., z0 + ((*vi).pos[2] + 0.5 - z0) / 10);
			(*vi).x[2] = u_z(generate);

			E1 = Ei_x(vi);

			p = fminf(1., expf((E0 - E1) / T));
			std::bernoulli_distribution acceptQ(p);

			if (acceptQ(generate))
				;
			else
			{
				(*vi).x[0] = x0;
				(*vi).x[1] = y0;
				(*vi).x[2] = z0;
			}
		}
=======
float Ei(vertex vi)
{
	float E = 0;

	int c = 0;
	for (int j = 0; (*vi.c[j]).nn != 0; j++)
		c++;

	for (int j = 0; j < c; j++)
		E += Edata(vi, *vi.c[j]);
	
	for (int j = 0; j < c; j++)
		for (int k = j + 1; k < c; k++)
			E += Eint(*vi.c[j], vi, *vi.c[k]);

	E += (1 - vi.sig)*fabsf(c - 2);
	E += vi.sig*fabsf(c - 1);

	int cj;
	for (int j = 0; j < c; j++)
	{
		cj = 0;
		for (int k = 0; (*(*vi.c[j]).c[k]).nn != 0; k++)
			cj++;

	}
}

float Ei_x(vertex vi)
{
	float E = 0;

	int c = 0;
	for (int j = 0; (*vi.c[j]).nn != 0; j++)
		c++;

	for (int j = 0; j < c; j++)
		E += Edata(vi, *vi.c[j]);

	for (int j = 0; j < c; j++)
	for (int k = j + 1; k < c; k++)
		E += Eint(*vi.c[j], vi, *vi.c[k]);
}

void mc_c(vertex vi)
{

}

void mc_x(vertex vi)
{
	int const n = 100;

	float** X = new float*[n];
	float* E = new float[n];

	E[0] = Ei_x(vi);

	for (int i = 1; i < n; i++)
	{

	}
>>>>>>> origin/master
}