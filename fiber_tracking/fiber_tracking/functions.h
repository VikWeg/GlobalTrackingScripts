
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
{
	float xij [3];
	
	for (int n = 0; n < 3; n++)
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

	float norm_ij = sqrtf(xij[0] * xij[0] + xij[1] * xij[1] + xij[2] * xij[2]);
	float norm_ik = sqrtf(xik[0] * xik[0] + xik[1] * xik[1] + xik[2] * xik[2]);

	return (xij[0] * xik[0] + xij[1] * xik[1] + xij[2] * xik[2]) / norm_ij / norm_ik;

}

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
}