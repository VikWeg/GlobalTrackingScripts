
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

float Ed(vertex vi, vertex vj)
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

float Ei(vertex vj, vertex vi, vertex vk)
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